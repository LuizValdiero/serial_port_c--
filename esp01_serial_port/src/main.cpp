#include <Arduino.h>
#include <time.h>
#include <string.h>


//unsigned char plaintext[] = "abcdefghijklmnop"; 
//unsigned char ciphertext[]= "qqrMsxlA7ksvbdHnSRNTozY="; 

#define MAX_BUFFER_SIZE 256

char data[MAX_BUFFER_SIZE];
int data_size;  

void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  //data_size = strlen("aabcc");
  //memcpy(data, "aabcc", data_size);
}
 
void loop() {
  delay(1000);
  while (1)
  {
    data_size = Serial.readBytes(data, MAX_BUFFER_SIZE);
    if (data_size > 0)
      break;
  };
  Serial.write(data, data_size);
}