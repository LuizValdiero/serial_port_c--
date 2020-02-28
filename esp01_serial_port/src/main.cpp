#include <Arduino.h>
#include <time.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  delay(10000);
  // put your main code here, to run repeatedly:
  char data[] = "esp-01\n\r";
  Serial.write(data);

}