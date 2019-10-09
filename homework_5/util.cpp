#include <util.h>
#include <iostream>
#include "util.h"
#include <GL/glut.h>




void utils::display_scene(){
    glClearColor( 0, 0, 0, 1 );  // (In fact, this is the default.)
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(1, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glColor3f( 1, 0, 0 ); // red
    glVertex3f( -0.8, -0.8, 0.0 );
    glColor3f( 0, 1, 0 ); // green
    glVertex3f( 0.8, -0.8, 0.0 );
    glColor3f( 0, 0, 1 ); // blue
    glVertex3f( 0, 0.9, 0.0 );
    glEnd();
    glutSwapBuffers(); // Required to copy color buffer onto the screen.

}

void utils::reshape_window(int w, int h) {
    // Window is reshaped, Adjust the graphics
    glViewport(0, 0, w, h);
}


