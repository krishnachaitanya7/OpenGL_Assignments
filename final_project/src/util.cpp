/*
 * Reference: ex13 from Moodle
 * */
#include <iostream>
#include "util.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "CSCIx229.h"
#include <math.h>
#include <vector>
#include "Terrain.h"

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void utils::display(){

    glEnable( GL_DEPTH_TEST );	// Depth testing must be turned on
    glEnable(GL_LIGHTING);		// Enable lighting calculations
    glEnable(GL_LIGHT0);  //light from above
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);	 // Set global ambient light
//    glShadeModel( GL_SMOOTH );	// Set the shading to smooth.  This gets overwritten as appropriate later
    glCullFace( GL_BACK );		// These two commands will cause backfaces to not be drawn
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
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
//    glFlush();
//    glutSwapBuffers();
//    glutPostRedisplay();

    const double len=2.0;  //  Length of axes
    //  Light position and colors
    float Emission[]  = {0.0,0.0,0.0,1.0};
    float Ambient[]   = {0.3,0.3,0.3,1.0};
    float Diffuse[]   = {1.0,1.0,1.0,1.0};
    float Specular[]  = {spc,spc,spc,1.0};
    float Position[]  = {static_cast<float>(2*Cos(zh)),Ylight,static_cast<float>(2*Sin(zh)),1};
    float Shinyness[] = {16};

    //  Erase the window and the depth buffer
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //  Enable Z-buffering in OpenGL
//    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
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

    //  Draw light position as sphere (still no lighting here)
//    glColor3f(1,1,1);
//    glPushMatrix();
//    glTranslated(Position[0],Position[1],Position[2]);
//    glutSolidSphere(0.03,10,10);
//    glPopMatrix();

    //  OpenGL should normalize normal vectors


    //  Set ambient, diffuse, specular components and position of light 0
//    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
//    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
//    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
//    glLightfv(GL_LIGHT0,GL_POSITION,Position);
//    //  Set materials
//    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
//    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,RGBA);
//    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,RGBA);
//    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
//    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

    //  Draw the model
    glPushMatrix();
    glRotatef(-180, 0.0, 1.0, 0.0);
    glScaled(scale,scale,scale);
    glCallList(obj);
    glPopMatrix();

    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    if (axes)
    {
        glBegin(GL_LINES);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(len,0.0,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,len,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,0.0,len);
        glEnd();
        //  Label axes
        glRasterPos3d(len,0.0,0.0);
        Print("X");
        glRasterPos3d(0.0,len,0.0);
        Print("Y");
        glRasterPos3d(0.0,0.0,len);
        Print("Z");
    }
    //  Display parameters
    glWindowPos2i(5,5);
    Print("Angle=%d,%d  Dim=%.1f Projection=%s",
          th,ph,dim,proj?"Perpective":"Orthogonal");
    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
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
    glColor3f(1,1,1);
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
//            float cA[] = {TB->getColor(s+hN,t,0), TB->getColor(s+hN,t,1), TB->getColor(s+hN,t,2)};
//            float cB[] = {TB->getColor(s, t+hN,0), TB->getColor(s, t+hN,1), TB->getColor(s, t+hN,2)};
//            float cC[] = {TB->getColor(s+hN,t+hN,0), TB->getColor(s+hN,t+hN,1), TB->getColor(s+hN,t+hN,2)};
//            float cD[] = {TB->getColor(s+hN,t+N,0), TB->getColor(s+hN,t+N,1), TB->getColor(s+hN,t+N,2)};
//            float cE[] = {TB->getColor(s+N,t+hN,0), TB->getColor(s+N,t+hN,1), TB->getColor(s+N,t+hN,2)};
//
//            //calculate expected color
//            float cAe[] = {(TB->getColor(s,t, 0) + TB->getColor(s + N, t, 0)) / 2.0f,
//                           (TB->getColor(s,t, 1) + TB->getColor(s + N, t, 1)) / 2.0f,
//                           (TB->getColor(s,t, 2) + TB->getColor(s + N, t, 2)) / 2.0f};
//            float cBe[] = {(TB->getColor(s,t, 0) + TB->getColor(s, t + N, 0)) / 2.0f,
//                           (TB->getColor(s,t, 1) + TB->getColor(s, t + N, 1)) / 2.0f,
//                           (TB->getColor(s,t, 2) + TB->getColor(s, t + N, 2)) / 2.0f};
//            float cCe[] = {(TB->getColor(s+N,t, 0) + TB->getColor(s, t + N, 0)) / 2.0f,
//                           (TB->getColor(s+N,t, 1) + TB->getColor(s, t + N, 1)) / 2.0f,
//                           (TB->getColor(s+N,t, 2) + TB->getColor(s, t + N, 2)) / 2.0f};
//            float cDe[] = {(TB->getColor(s,t+N, 0) + TB->getColor(s + N, t + N, 0)) / 2.0f,
//                           (TB->getColor(s,t+N, 1) + TB->getColor(s + N, t + N, 1)) / 2.0f,
//                           (TB->getColor(s,t+N, 2) + TB->getColor(s + N, t + N, 2)) / 2.0f};
//            float cEe[] = {(TB->getColor(s+N,t, 0) + TB->getColor(s + N, t + N, 0)) / 2.0f,
//                           (TB->getColor(s+N,t, 1) + TB->getColor(s + N, t + N, 1)) / 2.0f,
//                           (TB->getColor(s+N,t, 2) + TB->getColor(s + N, t + N, 2)) / 2.0f};
//
//            //calculate actual color
//            lerp(cAe, cA, alpha, 3, cAe);
//            lerp(cBe, cB, alpha, 3, cBe);
//            lerp(cCe, cC, alpha, 3, cCe);
//            lerp(cDe, cD, alpha, 3, cDe);
//            lerp(cEe, cE, alpha, 3, cEe);

            ///////////////////////////////////////////////////////
            ///  draw the 8 triangles using two triangle strips ///
            ///////////////////////////////////////////////////////
            glBegin(GL_TRIANGLE_STRIP);

            //glColor3f(TB->getColor(s,t,0), TB->getColor(s,t,1), TB->getColor(s,t,2));
