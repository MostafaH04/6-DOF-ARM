int E = 13;
int D = 12;
int C = 11;
int DP = 10;
int B = 9;
int A = 8;
int F = 7;
int G = 6;

void one()
{
  digitalWrite(E,HIGH);
  digitalWrite(D,LOW);
  digitalWrite(C,LOW);
  digitalWrite(DP,LOW);
  digitalWrite(B,LOW);
  digitalWrite(A,LOW);
  digitalWrite(F,HIGH);
  digitalWrite(G,LOW);
}

void setup () {
  pinMode (E, OUTPUT);
  pinMode (D, OUTPUT);
  pinMode (C, OUTPUT);
  pinMode (DP, OUTPUT);
  pinMode (B, OUTPUT);
  pinMode (A, OUTPUT);
  pinMode (F, OUTPUT);
  pinMode (G, OUTPUT);
}
void loop() {
    one();
}