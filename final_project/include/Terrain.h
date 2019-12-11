#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "TerrainBlock.h"

#ifndef TERRAIN_H
#define TERRAIN_H
 
#define RT3DIV2    0.8660254f    
#define RT3DIV3    0.5773503f    

class Terrain
{

	TerrainBlock **blocks;     
	int n;    
	int N;    
	int d;    
	float H;    
	float minHeight;    
	float maxHeight;    
	float blockLength;    

public:
	Terrain(int numBlocksSide, int depth, float roughness, float minheight, float maxheight, float blocklength);
	~Terrain();

	int getN();
	int getD();
	TerrainBlock *getBlockAtXY(float x, float y);
	TerrainBlock *getBlockAtST(float s, float t);
	TerrainBlock *getBlock(int s, int t);
	TerrainBlock *getBlockAt(int s, int t, int S, int T);
	float xToS(float x, float y);
	float yToT(float x, float y);
	float sToX(float s, float t);
	float tToY(float s, float t);
	float getSideLength();
	float getTerrainSideLength();
};

#endif