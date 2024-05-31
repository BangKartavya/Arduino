#include <Arduino.h>

int LED = 13;
hw_timer_t *my_timer = NULL;

void IRAM_ATTR onTimer() {
  digitalWrite(LED,!digitalRead(LED));
  Serial.println(millis());
}


void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  my_timer = timerBegin(0,80,true);
  timerAttachInterrupt(my_timer,&onTimer,true);
  timerAlarmWrite(my_timer,1000000,true);
  timerAlarmEnable(my_timer);
}

void loop() {}