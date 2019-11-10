#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "include/util.h"
#include "include/CSCIx229.h"

/*
 * Some Good Resources
 * https://blogs.igalia.com/itoral/2016/10/13/opengl-terrain-renderer-rendering-the-terrain-mesh/
 * https://www.3dgep.com/multi-textured-terrain-in-opengl/
 * */

int utils::axes=1;       //  Display axes
int utils::mode=0;       //  Shader mode
int utils::move=1;       //  Move light
int utils::proj=1;       //  Projection type
int utils::th=0;         //  Azimuth of view angle
int utils::ph=0;         //  Elevation of view angle
int utils::fov=55;       //  Field of view (for perspective)
float utils::spc=1;     //  Specular intensity
double utils::asp=1;    //  Aspect ratio
double utils::dim=3.0;   //  Size of world
double utils::scale=0.5; //  Image scale
int utils::zh=90;        //  Light azimuth
float utils::Ylight=2;
float utils::RGBA[4] = {1,1,1,1};  //  Colors
int utils::obj = 1;



int main(int argc,char* argv[]){
    utils my_utils;
    //  Initialize GLUT
    glutInit(&argc,argv);
//    if (argc!=2 && argc!=3 && argc!=6) Fatal("Usage: %s <obj> [scale [R G B]]\n",argv[0]);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Final Project");
    //  Set callbacks
    glutDisplayFunc(utils::display);
    glutReshapeFunc(utils::reshape);
    glutSpecialFunc(utils::special);
    glutKeyboardFunc(utils::key);
    glutIdleFunc(utils::idle);
    //  Load object
    utils::obj = LoadOBJ("Plane.obj");
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}