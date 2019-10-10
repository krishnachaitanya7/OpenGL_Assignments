#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "util.h"


// Initializing Variables

int utils::axes=1;       //  Display axes
int utils::mode=1;       //  Projection mode
int utils::move=1;       //  Move light
int utils::th=0;         //  Azimuth of view angle
int utils::ph=0;         //  Elevation of view angle
int utils::fov=55;       //  Field of view (for perspective)
int utils::light=1;      //  Lighting
double utils::asp=1;     //  Aspect ratio
double utils::dim=3.0;   //  Size of world
// Light values
int utils::one       =   1;  // Unit value
int utils::distance  =   2;  // Light distance
int utils::inc       =  10;  // Ball increment
int utils::smooth    =   1;  // Smooth/Flat shading
int utils::local     =   0;  // Local Viewer Model
int utils::emission  =   0;  // Emission intensity (%)
int utils::ambient   =  30;  // Ambient intensity (%)
int utils::diffuse   = 100;  // Diffuse intensity (%)
int utils::specular  =   0;  // Specular intensity (%)
int utils::shininess =   0;  // Shininess (power of two)
float utils::shiny   =   1;  // Shininess (value)
int utils::zh        =  90;  // Light azimuth
int utils::zph        =  0;  // Light azimuth
float utils::ylight  =   0;
double utils::elapsed_time = 0;


int main(int argc, char** argv) {
    glutInit( & argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Kodur Krishna Chaitanya: HW5");
    glPointSize(1);
    glutDisplayFunc(utils::display_scene);
    glutReshapeFunc(utils::reshape_window);

    glutSpecialFunc(utils::special);
    glutKeyboardFunc(utils::key);
    glutIdleFunc(utils::idle);

    glutMainLoop();
    return 0;
}
