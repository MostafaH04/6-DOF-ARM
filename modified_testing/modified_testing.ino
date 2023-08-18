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
StepperMotor *motors[6] = {&motor_1, &motor_1, &motor_1, &motor_1, &motor_1, &motor_1};
MultiStepperDrive stepperDriver(motors);

ISR(TIMER1_COMPA_vect)
{
  // debug statements
  Serial.println("Interrupt occurred"); // check if interrupt occurred

  Serial.print("Motor Heap Size: ");
  Serial.println(motorHeap.getSize());  

  TCNT1 = timer1_compare_match;

  motorHeap.decrement();
  
  // check which motor is right now
  if(!motorHeap.isEmpty())
  {
    Serial.println("Motors in Queue:");
    motorHeap.print();

    while(motorHeap.top() == 0)
    {
      HeapNode* current = motorHeap.dequeue();
      current->motor->runHeap();
    }
  }
}

void setup (void)
{
  Serial.begin(9600);
  motor_1.init(); 

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
    Serial.print("Setting speed to motor: ");
    Serial.println(i);
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


// // SID's VERSION WITH SERIAL MONITOR

// #include <Arduino_FreeRTOS.h>
// #include <semphr.h>
// #include "motor.hpp"

// // enable pin 
// #define ENABLE_PIN 12

// // Define the number of motors
// #define NUM_MOTORS 6

// // Array of motor step and dir pins
// int stepPins[NUM_MOTORS] = {11, 9, 7, 5, 3, 14};
// int dirPins[NUM_MOTORS] = {10, 8, 6, 4, 2, 15}; 

// MotorHeap motorHeap;
// StepperMotor *motors[NUM_MOTORS];

// MultiStepperDrive stepperDriver(motors);

// void setup(void)
// {
//   Serial.begin(9600);
//   pinMode(ENABLE_PIN, OUTPUT);

//   for (int i = 0; i < NUM_MOTORS; i++)
//   {
//     motors[i] = new StepperMotor(stepPins[i], dirPins[i], &motorHeap, STEPPER_8_MICROSTEP);
//     motors[i]->init();
//   }

//   xTaskCreate(main_task, "Main Task", 200, NULL, 3, NULL);
// }

// static void main_task(void *pvParameters)
// {
//   digitalWrite(ENABLE_PIN, HIGH);

//   for (int i = 0; i < NUM_MOTORS; i++)
//   {
//     float speed = 2000;

//     Serial.print("Setting speed to motor ");
//     Serial.print(i);
//     Serial.print(": ");
//     Serial.println(speed);

//     motors[i]->driveSpeed(speed);
//   }

//   vTaskDelay(5000 / portTICK_PERIOD_MS); // Run motors at 2000 steps for 5 seconds

//   for (int i = 0; i < NUM_MOTORS; i++)
//   {
//     motors[i]->driveSpeed(0); // Stop the motor

//     Serial.print("Motor ");
//     Serial.print(i);
//     Serial.println(" stopped running.");
//   }

//   vTaskDelay(5000 / portTICK_PERIOD_MS); // Delay for 5 seconds before next iteration

//   while (1)
//   {
//     vTaskDelay(1000); // Keep the task running
//   }
// }

// void loop(void)
// {
// }

// void vTaskDelayMicro(unsigned int delayMicro)
// {
//   TickType_t ticksToDelay = delayMicro * configTICK_RATE_HZ / 1000000;

//   vTaskDelay(ticksToDelay);
// }