//            ambDiff[0] = TB->getColor(s,t,0); ambDiff[1] = TB->getColor(s,t,1); ambDiff[2] = TB->getColor(s,t,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s,t,0), TB->getNormal(s,t,1), TB->getNormal(s,t,2));
            glTexCoord2f(0.1,0.9); glVertex3f(TB->get(s,t,0), TB->get(s,t,1), TB->get(s,t,2));
            //glColor3fv(cA);
//            ambDiff[0] = cA[0]; ambDiff[1] = cA[1]; ambDiff[2] = cA[2];
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+hN,t,0), TB->getNormal(s+hN,t,1), TB->getNormal(s+hN,t,2));
            glTexCoord2f(0.1,0.1); glVertex3f(TB->get(s+hN,t,0), TB->get(s+hN,t,1), aHA);
            //glColor3fv(cB);
//            ambDiff[0] = cB[0]; ambDiff[1] = cB[1]; ambDiff[2] = cB[2];
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s,t+hN,0),TB->getNormal(s,t+hN,1),TB->getNormal(s,t+hN,2));
            glTexCoord2f(0.9,0.9); glVertex3f(TB->get(s,t+hN,0), TB->get(s,t+hN,1), aHB);
            //glColor3fv(cC);
//            ambDiff[0] = cC[0]; ambDiff[1] = cC[1]; ambDiff[2] = cC[2];
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+hN,t+hN,0),TB->getNormal(s+hN,t+hN,1),TB->getNormal(s+hN,t+hN,2));
            glTexCoord2f(0.9,0.1); glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);
            //glColor3f(TB->getColor(s,t+N,0), TB->getColor(s,t+N,1), TB->getColor(s,t+N,2));
//            ambDiff[0] = TB->getColor(s,t+N,0); ambDiff[1] = TB->getColor(s,t+N,1); ambDiff[2] = TB->getColor(s,t+N,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s,t+N,0), TB->getNormal(s,t+N,1), TB->getNormal(s,t+N,2));
            glTexCoord2f(0.9,0.9); glVertex3f(TB->get(s,t+N,0), TB->get(s,t+N,1), TB->get(s,t+N,2));
            //glColor3fv(cD);
