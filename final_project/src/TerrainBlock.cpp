#include "TerrainBlock.h"
#include <iostream> 
#include <ctime> 
#include <stdio.h>
#include <math.h>

#define DUMMY_VAL -9999.0f     

TerrainBlock::TerrainBlock()
{
	n = 0;
}


TerrainBlock::TerrainBlock(int d, float H, float minHeight, float maxHeight, float minX, float maxX, float minY)
{

	n = 1 << (d - 1);    
	float range = maxHeight - minHeight;    
	float adjustment = pow(2.0f, -H);    
	float maxDisp = range / 2.0f;          

	int numPts = (n+1)*(n+1);    
	pts = new float[3*numPts];    
	colors = new float[3*numPts];    
	normals = new float[3*numPts];    

	  
	set(0, 0, 2, minHeight + range * randf());
	set(n, 0, 2, minHeight + range * randf());
	set(0, n, 2, minHeight + range * randf());
	set(n, n, 2, minHeight + range * randf());

	  
	int N = n;
	while (N > 1)
	{
		maxDisp *= adjustment;    

		for(int s = 0; s <= n; s += N)    
		{
			for(int t = 0; t <= n; t += N)   
			{
				  
				if (s != n)
				{
					float mid = (get(s,t, 2) + get(s + N, t, 2)) / 2.0f;    
					mid += maxDisp * (randf() - 0.5f) * 2.0f;    
					set(s + N / 2, t, 2, mid);    

					  
					if(t > 0)
					{
						mid = (get(s,t, 2) + get(s + N, t - N, 2)) / 2.0f;    
						mid += maxDisp * (randf() - 0.5f) * 2.0f;    
						set(s + N / 2, t - N / 2, 2, mid);    
					}
				}

				  
				if (t != n)
				{
					float mid = (get(s,t,2) + get(s, t + N,2)) / 2.0f;    
					mid += maxDisp * (randf() - 0.5f) * 2.0f;    
					set(s, t + N / 2, 2, mid);    
				}
			}
		}

		N >>= 1;    
	}

	  
	float xRange = maxX - minX;
	float xDelta = xRange / (float)n;

	float rt3div2 = sqrt(3.0f) / 2.0f;    
	
	for(int s = 0; s <= n; s ++)    
	{
		for(int t = 0; t <= n; t ++)   
		{
			float x = (s + 0.5f * t) * xDelta + minX;
			float y = rt3div2 * t * xDelta + minY;
			
			  
			set(s,t,0,x);
			set(s,t,1,y);

			  
			setColorOffHeight(s, t, get(s,t,2));
		}
	}


	long numTriangles = n*n*2;    
	float* triangleNorms = new float[numTriangles*3];    

	for(int s = 0; s < n; s ++)    
	{
		for(int t = 0; t < n; t ++)   
		{
			  
			float n1[] = {0,0,0};    
			float u1[] = {get(s+1,t,0) - get(s,t,0), get(s+1,t,1) - get(s,t,1), get(s+1,t,2) - get(s,t,2)};
			float v1[] = {get(s,t+1,0) - get(s,t,0), get(s,t+1,1) - get(s,t,1), get(s,t+1,2) - get(s,t,2)};
			cross(u1,v1,n1);    

			  
			int ind = (2*s + 2*n*t)*3+ 0;
			triangleNorms[(2*s + 2*n*t)*3+ 0] = n1[0];
			triangleNorms[(2*s + 2*n*t)*3+ 1] = n1[1];
			triangleNorms[(2*s + 2*n*t)*3+ 2] = n1[2];

			  
			float n2[] = {0,0,0};    
			float u2[] = {get(s+1,t,0) - get(s+1,t+1,0), get(s+1,t,1) - get(s+1,t+1,1), get(s+1,t,2) - get(s+1,t+1,2)};
			float v2[] = {get(s,t+1,0) - get(s+1,t+1,0), get(s,t+1,1) - get(s+1,t+1,1), get(s,t+1,2) - get(s+1,t+1,2)};
			cross(v2,u2,n2);    

			  
			triangleNorms[(2*s + 2*n*t + 1)*3+ 0] = n2[0];
			triangleNorms[(2*s + 2*n*t + 1)*3+ 1] = n2[1];
			triangleNorms[(2*s + 2*n*t + 1)*3+ 2] = n2[2];
		}
	}

	  
	for(int s = 0; s <= n; s ++)    
	{
		for(int t = 0; t <= n; t ++)   
		{
			  
			float nsum[] = {0,0,0};

			int index[] = {2*s + 2*n*t - 2*n, 
				           2*s + 2*n*t - 2*n + 1,
						   2*s + 2*n*t,
						   2*s + 2*n*t-1,
						   2*s + 2*n*t - 2,
						   2*s + 2*n*t - 2*n - 1};    
			for(int tri = 0; tri < 6; tri++){
				if(index[tri] > 0 && index[tri] < numTriangles){    
					  
					nsum[0] += triangleNorms[index[tri]*3 + 0];
					nsum[1] += triangleNorms[index[tri]*3 + 1];
					nsum[2] += triangleNorms[index[tri]*3 + 2];
				}
			}

			unitize(nsum);    
			
			  
			setNormal(s,t,0,nsum[0]);
			setNormal(s,t,1,nsum[1]);
			setNormal(s,t,2,nsum[2]);
		}
	}

	delete triangleNorms;    
	
}


