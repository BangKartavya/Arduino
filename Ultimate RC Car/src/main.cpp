#include <Arduino.h>

int ch1_pin = 16; // channel 1
int ch2_pin = 27; // channel 2


// Left Motor Driver

int R_EN_left = 19;
int L_EN_left = 21;
int R_PWM_left = 14; // pwm pins
int L_PWM_left = 22; // pwm pins

// Right Motor Driver

int R_EN_right = 5;
int L_EN_right = 4;
int R_PWM_right = 15; // pwm pins
int L_PWM_right = 13; // pwm pins

// Ultrasonic Sensor Pins

int TRIG = 12;
int ECHO = 34;

// Setting threshold distance

double THRESHOLD = 30; 

// FWD
int Ch1Ch2_start_Fwd = 1550;
int Ch1Ch2_End_Fwd = 2030;
// BACK
int Ch1Ch2_start_Bac = 1450;
int Ch1Ch2_End_Bac = 980;


void setup() {
  Serial.begin(115200); // to monitor the outputs

  // Set Trig pin to output and Echo pin to input

  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);

  // To make sure that trig is low before operation

  digitalWrite(TRIG,LOW);

  pinMode(R_EN_left, OUTPUT);
  pinMode(R_EN_right, OUTPUT);
  pinMode(L_EN_right, OUTPUT);
  pinMode(L_EN_left, OUTPUT);

  // enable all the enable pins
  digitalWrite(R_EN_left, HIGH);
  digitalWrite(R_EN_right, HIGH);
  digitalWrite(L_EN_right, HIGH);
  digitalWrite(L_EN_left, HIGH);

  // set the pwm pins to output;
  pinMode(R_PWM_right, OUTPUT);
  pinMode(L_PWM_right, OUTPUT);

  pinMode(R_PWM_left, OUTPUT);
  pinMode(L_PWM_left, OUTPUT);
}

