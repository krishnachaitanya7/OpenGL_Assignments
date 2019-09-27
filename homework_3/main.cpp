#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "util.h"

// Initialize required Variables for static class
float utils::horizontal_angle = 50.0f;
float utils::vertical_angle = 0.0f;

// Main program
int main(int argc, char** argv) {
    glutInit( & argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Kodur Krishna Chaitanya: HW3");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(utils::display_3d_scene);
    glutReshapeFunc(utils::reshape_window);
    glutSpecialFunc(utils::arrow_keys_pressed);
    glutMainLoop();
    return 0;
}
