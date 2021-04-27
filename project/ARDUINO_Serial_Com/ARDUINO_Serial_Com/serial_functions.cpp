#include <Arduino.h>
#include "serial_functions.h"

SerialCommand decodeSerialCommand(String *inputString)
{
    SerialCommand inCommand;
    Serial.println(*inputString);

    return inCommand;
}