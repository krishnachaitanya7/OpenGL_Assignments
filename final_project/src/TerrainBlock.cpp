#include "TerrainBlock.h"
#include <iostream> 
#include <ctime> 
#include <stdio.h>
#include <math.h>

#define DUMMY_VAL -9999.0f   //used to represent a value that can be replaced

TerrainBlock::TerrainBlock()
{
	n = 0;
}


TerrainBlock::TerrainBlock(int d, float H, float minHeight, float maxHeight, float minX, float maxX, float minY)
{

	n = 1 << (d - 1);  //calculate the edge length of the block
	float range = maxHeight - minHeight;  //get the range
	float adjustment = pow(2.0f, -H);  //determine the roughness value
	float maxDisp = range / 2.0f;        //get the max displacement

	int numPts = (n+1)*(n+1);  //the number of points
	pts = new float[3*numPts];  //allocate the points
	colors = new float[3*numPts];  //allocate the colors
	normals = new float[3*numPts];  //allocate the normals

	//now generate the starting vertexes
	set(0, 0, 2, minHeight + range * randf());
	set(n, 0, 2, minHeight + range * randf());
	set(0, n, 2, minHeight + range * randf());
	set(n, n, 2, minHeight + range * randf());

	//now run through (d-1) times
	int N = n;
	while (N > 1)
	{
		maxDisp *= adjustment;  //decrease the maximum displacement

		for(int s = 0; s <= n; s += N)  //run through the s direction
		{
			for(int t = 0; t <= n; t += N) //run through the t direction
			{
				//hook up the s direction if possible
				if (s != n)
				{
					float mid = (get(s,t, 2) + get(s + N, t, 2)) / 2.0f;  //get the midpoint height
					mid += maxDisp * (randf() - 0.5f) * 2.0f;  //displace it
					set(s + N / 2, t, 2, mid);  //set it

					//hook up the diagonal if possible
					if(t > 0)
					{
						mid = (get(s,t, 2) + get(s + N, t - N, 2)) / 2.0f;  //get the midpoint height
						mid += maxDisp * (randf() - 0.5f) * 2.0f;  //displace it
						set(s + N / 2, t - N / 2, 2, mid);  //set it
					}
				}

				//hook up the t direction if possible
				if (t != n)
				{
					float mid = (get(s,t,2) + get(s, t + N,2)) / 2.0f;  //get the midpoint height
					mid += maxDisp * (randf() - 0.5f) * 2.0f;  //displace it
					set(s, t + N / 2, 2, mid);  //set it
				}
			}
		}

		N >>= 1;  //divide by 2
	}

	//calculate x, y, & color
	float xRange = maxX - minX;
	float xDelta = xRange / (float)n;

	float rt3div2 = sqrt(3.0f) / 2.0f;  //used for translation from square to triangle space
	
	for(int s = 0; s <= n; s ++)  //run through the s direction
	{
		for(int t = 0; t <= n; t ++) //run through the t direction
		{
			float x = (s + 0.5f * t) * xDelta + minX;
			float y = rt3div2 * t * xDelta + minY;
			
			//set the value
			set(s,t,0,x);
			set(s,t,1,y);

			//calculate color
			setColorOffHeight(s, t, get(s,t,2));
		}
	}


	long numTriangles = n*n*2;  //how many triangles we have
	float* triangleNorms = new float[numTriangles*3];  //allocate a temp array

	for(int s = 0; s < n; s ++)  //run through the s direction
	{
		for(int t = 0; t < n; t ++) //run through the t direction
		{
			//calculate the lower-left triangle normal
			float n1[] = {0,0,0};  //allocate
			float u1[] = {get(s+1,t,0) - get(s,t,0), get(s+1,t,1) - get(s,t,1), get(s+1,t,2) - get(s,t,2)};
			float v1[] = {get(s,t+1,0) - get(s,t,0), get(s,t+1,1) - get(s,t,1), get(s,t+1,2) - get(s,t,2)};
			cross(u1,v1,n1);  //calculate the normal vector

			//save it
			int ind = (2*s + 2*n*t)*3+ 0;
			triangleNorms[(2*s + 2*n*t)*3+ 0] = n1[0];
			triangleNorms[(2*s + 2*n*t)*3+ 1] = n1[1];
			triangleNorms[(2*s + 2*n*t)*3+ 2] = n1[2];

			//calculate the top-right triangle normal
			float n2[] = {0,0,0};  //allocate
			float u2[] = {get(s+1,t,0) - get(s+1,t+1,0), get(s+1,t,1) - get(s+1,t+1,1), get(s+1,t,2) - get(s+1,t+1,2)};
			float v2[] = {get(s,t+1,0) - get(s+1,t+1,0), get(s,t+1,1) - get(s+1,t+1,1), get(s,t+1,2) - get(s+1,t+1,2)};
			cross(v2,u2,n2);  //calculate the normal vector

			//save it
			triangleNorms[(2*s + 2*n*t + 1)*3+ 0] = n2[0];
			triangleNorms[(2*s + 2*n*t + 1)*3+ 1] = n2[1];
			triangleNorms[(2*s + 2*n*t + 1)*3+ 2] = n2[2];
		}
	}

	//now compute the average of the norms for all points
	for(int s = 0; s <= n; s ++)  //run through the s direction
	{
		for(int t = 0; t <= n; t ++) //run through the t direction
		{
			//a given vertex has 6 triangles around it
			float nsum[] = {0,0,0};

			int index[] = {2*s + 2*n*t - 2*n, 
				           2*s + 2*n*t - 2*n + 1,
						   2*s + 2*n*t,
						   2*s + 2*n*t-1,
						   2*s + 2*n*t - 2,
						   2*s + 2*n*t - 2*n - 1};  //indexes for the 6 triangles
			for(int tri = 0; tri < 6; tri++){
				if(index[tri] > 0 && index[tri] < numTriangles){  //if said triangle exists
					//add it to the average
					nsum[0] += triangleNorms[index[tri]*3 + 0];
					nsum[1] += triangleNorms[index[tri]*3 + 1];
					nsum[2] += triangleNorms[index[tri]*3 + 2];
				}
			}

			unitize(nsum);  //get the unit vector
			
			//save it
			setNormal(s,t,0,nsum[0]);
			setNormal(s,t,1,nsum[1]);
			setNormal(s,t,2,nsum[2]);
		}
	}

	delete triangleNorms;  //delete the temp array
	
}

