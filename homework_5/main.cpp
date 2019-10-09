#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "util.h"

// Main program
int main(int argc, char** argv) {
    glutInit( & argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Kodur Krishna Chaitanya: HW5");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(utils::display_scene);
    glutReshapeFunc(utils::reshape_window);
    glutMainLoop();
    return 0;
}
