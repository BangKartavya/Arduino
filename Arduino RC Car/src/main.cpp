#include <Arduino.h>

// Channel pins

int CH1 = 3;
int CH2 = 5;

// Left Motor Driver

int L_PWM_LEFT = 10;
int R_PWM_LEFT = 11;

// Right Motor Driver

int L_PWM_RIGHT = 6;
int R_PWM_RIGHT = 9;

// Distance Sensor

int TRIG = 2;
int ECHO = A0;

// FWD
int Ch1Ch2_start_Fwd = 1550;
int Ch1Ch2_End_Fwd = 2030;
// BACK
int Ch1Ch2_start_Bac = 1450;
int Ch1Ch2_End_Bac = 980;

// Distance Threshold and Error

int THRESHOLD = 20;
int ERROR = 5;

void forward(int ch1_forward_speed, int ch2_forward_speed) {
  // spin both left and right motors clockwise

  analogWrite(R_PWM_RIGHT, ch2_forward_speed);
  analogWrite(R_PWM_LEFT, ch1_forward_speed);

  analogWrite(L_PWM_RIGHT, 0);
  analogWrite(L_PWM_LEFT, 0);
}

void backward(int ch1_backward_speed, int ch2_backward_speed) {
  // spin both motors anti clock
  analogWrite(L_PWM_LEFT, ch2_backward_speed);
  analogWrite(L_PWM_RIGHT, ch1_backward_speed);

  analogWrite(R_PWM_RIGHT, 0);
  analogWrite(R_PWM_LEFT, 0);
}

void left(int ch1_backward_speed) {
  // spin LEFT motor anti clock and RIGHT motor clock;
  analogWrite(R_PWM_RIGHT, ch1_backward_speed);
  analogWrite(L_PWM_LEFT, ch1_backward_speed);

  analogWrite(R_PWM_LEFT, 0);
  analogWrite(L_PWM_RIGHT, 0);
}

void right(int ch1_forward_speed) {
  // spin RIGHT motor anti clock and LEFT motor clock
  analogWrite(L_PWM_RIGHT, ch1_forward_speed);
  analogWrite(R_PWM_LEFT, ch1_forward_speed);

  analogWrite(L_PWM_LEFT, 0);
  analogWrite(R_PWM_RIGHT, 0);
}

void forward_right(int ch1_forward_speed) {
  // Spin both motors clockwise with LEFT motor being faster than RIGHT
  analogWrite(R_PWM_LEFT, ch1_forward_speed);
  analogWrite(R_PWM_RIGHT, ch1_forward_speed / 2);

  analogWrite(L_PWM_RIGHT, 0);
  analogWrite(L_PWM_LEFT, 0);
}

void forward_left(int ch1_forward_speed) {
  // Spin both motors clockwise with LEFT motor being slower than RIGHT
  analogWrite(R_PWM_RIGHT, ch1_forward_speed);
  analogWrite(R_PWM_LEFT, ch1_forward_speed / 2);

  analogWrite(L_PWM_RIGHT, 0);
  analogWrite(L_PWM_LEFT, 0);
}

void backward_right(int ch1_backward_speed) {
  // Spin both motors anticlockwise with LEFT motor being faster than RIGHT
  analogWrite(L_PWM_LEFT, ch1_backward_speed);
  analogWrite(L_PWM_RIGHT, ch1_backward_speed / 2);

  analogWrite(R_PWM_RIGHT, 0);
  analogWrite(R_PWM_LEFT, 0);
}

void backward_left(int ch1_backward_speed) {
  // Spin both motors anticlockwise with LEFT motor being slower than RIGHT
  analogWrite(L_PWM_RIGHT, ch1_backward_speed);
  analogWrite(L_PWM_LEFT, ch1_backward_speed / 2);

  analogWrite(R_PWM_RIGHT, 0);
  analogWrite(R_PWM_LEFT, 0);
}

void car_stop() {
  // stop any motion
  analogWrite(R_PWM_LEFT, 0);
  analogWrite(R_PWM_RIGHT, 0);
  analogWrite(L_PWM_LEFT, 0);
  analogWrite(L_PWM_RIGHT, 0);
}

