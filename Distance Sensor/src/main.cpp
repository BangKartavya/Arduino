#include <Arduino.h>

int TRIG = 14;
int ECHO = 27;

void setup() {
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  digitalWrite(12,LOW);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  
  int dur = pulseIn(ECHO,HIGH);
  double dist = (0.0343*dur)/2;
  
  Serial.println("Duration : "+String(dur) + "\tDistance : "+String(dist));
  
  delay(500);
}