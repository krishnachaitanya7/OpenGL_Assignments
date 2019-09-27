#include "util.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

void utils::reshape_window(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (double) w / (double) h, 1.0, 100.0);
}

void utils::arrow_keys_pressed(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        vertical_angle -= 1;
    } else if (key == GLUT_KEY_DOWN) {
        vertical_angle += 1;
    } else if (key == GLUT_KEY_RIGHT){
        horizontal_angle -= 1;
    } else if (key == GLUT_KEY_LEFT) {
        horizontal_angle += 1;
    }
    glutPostRedisplay();
}

void utils::display_3d_scene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-vertical_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-1.0f, -1.5f, -2.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0f, 1.0f, 0.0f);
    glRotatef(horizontal_angle, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    // Draw House from above declared scale and Rotation
    draw_house(0.0);
    glRotatef(horizontal_angle, 0.5f, 1.5f, 0.0f);
    glScalef(1.2f, 1.2f, 1.2f);
    // Draw House from above declared new scale and Rotation
    draw_house(1);
    glFlush ();
    glPopMatrix();
    glutSwapBuffers();
}

void utils::draw_house(float translation) {
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_TRIANGLES);
    glVertex3f (0.8+translation, 0.575, 0.0);
    glVertex3f (0.8+translation, 0.575, 0.5);
    glVertex3f (0.8+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_TRIANGLES);
    glVertex3f (0.1+translation, 0.575, 0.0);
    glVertex3f (0.1+translation, 0.575, 0.5);
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.575, 0.0);
    glVertex3f (0.8+translation, 0.575, 0.0);
    glVertex3f (0.8+translation, 0.8, 0.25);
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.575, 0.5);
    glVertex3f (0.8+translation, 0.57, 0.5);
    glVertex3f (0.8+translation, 0.8, 0.25);
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.1, 0.0);
    glVertex3f (0.8+translation, 0.1, 0.0);
    glVertex3f (0.8+translation, 0.575, 0.0);
    glVertex3f (0.1+translation, 0.575, 0.0);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.1, 0.5);
    glVertex3f (0.8+translation, 0.1, 0.5);
    glVertex3f (0.8+translation, 0.575, 0.5);
    glVertex3f (0.1+translation, 0.575, 0.5);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.1+translation, 0.1, 0.5);
    glVertex3f (0.1+translation, 0.1, 0.0);
    glVertex3f (0.1+translation, 0.575, 0.0);
    glVertex3f (0.1+translation, 0.575, 0.5);
    glEnd();
    glColor3f (get_rand(), get_rand(), get_rand());
    glBegin(GL_POLYGON);
    glVertex3f (0.8+translation, 0.1, 0.5);
    glVertex3f (0.8+translation, 0.1, 0.0);
    glVertex3f (0.8+translation, 0.575, 0.0);
    glVertex3f (0.8+translation, 0.575, 0.5);
    glEnd();
}

float utils::get_rand() {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r;
}






