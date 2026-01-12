const int wheel1_A_pin = 8;// Define motor1 drive pins
const int wheel1_B_pin = 9;// Define motor1 drive pins
const int wheel2_A_pin = 12;// Define motor2 drive pins
const int wheel2_B_pin = 13;// Define motor2 drive pins
const int wheel3_A_pin = 15;// Define motor3 drive pins
const int wheel3_B_pin = 14;// Define motor3 drive pins
const int wheel4_A_pin = 20;// Define motor4 drive pins
const int wheel4_B_pin = 21;// Define motor4 drive pins
const int button_pin = 3;
const int encoderPerCm = 2300 / 19;
const int fSpeed = 190;
const int slowDisF = 3000;
const int slowSpeed = 140;

const int encoderA1Pin = 6, encoderB1Pin = 7;
const int encoderA2Pin = 10, encoderB2Pin = 11;
const int encoderA3Pin = 16, encoderB3Pin = 17;
const int encoderA4Pin = 18, encoderB4Pin = 19;

// Position and state tracking
int encoderPos[4] = {0, 0, 0, 0};
int lastEncoded[4] = {0, 0, 0, 0};

// Function to handle each encoder
void GetEncoder(int encoderIndex, int encoderAPin, int encoderBPin) {
  int MSB = digitalRead(encoderAPin);
  int LSB = digitalRead(encoderBPin);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded[encoderIndex] << 2) | encoded;

  // Determine rotation direction
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPos[encoderIndex]++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPos[encoderIndex]--;

  lastEncoded[encoderIndex] = encoded;
}

// Interrupt Service Routines (ISRs) for each encoder
void Encoder1_ISR() { GetEncoder(0, encoderA1Pin, encoderB1Pin); }
void Encoder2_ISR() { GetEncoder(1, encoderA2Pin, encoderB2Pin); }
void Encoder3_ISR() { GetEncoder(2, encoderA3Pin, encoderB3Pin); }
void Encoder4_ISR() { GetEncoder(3, encoderA4Pin, encoderB4Pin); }

void setMotorSpeed(int motor, int speed) {
  int motorPinA;
  int motorPinB;
  switch(motor) {
  case 1:
    motorPinA = wheel1_A_pin;
    motorPinB = wheel1_B_pin;
    break;
  case 2:
    motorPinA = wheel2_A_pin;
    motorPinB = wheel2_B_pin;
    break;
  case 3:
    motorPinA = wheel3_A_pin;
    motorPinB = wheel3_B_pin;
    speed -= 0;
    break;
  case 4:
    motorPinA = wheel4_A_pin;
    motorPinB = wheel4_B_pin;
    speed -= 7;
    break;
}
  if (speed >= 0) {
    analogWrite(motorPinA,speed);
    analogWrite(motorPinB,0);
  }
  else {
    analogWrite(motorPinA,0);
    analogWrite(motorPinB,-speed);
  }
}

void stopMotors() {
  analogWrite(wheel1_A_pin,0);
  analogWrite(wheel1_B_pin,0);
  analogWrite(wheel2_A_pin,0);
  analogWrite(wheel2_B_pin,0);
  analogWrite(wheel3_A_pin,0);
  analogWrite(wheel3_B_pin,0);
  analogWrite(wheel4_A_pin,0);
  analogWrite(wheel4_B_pin,0);
}

void f(float distance) {
  encoderPos[1] = 0;
  lastEncoded[1] = 0;
  int counts = distance * encoderPerCm;
  int speed = fSpeed;
  setMotorSpeed(2, -speed);
  setMotorSpeed(4, speed);

  while (abs(encoderPos[1]) < counts) {
    setMotorSpeed(2, -speed);
    setMotorSpeed(4, speed);
    if (abs(encoderPos[1]) > counts - slowDisF) {
      speed = slowSpeed;
    } 
  }
  stopMotors();
  delay(500);
}

void b(float distance) {
  encoderPos[1] = 0;
  lastEncoded[1] = 0;
  int counts = distance * encoderPerCm;
  int speed = fSpeed;
  setMotorSpeed(2, speed);
  setMotorSpeed(4, -speed);

  while (abs(encoderPos[1]) < counts) {
    setMotorSpeed(2, speed);
    setMotorSpeed(4, -speed);
    /*if (abs(encoderPos[1]) > counts - slowDisF) {
      speed = slowSpeed;
    } */
  }
  stopMotors();
  delay(500);
}

void r(float distance) {
  encoderPos[0] = 0;
  lastEncoded[0] = 0;
  int counts = distance * encoderPerCm;
  int speed = fSpeed;
  setMotorSpeed(1, -speed);
  setMotorSpeed(3, speed);

  while (abs(encoderPos[0]) < counts) {
    setMotorSpeed(1, -speed);
    setMotorSpeed(3, speed);
    if (abs(encoderPos[0]) > counts - slowDisF) {
      speed = slowSpeed;
    } 
  }
  stopMotors();
  delay(500);
}

void l(float distance) {
  encoderPos[0] = 0;
  lastEncoded[0] = 0;
  int counts = distance * encoderPerCm;
  int speed = fSpeed;
  setMotorSpeed(1, speed);
  setMotorSpeed(3, -speed);

  while (abs(encoderPos[0]) < counts) {
    setMotorSpeed(1, speed);
    setMotorSpeed(3, -speed);
    if (abs(encoderPos[0]) > counts - slowDisF) {
      speed = slowSpeed;
    } 
  }
  stopMotors();
  delay(500);
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  // Set the motor PWM frequency to 16 kHz
  analogWriteFreq(16000);
  // Enable the motor drive pin to output mode
  pinMode(wheel1_A_pin,OUTPUT);
  pinMode(wheel1_B_pin,OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  // Set encoder pins as input
  pinMode(encoderA1Pin, INPUT);
  pinMode(encoderB1Pin, INPUT);
  pinMode(encoderA2Pin, INPUT);
  pinMode(encoderB2Pin, INPUT);
  pinMode(encoderA3Pin, INPUT);
  pinMode(encoderB3Pin, INPUT);
  pinMode(encoderA4Pin, INPUT);
  pinMode(encoderB4Pin, INPUT);
  // Attach interrupts to the A pins of the encoders
  attachInterrupt(digitalPinToInterrupt(encoderA1Pin), Encoder1_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB1Pin), Encoder1_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderA2Pin), Encoder2_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB2Pin), Encoder2_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderA3Pin), Encoder3_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB3Pin), Encoder3_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderA4Pin), Encoder4_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB4Pin), Encoder4_ISR, CHANGE);
}

void loop() {
  if (digitalRead(button_pin) == LOW){
    f(50);
    r(50);
    f(50);
    l(50);
    f(100);
    b(50);
  }
}