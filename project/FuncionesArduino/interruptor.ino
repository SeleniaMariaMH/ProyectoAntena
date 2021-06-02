#define interrupPin 2
 
void setup() {
  Serial.begin(9600);
  pinMode(interrupPin, INPUT);
}
 
void loop()
{
  attachInterrupt(digitalPinToInterrupt(interrupPin), IntInterruptor, RISING);
}

void IntInterruptor()
{
  Serial.println("HOMMING");
  delay(1500);
}
