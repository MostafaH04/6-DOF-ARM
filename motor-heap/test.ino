// // Sample code for task scheduling

// #include <Arduino_FreeRTOS.h>
// void TaskBlink1( void *pvParameters );
// void TaskBlink2( void *pvParameters );
// void Taskprint( void *pvParameters );

// void setup() {
//   Serial.begin(9600);
//   xTaskCreate(TaskBlink1,"Task1",128,NULL,1,NULL);
//   xTaskCreate(TaskBlink2,"Task2 ",128,NULL,1,NULL);
//   xTaskCreate(Taskprint,"Task3",128,NULL,1,NULL);   
//   vTaskStartScheduler();
// }

// void loop() {
// }

// void TaskBlink1(void *pvParameters) {
//   pinMode(13, OUTPUT);
//   while(1){
//     Serial.println("Task1");
//     digitalWrite(13, HIGH);   
//     vTaskDelay( 200 / portTICK_PERIOD_MS ); 
//     digitalWrite(13, LOW);    
//     vTaskDelay( 200 / portTICK_PERIOD_MS ); 
//   }
// }

// void TaskBlink2(void *pvParameters) {
//   pinMode(12, OUTPUT);
//   while(1){
//     Serial.println("Task2");
//     digitalWrite(12, HIGH);   
//     vTaskDelay( 200 / portTICK_PERIOD_MS ); 
//     digitalWrite(12, LOW);    
//     vTaskDelay( 200 / portTICK_PERIOD_MS ); 
//   }
// }

// void Taskprint(void *pvParameters) {
//   int counter = 0;
//   while(1) {
//     counter++;
//     Serial.println(counter);
//     vTaskDelay( 500 / portTICK_PERIOD_MS ); 
//   }
// }