#include "motor.hpp"

StepperMotor::StepperMotor(int step_pin, int dir_pin, int pwm_channel, MICROSTEPS microsteps):
  step(step_pin), dir(dir_pin)
{
  revoluteSteps = REVOLUTE_STEPS * microsteps;
  targetSpeed = 0;
  currentStep = 0;
  currentAngle = 0;

  stepAngle = 1.0 / (double)revoluteSteps;
  
  pwmChannel = pwm_channel;
  pwmFrequency = 20000; // default to 20kHz
  pwmDutyCycle = 50;    // default to 50% duty cycle

  initPWM();
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

  setPWMDutyCycle(abs(targetSpeed) * 100); // set PWM duty cycle, 100% of target speed
  
  double time_s = 1 / (currentSpeed / stepAngle);
  uint32_t delayTime = (time_s*1000000);
  delayMicroseconds(delayTime);
}

void StepperMotor::initPWM() {
  ledcSetup(pwmChannel, pwmFrequency, 8); // 8-bit resolution (0-255)
  ledcAttachPin(step, pwmChannel);
  ledcWrite(pwmChannel, 128); // 50% duty cycle initially
}

void StepperMotor::setPWMDutyCycle(int dutyCycle) {
  pwmDutyCycle = constrain(dutyCycle, 0, 100);
  int pwmValue = map(pwmDutyCycle, 0, 100, 0, 255);
  ledcWrite(pwmChannel, pwmValue);
}

void StepperMotor::setPWMFrequency(int frequency) {
  pwmFrequency = frequency;
  ledcSetup(pwmChannel, pwmFrequency, 8); // 8-bit resolution (0-255)
}