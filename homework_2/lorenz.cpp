#include "lorenz.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <cstring>
#include <iostream>
#include <csignal>

void lorenz_attractor::lorenz_attractor_math() {
    double dt = 0.001;
    double x = 1;
    double y = 1;
    double z = 1;
    const double width = 0.01;
    double dx;
    double dy;
    double dz;
    for(int i = 0; i<lorenz_attractor::N; i++){
        dx = sigma*(y-x);
        dy = x*(r-z)-y;
        dz = x*y - b*z;
        x += dt*dx;
        y += dt*dy;
        z += dt*dz;
        points_accumulator[i][0] = x*width;
        points_accumulator[i][1] = y*width;
        points_accumulator[i][2] = z*width;
    }
}

void lorenz_attractor::reshape_window(int width, int height) {
    GLfloat h = (GLfloat) height / (GLfloat) width;
    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-box_dim*h,+box_dim*h, -box_dim,+box_dim, -box_dim,+box_dim);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void lorenz_attractor::arrow_keys_pressed(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        ele_angle += 5;
    } else if (key == GLUT_KEY_DOWN) {
        ele_angle -= 5;
    } else if (key == GLUT_KEY_RIGHT){
        azi_angle += 5;
    } else if (key == GLUT_KEY_LEFT) {
        azi_angle -= 5;
    }
    glutPostRedisplay();
}

void lorenz_attractor::lorenz_keys(unsigned char key, int x, int y) {
    if (key=='a') {
        sigma -= 1;
    }  else if (key=='s'){
        sigma += 1;
    } else if (key=='v') {
        b -= 1;
    } else if (key=='b'){
        b += 1;
    } else if (key=='e') {
        r -= 1;
    } else if (key=='r'){
        r += 1;
    }
    glutPostRedisplay();

}

void lorenz_attractor::print_axes(int x, int y, int z, char *string) {
    glRasterPos3d(x,y,z);
    int len, i;
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
    }
}

void lorenz_attractor::print_program_directions(int x, int y){
    glWindowPos2i(x,y);
    std::string test {"Elevation: "+std::to_string(ele_angle)+" Azimuth: "+std::to_string(azi_angle)};
    const char *cstr = test.c_str();
    int len, i;
    len = (int)strlen(cstr);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, cstr[i]);
    }
}

void lorenz_attractor::display_lorenz() {
    lorenz_attractor::lorenz_attractor_math();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotated(ele_angle,1,0,0);
    glRotated(azi_angle,0,1,0);
    glBegin(GL_LINE_STRIP);
    double r1 = ((double) rand() / (RAND_MAX)) + 1;
    for (int i=0;i<N;i++){
        double rand_array[3] {points_accumulator[i][0]*r1, points_accumulator[i][1]*r1, points_accumulator[i][2]*r1};
        glColor3dv(rand_array);
        glVertex3dv(points_accumulator[i]);
    }
    glEnd();
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    // Draw Y Axis
    glVertex3d(0,0,0);
    glVertex3d(0,1,0);
    // Draw Z axis
    glVertex3d(0,0,0);
    glVertex3d(0,0,1);
    // Draw X axis
    glVertex3d(0,0,0);
    glVertex3d(1,0,0);
    glEnd();
    // Print X, Y Z axes
    lorenz_attractor::print_axes(1, 0, 0, "X");
    lorenz_attractor::print_axes(0, 1, 0, "Y");
    lorenz_attractor::print_axes(0, 0, 1, "Z");
    print_program_directions(2, 2);
    glFlush();
    glutSwapBuffers();
}
