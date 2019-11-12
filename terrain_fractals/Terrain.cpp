#include "Terrain.h"
#include "TerrainBlock.h"
#include <iostream> 
#include <ctime> 
#include <stdio.h>
#include <math.h>

Terrain::Terrain(int numBlocksSide, int depth, float roughness, float minheight, float maxheight, float blocklength)
{
	//save some vars
	n = numBlocksSide;
	d = depth;
	N = (1 << (d - 1));
	H = roughness;
	minHeight = minheight;
	maxHeight = maxheight;
	blockLength = blocklength;

	//initialze the random number generator
	srand((unsigned)time(0));  

	int numBlocks = n*n;
	blocks = new TerrainBlock *[numBlocks];  //create some new blocks

	//now we want to fill the blocks
	for(int s = 0; s < n; s++){
		for(int t = 0; t < n; t++){
			TerrainBlock *myTri = new TerrainBlock();
			
			float S = s * blocklength;
			float T = t * blocklength;

			myTri->initMesh(depth, S, S + blocklength, T);   //init the mesh

			if(s > 0){  //hook up to the left
				myTri->meshWith(blocks[(s-1)*n + t],0); 
			}
			if(t > 0){  //hook up underneath
				myTri->meshWith(blocks[s*n + (t-1)],3); 
			}
			if(s == n-1){  //hook right to root
				myTri->meshWith(blocks[(0)*n + t],2); 
			}
			if(t == n-1){  //hook up above to root
				myTri->meshWith(blocks[(s)*n + 0],1); 
			}

			myTri->computeMesh(0.95f, 0.0f, 4.0f);  //compute the mesh

			blocks[s*n + t] = myTri;  //save it
		}
	}

	//now compute the average of the norms for all points
	
	long numTriangles = N*N*2;
	for(int S = 0; S < n; S++){
		for(int T = 0; T < n; T++){

			TerrainBlock *TB = getBlock(S,T);  //grab the block

			for(int s = 0; s <= N; s ++)  //run through the s direction
			{
				for(int t = 0; t <= N; t ++) //run through the t direction
				{

					//a given vertex has 6 triangles around it
					float nsum[] = {0,0,0};

					int tris[] = {0,0,0,
								 -1,0,1,
								 -1,0,0,
								 -1,-1,1,
								  0,-1,0,
								  0,-1,1};
					for(int j = 0; j < 6; j++){
						int triS = s + tris[3*j + 0];
						int triT = t + tris[3*j + 1];
						int dim = tris[3*j + 2];
						TerrainBlock *tTB = getBlockAt(triS,triT,S,T);  //get the block this thingy is in
						triS = tTB->fitIn(triS); //fit it inside the block
						triT = tTB->fitIn(triT); 
						for(int i = 0;  i < 3; i++) {   //add it
							nsum[i] += tTB->getTriNorm(triS, triT, dim, i); 
						}
					}

					float mag = sqrt(nsum[0]*nsum[0] + nsum[1]*nsum[1] + nsum[2]*nsum[2]);
					nsum[0] /= mag;
					nsum[1] /= mag;
					nsum[2] /= mag;
			
					//save it
					TB->setNormal(s,t,0,nsum[0]);
					TB->setNormal(s,t,1,nsum[1]);
					TB->setNormal(s,t,2,nsum[2]);
				}
			}
		}
	}

}

Terrain::~Terrain()
{
//	for(int i = 0;  i < n*n; i++){
//		delete [i] blocks;  //clear the block
//	}
	delete blocks;  //clear memory
}

int Terrain::getN()
{
	return n;
}

int Terrain::getD()
{
	return d;
}

TerrainBlock *Terrain::getBlockAtXY(float x, float y)
{
	float s = xToS(x,y);
	float t = yToT(s,y);
	return getBlockAtST(s,t);
}

TerrainBlock *Terrain::getBlockAtST(float s, float t)
{
	int indexS = 0;
	int indexT = 0;

	//get s index
	while(s > blockLength){
		s -= blockLength;
		indexS ++;
	}
	while(s < 0){
		s += blockLength;
		indexS --;
	}

	//get t index
	while(t > blockLength){
		t -= blockLength;
		indexT++;
	}

	while(t < 0){
		t += blockLength;
		indexT --;
	}

	while(indexT < 0){
		indexT += n;
	}
	while(indexT >= n){
		indexT -= n;
	}

	while(indexS < 0){
		indexS += n;
	}
	while(indexS >= n){
		indexS -= n;
	}

	return blocks[indexS*n + indexT];  //return the block
}

TerrainBlock *Terrain::getBlock(int s, int t)
{
	
	//bounded input
	while(s < 0){
		s += n;
	}
	while(s >= n){
		s -= n;
	}
	while(t < 0){
		t += n;
	}
	while(t >= n){
		t -= n;
	}

	return blocks[s*n + t];
}

/**
 * Returns the traingle block which is at (S + s, T + t), where S & T are the big block
 * dimensions and s,t are the small block dimensions
 * this only works for blocks in the neighborhood of (S,T) +- 1
 */
TerrainBlock *Terrain::getBlockAt(int s, int t, int S, int T)
{

	TerrainBlock *retval;
	if(s >= 0 && s < N && t >= 0 && t < N){  //standard case, inside the mesh
		retval = getBlock(S,T);
	}
	else if(s < 0 && t >= 0 && t < N){
		retval = getBlock(S - 1, T);
	}
	else if(s >= N && t >= 0 && t < N){
		retval = getBlock(S + 1,T);
	}
	else if(s >= 0 && s < N && t < 0){
		retval = getBlock(S, T - 1);
	}
	else if(s >= 0 && s < N && t >= N){
		retval = getBlock(S, T + 1);
	}
	else if(s <0 && t < 0){
		retval = getBlock(S - 1, T-1);
	}
	else if(s < 0 && t >= N){
		retval = getBlock(S - 1, T + 1);
	}
	else if(s >= N && t < 0){
		retval = getBlock(S + 1, T - 1);
	}
	else{
		retval = getBlock(S + 1, T + 1);
	}

	return retval;
}

/**
 * Converts from triangle space to rectangle space for dim x
 */
float Terrain::xToS(float x, float y)
{
	return (x - RT3DIV3 * y);
}

/**
 * Converts from triangle space to rectangle space for dim y
 */
float Terrain::yToT(float x, float y)
{
	return (RT3DIV3 * 2.0f * y);
}

/**
 * Converts from rectangle space to traingle space for dim s
 */
float Terrain::sToX(float s, float t)
{
	return (s + 0.5f * t);
	
}

/**
 * Converts from rectangle space to traingle space for dim t
 */
float Terrain::tToY(float s, float t)
{
	return (RT3DIV2 * t);
}

/**
 * returns the side length of the blocks in the set
 */
float Terrain::getSideLength()
{
	return blockLength;
}

/**
 * returns the side length of the full terrain set
 */
float Terrain::getTerrainSideLength()
{
	return blockLength * n;
}