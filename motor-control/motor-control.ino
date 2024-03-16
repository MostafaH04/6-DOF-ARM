float angle_1;
float angle_2;
float angle_3;
float angle_4;
float angle_5;
float angle_6;

void setup() {
  Serial.begin(115200);
}

String readUART() {
  while(Serial.available() == 0){
    pinMode(13, OUTPUT);
  }
  String jointAngles = Serial.readStringUntil('\r');
  return jointAngles;
}

void split(String cmd) {
  char char_array[cmd.length() + 1];
  cmd.toCharArray(char_array, cmd.length() + 1);

  char *token = strtok(char_array, " ");

  if (token != NULL) {
    angle_1 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_2 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_3 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_4 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_5 = atof(token);
    token = strtok(NULL, " ");
  }
  if (token != NULL) {
    angle_6 = atof(token);
  }
}


void display() {
  // Blink pin 13 angle_1 times
  for (int i = 0; i < angle_1; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(5000);
  
  // Blink pin 13 angle_2 times
  for (int i = 0; i < angle_2; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  
  delay(5000);
  
  // Blink pin 13 angle_3 times
  for (int i = 0; i < angle_3; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);

  // Blink pin 13 angle_4 times
  for (int i = 0; i < angle_4; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);

  // Blink pin 13 angle_5 times
  for (int i = 0; i < angle_5; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);

  // Blink pin 13 angle_6 times
  for (int i = 0; i < angle_6; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(5000);
}

void driveSteppers(){

}


void loop() {
  String jointAngles = readUART();
  split(jointAngles);
  display();
  // driveSteppers();
}