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
    int parent(int in) const;
    int child(int in, bool right) const;
    void heapUp(int in);
    void heapDown(int in);
    void swap(int in_1, int in_2);

  public:
    MotorHeap();
 
    int getSize(void) const;
    bool isEmpty(void) const;
    void print(void) const; 
    uint32_t top(void) const;

    void decrement(void);

    HeapNode *dequeue(void);
    void enqueue(HeapNode *node);
};

class MultiStepperDrive 
{
  private:
    int timer1_compare_match;
    StepperMotor *motors[6];

  public:
    MultiStepperDrive(StepperMotor *motor[6]);

    void init(void);

    void driveSpeed(float radPerSec[6]);
    void driveSpeed(float radPerSec);
};

#endif