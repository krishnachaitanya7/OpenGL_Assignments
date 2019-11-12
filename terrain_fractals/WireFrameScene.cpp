/*
 * WireFrameScene.c
 *
 * Original Author: Samuel R. Buss
 * Student Author: Tim Wheeler
 *
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * 
 * Base code for homework project #3, Math 155A, Winter 2012
 *
 */

#include <math.h>			// For math routines (such as sqrt & trig).
#include <stdio.h>
#include <stdlib.h>		// For the "exit" function
#include <GL/glut.h>		// OpenGL Graphics Utility Library
#include "WireFrameScene.h"
#include "TerrainBlock.h"

///////////////////////////
/// Material Properties ///
///////////////////////////
float NoEmit[4] = {0.0, 0.0, 0.0, 1.0};

//Terrain material properties
float TerrainShininess = 10;	// Specular exponent (we don't use specular)
float TerrainAmbDiff[4] =  {0.1, 0.4, 0.1, 1.0};  // The ambient/diffuse colors
float TerrainSpecular[4] = {0.0, 0.0, 0.0, 1.0};	//the specular color (none!)

///////////////////////
/// Lighting Values ///
///////////////////////

//ambient light
float ambientLight[4] = {0.4, 0.4, 0.4, 1.0};  //ambient scene (white) light
float Ltdiff[4] = {0.2,  0.2,  0.2, 1.0};
float Ltspec[4] = {0.2, 0.2, 0.2, 1.0};

////////////////////////
///   Model Values   ///
////////////////////////

int TRI_DEPTH = 8;
int TRI_BLOCKS = 10;
Terrain *myTerrain;

////////////////////////
///   Plane Model    ///
////////////////////////

//position
float plane_x = 33.0;  //using x & y space to represent the plane location
float plane_y = 10.0;
float plane_alt = 5.2;  //the plane altitude

//dynamics
float PLANE_SPEED = 0.1;  //speed per tick
float plane_yaw = 0.0;  // yaw [rad]
float PLANE_YAW_RATE = 0.02;  //yaw rate [rad]

//gimbal
float Azimuth = 20.0f;			// Rotated up or down by this amount
float azimuth_max = 40.0f;
float azimuth_min = 0.0f;
float AngleStepSize = 3.0f;		// Step three degrees at a time
const float timeStep = 0.01;
float tick = 0;
const float AngleStepMax = 10.0f;
const float AngleStepMin = 0.1f;
const float PI = 3.14159265f; //pi

//misc
float maxRenderDepth = 3.0f;
int gradientDepth = 0;  //if 1, then render with gradient depth   if 0, render with static depth

///////////////////////////
/// Global State Values ///
///////////////////////////
int WireFrameOn = 0;			// == 1 for wire frame mode
int CullBackFacesOn = 1;		// == 1 if culling back faces.
int SmoothShading = 1;          // == 1 if smooth, 0 if flat


// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {
	case 'c':
		CullBackFacesOn = 1-CullBackFacesOn;
		if ( CullBackFacesOn ) {
			glEnable( GL_CULL_FACE );				// Enable culling of back faces
		}
		else {
			glDisable( GL_CULL_FACE );				// Show all faces (front and back)
		}
		glutPostRedisplay();
		break;
	case 'w':
		WireFrameOn = 1-WireFrameOn;
		if ( WireFrameOn ) {
			glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );		// Just show wireframes
		}
		else {
			glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );		// Show solid polygons
		}
		glutPostRedisplay();
		break;
	case 'R':
		AngleStepSize *= 1.5;
		if (AngleStepSize>AngleStepMax ) {
			AngleStepSize = AngleStepMax;
		}
		break;
	case 'r':
		AngleStepSize /= 1.5;
		if (AngleStepSize<AngleStepMin ) {
			AngleStepSize = AngleStepMin;
		}
		break;
	case 'D':
		maxRenderDepth += 0.2;
		if(maxRenderDepth > TRI_DEPTH){
			maxRenderDepth = TRI_DEPTH;
		}
		glutPostRedisplay();
		break;
	case 'd':
		maxRenderDepth -= 0.2;
		if(maxRenderDepth < 1){
			maxRenderDepth = 1;
		}
		glutPostRedisplay();
		break;
	case 'g':
		gradientDepth = (gradientDepth + 1) % 2;  //toggle state
		glutPostRedisplay();
		break;
	case 's':
		SmoothShading = (SmoothShading + 1) % 2;  //toggle state
		glutPostRedisplay();
		break;
	case 'z':
		plane_alt += 0.1f;  
		break;
	case 'Z':
		plane_alt -= 0.1f;  
		if(plane_alt < 4.0f){
			plane_alt = 4.0f;
		}
		break;
	case 'F':
		PLANE_SPEED += 0.1f;
		if(PLANE_SPEED > 2){
			PLANE_SPEED = 2;
		}
		break;
	case 'f':
		PLANE_SPEED -= 0.1f;
		if(PLANE_SPEED < 0){
			PLANE_SPEED = 0;
		}
		break;
	case 27:	// Escape key
		exit(1);
	}
}

// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of
//		special keys.
void mySpecialKeyFunc( int key, int x, int y )
{
	switch ( key ) {
	case GLUT_KEY_UP:	
		Azimuth -= AngleStepSize;
		if ( Azimuth > azimuth_max ) {
			Azimuth = azimuth_max;
		}
		break;
	case GLUT_KEY_DOWN:
		Azimuth += AngleStepSize;
		if ( Azimuth < azimuth_min ) {
			Azimuth = azimuth_min;
		}
		break;
	case GLUT_KEY_LEFT:
		plane_yaw -= PLANE_YAW_RATE;
		if ( plane_yaw < -PI ) {
			plane_yaw += 2*PI;
		}
		break;
	case GLUT_KEY_RIGHT:
		plane_yaw += PLANE_YAW_RATE;
		if ( plane_yaw >= PI ) {
			plane_yaw -= 2*PI;
		}
		break;
	}
	glutPostRedisplay();

}

/*
 * drawScene() handles the animation and the redrawing of the
 *		graphics window contents.
 */
void drawScene(void)
{
	
	tick += timeStep;
	if(tick > 10){
		tick = 0;
	}

	//move the plane, keep it in the square
	plane_y +=  PLANE_SPEED * cos(plane_yaw);
	plane_x +=  PLANE_SPEED * sin(plane_yaw);

	float planeS = myTerrain->xToS(plane_x, plane_y); //s,t loc of plane
	float planeT = myTerrain->yToT(plane_x, plane_y); 
	
	//fprintf(stdout, "%f %f %f  %f  %f\n", plane_x, plane_y, planeS, planeT, plane_alt);
	//fprintf(stdout, "%f\n", plane_yaw);

	float terrainSideLen = myTerrain->getTerrainSideLength();
	int N = myTerrain->getN();

	int shunted = 0;
	if(planeS < 0){
		planeS += terrainSideLen;
		shunted = 1;
	}
	else if(planeS >= terrainSideLen){
		planeS -= terrainSideLen;
		shunted = 1;
	}
	if(planeT < 0){
		planeT += terrainSideLen;
		shunted = 1;
	}
	else if(planeT >= terrainSideLen){
		planeT -= terrainSideLen;
		shunted = 1;
	}
	if(shunted){  //do the work if need be
		plane_x = myTerrain->sToX(planeS, planeT);  //convert back
		plane_y = myTerrain->tToY(planeS, planeT);
	}

	// Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//enable flat or smooth shading
	glShadeModel(SmoothShading == 1 ? GL_SMOOTH : GL_FLAT);  

	// Rotate the view
	glMatrixMode( GL_MODELVIEW );			// Current matrix affects objects positions
	glLoadIdentity();						// Initialize to the identity
	
	glRotatef( Azimuth, 1.0, 0.0, 0.0 ); //use the azimuth angle to look
	glRotatef( plane_yaw * 180.0 / PI, 0.0, 1.0, 0.0);  //yaw
	glTranslatef(-plane_x, -plane_alt, plane_y);  //translate to the plane's position

	//////////////////////////////////
	/////   Render the Terrain   /////
	//////////////////////////////////

	float terrainLength = myTerrain->getTerrainSideLength();
	int numGrid = 2;
	float viewRadius = terrainLength;

	float locX, locY, locS, locT, dispS, dispT, theta2, thetaR, thetaD, depth;

	float forwardPlane_x = plane_x + 2 * sin(plane_yaw);
	float forwardPlane_y = plane_y + 2 * cos(plane_yaw);
	float forwardPlane_s = myTerrain->xToS(forwardPlane_x, forwardPlane_y);
	float forwardPlane_t = myTerrain->yToT(forwardPlane_x, forwardPlane_y);

	for(int nS = -numGrid; nS <= numGrid; nS ++){
		for(int nT = -numGrid; nT <= numGrid; nT ++){
			dispS = terrainLength * nS;
			dispT = terrainLength * nT;
			for(int s = 0; s < myTerrain->getN(); s++){
				for(int t = 0; t < myTerrain->getN(); t++){

					TerrainBlock *TB = myTerrain->getBlock(s,t);
					locX = TB->get(0,0,0);
					locY = TB->get(0,0,1);
					locS = myTerrain->xToS(locX, locY) + dispS;
					locT = myTerrain->yToT(locX, locY) + dispT;

					float dS = locS - forwardPlane_s;
					float dT = locT - forwardPlane_t;
					float dist = sqrt(dS*dS + dT*dT);  //dist from tile to plane

					if(dist < viewRadius){  //only render in view
						theta2 = atan2(dT, dS);  //get angle of tile
						thetaR = PI/2.0 - theta2; //theta2;
						thetaD = thetaR - plane_yaw;

						while(thetaD < - PI){
							thetaD += 2*PI;
						}
						while(thetaD >= PI){
							thetaD -= 2*PI;
						}

						if(thetaD < PI / 2 && thetaD > -PI / 2){
							depth = (gradientDepth == 1 ? (viewRadius - dist) / viewRadius * maxRenderDepth : maxRenderDepth);
							renderBlock(TB, depth, dispS, dispT);
						}
					}
				}
			}
		}
	}

	// Flush the pipeline, swap the buffers
    glFlush();
	
    glutSwapBuffers();

	glutPostRedisplay();
}