void loop() {
  // Getting values from receiver
  int ch1 = pulseIn(ch1_pin, HIGH);
  int ch2 = pulseIn(ch2_pin, HIGH);

  Serial.println("ch1 : ");
  Serial.println(ch1);
  Serial.println('\t');
  Serial.println("ch2 : ");
  Serial.println(ch2);
  Serial.println("");


  // setting the corresponding analog values
  int ch1_forward_speed =  map(ch1,Ch1Ch2_start_Fwd,Ch1Ch2_End_Fwd,0,255);
  int ch2_forward_speed =  map(ch2,Ch1Ch2_start_Fwd,Ch1Ch2_End_Fwd,0,255);
  
  int ch1_backward_speed =  map(ch1,Ch1Ch2_start_Bac,Ch1Ch2_End_Bac,0,255);
  int ch2_backward_speed =  map(ch2,Ch1Ch2_start_Bac,Ch1Ch2_End_Bac,0,255);
  
  if (ch1 == 0 || ch2 == 0) {
    analogWrite(R_PWM_right, 0);
    analogWrite(L_PWM_right, 0);
    analogWrite(R_PWM_left, 0);
    analogWrite(L_PWM_left, 0);
  }

  // Going Forward
  else if ((ch1 > Ch1Ch2_start_Fwd && ch2 > Ch1Ch2_start_Fwd)) {
    // spin both left and right motors clockwise

    analogWrite(R_PWM_right, ch2_forward_speed);
    analogWrite(R_PWM_left, ch1_forward_speed);

    analogWrite(L_PWM_right, 0);
    analogWrite(L_PWM_left, 0);
  }
  // Going Right

  else if (ch1 > Ch1Ch2_start_Fwd && ch2 < Ch1Ch2_start_Bac) {
    // spin right motor anti clock and left motor clock
    analogWrite(L_PWM_right, ch1_forward_speed);
    analogWrite(R_PWM_left, ch1_forward_speed);

    analogWrite(L_PWM_left, 0);
    analogWrite(R_PWM_right, 0);
  }
  // Going Left

  else if (ch1 < Ch1Ch2_start_Bac && ch2 > Ch1Ch2_start_Fwd) {
    // spin left motor anti clock and right motor clock;
    analogWrite(R_PWM_right, ch1_backward_speed);
    analogWrite(L_PWM_left, ch1_backward_speed);

    analogWrite(R_PWM_left, 0);
    analogWrite(L_PWM_right, 0);
  }
  
  // Going Back
  else if ((ch1 < Ch1Ch2_start_Bac && ch1 > Ch1Ch2_End_Bac) && ch2 < Ch1Ch2_start_Bac) {
    // spin both motors anti clock
    analogWrite(L_PWM_left, ch2_backward_speed);
    analogWrite(L_PWM_right, ch1_backward_speed);

    analogWrite(R_PWM_right, 0);
    analogWrite(R_PWM_left, 0);
  } 
  
  // Forward Right
  else if(ch1>Ch1Ch2_start_Fwd && (ch2 < 1.003*1500 && ch2 > 0.997*1500)) {
    analogWrite(R_PWM_left,ch1_forward_speed);
    analogWrite(R_PWM_right,ch1_forward_speed/2);

    analogWrite(L_PWM_right,0);
    analogWrite(L_PWM_left,0);
  }
  
  // Forward Left
  else if(ch2>Ch1Ch2_start_Fwd && (ch1 < 1.003*1500 && ch2 > 0.997*1500)) {
    analogWrite(R_PWM_right,ch1_forward_speed);
    analogWrite(R_PWM_left,ch1_forward_speed/2);

    analogWrite(L_PWM_right,0);
    analogWrite(L_PWM_left,0);
  }
  
  // Backward Right
  else if(ch2<Ch1Ch2_start_Bac && (ch1 < 1.003*1500 && ch1 > 0.997*1500)) {
    analogWrite(L_PWM_left,ch1_backward_speed);
    analogWrite(L_PWM_right,ch1_backward_speed/2);

    analogWrite(R_PWM_right,0);
    analogWrite(R_PWM_left,0);
  }
  
  // Backward Left
  else if((ch1 < Ch1Ch2_start_Bac && ch1 > Ch1Ch2_End_Bac) && (ch2 < 1.003*1500 && ch2 > 0.997*1500)) {
    analogWrite(L_PWM_right,ch1_backward_speed);
    analogWrite(L_PWM_left,ch1_backward_speed/2);

    analogWrite(R_PWM_right,0);
    analogWrite(R_PWM_left,0);
  }


  else if((ch1 < 1.003*900 && ch1 > 0.997*900) && (ch1 < 1.003*1500 && ch1 > 0.997*1500)) {
    // Stick is in the middle stop the car
    analogWrite(R_PWM_left,0);
    analogWrite(R_PWM_right,0);
    analogWrite(L_PWM_left,0);
    analogWrite(L_PWM_right,0);
  }

  // Entered Failsafe mode - Activate the Sensor
  else if(ch1 < 1.003*900 && ch1 > 0.997*900) {
    digitalWrite(TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG,LOW);

    analogWrite(R_PWM_left,0);
    analogWrite(R_PWM_right,0);
    analogWrite(L_PWM_left,0);
    analogWrite(L_PWM_right,0);
    
    int DUR = pulseIn(ECHO,HIGH);
    double DIST = (0.0343*DUR)/2;
    
    Serial.println("Pulse Duration : ");
    Serial.println('\t');
    Serial.println(DUR);
    Serial.println("Distance : ");
    Serial.println('\t');
    Serial.println(DIST);

    if(DIST==0.00) {
      // Sensor Not Functional Stop the car
      analogWrite(R_PWM_left,0);
      analogWrite(R_PWM_right,0);
      analogWrite(L_PWM_left,0);
      analogWrite(L_PWM_right,0);
    }

    else if(DIST>THRESHOLD) {
      // Move the car forward as the owner is far away
      analogWrite(R_PWM_right,100);
      analogWrite(R_PWM_left,100);
    }

    else if(DIST<THRESHOLD) {
      //Move the car back as it got too close
      analogWrite(L_PWM_left,100);
      analogWrite(L_PWM_right,100);
    }

    if(DIST > (0.95*THRESHOLD) && (DIST < 1.05*THRESHOLD)) {
      // The car is within reach, stop it
      analogWrite(R_PWM_right,0);
      analogWrite(R_PWM_left,0);
      analogWrite(L_PWM_right,0);
      analogWrite(L_PWM_left,0);
    }

  }

  // Failcase
  else {
    // stop the car
    analogWrite(R_PWM_right, 0);
    analogWrite(L_PWM_right, 0);
    analogWrite(R_PWM_left, 0);
    analogWrite(L_PWM_left, 0);

    // Unexpected thing happen
  }
}