double ultrasonicRead(int TRIG,int ECHO) {
  // make the trigger pin high for 10 micro seconds
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // make the echo pin high and calculate the pulse duration
  int DUR = pulseIn(ECHO, HIGH);
  double DIST = (0.0343 * DUR) / 2; // calculate the distance
  return DIST;
}

void setup() {

  pinMode(CH1,INPUT);
  pinMode(CH2,INPUT);

  pinMode(L_PWM_LEFT,OUTPUT);
  pinMode(R_PWM_LEFT,OUTPUT);
  
  pinMode(L_PWM_RIGHT,OUTPUT);
  pinMode(R_PWM_RIGHT,OUTPUT);

  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

  digitalWrite(TRIG,LOW);
}


void loop() {
  int ch1 = pulseIn(CH1,HIGH);
  int ch2 = pulseIn(CH2,HIGH);

  // forward speeds

  int ch1_forward_speed = map(ch1,Ch1Ch2_End_Fwd,Ch1Ch2_End_Fwd,0,255);
  int ch2_forward_speed = map(ch2,Ch1Ch2_End_Fwd,Ch1Ch2_End_Fwd,0,255);

  // backward speeds

  int ch1_backward_speed = map(ch1,Ch1Ch2_start_Bac,Ch1Ch2_End_Bac,0,255);
  int ch2_backward_speed = map(ch2,Ch1Ch2_start_Bac,Ch1Ch2_End_Bac,0,255);

  if (ch1 == 0 || ch2 == 0) {
    // one or both channels disconnected so stop the car
    car_stop();
  }

  // Going Forward
  else if ((ch1 > Ch1Ch2_start_Fwd && ch2 > Ch1Ch2_start_Fwd)) {
    forward(ch1_forward_speed,ch2_forward_speed);
  }
  // Going Right

  else if (ch1 > Ch1Ch2_start_Fwd && ch2 < Ch1Ch2_start_Bac) {
    right(ch1_forward_speed);
  }
  // Going Left

  else if (ch1 < Ch1Ch2_start_Bac && ch2 > Ch1Ch2_start_Fwd) {
    left(ch1_backward_speed);
  }

  // Going Back
  else if ((ch1 < Ch1Ch2_start_Bac && ch1 > Ch1Ch2_End_Bac) && ch2 < Ch1Ch2_start_Bac) {
    backward(ch1_backward_speed,ch2_backward_speed);
  }

  // Forward Right
  else if (ch1 > Ch1Ch2_start_Fwd && (ch2 < 1.003 * 1500 && ch2 > 0.997 * 1500)) {
    forward_right(ch1_forward_speed);
  }

  // Forward Left
  else if (ch2 > Ch1Ch2_start_Fwd && (ch1 < 1.003 * 1500 && ch2 > 0.997 * 1500)) {
    forward_left(ch1_forward_speed);
  }

  // Backward Right
  else if (ch2 < Ch1Ch2_start_Bac && (ch1 < 1.003 * 1500 && ch1 > 0.997 * 1500)) {
    backward_right(ch1_backward_speed);
  }

  // Backward Left
  else if ((ch1 < Ch1Ch2_start_Bac && ch1 > Ch1Ch2_End_Bac) && (ch2 < 1.003 * 1500 && ch2 > 0.997 * 1500)) {
    backward_left(ch1_backward_speed);
  }
  
  // Stick in middle
  else if ((ch1 < 1.003 * 900 && ch1 > 0.997 * 900) && (ch1 < 1.003 * 1500 && ch1 > 0.997 * 1500)) {
    car_stop();
  }

  else if (ch1 < 1.003 * 900 && ch1 > 0.997 * 900) {
    // first stop the car to measure distance
    car_stop();

    double DIST = ultrasonicRead(TRIG,ECHO); // Call the ultrasonicRead function to get the distance from the left sensor

    Serial.println("Distance : " + String(DIST));

    if(DIST == 0.00) car_stop();
    else if(DIST > THRESHOLD - ERROR && DIST < THRESHOLD + ERROR) car_stop();
    else if(DIST > THRESHOLD + ERROR) backward(25,25);
    else if(DIST < THRESHOLD - ERROR) forward(25,25);
    else car_stop();
 
  }
  
  else {
    // unexpected thing happened
    car_stop();
  }

}
