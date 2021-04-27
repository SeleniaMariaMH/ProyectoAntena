

#ifndef SERIAL_FUNCT
#define SERIAL_FUNCT

struct SerialCommand
{
    /* Struct for saving the incoming command data*/

    short error = 0;
    unsigned int command;
    int *params;
};

SerialCommand decodeSerialCommand(String *inputString);

#endif
