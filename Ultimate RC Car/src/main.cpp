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
int TRIG = 25;
int ECHO = 34;
// Setting threshold distance

double THRESHOLD = 30; // Setting threshold distance to 30 cm, In future using a potentiometer to change the distance
double ERROR = 10; // setting acceptable error

// FWD
int Ch1Ch2_start_Fwd = 1550;
int Ch1Ch2_End_Fwd = 2030;
// BACK
int Ch1Ch2_start_Bac = 1450;
int Ch1Ch2_End_Bac = 980;

void forward(int ch1_forward_speed, int ch2_forward_speed) {
  // spin both left and right motors clockwise

  analogWrite(R_PWM_right, ch2_forward_speed);
  analogWrite(R_PWM_left, ch1_forward_speed);

  analogWrite(L_PWM_right, 0);
  analogWrite(L_PWM_left, 0);
}

void backward(int ch1_backward_speed, int ch2_backward_speed) {
  // spin both motors anti clock
  analogWrite(L_PWM_left, ch2_backward_speed);
  analogWrite(L_PWM_right, ch1_backward_speed);

  analogWrite(R_PWM_right, 0);
  analogWrite(R_PWM_left, 0);
}

void left(int ch1_backward_speed) {
  // spin left motor anti clock and right motor clock;
  analogWrite(R_PWM_right, ch1_backward_speed);
  analogWrite(L_PWM_left, ch1_backward_speed);

  analogWrite(R_PWM_left, 0);
  analogWrite(L_PWM_right, 0);
}

void right(int ch1_forward_speed) {
  // spin right motor anti clock and left motor clock
  analogWrite(L_PWM_right, ch1_forward_speed);
  analogWrite(R_PWM_left, ch1_forward_speed);

  analogWrite(L_PWM_left, 0);
  analogWrite(R_PWM_right, 0);
}

void forward_right(int ch1_forward_speed) {
  // Spin both motors clockwise with left motor being faster than right
  analogWrite(R_PWM_left, ch1_forward_speed);
  analogWrite(R_PWM_right, ch1_forward_speed / 2);

  analogWrite(L_PWM_right, 0);
  analogWrite(L_PWM_left, 0);
}

void forward_left(int ch1_forward_speed) {
  // Spin both motors clockwise with left motor being slower than right
  analogWrite(R_PWM_right, ch1_forward_speed);
  analogWrite(R_PWM_left, ch1_forward_speed / 2);

  analogWrite(L_PWM_right, 0);
  analogWrite(L_PWM_left, 0);
}

void backward_right(int ch1_backward_speed) {
  // Spin both motors anticlockwise with left motor being faster than right
  analogWrite(L_PWM_left, ch1_backward_speed);
  analogWrite(L_PWM_right, ch1_backward_speed / 2);

  analogWrite(R_PWM_right, 0);
  analogWrite(R_PWM_left, 0);
}

void backward_left(int ch1_backward_speed) {
  // Spin both motors anticlockwise with left motor being slower than right
  analogWrite(L_PWM_right, ch1_backward_speed);
  analogWrite(L_PWM_left, ch1_backward_speed / 2);

  analogWrite(R_PWM_right, 0);
  analogWrite(R_PWM_left, 0);
}

void car_stop() {
  // stop any motion
  analogWrite(R_PWM_left, 0);
  analogWrite(R_PWM_right, 0);
  analogWrite(L_PWM_left, 0);
  analogWrite(L_PWM_right, 0);
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
  Serial.begin(115200); // to monitor the outputs

  // Set Trig pin to output and Echo pin to input

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);


  // To make sure that trig is low before operation

  digitalWrite(TRIG, LOW);

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

  // setting the corresponding analog values
  int ch1_forward_speed = map(ch1, Ch1Ch2_start_Fwd, Ch1Ch2_End_Fwd, 0,255); // map the speed to analog values for the motor
  int ch2_forward_speed = map(ch2, Ch1Ch2_start_Fwd, Ch1Ch2_End_Fwd, 0,255); // map the speed to analog values for the motor

  int ch1_backward_speed = map(ch1, Ch1Ch2_start_Bac, Ch1Ch2_End_Bac, 0,255); // map the speed to analog values for the motor
  int ch2_backward_speed = map(ch2, Ch1Ch2_start_Bac, Ch1Ch2_End_Bac, 0,255); // map the speed to analog values for the motor

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

  // Entered Failsafe mode - Activate the Sensor
  else if (ch1 < 1.003 * 900 && ch1 > 0.997 * 900) {
    // first stop the car to measure distance
    car_stop();

    double DIST = ultrasonicRead(TRIG,ECHO); // Call the ultrasonicRead function to get the distance from the left sensor

    if(DIST == 0.00) car_stop();
    else if(DIST > THRESHOLD - ERROR && DIST < THRESHOLD + ERROR) car_stop();
    else if(DIST > THRESHOLD + ERROR) backward(25,25);
    else if(DIST < THRESHOLD - ERROR) forward(25,25);
    else car_stop();
 
  }

  // Failcase
  else {
    // stop the car
    car_stop();
    // Unexpected thing happen
  }
}