#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "util.h"
#include "CSCIx229.h"


// Initializing Variables

int utils::axes=0;
int utils::mode=1;
int utils::move=1;
int utils::th=50;
int utils::ph=0;
int utils::fov=27;
int utils::light=1;
double utils::asp=1;
double utils::dim=3.0;
int utils::ntex = 1;
// Light values
int utils::one       =   1;
int utils::distance  =   2;
int utils::inc       =  10;
int utils::smooth    =   1;
int utils::local     =   0;
int utils::emission  =   0;
int utils::ambient   =  30;
int utils::diffuse   = 100;
int utils::specular  =   0;
int utils::shininess =   0;
float utils::shiny   =   1;
int utils::zh        =  90;
int utils::zph        =  0;
float utils::ylight  =   0;
unsigned int utils::textures[2];


int main(int argc, char** argv) {
    glutInit( & argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Kodur Krishna Chaitanya: HW6");
    glPointSize(1);
    glutDisplayFunc(utils::display_scene);
    glutReshapeFunc(utils::reshape_window);
    glutSpecialFunc(utils::special);
    glutKeyboardFunc(utils::key);
    glutIdleFunc(utils::idle);
    utils::textures[0] = LoadTexBMP("crate.bmp");
    utils::textures[1] = LoadTexBMP("rock_texture.bmp");
    glutMainLoop();
    return 0;
}
