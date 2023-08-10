// #include <Arduino_FreeRTOS.h>
// #include <semphr.h>
// #include "motor.hpp"

// // enable pin 
// #define ENABLE_PIN 12

// // motor 1
// #define DIR_1 12
// #define STEP_1 11

// // motor 2
// #define DIR_2 8
// #define STEP_2 9

// // motor 3
// #define DIR_3 7
// #define STEP_3 6

// StepperMotor motor_1(STEP_1, DIR_1, STEPPER_8_MICROSTEP);
// StepperMotor motor_2(STEP_2, DIR_2, STEPPER_8_MICROSTEP);
// StepperMotor motor_3(STEP_3, DIR_3, STEPPER_8_MICROSTEP);

// void setup (void)
// {
//   Serial.begin(9600);

//   motor_1.init();
//   motor_1.setPWMFrequency(20000); // 20kHz initially
//   motor_1.setPWMDutyCycle(50); // 50% speed initially

//   motor_2.init();
//   motor_2.setPWMFrequency(20000); // 20kHz initially
//   motor_2.setPWMDutyCycle(50); // 50% speed initially

//   motor_3.init();
//   motor_3.setPWMFrequency(20000); // 20kHz initially
//   motor_3.setPWMDutyCycle(50); // 50% speed initially

//   pinMode(ENABLE_PIN, OUTPUT);

//   xTaskCreate(main_task, "Main Task", 200, NULL, 3, NULL);
// }

// static void main_task(void * pvParameters)
// {
//   float i = 2000;
//   digitalWrite(ENABLE_PIN, HIGH);
//   for (;;)
//   {
//     motor_1.driveSpeed(i);
//     motor_1.run();

//     motor_2.driveSpeed(i);
//     motor_2.run();

//     motor_3.driveSpeed(i);
//     motor_3.run();

//     i += 0.1;
//     if (i > 2)
//       i = 0.1;

//   }
// }

// void loop (void)
// {}

// void vTaskDelayMicro(unsigned int delayMicro)
// {
//   TickType_t ticksToDelay = delayMicro * configTICK_RATE_HZ / 1000000;

//   vTaskDelay(ticksToDelay);
// }


#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <TimerOne.h> // Include the TimerOne library
#include "StepperMotor.h" // Include your StepperMotor class definition

// Pin Definitions
#define ENABLE_PIN 12
#define DIR_1 12
#define STEP_1 11
#define DIR_2 8
#define STEP_2 9
#define DIR_3 7
#define STEP_3 6

// Stepper Motor Instances
StepperMotor motor_1(STEP_1, DIR_1, STEPPER_8_MICROSTEP);
StepperMotor motor_2(STEP_2, DIR_2, STEPPER_8_MICROSTEP);
StepperMotor motor_3(STEP_3, DIR_3, STEPPER_8_MICROSTEP);

// Task Handles
TaskHandle_t mainTaskHandle;

// Task Functions
void mainTask(void* pvParameters);

void setup() {
  Serial.begin(9600);
  
  // Initialize TimerOne for default 1ms interrupt
  Timer1.initialize();
  Timer1.attachInterrupt(timer1_ISR); // Attach a dummy ISR
  
  // Initialize Stepper Motors
  motor_1.init();
  motor_1.setPWMFrequency(20000); // 20kHz initially
  motor_1.setPWMDutyCycle(50); // 50% speed initially

  motor_2.init();
  motor_2.setPWMFrequency(20000); // 20kHz initially
  motor_2.setPWMDutyCycle(50); // 50% speed initially

  motor_3.init();
  motor_3.setPWMFrequency(20000); // 20kHz initially
  motor_3.setPWMDutyCycle(50); // 50% speed initially

  // Configure Enable Pin
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);

  // Create the main task
  xTaskCreate(mainTask, "Main Task", 200, NULL, 3, &mainTaskHandle);
}

void loop() {
  // This loop can remain empty
}

void mainTask(void* pvParameters) {
  float i = 2000;
  for (;;) {
    motor_1.driveSpeed(i);
    motor_1.run();

    motor_2.driveSpeed(i);
    motor_2.run();

    motor_3.driveSpeed(i);
    motor_3.run();

    i += 0.1;
    if (i > 2)
      i = 0.1;

    // Introduce a small delay between iterations
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 10 milliseconds
  }
}

void timer1_ISR() {
  // Empty ISR or any other action you want to perform at the interrupt
}
