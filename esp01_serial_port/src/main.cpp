#include <Arduino.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

typedef struct buffer_t {
    uint32_t buffer_size;
    unsigned char * buffer;
} buffer_t;

uint8_t iv[16];
uint8_t key[16] = {0x99, 0xF3, 0xCC, 0xA3, 0xFC, 0xC7, 0x10, 0x76, 0xAC, 0x16,
          0x86, 0x41, 0xD9, 0x06, 0xCE, 0xB5};

char input_char;

void serial_resynchronize() {
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

void setup() {
  Serial.begin(115200);
  delay(500);
  serial_resynchronize();
}

void serial_write_data(buffer_t * input) {
  uint8_t len = input->buffer_size;
  Serial.write(len);
  Serial.write(input->buffer, input->buffer_size);
  Serial.write("XXX", 3);
}

void loop() {
  unsigned char data_str[] = "12345678901";
  buffer_t data = { .buffer_size = 11, .buffer = data_str};
  serial_write_data(&data);
  delay(10);
  serial_resynchronize();
}

/*
// Initialize context calling one of:
void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);

// ... or reset IV at random point:
void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv);

// Then start encrypting and decrypting with the functions below:
void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf);
void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf);

void AES_CBC_encrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);

// Same function for encrypting as for decrypting in CTR mode 
void AES_CTR_xcrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
*/