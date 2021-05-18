
#define USE_TIMER_1     true

#include "TimerInterrupt.h"

#include "serial_functions.h"
#include "bsp_functions.h"

#if !defined(LED_BUILTIN)
  #define LED_BUILTIN     13
#endif

#define TIMER1_INTERVAL_MS    10000

//DEFINITION OF OBJECTS
ReducedMPU9250 mpu(false);

//EVENT FLAGS
bool f_serialNewLine = false; // whether the string is complete

//GLOBAL VARIABLES
String inputString = ""; // a String to hold incoming data
unsigned int outputPin1 = LED_BUILTIN;

//Function for analysing the system periodically and sending information the host
void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

  Serial.print("OK\n");

  digitalWrite(outputPin, toggle1);
  toggle1 = !toggle1;
}

void setup()
{
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  Serial.print(F("\nStarting Antenna Development Platform "));
  Serial.println(BOARD_TYPE);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  
  ITimer1.init();

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }

  //Inits the MPU
  if(!init_IMU(&mpu))
    Serial.println("ERROR: NOT ABLE TO INIT THE IMU");
}


int exeCommand(SerialCommand inCommand){

  int res = 0;
/*
  Serial.print(F("Recieved Command "));
  Serial.print(inCommand.command);
  Serial.print(F(" with parameters = "));
  Serial.println(inCommand.nParams);
  
  for(short i = 0 ; i < inCommand.nParams; i++){
    Serial.println(inCommand.params[i]);
  }
  */

  int idCommand = inCommand.command;
 
  //Get Acceleration and Magnetometer values
  if(inCommand.command == 'G'){
   

    float AccG[] = {0.0, 0.0, 0.0};
    float MagCal[] = {0.0, 0.0, 0.0};
    ValoresAccyMag(&mpu,AccG,MagCal);

    return 0;

  }
  // Calibrate magnetometer
  else if(inCommand.command == 'C'){
    

    
    mpu.CalibrateMagnetometer();
    Serial.println("CALIBRATING MAGNETOMETER");
    return 0;
  }

  // Introduce offset to the acelerometer
  else if(inCommand.command == 'U'){
    if(inCommand.nParams != 3){
      return -2;
    }
    int mode = inCommand.params[0].toInt();
    int offset = inCommand.params[1].toInt();
    int axis = inCommand.params[1].toInt();
    if(mode == 0){
      updateAccelerometerCalibrationScale(&mpu,offset,axis);
      Serial.println("Update scale");
    }else if(mode == 1){      
      updateAccelerometerCalibrationOffsets(&mpu,offset,axis);
      Serial.println("Update offset");
    }else{
      return -2;
    }
    return 0;
  }
  else{
    return -1;
  }

  /* switch(idCommand){
    //GETS THE DATA FROM THE IMU
    case 0:
      float AccG[] = {0.0, 0.0, 0.0};
      float MagCal[] = {0.0, 0.0, 0.0};
      ValoresAccyMag(&mpu,AccG,MagCal);
      break;

    //CALIBRATE THE MAGNETOMETER
    case 1:
      Serial.println("CALIBRATING MAGNETOMETER");
      mpu.CalibrateMagnetometer();
      break;

    default:
      res = -1;
      break;
      
  } */
  return res;
}



void loop()
{
  // put your main code here, to run repeatedly:
  // print the string when a newline arrives:
  if (f_serialNewLine)
  {
    SerialCommand inCommand = decodeSerialCommand(&inputString);

    if(exeCommand(inCommand) <0){
      Serial.println("E:0");
      Serial.println("ERROR, Unknown command");
    }

    //Serial.println("Command Executed");

    // clear the string:
    inputString = "";
    f_serialNewLine = false;
  }
}

/* 
  Serial event that saves the incoming data into a string until it recieves a \n
*/
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();

    // add it to the inputString:
    inputString += inChar;

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      f_serialNewLine = true;
    }
  }
}



int heartBeatSerial()
{

}