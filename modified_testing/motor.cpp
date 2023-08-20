#include "motor.hpp"

int timer1_compare_match;

StepperMotor::StepperMotor(int step_pin, int dir_pin, MotorHeap *heap_in, MICROSTEPS microsteps):
  step(step_pin), dir(dir_pin), heap(heap_in)
{
  revoluteSteps = REVOLUTE_STEPS * microsteps;
  targetSpeed = 0;
  currentStep = 0;
  currentAngle = 0;

  stepAngle = 2.0 * M_PI / (double)revoluteSteps;
  heapNode.motor = this;
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
  int delayTime = (time_s*1000000);
  delayMicroseconds(delayTime);
}

void StepperMotor::runHeap(void)
{
  currentSpeed = targetSpeed;
  int interruptDelay{};
  Serial.println("runHeap got called!");
  if (!digitalRead(step)){ // new cycle: step pin is low. calculate target speed
    double time_s = (1/(currentSpeed / stepAngle)); // sec/step
    lastStepTime = (time_s*1000000);
    interruptDelay = MIN_PULSE / INTERRUPT_INTERVAL_US;
    if (targetSpeed == 0)
      return;
    else if (targetSpeed > 0)
      digitalWrite(dir, HIGH);
    else
      digitalWrite(dir, LOW);
    digitalWrite(step, HIGH);
  }
  else{
    interruptDelay = (lastStepTime - MIN_PULSE) / INTERRUPT_INTERVAL_US;
    digitalWrite(step, LOW);
  }
  heapNode.count = interruptDelay;
  heap->enqueue(&heapNode);
}

MotorHeap::MotorHeap():
  size(1)
{}

int MotorHeap::getSize() const
{
  return size;
}

bool MotorHeap::isEmpty() const
{
  return size == 1;
}

bool MotorHeap::isFull() const
{
  return size == 7;
}

void MotorHeap::print() const
{
  for (int i = 1; i < size; i++){
    Serial.print(heap[i]->count);
    Serial.print(", ");
  }
  Serial.println();
}

int MotorHeap::parent(int in) const
{
  return in/2;
}

int MotorHeap::child(int in, bool right) const
{
  return in * 2 + right?1:0;
}

void MotorHeap::swap(int in_1, int in_2)
{
  // Serial.print("heap[in_2] BEFORE: ");
  // Serial.println(heap[in_2]->count);

  // Serial.print("heap[in_1] AFTER: ");
  // Serial.println(heap[in_1]->count);

  HeapNode *temp = heap[in_1];
  heap[in_1] = heap[in_2];
  heap[in_2] = temp;

  // Serial.print("heap[in_2] AFTER: ");
  // Serial.println(heap[in_2]->count);

  // Serial.print("heap[in_1] AFTER: ");
  // Serial.println(heap[in_1]->count);
}

void MotorHeap::heapUp(int in)
{
  // Serial.print("heap[in]->count, in: ");
  // Serial.print(heap[in]->count);
  // Serial.print(", ");
  // Serial.println(in);

  if (in >= size || heap[parent(in)]->count < heap[in]->count || in <= 1)
  {
    return;
  }

  swap(in, parent(in));
  heapUp(parent(in));
}

// void MotorHeap::heapUp(int in)
// {
//     // Ensure in is a valid index and not the root node
//     if (in > 1) {
//         int parentIndex = parent(in);
        
//         // Compare the child with its parent
//         if (heap[in]->count < heap[parentIndex]->count) {
//             // Swap child and parent
//             swap(in, parentIndex);
            
//             // Recursively heapUp the parent
//             heapUp(parentIndex);
//         }
//     }
// }


void MotorHeap::heapDown(int in)
{
  int childPos = child(in,false);
  if (childPos >= size)
    return;
  
  int rightChild = child(in, true);
  if (rightChild < size && 
    heap[rightChild]->count < heap[childPos]->count)
  {
    childPos = rightChild;
  }
  swap(childPos, in);
  heapDown(childPos);
}

HeapNode *MotorHeap::dequeue(void)
{
  // take from top replace with last child;
  if (!isEmpty())
  {  
    size --;
    HeapNode *out = heap[1];
    heap[1] = heap[size];
    heapDown(1);
    return out;
  }
}

void MotorHeap::enqueue(HeapNode *node)
{
  if (!isFull())
  {
    heap[size] = node;
    size++;
    heapUp(size-1);  
  }
}

int MotorHeap::top(void) const
{
  if(!isEmpty()){
    return heap[1]->count;
  }
  return -1;
}

void MotorHeap::decrement(void)
{
  for (int i = 1; i < size; i++)
  {
    heap[i]->count --;
  }
}

MultiStepperDrive::MultiStepperDrive(StepperMotor **motor):
  motors(motor)
{
}

void MultiStepperDrive::init(void)
{
  noInterrupts();

  // init Timer 1
  TCCR1A = 0;
  TCCR1B = 0;

  timer1_compare_match = 31;

  TCNT1 = timer1_compare_match;
  TCCR1B |= (1 << CS10);  

  TIMSK1 |= (1 << OCIE1A);

  // Set Prescalar 1
  
  interrupts();

}

void MultiStepperDrive::driveSpeed(float radPerSec[6])
{
  for (int i = 0; i < 6; i++)
  {
    motors[i]->driveSpeed(radPerSec[i]);
  }
}

void MultiStepperDrive::driveSpeed(float radPerSec)
{
  for (int i = 0; i < 6; i++)
  {
    motors[i]->driveSpeed(radPerSec);
  }
}
