#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial serialBT;
int L_PWM_LEFT ;
int R_PWM_LEFT ;

int L_PWM_RIGHT ;
int R_PWM_RIGHT ;

char dir;

void setup() {

  serialBT.begin("Honda");
  dir = 'S';
  Serial.begin(250000);

  pinMode(L_PWM_LEFT,OUTPUT);
  pinMode(L_PWM_RIGHT,OUTPUT);

  pinMode(R_PWM_LEFT,OUTPUT);
  pinMode(R_PWM_RIGHT,OUTPUT);
}

void fwd() {
  digitalWrite(R_PWM_LEFT,HIGH);
  digitalWrite(R_PWM_RIGHT,HIGH);
  
  digitalWrite(L_PWM_LEFT,LOW);
  digitalWrite(L_PWM_RIGHT,LOW);
}

void bck() {
  digitalWrite(R_PWM_LEFT,LOW);
  digitalWrite(R_PWM_RIGHT,LOW);
  
  digitalWrite(L_PWM_LEFT,HIGH);
  digitalWrite(L_PWM_RIGHT,HIGH);
}

void left() {
  digitalWrite(R_PWM_LEFT,LOW);
  digitalWrite(R_PWM_RIGHT,HIGH);
  
  digitalWrite(L_PWM_LEFT,HIGH);
  digitalWrite(L_PWM_RIGHT,LOW);
}

void right() {
  digitalWrite(R_PWM_LEFT,LOW);
  digitalWrite(R_PWM_RIGHT,HIGH);
  
  digitalWrite(L_PWM_LEFT,LOW);
  digitalWrite(L_PWM_RIGHT,HIGH);
}

void stp() {
  digitalWrite(R_PWM_LEFT,LOW);
  digitalWrite(R_PWM_RIGHT,LOW);
  
  digitalWrite(L_PWM_LEFT,LOW);
  digitalWrite(L_PWM_RIGHT,LOW);
}

void loop() {
  while(!serialBT.hasClient()) {
    Serial.println("No Connection");
    stp();
  }

  if(serialBT.available()) {
    dir = serialBT.read();
    
    switch(dir) {
      case 'S':
        stp();
        break;
        Serial.println("STOP!!!!!");
      case 'B':
        bck();
        Serial.println("Back");
        break;
      case 'F':
        fwd();
        Serial.println("Forward");
        break;
      case 'L':
        left();
        Serial.println("Left");
        break;
      case 'R':
        right();
        Serial.println("Right");
        break;
    }
    
  }

}