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
  attachInterrupt(b1,isr,FALLING);
}

void loop() {
  if(digitalRead(33)) {
    digitalWrite(2,HIGH);
    Serial.printf("Button has been pressed %u times",num);
    Serial.println("");
    flag = false;
  }
  else digitalWrite(2,LOW);
}