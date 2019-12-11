#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#ifndef TERRAINBLOCK_H
#define TERRAINBLOCK_H

class TerrainBlock
{

	float *pts;       
	float *normals;   
	float *colors;    
	float *triangleNorms;
	int n;    

public:
	TerrainBlock();
	TerrainBlock(int d, float H, float minHeight, float maxHeight, float minX, float maxX, float minY);
	~TerrainBlock();

	void initMesh(int d, float minS, float maxS, float minT);
	void meshWith(TerrainBlock *TB, int dim);
	void computeMesh(float H, float minHeight, float maxHeight);

	int getIndex(int s, int t, int dim);
	int getIndexTri(int s, int t, int half, int dim);
	void set(int s, int t, int dim, float val);
	void setColor(int s, int t, int dim, float val);
	void setNormal(int s, int t, int dim, float val);
	void setTriNorm(int s, int t, int half, int dim, float val);
	float get(int s, int t, int dim);
	float getColor(int s, int t, int dim);
	float getNormal(int s, int t, int dim);
	float getTriNorm(int s, int t, int half, int dim);
	float getTriNorm(int index);
	int getN();
	int fitIn(int dim);
private:
	float randf();
	void cross(float* a, float* b, float* v);
	void unitize(float* u);
	void setColorOffHeight(int s, int t, float height);
};

#endif 