/**
 * Renders a block to a given depth, limited to max depth
 * Depth is a float ranging from 0 to max_depth - 1.
 * x and y are the positions of the (s,t) = (0,0) point in 3D space
 * A value between two integers will cause lerping by height for the middle-range traingles
 * ex: depth = 2.5 causes a standard depth 2 display, plus the stage 3 triangles with 0.5 deviation
 *             between expected and actual height
 */
void renderBlock(TerrainBlock* TB, float depth, float dispS, float dispT)
{
	//set material properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, TerrainAmbDiff );
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, TerrainSpecular );
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, TerrainShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, NoEmit);

	float x = myTerrain->sToX(dispS, dispT);
	float y = myTerrain->tToY(dispS, dispT);

	int n = TB->getN();
	int N = n >> (int)floor(depth);  //calculate the main jump constant
	float alpha = depth - (int)floor(depth);  //get the alpha value

	if(N < 2){
		N = 2;  //clamp to 2
		alpha = 1.0;
	}

	
	int hN = N/2;  //the half N jump

	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(x,y,0);  //translate as appropriate

	float ambDiff[] = {0.0f, 0.0f, 0.0f, 1.0f};

	//draw the triangle
	for(int s = 0; s < n; s+=N)
	{
		
		for(int t = 0; t < n; t+=N)
		{
			//determine the points of the five sub-triangle vertexes
			float hA = TB->get(s+hN,t,2);     //bottom
			float hB = TB->get(s, t+hN,2);    //left
			float hC = TB->get(s+hN,t+hN,2);  //center
			float hD = TB->get(s+hN,t+N,2);   //top
			float hE = TB->get(s+N,t+hN,2);   //right

			//calculate the expected height
			float hAexpected = (TB->get(s,t, 2) + TB->get(s + N, t, 2)) / 2.0f;  
			float hBexpected = (TB->get(s,t, 2) + TB->get(s, t + N, 2)) / 2.0f;  
			float hCexpected = (TB->get(s+N,t, 2) + TB->get(s, t + N, 2)) / 2.0f;  
			float hDexpected = (TB->get(s,t+N, 2) + TB->get(s + N, t + N, 2)) / 2.0f;  
			float hEexpected = (TB->get(s+N,t, 2) + TB->get(s + N, t + N, 2)) / 2.0f;  

			//calculate the actual displayed height based on alpha
			float aHA = lerp(hAexpected, hA, alpha);
			float aHB = lerp(hBexpected, hB, alpha);
			float aHC = lerp(hCexpected, hC, alpha);
			float aHD = lerp(hDexpected, hD, alpha);
			float aHE = lerp(hEexpected, hE, alpha);

			//determine the colors of the five sub-triangle vertexes
			float cA[] = {TB->getColor(s+hN,t,0), TB->getColor(s+hN,t,1), TB->getColor(s+hN,t,2)};
			float cB[] = {TB->getColor(s, t+hN,0), TB->getColor(s, t+hN,1), TB->getColor(s, t+hN,2)};
			float cC[] = {TB->getColor(s+hN,t+hN,0), TB->getColor(s+hN,t+hN,1), TB->getColor(s+hN,t+hN,2)};
			float cD[] = {TB->getColor(s+hN,t+N,0), TB->getColor(s+hN,t+N,1), TB->getColor(s+hN,t+N,2)};
			float cE[] = {TB->getColor(s+N,t+hN,0), TB->getColor(s+N,t+hN,1), TB->getColor(s+N,t+hN,2)};

			//calculate expected color
			float cAe[] = {(TB->getColor(s,t, 0) + TB->getColor(s + N, t, 0)) / 2.0f,
				           (TB->getColor(s,t, 1) + TB->getColor(s + N, t, 1)) / 2.0f,
						   (TB->getColor(s,t, 2) + TB->getColor(s + N, t, 2)) / 2.0f};
			float cBe[] = {(TB->getColor(s,t, 0) + TB->getColor(s, t + N, 0)) / 2.0f,
				           (TB->getColor(s,t, 1) + TB->getColor(s, t + N, 1)) / 2.0f,
						   (TB->getColor(s,t, 2) + TB->getColor(s, t + N, 2)) / 2.0f};
			float cCe[] = {(TB->getColor(s+N,t, 0) + TB->getColor(s, t + N, 0)) / 2.0f,
				           (TB->getColor(s+N,t, 1) + TB->getColor(s, t + N, 1)) / 2.0f,
						   (TB->getColor(s+N,t, 2) + TB->getColor(s, t + N, 2)) / 2.0f};
			float cDe[] = {(TB->getColor(s,t+N, 0) + TB->getColor(s + N, t + N, 0)) / 2.0f,
				           (TB->getColor(s,t+N, 1) + TB->getColor(s + N, t + N, 1)) / 2.0f,
						   (TB->getColor(s,t+N, 2) + TB->getColor(s + N, t + N, 2)) / 2.0f};
			float cEe[] = {(TB->getColor(s+N,t, 0) + TB->getColor(s + N, t + N, 0)) / 2.0f,
				           (TB->getColor(s+N,t, 1) + TB->getColor(s + N, t + N, 1)) / 2.0f,
						   (TB->getColor(s+N,t, 2) + TB->getColor(s + N, t + N, 2)) / 2.0f};

			//calculate actual color
			lerp(cAe, cA, alpha, 3, cAe);
			lerp(cBe, cB, alpha, 3, cBe);
			lerp(cCe, cC, alpha, 3, cCe);
			lerp(cDe, cD, alpha, 3, cDe);
			lerp(cEe, cE, alpha, 3, cEe);

			///////////////////////////////////////////////////////
			///  draw the 8 triangles using two triangle strips ///
			///////////////////////////////////////////////////////
			glBegin(GL_TRIANGLE_STRIP);

			//glColor3f(TB->getColor(s,t,0), TB->getColor(s,t,1), TB->getColor(s,t,2));
			ambDiff[0] = TB->getColor(s,t,0); ambDiff[1] = TB->getColor(s,t,1); ambDiff[2] = TB->getColor(s,t,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s,t,0), TB->getNormal(s,t,1), TB->getNormal(s,t,2));
			glVertex3f(TB->get(s,t,0), TB->get(s,t,1), TB->get(s,t,2));
			//glColor3fv(cA);
			ambDiff[0] = cA[0]; ambDiff[1] = cA[1]; ambDiff[2] = cA[2];
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+hN,t,0), TB->getNormal(s+hN,t,1), TB->getNormal(s+hN,t,2));
			glVertex3f(TB->get(s+hN,t,0), TB->get(s+hN,t,1), aHA);
			//glColor3fv(cB);
			ambDiff[0] = cB[0]; ambDiff[1] = cB[1]; ambDiff[2] = cB[2];
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s,t+hN,0),TB->getNormal(s,t+hN,1),TB->getNormal(s,t+hN,2));
			glVertex3f(TB->get(s,t+hN,0), TB->get(s,t+hN,1), aHB);
			//glColor3fv(cC);
			ambDiff[0] = cC[0]; ambDiff[1] = cC[1]; ambDiff[2] = cC[2];
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+hN,t+hN,0),TB->getNormal(s+hN,t+hN,1),TB->getNormal(s+hN,t+hN,2));
			glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);
			//glColor3f(TB->getColor(s,t+N,0), TB->getColor(s,t+N,1), TB->getColor(s,t+N,2));
			ambDiff[0] = TB->getColor(s,t+N,0); ambDiff[1] = TB->getColor(s,t+N,1); ambDiff[2] = TB->getColor(s,t+N,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s,t+N,0), TB->getNormal(s,t+N,1), TB->getNormal(s,t+N,2));
			glVertex3f(TB->get(s,t+N,0), TB->get(s,t+N,1), TB->get(s,t+N,2));
			//glColor3fv(cD);
			ambDiff[0] = cD[0]; ambDiff[1] = cD[1]; ambDiff[2] = cD[2];
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
			glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);

			glEnd();

			glBegin(GL_TRIANGLE_STRIP);

			//glColor3f(TB->getColor(s+hN,t,0), TB->getColor(s+hN,t,1), TB->getColor(s+hN,t,2));
			ambDiff[0] = TB->getColor(s+hN,t,0); ambDiff[1] = TB->getColor(s+hN,t,1); ambDiff[2] = TB->getColor(s+hN,t,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+hN,t,0), TB->getNormal(s+hN,t,1), TB->getNormal(s+hN,t,2));
			glVertex3f(TB->get(s+hN,t,0), TB->get(s+hN,t,1), aHA);
			//glColor3f(TB->getColor(s+N,t,0), TB->getColor(s+N,t,1), TB->getColor(s+N,t,2));
			ambDiff[0] = TB->getColor(s+N,t,0); ambDiff[1] = TB->getColor(s+N,t,1); ambDiff[2] = TB->getColor(s+N,t,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+N,t,0), TB->getNormal(s+N,t,1), TB->getNormal(s+N,t,2));
			glVertex3f(TB->get(s+N,t,0), TB->get(s+N,t,1), TB->get(s+N,t,2));
			//glColor3f(TB->getColor(s+hN,t+hN,0), TB->getColor(s+hN,t+hN,1), TB->getColor(s+hN,t+hN,2));
			ambDiff[0] = TB->getColor(s+hN,t+hN,0); ambDiff[1] = TB->getColor(s+hN,t+hN,1); ambDiff[2] = TB->getColor(s+hN,t+hN,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+hN,t+hN,0), TB->getNormal(s+hN,t+hN,1), TB->getNormal(s+hN,t+hN,2));
			glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);
			//glColor3f(TB->getColor(s+N,t+hN,0), TB->getColor(s+N,t+hN,1), TB->getColor(s+N,t+hN,2));
			ambDiff[0] = TB->getColor(s+N,t+hN,0); ambDiff[1] = TB->getColor(s+N,t+hN,1); ambDiff[2] = TB->getColor(s+N,t+hN,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+N,t+hN,0), TB->getNormal(s+N,t+hN,1), TB->getNormal(s+N,t+hN,2));
			glVertex3f(TB->get(s+N,t+hN,0), TB->get(s+N,t+hN,1), aHE);
			//glColor3f(TB->getColor(s+hN,t+N,0), TB->getColor(s+hN,t+N,1), TB->getColor(s+hN,t+N,2));
			ambDiff[0] = TB->getColor(s+hN,t+N,0); ambDiff[1] = TB->getColor(s+hN,t+N,1); ambDiff[2] = TB->getColor(s+hN,t+N,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
			glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);
			//glColor3f(TB->getColor(s+N,t+N,0), TB->getColor(s+N,t+N,1), TB->getColor(s+N,t+N,2));
			ambDiff[0] = TB->getColor(s+N,t+N,0); ambDiff[1] = TB->getColor(s+N,t+N,1); ambDiff[2] = TB->getColor(s+N,t+N,2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
			glNormal3f(TB->getNormal(s+N,t+N,0), TB->getNormal(s+N,t+N,1), TB->getNormal(s+N,t+N,2));
			glVertex3f(TB->get(s+N,t+N,0), TB->get(s+N,t+N,1), TB->get(s+N,t+N,2));

			glEnd();
		}

	}

	glPopMatrix();
}

