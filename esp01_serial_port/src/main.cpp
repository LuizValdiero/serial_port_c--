#include <Arduino.h>
#include <time.h>
#include <string.h>

#include <Crypto.h>
#include <AES.h>
#include <CTR.h>


// Cipher ctraes128
AES128 aes128;
CTR<AES128> ctraes128;

struct aes_ctr_key_t {
  byte key[16];
  byte iv[16];
};

struct data_t
{
  byte * plaintext;
  byte * ciphertext;
  int size;
};


static aes_ctr_key_t const aes_ctr_key {
  .key = {0x99, 0xF3, 0xCC, 0xA3, 0xFC, 0xC7, 0x10, 0x76, 0xAC, 0x16,
          0x86, 0x41, 0xD9, 0x06, 0xCE, 0xB5},
  .iv = {0x65, 0x04, 0xEF, 0x3F, 0x0D, 0xBF, 0xBE, 0x2A, 0xDD, 0x1D,
          0x1D, 0x39, 0x60, 0xC3, 0x39, 0x73}
};


int encrypt( Cipher * cipher,
  const struct aes_ctr_key_t * key,
  const struct data_t * data) 
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

  memset(data->ciphertext, 0x00, data->size);
  cipher->encrypt(data->ciphertext, data->plaintext, data->size);


  return 0;
}


int decrypt( Cipher * cipher,
  const struct aes_ctr_key_t * key,
  const struct data_t * data) 
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

  //memset(data->ciphertext, 0x00, data->size);
  cipher->decrypt(data->plaintext, data->ciphertext, data->size);


  return 0;
}

// "esp01 " + time + "\n\r"
char data[30];

data_t data_buf;

void setup() {
  Serial.begin(115200);

  Serial.println("State Sizes:");
  Serial.print("AES128 ... ");
  Serial.println(sizeof(aes128));
  Serial.println(sizeof(ctraes128));
  

}

void loop() {
  delay(10000);
  
  //qual sua importancia ??? estudar
  //crypto_feed_watchdog();
  long unsigned time = millis();
  
  char ddata[]= "test openssl - ok"; 
  memcpy(data, ddata, 17);

  /*
  String val = (String)time;
  Serial.print("time:");
  val.toCharArray(&data[6], val.length()+1);
  Serial.println(val);
  Serial.println(sizeof(val));
  Serial.println(val.length());
  Serial.println();
  */
  
  //data_buf.size = 6 + val.length();
  data_buf.size = 17;
  data_buf.plaintext = (byte*) (void *) data;
  char cipherdata[data_buf.size];
  data_buf.ciphertext = (byte*) (void *) cipherdata;

  if (encrypt(&ctraes128, &aes_ctr_key, &data_buf)) {
    Serial.print("Error: encrypt \n");
  }

  Serial.println("\n plaintext1");
  Serial.write(data_buf.plaintext, data_buf.size);
  Serial.println("\n ciphertext");
  Serial.write(data_buf.ciphertext, data_buf.size);
  Serial.print("\n\r");
  for (int i = 0; i < data_buf.size; i++){
    Serial.print(data_buf.ciphertext[i], HEX);
  }
  Serial.println("\n exit print buffer");
  if (decrypt(&ctraes128, &aes_ctr_key, &data_buf)) {
    Serial.print("Error: decrypt \n");
  }
  Serial.println("\n plaintext2");
  Serial.write(data_buf.plaintext, data_buf.size);
  Serial.println("");
}