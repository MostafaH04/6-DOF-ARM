#include "motor.hpp"

StepperMotor::StepperMotor(int step_pin, int dir_pin, MotorHeap *heap_in, MICROSTEPS microsteps):
  step(step_pin), dir(dir_pin), heap(heap_in)
{
  revoluteSteps = REVOLUTE_STEPS * microsteps;
  targetSpeed = 0;
  currentStep = 0;
  currentAngle = 0;

  stepAngle = 2.0 * M_PI / (double)revoluteSteps;
  heapNode.motor = this;
}

void StepperMotor::init(void)
{
  pinMode(step, OUTPUT);
  pinMode(dir, OUTPUT);
}

float StepperMotor::getAngle(void) const
{
  int currentRot = currentStep % revoluteSteps;
  return currentRot * stepAngle;
}

float StepperMotor::getAngleDeg(void) const
{
  return getAngle() * RAD2DEG;
}

void StepperMotor::controlMethod(stepper_control constrol_state)
{
  (*this).control_state = control_state;
}

void StepperMotor::driveSpeed(float radPerSec)
{
  targetSpeed = radPerSec;
}

float StepperMotor::getRPM(void) const
{
  return currentSpeed / (2*M_PI);
}

void StepperMotor::stepMotor(void)
{
  if (targetSpeed == 0)
    return;
  else if (targetSpeed > 0)
    digitalWrite(dir, HIGH);
  else
    digitalWrite(dir, LOW);

  digitalWrite(step, HIGH);
  delayMicroseconds(MIN_PULSE);
  digitalWrite(step, LOW);
  currentStep ++;
}

void StepperMotor::run(void)
{
  currentSpeed = targetSpeed; 
  stepMotor();
  
  double time_s = 1 / (currentSpeed / stepAngle);
  uint32_t delayTime = (time_s*1000000);
  delayMicroseconds(delayTime);
}

void StepperMotor::runHeap(void)
{
  currentSpeed = targetSpeed;
  uint32_t interruptDelay{};

  if (!digitalRead(step)){ // new cycle: step pin is low. calculate target speed
    double time_s = (1/(currentSpeed / stepAngle)); // sec/step
    lastStepTime = (time_s*1000000);
    interruptDelay = MIN_PULSE / INTERRUPT_INTERVAL_US;
    if (targetSpeed == 0)
      return;
    else if (targetSpeed > 0)
      digitalWrite(dir, HIGH);
    else
      digitalWrite(dir, LOW);
    digitalWrite(step, HIGH);
  }
  else{
    interruptDelay = (lastStepTime - MIN_PULSE) / INTERRUPT_INTERVAL_US;
    digitalWrite(step, LOW);
  }

  heapNode.count = interruptDelay;
  heap->enqueue(&heapNode);
}

MotorHeap::MotorHeap():
  size(0)
{}

int MotorHeap::getSize()
{
  return size;
}

bool MotorHeap::isEmpty()
{
  return size == 0;
}

void MotorHeap::print()
{
  for (int i = 0; i < size; i++)
    Serial.print(heap[i]->count);
  Serial.println();
}