//            ambDiff[0] = cD[0]; ambDiff[1] = cD[1]; ambDiff[2] = cD[2];
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
            glTexCoord2f(0.9,0.1); glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);

            glEnd();

            glBegin(GL_TRIANGLE_STRIP);

            //glColor3f(TB->getColor(s+hN,t,0), TB->getColor(s+hN,t,1), TB->getColor(s+hN,t,2));
//            ambDiff[0] = TB->getColor(s+hN,t,0); ambDiff[1] = TB->getColor(s+hN,t,1); ambDiff[2] = TB->getColor(s+hN,t,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+hN,t,0), TB->getNormal(s+hN,t,1), TB->getNormal(s+hN,t,2));
            glTexCoord2f(0.1,0.9); glVertex3f(TB->get(s+hN,t,0), TB->get(s+hN,t,1), aHA);
            //glColor3f(TB->getColor(s+N,t,0), TB->getColor(s+N,t,1), TB->getColor(s+N,t,2));
//            ambDiff[0] = TB->getColor(s+N,t,0); ambDiff[1] = TB->getColor(s+N,t,1); ambDiff[2] = TB->getColor(s+N,t,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+N,t,0), TB->getNormal(s+N,t,1), TB->getNormal(s+N,t,2));
            glTexCoord2f(0.1,0.1);glVertex3f(TB->get(s+N,t,0), TB->get(s+N,t,1), TB->get(s+N,t,2));
            //glColor3f(TB->getColor(s+hN,t+hN,0), TB->getColor(s+hN,t+hN,1), TB->getColor(s+hN,t+hN,2));
//            ambDiff[0] = TB->getColor(s+hN,t+hN,0); ambDiff[1] = TB->getColor(s+hN,t+hN,1); ambDiff[2] = TB->getColor(s+hN,t+hN,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+hN,t+hN,0), TB->getNormal(s+hN,t+hN,1), TB->getNormal(s+hN,t+hN,2));
            glTexCoord2f(0.9,0.9); glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);
            //glColor3f(TB->getColor(s+N,t+hN,0), TB->getColor(s+N,t+hN,1), TB->getColor(s+N,t+hN,2));
//            ambDiff[0] = TB->getColor(s+N,t+hN,0); ambDiff[1] = TB->getColor(s+N,t+hN,1); ambDiff[2] = TB->getColor(s+N,t+hN,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+N,t+hN,0), TB->getNormal(s+N,t+hN,1), TB->getNormal(s+N,t+hN,2));
            glTexCoord2f(0.9,0.1); glVertex3f(TB->get(s+N,t+hN,0), TB->get(s+N,t+hN,1), aHE);
            //glColor3f(TB->getColor(s+hN,t+N,0), TB->getColor(s+hN,t+N,1), TB->getColor(s+hN,t+N,2));
//            ambDiff[0] = TB->getColor(s+hN,t+N,0); ambDiff[1] = TB->getColor(s+hN,t+N,1); ambDiff[2] = TB->getColor(s+hN,t+N,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
            glTexCoord2f(0.9,0.9); glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);
            //glColor3f(TB->getColor(s+N,t+N,0), TB->getColor(s+N,t+N,1), TB->getColor(s+N,t+N,2));
//            ambDiff[0] = TB->getColor(s+N,t+N,0); ambDiff[1] = TB->getColor(s+N,t+N,1); ambDiff[2] = TB->getColor(s+N,t+N,2);
//            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff );
            glNormal3f(TB->getNormal(s+N,t+N,0), TB->getNormal(s+N,t+N,1), TB->getNormal(s+N,t+N,2));
            glTexCoord2f(0.9,0.1); glVertex3f(TB->get(s+N,t+N,0), TB->get(s+N,t+N,1), TB->get(s+N,t+N,2));

            glEnd();
        }

    }

    glPopMatrix();
}

float utils::lerp(float u, float v, float a){
    return u * (1.0f-a) + v*a;
}

/**
 * returns the linear interpolation between vectors u and v
 */
void utils::lerp(float* u, float* v, float a, int len, float* retval){
    for(int i = 0; i < len; i++)
    {
        retval[i] = u[i] * (1.0f-a) + v[i]*a;
    }
}

void utils::initTerrain(){

    myTerrain = new Terrain(TRI_BLOCKS, TRI_DEPTH, 0.8, 0.0f, 4.0f, 4.0f);
}



