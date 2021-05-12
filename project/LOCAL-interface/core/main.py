from GetOurPosition import GetOurPosition
from GetOurPosition import DefineGPSPortAndBaudRate
#   main program
# variables.
ourLat = 0
ourLon = 0
portName = "None"
baudRate = 0

# multiplex to GPS.

# know our position.
(portName, baudRate) = DefineGPSPortAndBaudRate()
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

