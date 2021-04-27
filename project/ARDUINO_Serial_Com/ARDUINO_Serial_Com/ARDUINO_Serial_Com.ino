
#include "serial_functions.h"

//EVENT FLAGS
bool f_serialNewLine = false; // whether the string is complete

//GLOBAL VARIABLES
String inputString = ""; // a String to hold incoming data

void setup()
{
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  // print the string when a newline arrives:
  if (f_serialNewLine)
  {
    decodeSerialCommand(&inputString);
    // clear the string:
    inputString = "";
    f_serialNewLine = false;
  }
}

/* 
  Serial event that 
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