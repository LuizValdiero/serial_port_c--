#include <Arduino.h>
#include <time.h>
#include <string.h>


//unsigned char plaintext[] = "abcdefghijklmnop"; 
//unsigned char ciphertext[]= "qqrMsxlA7ksvbdHnSRNTozY="; 
int plaintext_size;  

void setup() {
  delay(1000);
  Serial.begin(115200);
  plaintext_size = strlen("abcdefghijklmnop");   
}
 
void loop() {
  delay(10000);
  Serial.write("aa", strlen("aa"));
  delay(10000);
  Serial.write("aaaa", strlen("aaaa"));
  delay(10000);
  Serial.write("abcdefghijklmnop", plaintext_size);
  delay(10000);
  Serial.write("��ζv", strlen("��ζv"));
}