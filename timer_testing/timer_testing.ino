#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long timer1_compare_match = 31250; // Example value

void setup() {
  pinMode(5, OUTPUT);
  noInterrupts();
  TCCR1A = 0;     
  TCCR1B = 0;     
  TCNT1 = 0;      
  OCR1A = 31250;  // da compare match

  TCCR1B |= (1 << WGM12); // CTC mode (what is this?)
  TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 
  TIMSK1 |= (1 << OCIE1A);
  interrupts(); 
}

void loop() {
  // Your main code here
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = timer1_compare_match; 
  digitalWrite(5, !digitalRead(5));
}
