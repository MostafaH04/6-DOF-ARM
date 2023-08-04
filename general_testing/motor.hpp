#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "config.hpp"
#include "Arduino.h"

// TODO: determine expirmentally
#define MIN_PULSE 20

#define REVOLUTE_STEPS 200

#define RAD2DEG (180 / M_PI)
#define DEG2RAD (M_PI / 180)

enum MICROSTEPS
{
  STEPPER_8_MICROSTEP = 8,
  STEPPER_16_MICROSTEP = 16,
  STEPPER_32_MICROSTEP = 32,
  STEPPER_64_MICROSTEP = 64
};

enum stepper_control
{
  STEPPER_POSITION_DEG,
  STEPPER_POSITION_RAD,
  STEPPER_POSITION_STEP,
  STEPPER_SPEED  
};

class StepperMotor
{
  private:
    unsigned int step, dir;
    int revoluteSteps;

    double stepAngle;

    float targetSpeed;
    float currentSpeed;
    int currentStep;
    float currentAngle;

    stepper_control control_state;

    SemaphoreHandle_t stepperMutex;
    
    void stepMotor(void);

    int pwmChannel;
    int pwmFrequency; // in Hz
    int pwmDutyCycle; // percentage (0-100)

    void initPWM(void);

  public:
    StepperMotor(int step_pin, int dir_pin, MICROSTEPS microsteps);

    void init(void);
    float getAngle(void) const;
    float getAngleDeg(void) const;
    void controlMethod(stepper_control control_state);
    void driveSpeed(float radPerSec);
    float getRPM(void) const;
    void run(void);
    void setPWMFrequency(int frequency);
    void setPWMDutyCycle(int dutyCycle);
};

#endif