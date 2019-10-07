#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "util.h"

// Initialize required Variables for static class
float utils::azimuth_angle = 0.0f;
float utils::elevation_angle = 0.0f;
double utils::cam_x = 0;
double utils::cam_y = 0;
double utils::cam_z = 10;
double utils::cam_look_x = 0;
double utils::cam_look_y = 0;
double utils::cam_look_z = 0;
int utils::fov = 60;
double utils::dims = 3;
int utils::mode_of_view = 1;




// Main program
int main(int argc, char** argv) {
    glutInit( & argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Kodur Krishna Chaitanya: HW4");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(utils::view_mode_and_display);
    glutReshapeFunc(utils::reshape_window);
    glutSpecialFunc(utils::arrow_keys_pressed);
    glutKeyboardFunc(utils::keys_pressed);
    glutMainLoop();
    return 0;
}
