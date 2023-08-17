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

const int pwmPin = 9;  // Pin for PWM output
volatile boolean pwmState = false;  // Current state of the PWM signal
unsigned long pwmInterval = 78;    // Desired interval for the PWM signal (adjust as needed)
unsigned long previousMillis = 0;   // Store the last time the PWM state was changed

void setup() {
  pinMode(pwmPin, OUTPUT);
  
  // Configure Timer2 for interrupt-based PWM
  TCCR2A = _BV(WGM20) | _BV(WGM21); // Set timer mode to Fast PWM
  TCCR2B = _BV(CS21);               // Set prescaler to 8 (PWM frequency formula: F_CPU / (prescaler * 256))
  
  // Enable Timer2 overflow interrupt
  TIMSK2 = _BV(TOIE2);
  
  // Start the timer
  TCNT2 = 0;

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  calculateAndPrintPWMFrequency();
}

// Timer2 overflow interrupt service routine
ISR(TIMER2_OVF_vect) {
  unsigned long currentMillis = millis();
  
  // Check if the desired PWM interval has elapsed
  if (currentMillis - previousMillis >= pwmInterval) {
    previousMillis = currentMillis;
    pwmState = !pwmState;
    digitalWrite(pwmPin, pwmState);
  }
}

void calculateAndPrintPWMFrequency() {
  unsigned long prescalerValue = 8;  // Prescaler value used (CS21)
  unsigned long timerOverflow = 256; // Timer overflow value (8-bit timer)
  unsigned long pwmFrequency = F_CPU / (prescalerValue * timerOverflow * pwmInterval);
  
  Serial.print("PWM Frequency: ");
  Serial.print(pwmFrequency);
  Serial.println(" Hz");
}