// Initialize OpenGL's rendering modes
void initRendering()
{

	glEnable( GL_DEPTH_TEST );	// Depth testing must be turned on
	glEnable(GL_LIGHTING);		// Enable lighting calculations
	glEnable(GL_LIGHT0);  //light from above
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);	 // Set global ambient light
	glShadeModel( GL_SMOOTH );	// Set the shading to smooth.  This gets overwritten as appropriate later
	glCullFace( GL_BACK );		// These two commands will cause backfaces to not be drawn

	// Possibly turn on culling of back faces.
	if ( CullBackFacesOn ) {
		glEnable( GL_CULL_FACE );
	}

	// Possibly turn on wireframe mode.
	if ( WireFrameOn ) {
		glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );		// Just show wireframes
	}
}

void initTerrain()
{
	myTerrain = new Terrain(TRI_BLOCKS, TRI_DEPTH, 0.8, 0.0f, 4.0f, 8.0f);
}

// Called when the window is resized
//		w, h - width and height of the window in pixels.
void resizeWindow(int w, int h)
{
	double aspectRatio;

	// Define the portion of the window used for OpenGL rendering.
	glViewport( 0, 0, w, h );	// View port uses whole window

	// Set up the projection view matrix: perspective projection
	// Determine the min and max values for x and y that should appear in the window.
	// The complication is that the aspect ratio of the window may not match the
	//		aspect ratio of the scene we want to view.
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	aspectRatio = (double)w / (double)h;
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //gluPerspective( 12.0, aspectRatio, 20.0, 50.0 );
	gluPerspective( 12.0, aspectRatio, 0.1, 1000.0 );

}

