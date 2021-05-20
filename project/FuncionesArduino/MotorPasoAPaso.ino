// motor paso a paso con DRV8825.

#include <Stepper.h>

#define steps 200
#define reduccion 3.6
#define microPasos 16
#define vueltaAntena (steps*microPasos*reduccion)
#define gradosPorPaso (360/vueltaAntena)
#define movMax 5

const int dirPin = 8;
const int stepPin = 9;
const int enable = 7;
float PosIni = 0;


void setup() 
{
  // Marcar los pines como salida
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enable, OUTPUT);
}

// frecuencia = 1/nºsegundos de delay
// 200 pasos * nºmicropasos y antena 72.
void loop() 
{
  MovStepperMotor(dirPin, stepPin, enable, 10);
  delay(1000);
}

void MovStepperMotor(const int dirPin, const int stepPin, const int enable, float PosFinal)
{
  int stepDelay = 250;
  digitalWrite(enable, LOW);      // Enable.

  if(PosIni > PosFinal)
  {
    digitalWrite(dirPin, LOW);   // --> COMPROBAR SI ES EL LADO CORRECTO
    if ((PosIni - PosFinal) >= movMax)
    {
      for (int x = 0; x < (movMax/gradosPorPaso); x++) 
      {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepDelay);
      } 
      PosIni = PosIni - movMax;
    }
    else
    {
      for (int x = 0; x < ((PosIni - PosFinal)/gradosPorPaso); x++) 
      {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepDelay);
      }
      PosIni = PosFinal;
    }
  }
  else if(PosIni < PosFinal)
  {
    digitalWrite(dirPin, HIGH);   // --> COMPROBAR SI ES EL LADO CORRECTO
    if ((PosFinal - PosIni) >= movMax)
    {
      for (int x = 0; x < (movMax/gradosPorPaso); x++) 
      {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepDelay);
      } 
      PosIni = PosIni + movMax;
    }
    else
    {
      for (int x = 0; x < ((PosFinal - PosIni)/gradosPorPaso); x++) 
      {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepDelay);
      }
      PosIni = PosFinal;
    }
  }
  else
  {
    PosIni = PosFinal;
  }
}
