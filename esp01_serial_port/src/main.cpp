#include <Arduino.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#define CTR 1
//#define AES128

// https://github.com/kokke/tiny-AES-c
#include <aes.h>

typedef struct buffer_t {
    uint32_t buffer_size;
    unsigned char * buffer;
} buffer_t;


buffer_t output;
buffer_t data_size;

struct AES_ctx aes_ctx;
uint8_t iv[16];
uint8_t key[16] = {0x99, 0xF3, 0xCC, 0xA3, 0xFC, 0xC7, 0x10, 0x76, 0xAC, 0x16,
          0x86, 0x41, 0xD9, 0x06, 0xCE, 0xB5};

void setup() {
  delay(5000);
  Serial.begin(115200);
  delay(5000);

//
AES_init_ctx(&aes_ctx, key);
//



  data_size.buffer = (unsigned char *) malloc(4);
  data_size.buffer_size = 4;

}
 
void loop() {
  delay(2000);

  char * data_char = "123456789";
  int size = strlen(data_char);
  memcpy(data_size.buffer, &size, sizeof(int));

  Serial.write(data_size.buffer, data_size.buffer_size);

  output.buffer = (unsigned char *) malloc(size);
  output.buffer_size = size;
  memcpy(output.buffer, data_char, size);
  
  AES_ctx_set_iv(&aes_ctx, iv);
  AES_CTR_xcrypt_buffer(&aes_ctx, output.buffer, output.buffer_size);
  
  Serial.write(output.buffer, output.buffer_size);
  free(output.buffer);
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