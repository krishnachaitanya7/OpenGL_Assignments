#include "Terrain.h"
#include "TerrainBlock.h"
#include <iostream> 
#include <ctime> 
#include <stdio.h>
#include <math.h>

Terrain::Terrain(int numBlocksSide, int depth, float roughness, float minheight, float maxheight, float blocklength)
{
	  
	n = numBlocksSide;
	d = depth;
	N = (1 << (d - 1));
	H = roughness;
	minHeight = minheight;
	maxHeight = maxheight;
	blockLength = blocklength;

	  
	srand((unsigned)time(0));  

	int numBlocks = n*n;
	blocks = new TerrainBlock *[numBlocks];    

	  
	for(int s = 0; s < n; s++){
		for(int t = 0; t < n; t++){
			TerrainBlock *myTri = new TerrainBlock();
			
			float S = s * blocklength;
			float T = t * blocklength;

			myTri->initMesh(depth, S, S + blocklength, T);     

			if(s > 0){    
				myTri->meshWith(blocks[(s-1)*n + t],0); 
			}
			if(t > 0){    
				myTri->meshWith(blocks[s*n + (t-1)],3); 
			}
			if(s == n-1){    
				myTri->meshWith(blocks[(0)*n + t],2); 
			}
			if(t == n-1){    
				myTri->meshWith(blocks[(s)*n + 0],1); 
			}

			myTri->computeMesh(0.95f, 0.0f, 4.0f);    

			blocks[s*n + t] = myTri;    
		}
	}

	  
	
	long numTriangles = N*N*2;
	for(int S = 0; S < n; S++){
		for(int T = 0; T < n; T++){

			TerrainBlock *TB = getBlock(S,T);    

			for(int s = 0; s <= N; s ++)    
			{
				for(int t = 0; t <= N; t ++)   
				{

					  
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
						TerrainBlock *tTB = getBlockAt(triS,triT,S,T);    
						triS = tTB->fitIn(triS);   
						triT = tTB->fitIn(triT); 
						for(int i = 0;  i < 3; i++) {     
							nsum[i] += tTB->getTriNorm(triS, triT, dim, i); 
						}
					}

					float mag = sqrt(nsum[0]*nsum[0] + nsum[1]*nsum[1] + nsum[2]*nsum[2]);
					nsum[0] /= mag;
					nsum[1] /= mag;
					nsum[2] /= mag;
			
					  
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
  
  
  
	delete blocks;    
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

	  
	while(s > blockLength){
		s -= blockLength;
		indexS ++;
	}
	while(s < 0){
		s += blockLength;
		indexS --;
	}

	  
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

	return blocks[indexS*n + indexT];    
}

TerrainBlock *Terrain::getBlock(int s, int t)
{
	
	  
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


TerrainBlock *Terrain::getBlockAt(int s, int t, int S, int T)
{

	TerrainBlock *retval;
	if(s >= 0 && s < N && t >= 0 && t < N){    
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


float Terrain::xToS(float x, float y)
{
	return (x - RT3DIV3 * y);
}


float Terrain::yToT(float x, float y)
{
	return (RT3DIV3 * 2.0f * y);
}


float Terrain::sToX(float s, float t)
{
	return (s + 0.5f * t);
	
}


float Terrain::tToY(float s, float t)
{
	return (RT3DIV2 * t);
}


float Terrain::getSideLength()
{
	return blockLength;
}


float Terrain::getTerrainSideLength()
{
	return blockLength * n;
}