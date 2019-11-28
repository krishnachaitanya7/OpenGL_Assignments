#include <iostream>
#include "util.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "CSCIx229.h"
#include <math.h>
#include <vector>
#include "Terrain.h"

/*
 *  OpenGL (GLUT) calls this routine to display the
 *
 */
void utils::display(){
    glEnable( GL_DEPTH_TEST );	// Depth testing must be turned on
    glEnable(GL_LIGHTING);		// Enable lighting calculations
    glEnable(GL_LIGHT0);  //light from above
    float emission = 0;
    float shiny = 1.0;
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,static_cast<float>(0.01*emission),1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
//    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    tick += timeStep;
    if(tick > 10){
        tick = 0;
    }
    //move the plane, keep it in the square
    plane_y +=  PLANE_SPEED * cos(plane_yaw);
    plane_x +=  PLANE_SPEED * sin(plane_yaw);

    float planeS = myTerrain->xToS(plane_x, plane_y); //s,t loc of plane
    float planeT = myTerrain->yToT(plane_x, plane_y);

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
    glShadeModel(SmoothShading == 1 ? GL_SMOOTH : GL_FLAT);
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

    glLoadIdentity();
    if (proj)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
        //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }

    //  Draw the model
    glPushMatrix();
    glRotatef(-180, 0.0, 1.0, 0.0);
    glScaled(scale,scale,scale);
    glCallList(obj);
    glPopMatrix();
    //  Draw axes - no lighting from here on
//    if (axes)
//    {
//        glBegin(GL_LINES);
//        glVertex3d(0.0,0.0,0.0);
//        glVertex3d(len,0.0,0.0);
//        glVertex3d(0.0,0.0,0.0);
//        glVertex3d(0.0,len,0.0);
//        glVertex3d(0.0,0.0,0.0);
//        glVertex3d(0.0,0.0,len);
//        glEnd();
//        //  Label axes
//        glRasterPos3d(len,0.0,0.0);
//        Print("X");
//        glRasterPos3d(0.0,len,0.0);
//        Print("Y");
//        glRasterPos3d(0.0,0.0,len);
//        Print("Z");
//    }
    //  Display parameters
    glWindowPos2i(5,5);
    Print("Angle=%d,%d  Dim=%.1f Projection=%s",
          th,ph,dim,proj?"Perpective":"Orthogonal");
    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void utils::idle(){
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    if (move) zh = fmod(90*t,360.0);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void utils::special(int key,int x,int y){
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
        //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
        //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP){
//        ph += 5;
        Azimuth -= AngleStepSize;
        if ( Azimuth > azimuth_max ) {
            Azimuth = azimuth_max;
        }}
        //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN){
//        ph -= 5;
        Azimuth += AngleStepSize;
        if ( Azimuth < azimuth_min ) {
            Azimuth = azimuth_min;
        }}
        //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
        //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project(proj?fov:0,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void utils::key(unsigned char ch,int x,int y){
    //  Exit on ESC
    if (ch == 27)
        exit(0);
        //  Reset view angle
    else if (ch == '0')
        th = ph = 0;
        //  Toggle axes
    else if (ch == 'a' || ch == 'A')
        axes = 1-axes;
        //  Toggle projection type
    else if (ch == 'p' || ch == 'P')
        proj = 1-proj;
        //  Toggle specular light
    else if (ch == 'l' || ch == 'L')
        spc = 1-spc;
        //  Toggle light movement
    else if (ch == 's' || ch == 'S')
        move = 1-move;
        //  Move light
    else if (ch == '[')
        zh -= 1;
    else if (ch == ']')
        zh += 1;
        //  Light elevation
    else if (ch == '+')
        Ylight += 0.1;
    else if (ch == '-')
        Ylight -= 0.1;
    //  Reproject
    Project(proj?fov:0,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void utils::reshape(int width,int height){
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project(proj?fov:0,asp,dim);
}

void utils::renderBlock(TerrainBlock* TB, float depth, float dispS, float dispT){

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

    //draw the triangle
    for(int s = 0; s < n; s+=N)
    {

        for(int t = 0; t < n; t+=N)
        {

            float hA = TB->get(s+hN,t,2);
            float hB = TB->get(s, t+hN,2);
            float hC = TB->get(s+hN,t+hN,2);
            float hD = TB->get(s+hN,t+N,2);
            float hE = TB->get(s+N,t+hN,2);

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

            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(TB->getNormal(s,t,0), TB->getNormal(s,t,1), TB->getNormal(s,t,2));
            glTexCoord2f(0,0);
            glVertex3f(TB->get(s,t,0), TB->get(s,t,1), TB->get(s,t,2));

            glNormal3f(TB->getNormal(s+hN,t,0), TB->getNormal(s+hN,t,1), TB->getNormal(s+hN,t,2));
            glTexCoord2f(0,1);
            glVertex3f(TB->get(s+hN,t,0), TB->get(s+hN,t,1), aHA);

            glNormal3f(TB->getNormal(s,t+hN,0),TB->getNormal(s,t+hN,1),TB->getNormal(s,t+hN,2));
            glTexCoord2f(0,0);
            glVertex3f(TB->get(s,t+hN,0), TB->get(s,t+hN,1), aHB);

            glNormal3f(TB->getNormal(s+hN,t+hN,0),TB->getNormal(s+hN,t+hN,1),TB->getNormal(s+hN,t+hN,2));
            glTexCoord2f(0.9,0.1);
            glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);

            glNormal3f(TB->getNormal(s,t+N,0), TB->getNormal(s,t+N,1), TB->getNormal(s,t+N,2));
            glTexCoord2f(0.9,0.9);
            glVertex3f(TB->get(s,t+N,0), TB->get(s,t+N,1), TB->get(s,t+N,2));

            glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
            glTexCoord2f(0.9,0.1);
            glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);

            glEnd();

            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(TB->getNormal(s+hN,t,0), TB->getNormal(s+hN,t,1), TB->getNormal(s+hN,t,2));
            glTexCoord2f(0.1,0.9);
            glVertex3f(TB->get(s+hN,t,0), TB->get(s+hN,t,1), aHA);

            glNormal3f(TB->getNormal(s+N,t,0), TB->getNormal(s+N,t,1), TB->getNormal(s+N,t,2));
            glTexCoord2f(0.1,0.1);
            glVertex3f(TB->get(s+N,t,0), TB->get(s+N,t,1), TB->get(s+N,t,2));

            glNormal3f(TB->getNormal(s+hN,t+hN,0), TB->getNormal(s+hN,t+hN,1), TB->getNormal(s+hN,t+hN,2));
            glTexCoord2f(0.9,0.9);
            glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);

            glNormal3f(TB->getNormal(s+N,t+hN,0), TB->getNormal(s+N,t+hN,1), TB->getNormal(s+N,t+hN,2));
            glTexCoord2f(0.9,0.1);
            glVertex3f(TB->get(s+N,t+hN,0), TB->get(s+N,t+hN,1), aHE);

            glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
            glTexCoord2f(0.9,0.9);
            glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);

            glNormal3f(TB->getNormal(s+N,t+N,0), TB->getNormal(s+N,t+N,1), TB->getNormal(s+N,t+N,2));
            glTexCoord2f(0.9,0.1);
            glVertex3f(TB->get(s+N,t+N,0), TB->get(s+N,t+N,1), TB->get(s+N,t+N,2));

            glEnd();
        }
    }
    glPopMatrix();



}

float utils::lerp(float u, float v, float a){
    return u * (1.0f-a) + v*a;
}


void utils::initTerrain(){
    myTerrain = new Terrain(TRI_BLOCKS, TRI_DEPTH, 0.8, 0.0f, 0.0f, 4.0f);
}



