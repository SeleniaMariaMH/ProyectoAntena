// Servomotor.

#include <Servo.h>

Servo miServo;    // Creo objeto de la clase Servo.
#define servoPin 9
void setup() 
{
  miServo.attach(servoPin);   // Pin D10 - digital PWM.
}

void loop() 
{
  MovServoMotor(&miServo, 0.00, 1500);
  MovServoMotor(&miServo, 255.549846, 1500); 
  MovServoMotor(&miServo, 1022.025464, 1500);
  MovServoMotor(&miServo, 500.549846, 1500); 
}

void MovServoMotor(Servo *miServo, float posFinal, int tiempoEspera)
{
  posFinal = map(posFinal, 0, 1023, 0 ,180);  // scale it to use it with the servo (value between 0 and 180)
  miServo->write(posFinal);
  delay(tiempoEspera);
}
