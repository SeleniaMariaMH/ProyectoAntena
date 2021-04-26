# Rotation matrix -> R = [(A×M)×A, A×M, A];
def CalculateRotationMatrix(magnetometerData, accelerometerData):

    rX = CrossProduct(CrossProduct(accelerometerData, magnetometerData), accelerometerData)
    rY = CrossProduct(accelerometerData, magnetometerData)
    rZ = accelerometerData

    rX[:] = [x / CalculateNorm(rX) for x in rX]
    rY[:] = [x / CalculateNorm(rY) for x in rY]
    rZ[:] = [x / CalculateNorm(rZ) for x in rZ]

    r = [rX, rY, rZ]

    print("Rotation matrix: ", r)

    return r

# Calculate cross product for 2 vectors of 3 components
def CrossProduct(vectorA, vectorB):
    vectorCX = vectorA[1] * vectorB[2] - vectorA[2] * vectorB[1]
    vectorCY = vectorA[2] * vectorB[0] - vectorA[0] * vectorB[2]
    vectorCZ = vectorA[0] * vectorB[1] - vectorA[1] * vectorB[0]

    vectorC = [vectorCX, vectorCY, vectorCZ]

    return vectorC

# Calculate normal of a 3 components vector
def CalculateNorm(vector):
    normValue = (vector[0]**2 + vector[1]**2 + vector[2]**2) ** 0.5

    return normValue
