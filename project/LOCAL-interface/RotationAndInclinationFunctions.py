import math

# to convert from degrees to radians
def DegToRad(degData):
    return degData * math.pi/180

# to convert from radians to degrees
def RadToDeg(radData):
    return radData * 180/math.pi

# to calculate the position vector between two points from theirs latitude, longitude, and height values based on earth's global system
# @param:   latitude_ini (deg), longitude_ini (deg), height_ini (m) = ZERO,
#           latitude_end (deg), longitude_end (deg), height_end (m)
# @return:  position vector [axisX axisY axisZ]
def PositionVector(latIni, lonIni, hInit, latEnd, lonEnd, hEnd):

    rEarth = 6371000  # Earth radius (meters)

    axisX = ((DegToRad(lonEnd) - DegToRad(lonIni)) * math.cos((DegToRad(latEnd) + DegToRad(lonIni))/2)) * rEarth
    axisY = (DegToRad(latEnd) - DegToRad(latIni)) * rEarth
    axisZ = hEnd - hInit

    posVector = [axisX, axisY, axisZ]

    return posVector

# to change the reference system of a vector using a rotation matrix
def VectorToVector(vector, rotMat):

    axisX = rotMat[0][0] * vector[0] + rotMat[0][1] * vector[1] + rotMat[0][2] * vector[2]
    axisY = rotMat[1][0] * vector[0] + rotMat[1][1] * vector[1] + rotMat[1][2] * vector[2]
    axisZ = rotMat[2][0] * vector[0] + rotMat[2][1] * vector[1] + rotMat[2][2] * vector[2]

    vectorRet = [axisX, axisY, axisZ]

    return vectorRet

# to calculate the angle of rotation and inclination of a vector
# to orient the antenna the vector must be referenced to the antenna reference system
# @param:   vector referenced to the antenna system (meters)
# @return:  angle of rotation (degrees) and inclination (degrees)
def RotationAndInclination(vector):

    dist = math.sqrt((vector[0] ** 2) + (vector[1] ** 2))

    rotRad = math.atan(vector[1]/vector[0])
    incRad = math.atan(vector[2]/dist)

    rotDeg = RadToDeg(rotRad)
    incDeg = RadToDeg(incRad)

    return rotDeg, incDeg
