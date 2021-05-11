from GetOurPosition import GetOurPosition

#   main program

# multiplex to GPS.

# know our position.
ourLat = 0
ourLon = 0

portName = "/dev/tty.wchusbserial1410"
baudRate = 9600
timeOut = 5
numMeasure = 25

(ourLat, ourLon) = GetOurPosition(portName, baudRate, timeOut, numMeasure)

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

