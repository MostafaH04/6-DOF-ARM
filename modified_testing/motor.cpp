#include "motor.hpp"

StepperMotor::StepperMotor(int step_pin, int dir_pin, MICROSTEPS microsteps):
  step(step_pin), dir(dir_pin)
{
  revoluteSteps = REVOLUTE_STEPS * microsteps;
  targetSpeed = 0;
  currentStep = 0;
  currentAngle = 0;

  stepAngle = 1.0 / (double)revoluteSteps;
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