/**
 * Initialize the mesh by declaring the arrays and spacing out the x-direction
 * The value DUMMY_VAL is given to all heights such that they can be marked for replacement in computeMesh
 */
void TerrainBlock::initMesh(int d, float minS, float maxS, float minT)
{
	n = 1 << (d - 1);  //calculate the edge length of the block

	int numPts = (n+1)*(n+1);  //the number of points
	pts = new float[3*numPts];  //allocate the points
	colors = new float[3*numPts];  //allocate the colors
	normals = new float[3*numPts];  //allocate the normals

	

	//calculate x & y, set height to DUMMY_VAL
	
	float rt3div2 = sqrt(3.0f) / 2.0f;  //used for translation from square to triangle space
	float sRange = maxS - minS;
	float sDelta = sRange / (float)n;
	
	for(int s = 0; s <= n; s ++)  //run through the s direction
	{
		for(int t = 0; t <= n; t ++) //run through the t direction
		{
			float S = s * sDelta + minS;
			float T = t * sDelta + minT;

			float x = S + 0.5f * T;
			float y = rt3div2 * T;
			
			//set the values
			set(s,t,0,x);
			set(s,t,1,y);
			set(s,t,2,DUMMY_VAL);
		}
	}
}

/**
 * Set the values on the side of this mesh to merge with those of the given mesh
 * dim = 0 merges s = 0
 * dim = 1 merges t = max
 * dim = 2 merges s = max
 * dim = 3 merges t = 0
 * TB must have the same n value as this terrain block
 */
