from GetOurPosition import GetOurPosition
from GetOurPosition import SelectUSBPortAndBaudRate
#   main program
# variables.
ourLat = 0
ourLon = 0
portName = "None"
baudRate = 0
status = True

welcomeMessage = "######################## Antenna Project ######################## \n" \
                 "Team members: \n" \
                 "  ·   MEDINA HERNÁNDEZ, SELENIA MARÍA. \n" \
                 "  ·   BERCIANO RODRÍGUEZ, GEMMA. \n" \
                 "  ·   VEGA GARCÍA, CARLOS. \n" \
                 "######################## Antenna Project ########################"

# Welcome message.
print(welcomeMessage)

# Select USB port and baud rate.
(portName, baudRate) = SelectUSBPortAndBaudRate()

# multiplex to GPS.

# know our position.
(ourLat, ourLon) = GetOurPosition(portName, baudRate)

# multiplex to arduino.

# get magnetometer and accelerometer values.

# calculate our rotation matrix.


# while(True):

    # know dron position.

    # calculate vector position.

    # change vector position reference.

    # calculate rotation and inclination angles.

    # send rotation and inclination angles to arduino.

    # get arduino response.

