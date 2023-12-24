#include <AccelStepper.h>
#include <MultiStepper.h>

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

class Positions {
 private:
    long positions[6];

 public:
    Positions() {
        for (int i = 0; i < 6; i++) {
            positions[i] = 0;
        }
    }
    void setPosition(long motor, long position) {
        positions[motor] = constrain(position, 0, 200);
    }

    void setPositions(long pos1, long pos2, long pos3, long pos4, long pos5, long pos6) {
        positions[0] = pos1;
        positions[1] = pos2;
        positions[2] = pos3;
        positions[3] = pos4;
        positions[4] = pos5;
        positions[5] = pos6;
    }

    void setPositionsAngles(long angle1, long angle2, long angle3, long angle4, long angle5, long angle6) {
        positions[0] = angle1 / 1.8;
        positions[1] = angle2 / 1.8;
        positions[2] = angle3 / 1.8;
        positions[3] = angle4 / 1.8;
        positions[4] = angle5 / 1.8;
        positions[5] = angle6 / 1.8;
    }

    long *getPositions() {
        return positions;
    }
};

void setup() {
    Serial.begin(9600);

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

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
}

void loop() {
    Positions positions;
    long angle0 = (long)map(analogRead(A0), 0, 1023, 0, 360);
    long angle1 = (long)map(analogRead(A1), 0, 1023, 0, 360);
    long angle2 = (long)map(analogRead(A2), 0, 1023, 0, 360);
    long angle3 = (long)map(analogRead(A3), 0, 1023, 0, 360);
    long angle4 = (long)map(analogRead(A4), 0, 1023, 0, 360);
    long angle5 = (long)map(analogRead(A5), 0, 1023, 0, 360);
    positions.setPositionsAngles(angle0, angle1, angle2, angle3, angle4, angle5);
    steppers.moveTo(positions.getPositions());
    steppers.run();
}