#include <Arduino.h>

int b1 = 33;
bool flag = false;
long long int num = 0;

void IRAM_ATTR isr() {
  flag = true;
  num++;
}

void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(b1,INPUT_PULLUP);
  attachInterrupt(b1,isr,RISING);
}

void loop() {
  if(!digitalRead(b1)) {
    digitalWrite(2,HIGH);
    flag = false;
  }
  else digitalWrite(2,LOW);
}