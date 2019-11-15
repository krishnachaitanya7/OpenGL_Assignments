/*
 * Reference: ex13 from Moodle
 * */
#include <iostream>
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/glut.h>
#include "utils.h"
#include <chrono>
#include <thread>


void utils::display(){
    glEnable( GL_DEPTH_TEST );	// Depth testing must be turned on
    glEnable(GL_LIGHTING);		// Enable lighting calculations
    glEnable(GL_LIGHT0);  //light from above
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);	 // Set global ambient light
//    glShadeModel( GL_SMOOTH );	// Set the shading to smooth.  This gets overwritten as appropriate later

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glTranslatef(0.2f, 0.3f, 0.0f);
    Particles->Draw();

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
    for(int i=0; i<10; ++i){
        glPushMatrix();
        Particles->Update(0.1f, 2, glm::vec2(4.0f / 2));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
        glutPostRedisplay();
    }



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
}
