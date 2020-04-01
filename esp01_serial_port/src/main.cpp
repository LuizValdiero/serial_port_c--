#include <Arduino.h>
#include <time.h>
#include <string.h>

#include <Crypto.h>
#include <AES.h>
#include <CTR.h>


// Cipher ctraes128
//AES128 aes128;
CTR<AES128> ctraes128;

struct aes_ctr_key_t {
  unsigned char key[16];
  unsigned char iv[16];
};

struct data_t
{
  unsigned char * plaintext;
  unsigned char * ciphertext;
  int size;
};

/*
static aes_ctr_key_t const aes_ctr_key {
  .key = {0x99, 0xF3, 0xCC, 0xA3, 0xFC, 0xC7, 0x10, 0x76, 0xAC, 0x16,
          0x86, 0x41, 0xD9, 0x06, 0xCE, 0xB5},
  .iv = {0x65, 0x04, 0xEF, 0x3F, 0x0D, 0xBF, 0xBE, 0x2A, 0xDD, 0x1D,
          0x1D, 0x39, 0x60, 0xC3, 0x39, 0x73}
};
*/

static aes_ctr_key_t const aes_ctr_key {
  .key = {0x99, 0xF3, 0xCC, 0xA3, 0xFC, 0xC7, 0x10, 0x76, 0xAC, 0x16,
          0x86, 0x41, 0xD9, 0x06, 0xCE, 0xB5},
  .iv = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

int prepare_cipher( Cipher * cipher,
  const struct aes_ctr_key_t * key)
{
  cipher->clear();
  if (!cipher->setKey(key->key, cipher->keySize())) {
    Serial.print("Error: setKey");
    return 1;
  }
  if (!cipher->setKey(key->iv, cipher->ivSize())) {
    Serial.print("Error: setIV");
    return 1;
  }
  //(cipher (CTR* ))->setCounterSize(4);

  return 0;
}

int encrypt( Cipher * cipher,
  const struct data_t * data) 
{
  memset(data->ciphertext, 0x00, data->size);
  cipher->encrypt(data->ciphertext, data->plaintext, data->size);
  return 0;
}


int decrypt( Cipher * cipher,
  const struct data_t * data) 
{
  memset(data->plaintext, 0x00, data->size);
  cipher->decrypt(data->plaintext, data->ciphertext, data->size);

  return 0;
}

#define MAX_DATA_SIZE 30
// "esp01 " + time + "\n\r"

data_t data_buf;

void setup() {
  Serial.begin(115200);
  ctraes128.setCounterSize(16);
}

void loop() {
  delay(10000);
  ctraes128.clear();
  
  //crypto_feed_watchdog();

  //unsigned char data[MAX_DATA_SIZE];  
  unsigned char data[]= "abcdefghijklmnop"; 
  data_buf.size = strlen("abcdefghijklmnop");
  //memcpy(data, ddata, data_buf.size);

  /*
  String val = (String)time;
  Serial.print("time:");
  val.toCharArray(&data[6], val.length()+1);
  Serial.println(val);
  Serial.println(sizeof(val));
  Serial.println(val.length());
  Serial.println();
  */

  data_buf.plaintext = data;
  data_buf.ciphertext = (unsigned char *) malloc(data_buf.size);

  prepare_cipher(&ctraes128, &aes_ctr_key);


  if (encrypt(&ctraes128, &data_buf)) {
    Serial.print("Error: encrypt \n");
  }
  //*
  for (int i = 0; i < data_buf.size; i++){
    Serial.print(data_buf.plaintext[i], HEX);
  }
  Serial.println("");
  delay(500);
  
  // */
  //*
  for (int i = 0; i < data_buf.size; i++){
    Serial.print(data_buf.ciphertext[i], HEX);
  }
  Serial.println("");
  delay(500);
  //*/
  /*
  prepare_cipher(&ctraes128, &aes_ctr_key);

  if (decrypt(&ctraes128, &data_buf)) {
    Serial.print("Error: decrypt \n");
  }
  */
  Serial.write(data_buf.plaintext, data_buf.size);
  delay(500);
  Serial.write(data_buf.ciphertext, data_buf.size);
  delay(500);
  Serial.write(data_buf.plaintext, data_buf.size);
}