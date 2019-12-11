#include <iostream>
#include <random>
#include "util.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "CSCIx229.h"
#include <math.h>
#include <vector>
#include <functional>
#include "Terrain.h"

  
float utils::rand_float(){
    double r = ((double) rand() / (RAND_MAX));
    return (float)r;
}
void utils::display(){
    if(day_night){
        glClearColor(0.0, 0.0, 0.0, 0.0);
    } else{
        glClearColor(1.0, 1.0, 1.0, 0.0);
    }
    glEnable( GL_DEPTH_TEST );	  
    glEnable(GL_LIGHTING);		  
    glEnable(GL_LIGHT0);    
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

      
    if(fog) {
        GLuint fogMode[] = {GL_EXP, GL_EXP2, GL_LINEAR};     
        GLuint fogfilter = 1;                      
        GLfloat fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};        
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);            

        glFogi(GL_FOG_MODE, fogMode[fogfilter]);          
        glFogfv(GL_FOG_COLOR, fogColor);              
        glFogf(GL_FOG_DENSITY, 0.09f);                
        glHint(GL_FOG_HINT, GL_DONT_CARE);            
        glFogf(GL_FOG_START, 0.0f);               
        glFogf(GL_FOG_END, 1.0f);                 
        glEnable(GL_FOG);
    } else{
        glDisable(GL_FOG);
    }


      

  
    if(toggletexture){
        glBindTexture(GL_TEXTURE_2D,textures[0]);
  
    } else{
        glBindTexture(GL_TEXTURE_2D,textures[1]);
  
    }
    tick += timeStep;
    if(tick > 10){
        tick = 0;
    }

      
    plane_y += PLANE_SPEED * cos(plane_yaw);
    plane_x += PLANE_SPEED * sin(plane_yaw);

    float planeS = myTerrain->xToS(plane_x, plane_y);   
    float planeT = myTerrain->yToT(plane_x, plane_y);

    float terrainSideLen = myTerrain->getTerrainSideLength();
    int N = myTerrain->getN();

    int shunted = 0;
    if (planeS < 0) {
        planeS += terrainSideLen;
        shunted = 1;
    } else if (planeS >= terrainSideLen) {
        planeS -= terrainSideLen;
        shunted = 1;
    }
    if (planeT < 0) {
        planeT += terrainSideLen;
        shunted = 1;
    } else if (planeT >= terrainSideLen) {
        planeT -= terrainSideLen;
        shunted = 1;
    }
    if (shunted) {    
        plane_x = myTerrain->sToX(planeS, planeT);    
        plane_y = myTerrain->tToY(planeS, planeT);
    }

      
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(SmoothShading == 1 ? GL_SMOOTH : GL_FLAT);
    glMatrixMode(GL_MODELVIEW);              
    glLoadIdentity();                          

    glRotatef(Azimuth, 1.0, 0.0, 0.0);   
    glRotatef(plane_yaw * 180.0 / PI, 0.0, 1.0, 0.0);    
    glTranslatef(-plane_x, -plane_alt, plane_y);    

      
      
      

    float terrainLength = myTerrain->getTerrainSideLength();
    int numGrid = 2;
    float viewRadius = terrainLength;

    float locX, locY, locS, locT, dispS, dispT, theta2, thetaR, thetaD, depth;

    float forwardPlane_x = plane_x + 2 * sin(plane_yaw);
    float forwardPlane_y = plane_y + 2 * cos(plane_yaw);
    float forwardPlane_s = myTerrain->xToS(forwardPlane_x, forwardPlane_y);
    float forwardPlane_t = myTerrain->yToT(forwardPlane_x, forwardPlane_y);

    for (int nS = -numGrid; nS <= numGrid; nS++) {
        for (int nT = -numGrid; nT <= numGrid; nT++) {
            dispS = terrainLength * nS;
            dispT = terrainLength * nT;
            for (int s = 0; s < myTerrain->getN(); s++) {
                for (int t = 0; t < myTerrain->getN(); t++) {

                    TerrainBlock *TB = myTerrain->getBlock(s, t);
                    locX = TB->get(0, 0, 0);
                    locY = TB->get(0, 0, 1);
                    locS = myTerrain->xToS(locX, locY) + dispS;
                    locT = myTerrain->yToT(locX, locY) + dispT;

                    float dS = locS - forwardPlane_s;
                    float dT = locT - forwardPlane_t;
                    float dist = sqrt(dS * dS + dT * dT);    

                    if (dist < viewRadius) {    
                        theta2 = atan2(dT, dS);    
                        thetaR = PI / 2.0 - theta2;   
                        thetaD = thetaR - plane_yaw;

                        while (thetaD < -PI) {
                            thetaD += 2 * PI;
                        }
                        while (thetaD >= PI) {
                            thetaD -= 2 * PI;
                        }

                        if (thetaD < PI / 2 && thetaD > -PI / 2) {
                            depth = (gradientDepth == 1 ? (viewRadius - dist) / viewRadius * maxRenderDepth
                                                        : maxRenderDepth);

                            renderBlock(TB, depth, dispS, dispT);
                        }
                    }
                }
            }
        }
    }
    if(!display_terrain){
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glLoadIdentity();
    if (proj)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0, plane_z, 0 , 0,Cos(ph),0);
  

    }
          
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }

      
    glPushMatrix();
    glRotatef(-180, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, plane_z);
    glScaled(scale,scale,scale);
    glCallList(obj);

    if(collision_detection()){
        PLANE_SPEED = 0.0;
        plane_z=1.5;
        glTranslatef(0.0, 1.5, 0.0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glDisable(GL_DEPTH_TEST);
        pe0.draw();
        pe0.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.0, 0.5, 0.0);
        pe1.draw();
        pe1.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0, 0.5, 0.0);
        pe2.draw();
        pe2.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.5, 0, 0.0);
        pe3.draw();
        pe3.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.5, 0, 0.0);
        pe4.draw();
        pe4.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.5, 0, 0.0);
        pe5.draw();
        pe5.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.5, 0, 0.0);
        pe6.draw();
        pe6.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.5, 0, 0.0);
        pe7.draw();
        pe7.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.5, 0, 0.0);
        pe8.draw();
        pe8.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(0.5, 0, 0.0);
        pe9.draw();
        pe9.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(-4.0, 0, 0.0);
        pe10.draw();
        pe10.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(-0.5, 0, 0.0);
        pe11.draw();
        pe11.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(-0.5, 0, 0.0);
        pe12.draw();
        pe12.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(-0.5, 0, 0.0);
        pe13.draw();
        pe13.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(-0.5, 0, 0.0);
        pe14.draw();
        pe14.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(-0.5, 0, 0.0);
        pe15.draw();
        pe15.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glTranslatef(-0.5, 0, 0.0);
        pe16.draw();
        pe16.setDirection(rand()%1000, rand_float(), rand_float(), rand_float());
        glDisable(GL_BLEND);
        glColor3f(1.0,1.0,1.0);

  
          
    }
    glPopMatrix();
      
    int len = 5;
      

    ErrCheck("display");
    glFlush();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */

bool utils::collision_detection() {
    float plane_lowest_y_point = -0.000578 + plane_z;   
    float least_distance = 100000000.0;   
    for(float i: y_points){
        if(i-plane_lowest_y_point < least_distance){
            least_distance = i - plane_lowest_y_point ;
        }
    }
  
    return least_distance < -1.85;
  

}
void utils::idle(){
      
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    if (move) zh = fmod(90*t,360.0);
      
    glutPostRedisplay();
}


void utils::special(int key,int x,int y){
      
    if (key == GLUT_KEY_RIGHT)
        th += 5;
          
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
          
    else if (key == GLUT_KEY_UP){

        plane_z-=0.1;
        if(plane_z < -1){
            plane_z = -1;
            display_terrain = false;
        }
        }
          
    else if (key == GLUT_KEY_DOWN){

        plane_z+=0.1;
        if(plane_z >= -1){
            display_terrain = true;
        }
        }
          
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
          
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.1;
      
    th %= 360;
    ph %= 360;
      
    Project(proj?fov:0,asp,dim);
      
    glutPostRedisplay();
}

void utils::key(unsigned char ch,int x,int y){
      
    if (ch == 27)
        exit(0);
          
    else if (ch == '0')
        th = ph = 0;
          
    else if (ch == 'a' || ch == 'A')
        axes = 1-axes;
          
    else if (ch == 'p' || ch == 'P')
        proj = 1-proj;
          
    else if (ch == 'l' || ch == 'L')
        spc = 1-spc;
          
    else if (ch == 's' || ch == 'S')
        move = 1-move;
          
    else if (ch == '[')
        zh -= 1;
    else if (ch == ']')
        zh += 1;
          
    else if (ch == '+')
        Ylight += 0.1;
    else if (ch == '-')
        Ylight -= 0.1;
    else if(ch == 't'){
        if(toggletexture){
            toggletexture = false;
        } else{
            toggletexture = true;
        }
    }
    else if(ch == 'b'){
        PLANE_SPEED = -1.0;
    }
    else if(ch == 'f'){
        PLANE_SPEED = 1.0;
    }
    else if(ch == 'd'){
        day_night = !day_night;

    }
    else if(ch == 'g'){
        fog = !fog;
    }
      
    Project(proj?fov:0,asp,dim);
      
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void utils::reshape(int width,int height){
      
    asp = (height>0) ? (double)width/height : 1;
      
    glViewport(0,0, width,height);
      
    Project(proj?fov:0,asp,dim);
}

void utils::renderBlock(TerrainBlock* TB, float depth, float dispS, float dispT){

    float x = myTerrain->sToX(dispS, dispT);
    float y = myTerrain->tToY(dispS, dispT);

    int n = TB->getN();
    int N = n >> (int)floor(depth);    
    float alpha = depth - (int)floor(depth);    

    if(N < 2){
        N = 2;    
        alpha = 1.0;
    }


    int hN = N/2;    

    glPushMatrix();

    glRotatef(-90, 1.0, 0.0, 0.0);
    glTranslatef(x,y,0+plane_z);    


      
    y_points.clear();
    for(int s = 0; s < n; s+=N)
    {

        for(int t = 0; t < n; t+=N)
        {

            float hA = TB->get(s+hN,t,2);
            float hB = TB->get(s, t+hN,2);
            float hC = TB->get(s+hN,t+hN,2);
            float hD = TB->get(s+hN,t+N,2);
            float hE = TB->get(s+N,t+hN,2);

              
            float hAexpected = (TB->get(s,t, 2) + TB->get(s + N, t, 2)) / 2.0f;
            float hBexpected = (TB->get(s,t, 2) + TB->get(s, t + N, 2)) / 2.0f;
            float hCexpected = (TB->get(s+N,t, 2) + TB->get(s, t + N, 2)) / 2.0f;
            float hDexpected = (TB->get(s,t+N, 2) + TB->get(s + N, t + N, 2)) / 2.0f;
            float hEexpected = (TB->get(s+N,t, 2) + TB->get(s + N, t + N, 2)) / 2.0f;

              
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
            glTexCoord2f(1,1);
            glVertex3f(TB->get(s,t+hN,0), TB->get(s,t+hN,1), aHB);

            glNormal3f(TB->getNormal(s+hN,t+hN,0),TB->getNormal(s+hN,t+hN,1),TB->getNormal(s+hN,t+hN,2));
            glTexCoord2f(1,0);
            glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);

            glNormal3f(TB->getNormal(s,t+N,0), TB->getNormal(s,t+N,1), TB->getNormal(s,t+N,2));
            glTexCoord2f(0,0);
            glVertex3f(TB->get(s,t+N,0), TB->get(s,t+N,1), TB->get(s,t+N,2));

            glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
            glTexCoord2f(1,1);
            glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);

            glEnd();

            y_points.push_back(TB->get(s,t,1));
            y_points.push_back(TB->get(s+hN,t,1));
            y_points.push_back(TB->get(s,t+hN,1));
            y_points.push_back(TB->get(s+hN,t+hN,1));
            y_points.push_back(TB->getNormal(s,t+N,1));
            y_points.push_back(TB->get(s+hN,t+N,1));
            y_points.push_back(TB->get(s+N,t,1));
            y_points.push_back(TB->get(s+N,t+hN,1));
            y_points.push_back(TB->get(s+N,t+N,1));
            y_points.push_back(TB->get(s+hN,t+hN,1));
            y_points.push_back(TB->get(s+N,t+hN,1));



            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(TB->getNormal(s+hN,t,0), TB->getNormal(s+hN,t,1), TB->getNormal(s+hN,t,2));
            glTexCoord2f(0,0);
            glVertex3f(TB->get(s+hN,t,0), TB->get(s+hN,t,1), aHA);

            glNormal3f(TB->getNormal(s+N,t,0), TB->getNormal(s+N,t,1), TB->getNormal(s+N,t,2));
            glTexCoord2f(0,1);
            glVertex3f(TB->get(s+N,t,0), TB->get(s+N,t,1), TB->get(s+N,t,2));

            glNormal3f(TB->getNormal(s+hN,t+hN,0), TB->getNormal(s+hN,t+hN,1), TB->getNormal(s+hN,t+hN,2));
            glTexCoord2f(1,1);
            glVertex3f(TB->get(s+hN,t+hN,0), TB->get(s+hN,t+hN,1), aHC);

            glNormal3f(TB->getNormal(s+N,t+hN,0), TB->getNormal(s+N,t+hN,1), TB->getNormal(s+N,t+hN,2));
            glTexCoord2f(1,0);
            glVertex3f(TB->get(s+N,t+hN,0), TB->get(s+N,t+hN,1), aHE);

            glNormal3f(TB->getNormal(s+hN,t+N,0), TB->getNormal(s+hN,t+N,1), TB->getNormal(s+hN,t+N,2));
            glTexCoord2f(0,0);
            glVertex3f(TB->get(s+hN,t+N,0), TB->get(s+hN,t+N,1), aHD);

            glNormal3f(TB->getNormal(s+N,t+N,0), TB->getNormal(s+N,t+N,1), TB->getNormal(s+N,t+N,2));
            glTexCoord2f(1,1);
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