void TerrainBlock::meshWith(TerrainBlock *TB, int dim)
{
	if(TB->getN() != getN()){
		return;  //cannot do!
	}

	if(dim == 0){  //merge s = 0
		int s = 0;
		int S = n;
		for(int t = 0; t <= n; t++){
			set(s,t,2,TB->get(S,t,2));  //set it to be the same
		}
	}
	else if(dim == 1){ //merge t = n
		int t = n;
		int T = 0;
		for(int s = 0; s <= n; s++){
			set(s,t,2,TB->get(s,T,2));  //set it to be the same
		}
	}
	else if(dim == 2){ //merge s = n
		int s = n;
		int S = 0;
		for(int t = 0; t <= n; t++){
			set(s,t,2,TB->get(S,t,2));  //set it to be the same
		}
	}
	else if(dim == 3){ //merge t = 0
		int t = 0;
		int T = n;
		for(int s = 0; s <= n; s++){
			set(s,t,2,TB->get(s,T,2));  //set it to be the same
		}
	}
}

/**
 * Compute the mesh, replacing all DUMMY_VAL values with nicely computed midpoint
 * displacements.
 * Will also compute the colors and the normal vectors
 * Note, we assume that initMesh() has been called
 */
void TerrainBlock::computeMesh(float H, float minHeight, float maxHeight)
{
	float range = maxHeight - minHeight;  //get the range
	float adjustment = pow(2.0f, -H);  //determine the roughness value
	float maxDisp = range / 2.0f;        //get the max displacement

	//now generate the starting vertexes, as appropriate
	if(get(0,0,2) == DUMMY_VAL){ set(0, 0, 2, minHeight + range * randf()); }
	if(get(n,0,2) == DUMMY_VAL){ set(n, 0, 2, minHeight + range * randf()); }
	if(get(0,n,2) == DUMMY_VAL){ set(0, n, 2, minHeight + range * randf()); }
	if(get(n,n,2) == DUMMY_VAL){ set(n, n, 2, minHeight + range * randf()); }

	//now run through (d-1) times
	int N = n;
	while (N > 1)
	{
		maxDisp *= adjustment;  //decrease the maximum displacement

		for(int s = 0; s <= n; s += N)  //run through the s direction
		{
			for(int t = 0; t <= n; t += N) //run through the t direction
			{
				//hook up the s direction if possible
				if (s != n)
				{
					if(get(s + N/2, t, 2) == DUMMY_VAL){  //if necessary
						float mid = (get(s,t, 2) + get(s + N, t, 2)) / 2.0f;  //get the midpoint height
						mid += maxDisp * (randf() - 0.5f) * 2.0f;  //displace it
						set(s + N / 2, t, 2, mid);  //set it
					}

					//hook up the diagonal if possible and necessary
					if(t > 0 && get(s + N/2, t - N/2, 2) == DUMMY_VAL)
					{
						float mid = (get(s,t, 2) + get(s + N, t - N, 2)) / 2.0f;  //get the midpoint height
						mid += maxDisp * (randf() - 0.5f) * 2.0f;  //displace it
						set(s + N / 2, t - N / 2, 2, mid);  //set it
					}
				}

				//hook up the t direction if possible & necessary
				if (t != n && get(s, t + N/2, 2) == DUMMY_VAL)
				{
					float mid = (get(s,t,2) + get(s, t + N,2)) / 2.0f;  //get the midpoint height
					mid += maxDisp * (randf() - 0.5f) * 2.0f;  //displace it
					set(s, t + N / 2, 2, mid);  //set it
				}
			}
		}

		N >>= 1;  //divide by 2
	}

	//calculate color
	for(int s = 0; s <= n; s ++)  //run through the s direction
	{
		for(int t = 0; t <= n; t ++) //run through the t direction
		{
			//calculate color
			setColorOffHeight(s, t, get(s,t,2));
		}
	}

	long numTriangles = n*n*2;  //how many triangles we have
	triangleNorms = new float[numTriangles*3];  //allocate a temp array

	for(int s = 0; s < n; s ++)  //run through the s direction
	{
		for(int t = 0; t < n; t ++) //run through the t direction
		{
			//calculate the lower-left triangle normal
			float n1[] = {0,0,0};  //allocate
			float u1[] = {get(s+1,t,0) - get(s,t,0), get(s+1,t,1) - get(s,t,1), get(s+1,t,2) - get(s,t,2)};
			float v1[] = {get(s,t+1,0) - get(s,t,0), get(s,t+1,1) - get(s,t,1), get(s,t+1,2) - get(s,t,2)};
			cross(u1,v1,n1);  //calculate the normal vector

			//save it
			setTriNorm(s,t,0,0,n1[0]);
			setTriNorm(s,t,0,1,n1[1]);
			setTriNorm(s,t,0,2,n1[2]);

			//calculate the top-right triangle normal
			float n2[] = {0,0,0};  //allocate
			float u2[] = {get(s+1,t,0) - get(s+1,t+1,0), get(s+1,t,1) - get(s+1,t+1,1), get(s+1,t,2) - get(s+1,t+1,2)};
			float v2[] = {get(s,t+1,0) - get(s+1,t+1,0), get(s,t+1,1) - get(s+1,t+1,1), get(s,t+1,2) - get(s+1,t+1,2)};
			cross(v2,u2,n2);  //calculate the normal vector

			//save it
			setTriNorm(s,t,1,0,n2[0]);
			setTriNorm(s,t,1,1,n2[1]);
			setTriNorm(s,t,1,2,n2[2]);
		
		}
	}
}


