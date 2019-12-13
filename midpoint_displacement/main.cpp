#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "CSCIx229.h"
#include <iostream>
#include "Terrain.h"
#include "TerrainBlock.h"

int dim = 2;
int ph = 0;
int th = 90;
int plane_z = 90;
Terrain* myTerrain = new Terrain(10, 8, 0.8, 0.0f, 0.0f, 4.0f);


void special(int key,int x,int y){

    if (key == GLUT_KEY_RIGHT)
        th += 5;

    else if (key == GLUT_KEY_LEFT)
        th -= 5;

    else if (key == GLUT_KEY_UP){

        plane_z-=0.1;
        if(plane_z < -1){
            plane_z = -1;
//            display_terrain = false;
        }
    }

    else if (key == GLUT_KEY_DOWN){

        plane_z+=0.1;
        if(plane_z >= -1){
//            display_terrain = true;
        }
    }

    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;

    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.1;

    th %= 360;
    ph %= 360;



    glutPostRedisplay();
}

float lerp(float u, float v, float a){
    return u * (1.0f-a) + v*a;
}

void renderBlock(TerrainBlock* TB, float depth, float dispS, float dispT){

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

void display(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    double Ex = -2*dim*Sin(th)*Cos(ph);
    double Ey = +2*dim        *Sin(ph);
    double Ez = +2*dim*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0, plane_z, 0 , 0,Cos(ph),0);





    float terrainLength = myTerrain->getTerrainSideLength();
    int numGrid = 2;
    float viewRadius = terrainLength;

    float locX, locY, locS, locT, dispS, dispT, theta2, thetaR, thetaD, depth;



    for (int nS = -numGrid; nS <= numGrid; nS++) {
        for (int nT = -numGrid; nT <= numGrid; nT++) {
            dispS = terrainLength * nS;
            dispT = terrainLength * nT;
            for (int s = 0; s < myTerrain->getN(); s++) {
                for (int t = 0; t < myTerrain->getN(); t++) {

                    TerrainBlock *TB = myTerrain->getBlock(s, t);



                    renderBlock(TB, depth, dispS, dispT);

                }
            }
        }
    }
    ErrCheck("display");
    glFlush();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
    glutPostRedisplay();
}


int main(int argc,char* argv[]){

    //  Initialize GLUT
    glutInit(&argc,argv);
//    if (argc!=2 && argc!=3 && argc!=6) Fatal("Usage: %s <obj> [scale [R G B]]\n",argv[0]);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);


    glutInitWindowSize(600,600);
    glutCreateWindow("Kodur Krishna Chaitanya: Final Project");

    //  Set callbacks

//    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutSpecialFunc(special);
//    glutKeyboardFunc(key);

//    utils::pe = &pde;

    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}