/**
 * returns the linear interpolation between u and v.
 * a is [0,1]
 */
float lerp(float u, float v, float a)
{
	return u * (1.0f-a) + v*a;
}

/**
 * returns the linear interpolation between vectors u and v
 */
void lerp(float* u, float* v, float a, int len, float* retval)
{
	for(int i = 0; i < len; i++)
	{
		retval[i] = u[i] * (1.0f-a) + v[i]*a;
	}
}

// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{

	initTerrain();  //initialize the terrain
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // We're going to animate it, so double buffer   //
	glutInitWindowSize( 480, 360 ); // Window position (from top corner), and size (width% and hieght)
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Tim Wheeler's Flight Simulation" );

	fprintf(stdout, "Left and Right arrow keys control yaw.\n");
	fprintf(stdout, "Up and Down arrow keys control camera pitch.\n");
	fprintf(stdout, "Press \"w\" to toggle wireframe mode.\n");
	fprintf(stdout, "Press \"c\" to toggle culling of back faces.\n");
	fprintf(stdout, "Press \"s\" to toggle smooth shading.\n");
	fprintf(stdout, "Press \"d\" and \"D\" to decrease and increase render depth.\n");
	fprintf(stdout, "Press \"f\" and \"F\" to decrease and increase speed.\n");
	fprintf(stdout, "Press \"g\" to toggle constant depth or distance-based depth rendering.\n");
	fprintf(stdout, "Press \"z\" and \"Z\" to increase and decrease altitude.\n");

	initRendering();                            // Initialize OpenGL as we like it..
	glutDisplayFunc( drawScene );               // call this whenever window needs redrawing
	glutReshapeFunc( resizeWindow );            // Set up the callback function for resizing windows
	glutKeyboardFunc( myKeyboardFunc );			// Handles "normal" ascii symbols
	glutSpecialFunc( mySpecialKeyFunc );		// Handles "special" keyboard keys
	glutMainLoop();        
	
	return 0;
}
