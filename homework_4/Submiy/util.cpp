#include "util.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <cstring>


void utils::reshape_window(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (double) w / (double) h, 1.0, 100.0);
    glutPostRedisplay();
}

void utils::arrow_keys_pressed(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        elevation_angle -= 1;
    } else if (key == GLUT_KEY_DOWN) {
        elevation_angle += 1;
    } else if (key == GLUT_KEY_RIGHT){
        azimuth_angle -= 1;
    } else if (key == GLUT_KEY_LEFT) {
        azimuth_angle += 1;
    }
    elevation_angle = (float)((int)elevation_angle % 360);
    azimuth_angle = (float)((int)azimuth_angle % 360);
    update_projection_matrix();
    glutPostRedisplay();
}

void utils::keys_pressed(unsigned char ch, int x, int y) {
    if (ch == 'm') {
        mode_of_view = mode_of_view + 1;
        mode_of_view %= 3;
    }
    else if (ch == '+') {
        fov--;
    }
    else if (ch == '-') {
        fov++;
    }
    else if (ch == 'x') {
        azimuth_angle = 0;
        elevation_angle = 0;
    }
    else if (ch == 'y') {
        azimuth_angle = 0;
        elevation_angle = 90;
    }
    else if (ch == 'z') {
        azimuth_angle = 0;
        elevation_angle = -90;
    }
    glutPostRedisplay();
    update_projection_matrix();
    glutPostRedisplay();
}

void utils::update_projection_matrix(){
    glMatrixMode(GL_PROJECTION);
    double asp = 1;
    glLoadIdentity();
    if (mode_of_view == 1 || mode_of_view == 2)
        gluPerspective(fov,asp,utils::dims/4,4*utils::dims);
    else
        glOrtho(-asp*utils::dims,+asp*utils::dims, -utils::dims,+utils::dims, -utils::dims,+utils::dims);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void utils::print_orientation() {
    glWindowPos2i(5,5);
    glColor3f(1.0, 1.0, 1.0);
    std::string test;
    if (mode_of_view == 0){
        test = "Mode: Orthogonal";
    } else if (mode_of_view == 1){
        test = "Mode: Perspective";
    } else {
        test = "Mode: First Person";
    }
    const char *cstr = test.c_str();
    int len, i;
    len = (int)strlen(cstr);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, cstr[i]);
    }
}


void utils::view_mode_and_display(){
    display_3d_scene();
    std::cout << "Azimuth: " << azimuth_angle << " Elevation: " << elevation_angle << std::endl;
}


void utils::display_3d_scene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    if (mode_of_view == 0){
        // Orthogonal
        glRotatef(elevation_angle,1,0,0);
        glRotatef(azimuth_angle,0,1,0);
    }
    else if(mode_of_view == 1){
        // Perspective
        double Ex = 2*dims*Sin(azimuth_angle)*Cos(elevation_angle);
        double Ey = 2*dims*Sin(elevation_angle);
        double Ez = 2*dims*Cos(azimuth_angle)*Cos(elevation_angle);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(elevation_angle),0);
    }
    else if(mode_of_view == 2){
        // First Person View
        cam_look_x = -2*dims*Sin(azimuth_angle)*Cos(elevation_angle);
        cam_look_y = -2*dims*Sin(elevation_angle);
        cam_look_z = -2*dims*Cos(azimuth_angle)*Cos(elevation_angle);
        gluLookAt(cam_x, cam_y, cam_z, cam_look_x + cam_x, cam_look_y + cam_y, cam_look_z + cam_z, 0, 1, 0);
    }

    glRotatef(-elevation_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-1.0f, -1.5f, -2.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0f, 1.0f, 0.0f);
    glRotatef(azimuth_angle, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    // Draw House from above declared scale and Rotation
    draw_house(0.0);
    glRotatef(azimuth_angle, 0.5f, 1.5f, 0.0f);
    glScalef(1.2f, 1.2f, 1.2f);
    // Draw House from above declared new scale and Rotation
    draw_house(1);
    print_orientation();
    glFlush ();
    glPopMatrix();
    glutSwapBuffers();
}

void utils::draw_house(float translation) {
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_TRIANGLES);
    glVertex3f (0.8+translation, 0.6, 0.0);
    glVertex3f (0.8+translation, 0.6, 0.5);
    glVertex3f (0.8+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_TRIANGLES);
    glVertex3f (0.1+translation, 0.6, 0.0);
    glVertex3f (0.1+translation, 0.6, 0.5);
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.6, 0.0);
    glVertex3f (0.8+translation, 0.6, 0.0);
    glVertex3f (0.8+translation, 0.8, 0.25);
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.6, 0.5);
    glVertex3f (0.8+translation, 0.57, 0.5);
    glVertex3f (0.8+translation, 0.8, 0.25);
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.1, 0.0);
    glVertex3f (0.8+translation, 0.1, 0.0);
    glVertex3f (0.8+translation, 0.6, 0.0);
    glVertex3f (0.1+translation, 0.6, 0.0);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.1, 0.5);
    glVertex3f (0.8+translation, 0.1, 0.5);
    glVertex3f (0.8+translation, 0.6, 0.5);
    glVertex3f (0.1+translation, 0.6, 0.5);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.1, 0.5);
    glVertex3f (0.1+translation, 0.1, 0.0);
    glVertex3f (0.1+translation, 0.6, 0.0);
    glVertex3f (0.1+translation, 0.6, 0.5);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.8+translation, 0.1, 0.5);
    glVertex3f (0.8+translation, 0.1, 0.0);
    glVertex3f (0.8+translation, 0.6, 0.0);
    glVertex3f (0.8+translation, 0.6, 0.5);
    glEnd();
}

float utils::get_rand() {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r;
}

double utils::Sin(double x) {
    return sin((x)*3.1415927/180);
}

double utils::Cos(double x) {
    return cos((x)*3.1415927/180);
}








