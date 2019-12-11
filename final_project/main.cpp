#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/glut.h>
#include "include/util.h"
#include "CSCIx229.h"
#include "TerrainBlock.h"
#include <iostream>


/*
 * References
 * https://blogs.igalia.com/itoral/2016/10/13/opengl-terrain-renderer-rendering-the-terrain-mesh/
 * https://www.3dgep.com/multi-textured-terrain-in-opengl/
 * http://tim.hibal.org/blog/fractal-terrain-midpoint-displacement-algorithm/
 * https://www.codeproject.com/Articles/9673/Quagmire-Particle-Engine
 * */

int utils::axes=1;
int utils::move=1;       //  Move light
int utils::proj=1;       //  Projection type
int utils::th=0;         //  Azimuth of view angle
int utils::ph=0;         //  Elevation of view angle
float utils::plane_z=-1.0;
int utils::fov=55;       //  Field of view (for perspective)
float utils::spc=1;     //  Specular intensity
double utils::asp=1;    //  Aspect ratio
double utils::dim=3.0;   //  Size of world
double utils::scale=0.4; //  Image scale
int utils::zh=90;        //  Light azimuth
float utils::Ylight=2;
int utils::obj = 1;
bool utils::display_terrain = true;

int utils::TRI_DEPTH = 8;
int utils::TRI_BLOCKS = 10;
Terrain *utils::myTerrain;

//position
float utils::plane_x = 33.0;  //using x & y space to represent the plane location
float utils::plane_y = 10.0;
float utils::plane_alt = 5.2;  //the plane altitude

//dynamics
float utils::PLANE_SPEED = 1.0;
float utils::plane_yaw = 10.0;
float utils::Azimuth = -7.0f;
const float utils::timeStep = 0.01;
float utils::tick = 0;
const float utils::PI = 3.14159265f;
float utils::maxRenderDepth = 3.0f;
int utils::gradientDepth = 0;
int utils::SmoothShading = 1;
unsigned int utils::textures[2];
bool utils::toggletexture = true;
bool utils::day_night = true;
bool utils::fog = false;
std::vector<float> utils::y_points;
QdFireEngine utils::pe0(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe1(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe2(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe3(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe4(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe5(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe6(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe7(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe8(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe9(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe10(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe11(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe12(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe13(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe14(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe15(1.0f, 0.2f, 0.0f, 0.5f);
QdFireEngine utils::pe16(1.0f, 0.2f, 0.0f, 0.5f);
//




int main(int argc,char* argv[]){
    glewExperimental = GL_TRUE;
    //  Initialize GLUT
    glutInit(&argc,argv);
    utils::initTerrain();
//    if (argc!=2 && argc!=3 && argc!=6) Fatal("Usage: %s <obj> [scale [R G B]]\n",argv[0]);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);


    glutInitWindowSize(600,600);
    glutCreateWindow("Kodur Krishna Chaitanya: Final Project");
    if(glewInit() != GLEW_OK){
        std::cout << "GLEW NOT OKAY!"<< std::endl;
    }
    //  Set callbacks

//    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(utils::display);
    glutReshapeFunc(utils::reshape);
    glutSpecialFunc(utils::special);
    glutKeyboardFunc(utils::key);

//    glutIdleFunc(utils::idle);
    //  Load object
    utils::obj = LoadOBJ("Plane.obj");
    utils::textures[0] = LoadTexBMP("crate.bmp");
    utils::textures[1] = LoadTexBMP("crate1.bmp");

    utils::pe0.setLimit(-2, 2, -1, 3);
    utils::pe0.init(10000);

    utils::pe1.setLimit(-2, 2, -1, 3);
    utils::pe1.init(1000);

    utils::pe2.setLimit(-2, 2, -1, 3);
    utils::pe2.init(10000);

    utils::pe3.setLimit(-2, 2, -1, 3);
    utils::pe3.init(1000);

    utils::pe4.setLimit(-2, 2, -1, 3);
    utils::pe4.init(1000);

    utils::pe5.setLimit(-2, 2, -1, 3);
    utils::pe5.init(1000);

    utils::pe6.setLimit(-2, 2, -1, 3);
    utils::pe6.init(10000);

    utils::pe7.setLimit(-2, 2, -1, 3);
    utils::pe7.init(10000);

    utils::pe8.setLimit(-2, 2, -1, 3);
    utils::pe8.init(10000);

    utils::pe9.setLimit(-2, 2, -1, 3);
    utils::pe9.init(1000);

    utils::pe10.setLimit(-2, 2, -1, 3);
    utils::pe10.init(1000);

    utils::pe11.setLimit(-2, 2, -1, 3);
    utils::pe11.init(10000);

    utils::pe12.setLimit(-2, 2, -1, 3);
    utils::pe12.init(1000);

    utils::pe13.setLimit(-2, 2, -1, 3);
    utils::pe13.init(10000);

    utils::pe14.setLimit(-2, 2, -1, 3);
    utils::pe14.init(10000);

    utils::pe15.setLimit(-2, 2, -1, 3);
    utils::pe15.init(10000);

    utils::pe16.setLimit(-2, 2, -1, 3);
    utils::pe16.init(1000);


//    utils::pe = &pde;

    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}