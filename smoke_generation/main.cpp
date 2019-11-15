#define GL_GLEXT_PROTOTYPES

#include "utils.h"
#include <GL/glut.h>
#include "particle_generator.h"
#include "resource_manager.h"


/*
 * Some Good Resources
 * https://blogs.igalia.com/itoral/2016/10/13/opengl-terrain-renderer-rendering-the-terrain-mesh/
 * https://www.3dgep.com/multi-textured-terrain-in-opengl/
 * */

int utils::axes=1;       /*  Display axes*/
int utils::mode=0;       //  Shader mode
int utils::move=1;       //  Move light
int utils::proj=1;       //  Projection type
int utils::th=0;         //  Azimuth of view angle
int utils::ph=0;         //  Elevation of view angle
int utils::fov=55;       //  Field of view (for perspective)
float utils::spc=1;     //  Specular intensity
double utils::asp=1;    //  Aspect ratio
double utils::dim=3.0;   //  Size of world
double utils::scale=0.4; //  Image scale
int utils::zh=90;        //  Light azimuth
float utils::Ylight=2;
float utils::RGBA[4] = {1,1,1,1};  //  Colors
int utils::obj = 1;

float utils::NoEmit[4] = {0.0, 0.0, 0.0, 1.0};

//Terrain material properties
float utils::TerrainShininess = 10;	// Specular exponent (we don't use specular)
float utils::TerrainAmbDiff[4] =  {0.1, 0.4, 0.1, 1.0};  // The ambient/diffuse colors
float utils::TerrainSpecular[4] = {0.0, 0.0, 0.0, 1.0};	//the specular color (none!)

///////////////////////
/// Lighting Values ///
///////////////////////

//ambient light
float utils::ambientLight[4] = {0.4, 0.4, 0.4, 1.0};  //ambient scene (white) light
float utils::Ltdiff[4] = {0.2,  0.2,  0.2, 1.0};
float utils::Ltspec[4] = {0.2, 0.2, 0.2, 1.0};

////////////////////////
///   Model Values   ///
////////////////////////

int utils::TRI_DEPTH = 8;
int utils::TRI_BLOCKS = 10;

////////////////////////
///   Plane Model    ///
////////////////////////

//position
float utils::plane_x = 33.0;  //using x & y space to represent the plane location
float utils::plane_y = 10.0;
float utils::plane_alt = 5.2;  //the plane altitude

//dynamics
float utils::PLANE_SPEED = 1.0;  //speed per tick
float utils::plane_yaw = 10.0;  // yaw [rad]
float utils::PLANE_YAW_RATE = 0.02;  //yaw rate [rad]

//gimbal
float utils::Azimuth = -7.0f;			// Rotated up or down by this amount
float utils::azimuth_max = 40.0f;
float utils::azimuth_min = 0.0f;
float utils::AngleStepSize = 3.0f;		// Step three degrees at a time
const float utils::timeStep = 0.01;
float utils::tick = 0;
const float utils::AngleStepMax = 10.0f;
const float utils::AngleStepMin = 0.1f;
const float utils::PI = 3.14159265f; //pi

//misc
float utils::maxRenderDepth = 3.0f;
int utils::gradientDepth = 0;  //if 1, then render with gradient depth   if 0, render with static depth

///////////////////////////
/// Global State Values ///
///////////////////////////
int utils::WireFrameOn = 0;			// == 1 for wire frame mode
int utils::CullBackFacesOn = 1;		// == 1 if culling back faces.
int utils::SmoothShading = 1;          // == 1 if smooth, 0 if flat
unsigned int utils::textures[1];
ParticleGenerator* utils::Particles;

int main(int argc,char* argv[]){

    //  Initialize GLUT

    glutInit(&argc,argv);


//    if (argc!=2 && argc!=3 && argc!=6) Fatal("Usage: %s <obj> [scale [R G B]]\n",argv[0]);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600,600);
    glutCreateWindow("Smoke");
    GLenum err = glewInit();
    //  Set callbacks
    glClearColor(1.0, 1.0, 1.0, 0.0);
    ResourceManager::LoadShader("particle.vs", "particle.frag", nullptr, "particle");
    ResourceManager::LoadTexture("particle.png", GL_TRUE, "particle");
    utils::Particles = new ParticleGenerator(
            ResourceManager::GetShader("particle"),
            ResourceManager::GetTexture("particle"),
            500
    );
    glutDisplayFunc(utils::display);
    glutReshapeFunc(utils::reshape);
    glutSpecialFunc(utils::special);
    glutKeyboardFunc(utils::key);
    glutMainLoop();
    return 0;
}