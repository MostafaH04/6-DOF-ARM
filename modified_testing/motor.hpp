#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "common.hpp"
#include "Arduino.h"

// TODO: determine expirmentally
#define MIN_PULSE 20
#define INTERRUPT_INTERVAL_US 2
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
    
    void stepMotor(void);
    uint32_t lastStepTime;

    MotorHeap *heap;
    HeapNode heapNode;

  public:
    StepperMotor(int step_pin, int dir_pin, MotorHeap *heap_in, MICROSTEPS microsteps);

    void init(void);

    float getAngle(void) const;
    float getAngleDeg(void) const;

    void controlMethod(stepper_control control_state);

    void driveSpeed(float radPerSec);

    float getRPM(void) const;

    void run(void);
    void runHeap(void);
};

struct HeapNode {
    uint32_t count;
    StepperMotor *motor;
};

class MotorHeap
{
  private:
    HeapNode *heap[7];
    int size;
    void heapify(int i);
    void swap(int i, int j);

  public:
    MotorHeap();
 
    int getSize(void) const;
    bool isEmpty(void) const;
    void print(void) const; 

    void dequeue(void);
    void enqueue(HeapNode *node);
};

class MultiStepperDrive 
{
  private:
    void drive(void);
    int num_motors;
    StepperMotor **motors;

  public:
    MultiStepperDrive(StepperMotor *motor, int size);

    void init(void);

    void driveSpeed(float *radPerSec, int size);
    void driveSpeed(float radPerSec);
};

#endif