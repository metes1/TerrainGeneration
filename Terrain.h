#ifndef TERRAIN_H
#define TERRAIN_H

#include "mathLib3D.h"

class Terrain {
public:
    Terrain(int inX, int inZ, int choose);
    int sizeX;
    int sizeZ; 
    float minHeight;
    float maxHeight;
    float heightMap[300][300];
    Vec3D normals[300][300];

private:
    void generateHeightMap();
    void faultalg();
    void getMinMax();
    void getNormals();
};

#endif