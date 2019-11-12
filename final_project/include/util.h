#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

#include "TerrainBlock.h"
#include "Terrain.h"

class utils {
public:
    /*
     * Reference: ex13 from Moodle
     * */
    static int axes;       //  Display axes
    static int mode;       //  Shader mode
    static int move;       //  Move light
    static int proj;       //  Projection type
    static int th;         //  Azimuth of view angle
    static int ph;         //  Elevation of view angle
    static int fov;       //  Field of view (for perspective)
    static float spc;     //  Specular intensity
    static double asp;     //  Aspect ratio
    static double dim;   //  Size of world
    static double scale; //  Image scale
    static int zh;        //  Light azimuth
    static float Ylight;   //  Light elevation
    static int obj;          //  Object display list
    static float RGBA[4];  //  Colors
    static void display();
    static void idle();
    static void special(int key,int x,int y);
    static void key(unsigned char ch,int x,int y);
    static void reshape(int width,int height);

    static void renderBlock(TerrainBlock* TB, float depth, float dispS, float dispT);
    static float lerp(float u, float v, float a);
    static void lerp(float* u, float* v, float a, int len, float* retval);
    static void initTerrain();



    static float NoEmit[4];

//Terrain material properties
    static float TerrainShininess;	// Specular exponent (we don't use specular)
    static float TerrainAmbDiff[4];  // The ambient/diffuse colors
    static float TerrainSpecular[4];	//the specular color (none!)

///////////////////////
/// Lighting Values ///
///////////////////////

//ambient light
    static float ambientLight[4];  //ambient scene (white) light
    static float Ltdiff[4];
    static float Ltspec[4];

////////////////////////
///   Model Values   ///
////////////////////////

    static int TRI_DEPTH;
    static int TRI_BLOCKS;


////////////////////////
///   Plane Model    ///
////////////////////////

//position
    static float plane_x;  //using x & y space to represent the plane location
    static float plane_y;
    static float plane_alt;  //the plane altitude

//dynamics
    static float PLANE_SPEED;  //speed per tick
    static float plane_yaw;  // yaw [rad]
    static float PLANE_YAW_RATE;  //yaw rate [rad]

//gimbal
    static float Azimuth;			// Rotated up or down by this amount
    static float azimuth_max;
    static float azimuth_min;
    static float AngleStepSize;		// Step three degrees at a time
    static const float timeStep;
    static float tick;
    static const float AngleStepMax;
    static const float AngleStepMin;
    static const float PI; //pi

//misc
    static float maxRenderDepth;
    static int gradientDepth;  //if 1, then render with gradient depth   if 0, render with static depth

///////////////////////////
/// Global State Values ///
///////////////////////////
    static int WireFrameOn;			// == 1 for wire frame mode
    static int CullBackFacesOn;		// == 1 if culling back faces.
    static int SmoothShading;       // == 1 if smooth, 0 if flat

    static Terrain *myTerrain;

};
#endif //OPENGL_ASSIGNMENTS_LORENZ_H
