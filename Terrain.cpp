#include "Terrain.h"
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include "mathLib3D.h"

using namespace std;

Terrain::Terrain(int inX, int inZ, int choose)
{
	sizeX = inX;
	sizeZ = inZ;
	if (choose == 1) {
		generateHeightMap();
	}
	else {
		faultalg(); 
	} 
	getMinMax();
	getNormals();
}

void Terrain::generateHeightMap()
{
	int it = (sizeX + sizeZ)/1.5; //number of iterations
	int cX, cZ, disp, cSize;

	for (int i = 0; i < it; i++) {
		//randomizes circle attributes
		cX = (rand() % (sizeX-1));
		cZ = (rand() % (sizeZ-1));
		disp = (rand() % 12) - 2;
		cSize = (rand() % 20) + 1*sizeX/5;

		//circle algorithm
		for (int iX = 0; iX < sizeX; iX++) {
			for (int iZ = 0; iZ < sizeZ; iZ++) {
				float pd = (sqrt(pow(iX-cX, 2.0) + pow(iZ-cZ, 2)))*2.0 / float(cSize);
				if (fabs(pd) <= 1.0)
					heightMap[iX][iZ] += disp/2.0 + cos(pd*3.14)*disp/2.0;
			}
		}
	}
}

//get min and max height values
void Terrain::getMinMax()
{
	minHeight = 0;
	maxHeight = 0;

	for (int iX = 0; iX < sizeX; iX++) {
		for (int iZ = 0; iZ < sizeZ; iZ++) {
			if (heightMap[iX][iZ] < minHeight)
				minHeight = heightMap[iX][iZ];
			else if (heightMap[iX][iZ] > maxHeight)
				maxHeight = heightMap[iX][iZ];
		}
	}
}

// taken from http://www.lighthouse3d.com/opengl/terrain/index.php?impdetails as cited in assignment
void Terrain::faultalg() {
	int it = (sizeX + sizeZ)/1.5; //number of iterations
	float cX, cZ, disp, cSize;
	float displacement = 1;
	for( int i = 0; i < it; i++) {
		float stat = static_cast <float> (rand()); 
		cX = sin(stat);
		cZ = cos(stat);    
		disp = sqrt(pow(sizeX,2) + pow(sizeZ,2));  
		float c = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * disp - disp/2;
        for(int m = 0; m<sizeX; m++){
            for(int r=0; r<sizeZ; r++){
                if (cX *m + cZ*r - c > 0){                   
                    heightMap[m][r] += displacement;
                }
                else{
                    heightMap[m][r] -= displacement;                
                }
            }
        }
    }
}

//calculate vertex normals
void Terrain::getNormals()
{
	for (int z = 1; z < sizeZ-1; z++){
        for (int x = 1; x < sizeX-1; x++){
        	Vec3D vecR(1, heightMap[x+1][z]-heightMap[x][z], 0);
        	Vec3D vecL(-1, heightMap[x-1][z]-heightMap[x][z], 0);
        	Vec3D vecD(0, heightMap[x][z+1]-heightMap[x][z], 1);
        	Vec3D vecU(0, heightMap[x][z-1]-heightMap[x][z], -1);

		    //get cross products
		    Vec3D vecUR = vecU.cross(vecR);
		    Vec3D vecRD = vecR.cross(vecD);
		    Vec3D vecDL = vecD.cross(vecL);
		    Vec3D vecLU = vecL.cross(vecU);

		    //get average normal
		    float aX = (vecUR.dx + vecRD.dx + vecDL.dx + vecLU.dx)/4.0;
		    float aY = (vecUR.dy + vecRD.dy + vecDL.dy + vecLU.dy)/4.0;
		    float aZ = (vecUR.dz + vecRD.dz + vecDL.dz + vecLU.dz)/4.0;
		    Vec3D avgN = Vec3D(aX, aY, aZ);

		    normals[x][z] = avgN.normalize();
        }
    }
}