#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "motor.hpp"

// enable pin 
#define ENABLE_PIN 12

// pwm channel 0 -> pin 6 (OC0A)
// pwm channel 1 -> pin 6 (OC0B)
// pwm channel 2 -> pin 6 (OC0B)
#define PWM_CHANNEL 0

// motor 1
#define STEP_1 11
#define DIR_1 10

// motor 2
#define STEP_2 9
#define DIR_2 8

// motor 3
#define STEP_3 7
#define DIR_3 6

// motor 4
#define STEP_4 5
#define DIR_4 4

// motor 5
#define STEP_5 3
#define DIR_5 2

// motor 6
#define STEP_6 14
#define DIR_6 15


StepperMotor motor_1(STEP_1, DIR_1, PWM_CHANNEL_1, STEPPER_8_MICROSTEP);

void setup (void)
{
  Serial.begin(9600);

  motor_1.init();
  motor_1.setPWMFrequency(20000); // 20kHz initially, redundant since default is 20kHz
  motor_1.setPWMDutyCycle(50); // 50% speed initially, redundant since default is 50%

  pinMode(ENABLE_PIN, OUTPUT);

  xTaskCreate(main_task, "Main Task", 200, NULL, 3, NULL);
}

static void main_task(void * pvParameters)
{
  float i = 2000;
  digitalWrite(ENABLE_PIN, HIGH);
  for (;;)
  {
    motor_1.driveSpeed(i);
    motor_1.run();
    i += 0.1;
    if (i > 2)
      i = 0.1;

  }
}

void loop (void)
{}

void vTaskDelayMicro(unsigned int delayMicro)
{
  TickType_t ticksToDelay = delayMicro * configTICK_RATE_HZ / 1000000;

  vTaskDelay(ticksToDelay);
}