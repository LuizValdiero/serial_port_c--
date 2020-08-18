#include <Arduino.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <serial_keys.h>

#include <defines.h>
#include "../include/serial_package.h"
#include "../include/data_handler.h"
#include "../include/crypto.h"
#include "../include/utils/time_handler.h"

#define DATA_BUFFER_SIZE 70
unsigned char data_str[DATA_BUFFER_SIZE];

void serial_resynchronize();
void serial_write_data(buffer_t * input);
int create_serial_package(buffer_t * package_out, struct cipher_handle_t * cipher);

struct cipher_handle_t cipher;
  
void setup() {
  Serial.begin(115200);
  delay(500);
  initialize_crypto(&cipher, key, key_size);
	
  serial_resynchronize();
}

//finish_crypto(&cipher);

void loop() {
  buffer_t data = { .buffer_size = DATA_BUFFER_SIZE, .buffer = data_str};
  create_serial_package(&data, &cipher);
  serial_write_data(&data);
  delay(60000);
  serial_resynchronize();
}


void serial_resynchronize() {
  char input_char;
  int i = 0;
  while (i < 3) {
    if (Serial.available()) {
      Serial.readBytes(&input_char, 1);
      if(input_char=='X') {
        i++;
      }
    }
  }
}

void serial_write_data(buffer_t * input) {
  uint8_t len = input->buffer_size;
  Serial.write(len);
  // add Serial avaliable
  Serial.write(input->buffer, input->buffer_size);
  Serial.write("XXX", 3);
}

int create_serial_package(buffer_t * package_out, \
                struct cipher_handle_t * cipher)
{
  int res;
	serial_header_t header;
	buffer_t encrypted_data;
	buffer_t plain_data;

	uint32_t total_size = 0;

  uint64_t time = 1597766982000000 + (millis()*100);

  record_t record;
  record.version = 17;
  record.unit = 2224179556;
  record.value = 20;
  record.uncertainty = 0;
  record.x = 0;
  record.y = 1;
  record.z = 2;
  record.t = time;
  record.dev = 0;
  
  res = create_data_package(RECORD, &plain_data, (void *) &record);
  
  if(res != CODE_SUCCESS) {
    return res;
  }

  unsigned char iv_char[16];
	buffer_t iv = { .buffer_size = 16, .buffer = iv_char};
	
  gerate_iv(cipher, &iv);
	memcpy(header.iv, iv.buffer, iv.buffer_size);

  cipher->nc_off = 0;
	encrypt_data(&cipher->aes, &cipher->nc_off, &iv, \
                &plain_data, &encrypted_data);
	
  free(plain_data.buffer);
    
	total_size = sizeof(header) + encrypted_data.buffer_size;
	
  if (package_out->buffer_size < total_size) {
		free(encrypted_data.buffer);
		return CODE_ERROR_SHORT_BUFFER;
	}

	header.encrypted_size = encrypted_data.buffer_size;

	mount_serial_package(package_out, &header, &encrypted_data);
	package_out->buffer_size = total_size;
    
	free(encrypted_data.buffer);
    
	return CODE_SUCCESS;
}