void TerrainBlock::initMesh(int d, float minS, float maxS, float minT)
{
	n = 1 << (d - 1);    

	int numPts = (n+1)*(n+1);    
	pts = new float[3*numPts];    
	colors = new float[3*numPts];    
	normals = new float[3*numPts];    

	

	  
	
	float rt3div2 = sqrt(3.0f) / 2.0f;    
	float sRange = maxS - minS;
	float sDelta = sRange / (float)n;
	
	for(int s = 0; s <= n; s ++)    
	{
		for(int t = 0; t <= n; t ++)   
		{
			float S = s * sDelta + minS;
			float T = t * sDelta + minT;

			float x = S + 0.5f * T;
			float y = rt3div2 * T;
			
			  
			set(s,t,0,x);
			set(s,t,1,y);
			set(s,t,2,DUMMY_VAL);
		}
	}
}


void TerrainBlock::meshWith(TerrainBlock *TB, int dim)
{
	if(TB->getN() != getN()){
		return;    
	}

	if(dim == 0){    
		int s = 0;
		int S = n;
		for(int t = 0; t <= n; t++){
			set(s,t,2,TB->get(S,t,2));    
		}
	}
	else if(dim == 1){   
		int t = n;
		int T = 0;
		for(int s = 0; s <= n; s++){
			set(s,t,2,TB->get(s,T,2));    
		}
	}
	else if(dim == 2){   
		int s = n;
		int S = 0;
		for(int t = 0; t <= n; t++){
			set(s,t,2,TB->get(S,t,2));    
		}
	}
	else if(dim == 3){   
		int t = 0;
		int T = n;
		for(int s = 0; s <= n; s++){
			set(s,t,2,TB->get(s,T,2));    
		}
	}
}


void TerrainBlock::computeMesh(float H, float minHeight, float maxHeight)
{
	float range = maxHeight - minHeight;    
	float adjustment = pow(2.0f, -H);    
	float maxDisp = range / 2.0f;          

	  
	if(get(0,0,2) == DUMMY_VAL){ set(0, 0, 2, minHeight + range * randf()); }
	if(get(n,0,2) == DUMMY_VAL){ set(n, 0, 2, minHeight + range * randf()); }
	if(get(0,n,2) == DUMMY_VAL){ set(0, n, 2, minHeight + range * randf()); }
	if(get(n,n,2) == DUMMY_VAL){ set(n, n, 2, minHeight + range * randf()); }

	  
	int N = n;
	while (N > 1)
	{
		maxDisp *= adjustment;    

		for(int s = 0; s <= n; s += N)    
		{
			for(int t = 0; t <= n; t += N)   
			{
				  
				if (s != n)
				{
					if(get(s + N/2, t, 2) == DUMMY_VAL){    
						float mid = (get(s,t, 2) + get(s + N, t, 2)) / 2.0f;    
						mid += maxDisp * (randf() - 0.5f) * 2.0f;    
						set(s + N / 2, t, 2, mid);    
					}

					  
					if(t > 0 && get(s + N/2, t - N/2, 2) == DUMMY_VAL)
					{
						float mid = (get(s,t, 2) + get(s + N, t - N, 2)) / 2.0f;    
						mid += maxDisp * (randf() - 0.5f) * 2.0f;    
						set(s + N / 2, t - N / 2, 2, mid);    
					}
				}

				  
				if (t != n && get(s, t + N/2, 2) == DUMMY_VAL)
				{
					float mid = (get(s,t,2) + get(s, t + N,2)) / 2.0f;    
					mid += maxDisp * (randf() - 0.5f) * 2.0f;    
					set(s, t + N / 2, 2, mid);    
				}
			}
		}

		N >>= 1;    
	}

	  
	for(int s = 0; s <= n; s ++)    
	{
		for(int t = 0; t <= n; t ++)   
		{
			  
			setColorOffHeight(s, t, get(s,t,2));
		}
	}

	long numTriangles = n*n*2;    
	triangleNorms = new float[numTriangles*3];    

	for(int s = 0; s < n; s ++)    
	{
		for(int t = 0; t < n; t ++)   
		{
			  
			float n1[] = {0,0,0};    
			float u1[] = {get(s+1,t,0) - get(s,t,0), get(s+1,t,1) - get(s,t,1), get(s+1,t,2) - get(s,t,2)};
			float v1[] = {get(s,t+1,0) - get(s,t,0), get(s,t+1,1) - get(s,t,1), get(s,t+1,2) - get(s,t,2)};
			cross(u1,v1,n1);    

			  
			setTriNorm(s,t,0,0,n1[0]);
			setTriNorm(s,t,0,1,n1[1]);
			setTriNorm(s,t,0,2,n1[2]);

			  
			float n2[] = {0,0,0};    
			float u2[] = {get(s+1,t,0) - get(s+1,t+1,0), get(s+1,t,1) - get(s+1,t+1,1), get(s+1,t,2) - get(s+1,t+1,2)};
			float v2[] = {get(s,t+1,0) - get(s+1,t+1,0), get(s,t+1,1) - get(s+1,t+1,1), get(s,t+1,2) - get(s+1,t+1,2)};
			cross(v2,u2,n2);    

			  
			setTriNorm(s,t,1,0,n2[0]);
			setTriNorm(s,t,1,1,n2[1]);
			setTriNorm(s,t,1,2,n2[2]);
		
		}
	}
}


