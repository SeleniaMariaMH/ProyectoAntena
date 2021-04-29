## Proyecto.

-   ./LOCAL-interface/RotationMatrixFunction.py: function to calculate the rotation matrix from the magnetometer and accelerometer values

-   ./LOCAL-interface/RotationAndInclinationFunctions.py: contains the following list of functions:
    -   DegToRad(): to convert from degrees to radians
    -   RadToDeg(): to convert from radians to degrees
    -   PositionVector(): to calculate the position vector between two points from theirs latitude, longitude, and height values based on earth's global system
    -   VectorToVector(): to change the reference system of a vector using a rotation matrix
    -    RotationAndInclination(): to calculate the angle of rotation and inclination of a vector

-    ./LOCAL-interface/GetOurPosition.py: function to get our position (latitude & longitude) via GPS.
