#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "lorenz.h"
#include <iostream>

int main(int argc, char *argv[]){
    std::string test2 {"Press Keys: sigma: s(increase) a(decrease), bias: b(increase) v(decrease), rho: r(increase) e(decrease)"};
    std::cout << test2 << std::endl;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);H
            glutInitWindowSize(500,500);
    glutCreateWindow("Krishna Chaitanya: Lorenz Attractor");
    glutDisplayFunc(lorenz_attractor::display_lorenz);
    glutReshapeFunc(lorenz_attractor::reshape_window);
    glutKeyboardFunc(lorenz_attractor::lorenz_keys);
    glutSpecialFunc(lorenz_attractor::arrow_keys_pressed);
    glutMainLoop();
    return 0;
}