void TerrainBlock::setColorOffHeight(int s, int t, float height)
{

	  
	float r = 1.0;
	float g = 1.0;
	float b = 1.0;


	if(height < 4.0 && height >= 3.0){    
		float alpha = (height - 3.0f);
		r = 0.5f + alpha * 0.5f;
		g = r;
		b = r;
	}
	else if(height < 3.0 && height >= 1.0){    
		float alpha = (height - 1.0f)/2.0f;
		r = 0.2f + alpha * 0.3f;
		g = 1.0f + alpha * -0.5f;
		b = 0.2f + alpha * 0.3f;
	}
	else if(height < 1.0 && height >= -1.0){    
		float alpha = (height + 1.0f)/2.0f;
		r = 0.6f + alpha * -0.4f;
		g = 0.4f + alpha * 0.6f;
		b = 0.2f;
	}

	setColor(s,t,0,r);    
	setColor(s,t,1,g);    
	setColor(s,t,2,b);    
	
}


TerrainBlock::~TerrainBlock()
{
	  
	delete[] pts;  
	delete[] colors;
	delete[] normals;
	delete[] triangleNorms;
}


int TerrainBlock::getIndex(int s, int t, int dim)
{
	return dim + 3*(s + (n+1) * t);
}

  
int TerrainBlock::getIndexTri(int s, int t, int half, int dim)
{
	return dim + 3*(half + 2*(s + n*t));
}


void TerrainBlock::set(int s, int t, int dim, float val)
{
	int index = getIndex(s,t,dim);
	pts[index] = val;
}

void TerrainBlock::setColor(int s, int t, int dim, float val)
{
	int index = getIndex(s,t,dim);
	colors[index] = val; 
}

void TerrainBlock::setNormal(int s, int t, int dim, float val)
{
	int index = getIndex(s,t,dim);
	normals[index] = val; 
}

void TerrainBlock::setTriNorm(int s, int t, int half, int dim, float val)
{
	int index = getIndexTri(s,t,half,dim);
	triangleNorms[index] = val; 
}


float TerrainBlock::get(int s, int t, int dim)
{
	int index = getIndex(s,t,dim);
	return pts[index];
}

float TerrainBlock::getColor(int s, int t, int dim)
{
	int index = getIndex(s,t,dim);
	return colors[index];
}

float TerrainBlock::getNormal(int s, int t, int dim)
{
	int index = getIndex(s,t,dim);
	return normals[index];
}

float TerrainBlock::getTriNorm(int s, int t, int half, int dim)
{
	int index = getIndexTri(s,t,half,dim);
	return triangleNorms[index];
}

float TerrainBlock::getTriNorm(int index)
{
	return triangleNorms[index];
}


int TerrainBlock::getN()
{
	return n;
}


int TerrainBlock::fitIn(int dim)
{
	while(dim < 0){
		dim += n;
	}
	while(dim >= n){
		dim -= n;
	}
	return dim;
}


float TerrainBlock::randf()
{
	return rand()/((float)RAND_MAX);   
}


void TerrainBlock::cross(float* a, float* b, float* v)
{
	v[0] = a[1]*b[2] - a[2]*b[1];
	v[1] = a[2]*b[0] - a[0]*b[2];
	v[2] = a[0]*b[1] - a[1]*b[0];
}

void TerrainBlock::unitize(float* u){
	float mag = sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
	u[0] /= mag;
	u[1] /= mag;
	u[2] /= mag;
}