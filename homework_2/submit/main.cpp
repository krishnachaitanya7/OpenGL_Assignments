#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "lorenz.h"
#include <iostream>
// variables Initialization
double lorenz_attractor::sigma = 10.0;
double lorenz_attractor::r = 28;
double lorenz_attractor::b = 8/3;
int lorenz_attractor::azi_angle = 0;
int lorenz_attractor::ele_angle = 0;
double lorenz_attractor::box_dim = 2;
double lorenz_attractor::points_accumulator[N][3];

int main(int argc, char *argv[]){
    std::string test2 {"Press Keys: sigma: s(increase) a(decrease), bias: b(increase) v(decrease), rho: r(increase) e(decrease)"};
    std::cout << test2 << std::endl;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500,500);
    glutCreateWindow("Krishna Chaitanya: Lorenz Attractor");
    glutDisplayFunc(lorenz_attractor::display_lorenz);
    glutReshapeFunc(lorenz_attractor::reshape_window);
    glutKeyboardFunc(lorenz_attractor::lorenz_keys);
    glutSpecialFunc(lorenz_attractor::arrow_keys_pressed);
    glutMainLoop();
    return 0;
}

