#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "motor.hpp"

// enable pin 
#define ENABLE_PIN 12

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

MotorHeap motorHeap;
StepperMotor motor_1(STEP_1,DIR_1, &motorHeap, STEPPER_8_MICROSTEP);
StepperMotor motor_2(STEP_2,DIR_2, &motorHeap, STEPPER_8_MICROSTEP);
StepperMotor motor_3(STEP_3,DIR_3, &motorHeap, STEPPER_8_MICROSTEP);
StepperMotor motor_4(STEP_4,DIR_4, &motorHeap, STEPPER_8_MICROSTEP);
StepperMotor motor_5(STEP_5,DIR_5, &motorHeap, STEPPER_8_MICROSTEP);
StepperMotor motor_6(STEP_6,DIR_6, &motorHeap, STEPPER_8_MICROSTEP);



StepperMotor *motors[6] = {&motor_1, &motor_2, &motor_3, &motor_4, &motor_5, &motor_6};
MultiStepperDrive stepperDriver(motors);

ISR(TIMER1_COMPA_vect)
{
  noInterrupts();

  motorHeap.decrement();
  if(!motorHeap.isEmpty())
  {
    while(motorHeap.top() == 0)
    {
      Serial.println("top == 0");
      HeapNode* current = motorHeap.dequeue();
      current->motor->runHeap(); // problem solved!
    }
  }

  interrupts();
  TCNT1 = timer1_compare_match;
}

void setup (void)
{
  Serial.begin(9600);

  pinMode(ENABLE_PIN, OUTPUT);

  motor_1.init();
  motor_2.init();
  motor_3.init();
  motor_4.init();
  motor_5.init();
  motor_6.init();

  xTaskCreate(main_task, "Main Task", 200, NULL, 3, NULL);
  xTaskCreate(motor_task, "Motor Task", 200, NULL, 2, NULL);

  stepperDriver.init();
  vTaskStartScheduler();
}

static void main_task(void * pvParameters)
{
  for(;;)
  {
    Serial.println("main task");
    vTaskDelay(1000);
  }
}

static void motor_task(void * pvParameters)
{
  for(;;)
  {
    stepperDriver.driveSpeed(100);
    vTaskDelayMicro(1000);

    motor_1.runHeap();
    motor_2.runHeap();
    motor_3.runHeap();
    motor_4.runHeap();
    motor_5.runHeap();
    motor_6.runHeap();
    vTaskDelayMicro(1000);
  }
}

void loop (void)
{
}

void vTaskDelayMicro(unsigned int delayMicro)
{
  TickType_t ticksToDelay = delayMicro * configTICK_RATE_HZ / 1000000;
  vTaskDelay(ticksToDelay);
}


