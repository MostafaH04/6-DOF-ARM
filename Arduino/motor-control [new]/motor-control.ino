#include <AccelStepper.h>
#include <MultiStepper.h>
#include "positions.h"

#define STEPPER1_DIR_PIN 2
#define STEPPER1_STEP_PIN 3

#define STEPPER2_DIR_PIN 4
#define STEPPER2_STEP_PIN 5

#define STEPPER3_DIR_PIN 6
#define STEPPER3_STEP_PIN 7

#define STEPPER4_DIR_PIN 8
#define STEPPER4_STEP_PIN 9

#define STEPPER5_DIR_PIN 10
#define STEPPER5_STEP_PIN 11
    
#define STEPPER6_DIR_PIN 12
#define STEPPER6_STEP_PIN 13

AccelStepper stepper1(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);
AccelStepper stepper3(AccelStepper::DRIVER, STEPPER3_STEP_PIN, STEPPER3_DIR_PIN);
AccelStepper stepper4(AccelStepper::DRIVER, STEPPER4_STEP_PIN, STEPPER4_DIR_PIN);
AccelStepper stepper5(AccelStepper::DRIVER, STEPPER5_STEP_PIN, STEPPER5_DIR_PIN);
AccelStepper stepper6(AccelStepper::DRIVER, STEPPER6_STEP_PIN, STEPPER6_DIR_PIN);

MultiStepper steppers;

long angle_1;
long angle_2;
long angle_3;
long angle_4;
long angle_5;
long angle_6;

void setup() {
  Serial.begin(115200);

  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(100);
  stepper3.setMaxSpeed(100);
  stepper4.setMaxSpeed(100);
  stepper5.setMaxSpeed(100);
  stepper6.setMaxSpeed(100);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
  steppers.addStepper(stepper5);
  steppers.addStepper(stepper6);
}

String readUART() {
  while(Serial.available() == 0){
    pinMode(13, OUTPUT);
  }
  String jointAngles = Serial.readStringUntil('\r');
  return jointAngles;
}

void split(String cmd) {
  char char_array[cmd.length() + 1];
  cmd.toCharArray(char_array, cmd.length() + 1);

  char *token = strtok(char_array, " ");

  if (token != NULL) {
    angle_1 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_2 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_3 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_4 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_5 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_6 = atof(token);
  }
}


void display() {
  // Blink pin 13 angle_1 times
  for (int i = 0; i < angle_1; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(5000);
  
  // Blink pin 13 angle_2 times
  for (int i = 0; i < angle_2; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(5000);
  
  // Blink pin 13 angle_3 times
  for (int i = 0; i < angle_3; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);

  // Blink pin 13 angle_4 times
  for (int i = 0; i < angle_4; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);

  // Blink pin 13 angle_5 times
  for (int i = 0; i < angle_5; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);

  // Blink pin 13 angle_6 times
  for (int i = 0; i < angle_6; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);
}

void driveSteppers(){
  Positions positions;
  long motorAngle1 = (long)map(angle_1, 0, 1023, 0, 360);
  long motorAngle2 = (long)map(angle_2, 0, 1023, 0, 360);
  long motorAngle3 = (long)map(angle_3, 0, 1023, 0, 360);
  long motorAngle4 = (long)map(angle_4, 0, 1023, 0, 360);
  long motorAngle5 = (long)map(angle_5, 0, 1023, 0, 360);
  long motorAngle6 = (long)map(angle_6, 0, 1023, 0, 360);
  positions.setPositionsAngles(motorAngle1, motorAngle2, motorAngle3, motorAngle4, motorAngle5, motorAngle6);
  steppers.moveTo(positions.getPositions());
  steppers.run();
}


void loop() {
  String jointAngles = readUART();
  split(jointAngles);
  display();
  driveSteppers();
}