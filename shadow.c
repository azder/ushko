
// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length 1.
void ReduceToUnit(float vector[3])
{
    float length;

    // Calculate the length of the vector
    length = (float) sqrt((vector[0] * vector[0]) +
                          (vector[1] * vector[1]) +
                          (vector[2] * vector[2]));

    // Keep the program from blowing up by providing an acceptable
    // value for vectors whose length may be calculated too close to zero.
    if (length == 0.0f)
        length = 1.0f;

    // Dividing each element by the length will result in a
    // unit normal vector.
    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;
}

// Points p1, p2, & p3 specified in counterclockwise order
void calcNormal(float v[3][3], float out[3])
{
    float v1[3], v2[3];
    static const int x = 0;
    static const int y = 1;
    static const int z = 2;

    // Calculate two vectors from the three points
    v1[x] = v[0][x] - v[1][x];
    v1[y] = v[0][y] - v[1][y];
    v1[z] = v[0][z] - v[1][z];
    v2[x] = v[1][x] - v[2][x];
    v2[y] = v[1][y] - v[2][y];
    v2[z] = v[1][z] - v[2][z];

    // Take the cross product of the two vectors to get
    // the normal vector which will be stored in out[]
    out[x] = v1[y] * v2[z] - v1[z] * v2[y];
    out[y] = v1[z] * v2[x] - v1[x] * v2[z];
    out[z] = v1[x] * v2[y] - v1[y] * v2[x];

    // Normalize the vector (shorten length to one)
    ReduceToUnit(out);
}

// Creates a shadow projection matrix out of the plane equation
// coefficients and the position of the light. The return value is stored
// in destMat[][]
void MakeShadowMatrix(GLfloat points[3][3], GLfloat lightPos[4],
                      GLfloat destMat[4][4])
{
    GLfloat planeCoeff[4];
    GLfloat dot;

    // Find the plane equation coefficients
    // Find the first three coefficients the same way we
    // find a normal.
    calcNormal(points, planeCoeff);

    // Find the last coefficient by back substitutions
    planeCoeff[3] = -(
            (planeCoeff[0] * points[2][0]) +
            (planeCoeff[1] * points[2][1]) +
            (planeCoeff[2] * points[2][2]));

    // Dot product of plane and light position
    dot = planeCoeff[0] * lightPos[0] +
          planeCoeff[1] * lightPos[1] +
          planeCoeff[2] * lightPos[2] +
          planeCoeff[3] * lightPos[3];

    // Now do the projection
    // First column
    destMat[0][0] = dot - lightPos[0] * planeCoeff[0];
    destMat[1][0] = 0.0f - lightPos[0] * planeCoeff[1];
    destMat[2][0] = 0.0f - lightPos[0] * planeCoeff[2];
    destMat[3][0] = 0.0f - lightPos[0] * planeCoeff[3];
    // Second column
    destMat[0][1] = 0.0f - lightPos[1] * planeCoeff[0];
    destMat[1][1] = dot - lightPos[1] * planeCoeff[1];
    destMat[2][1] = 0.0f - lightPos[1] * planeCoeff[2];
    destMat[3][1] = 0.0f - lightPos[1] * planeCoeff[3];
    // Third Column
    destMat[0][2] = 0.0f - lightPos[2] * planeCoeff[0];
    destMat[1][2] = 0.0f - lightPos[2] * planeCoeff[1];
    destMat[2][2] = dot - lightPos[2] * planeCoeff[2];
    destMat[3][2] = 0.0f - lightPos[2] * planeCoeff[3];
    // Fourth Column
    destMat[0][3] = 0.0f - lightPos[3] * planeCoeff[0];
    destMat[1][3] = 0.0f - lightPos[3] * planeCoeff[1];
    destMat[2][3] = 0.0f - lightPos[3] * planeCoeff[2];
    destMat[3][3] = dot - lightPos[3] * planeCoeff[3];
}