void TerrainBlock::setColorOffHeight(int s, int t, float height)
{

	//init to white
	float r = 1.0;
	float g = 1.0;
	float b = 1.0;


	if(height < 4.0 && height >= 3.0){  //gray  (0.5, 0.5, 0.5)
		float alpha = (height - 3.0f);
		r = 0.5f + alpha * 0.5f;
		g = r;
		b = r;
	}
	else if(height < 3.0 && height >= 1.0){  //green (0.2, 1.0, 0.2)
		float alpha = (height - 1.0f)/2.0f;
		r = 0.2f + alpha * 0.3f;
		g = 1.0f + alpha * -0.5f;
		b = 0.2f + alpha * 0.3f;
	}
	else if(height < 1.0 && height >= -1.0){  //red (0.6, 0.4, 0.2)
		float alpha = (height + 1.0f)/2.0f;
		r = 0.6f + alpha * -0.4f;
		g = 0.4f + alpha * 0.6f;
		b = 0.2f;
	}

	setColor(s,t,0,r);  //red
	setColor(s,t,1,g);  //green
	setColor(s,t,2,b);  //blue
	
}

/**
* deletes it
*/
TerrainBlock::~TerrainBlock()
{
	//delete our arrays
	delete[] pts;  
	delete[] colors;
	delete[] normals;
	delete[] triangleNorms;
}

/**
 * Returns the index in pts for the given location
 */
int TerrainBlock::getIndex(int s, int t, int dim)
{
	return dim + 3*(s + (n+1) * t);
}

//half is 0 of bottom half, 1 if top half
int TerrainBlock::getIndexTri(int s, int t, int half, int dim)
{
	return dim + 3*(half + 2*(s + n*t));
}

/**
 * sets the given value
 */
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

/**
 * returns the value
 */
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

/**
 * returns the block dimension
 */
int TerrainBlock::getN()
{
	return n;
}

/**
 *  Fits dim inside [0,n)
 */
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

/**
 * Returns a random number between 0.0f and 1.0f
 */
float TerrainBlock::randf()
{
	return rand()/((float)RAND_MAX);   
}

/**
 * computes a cross b and puts the result in v
 */
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