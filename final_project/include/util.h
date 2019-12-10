#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

#include <vector>
#include "TerrainBlock.h"
#include "Terrain.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <array>
#include "FireEngine.h"

class utils {
public:
    /*
     * Reference: ex13 from Moodle
     * */
    static int axes;
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
    static int obj;
    static float plane_z;
    static bool display_terrain;
    static unsigned int textures[2];
    static unsigned int shaderint;
    static std::vector<float> y_points;
    static void display();
    static void idle();
    static void special(int key,int x,int y);
    static void key(unsigned char ch,int x,int y);
    static void reshape(int width,int height);
    static bool collision_detection();
    static float rand_float();



    static void renderBlock(TerrainBlock* TB, float depth, float dispS, float dispT);
    static float lerp(float u, float v, float a);
    static void initTerrain();
    static int TRI_DEPTH;
    static int TRI_BLOCKS;
    static float plane_x;
    static float plane_y;
    static float plane_alt;

//dynamics
    static float PLANE_SPEED;
    static float plane_yaw;

//gimbal
    static float Azimuth;
    static const float timeStep;
    static float tick;
    static const float PI; //pi
//misc
    static float maxRenderDepth;
    static int gradientDepth;
    static int SmoothShading;

    static Terrain *myTerrain;

    // Particle Engine
    static QdFireEngine pe0;
    static QdFireEngine pe1;
    static QdFireEngine pe2;
    static QdFireEngine pe3;
    static QdFireEngine pe4;
    static QdFireEngine pe5;
    static QdFireEngine pe6;
    static QdFireEngine pe7;
    static QdFireEngine pe8;
    static QdFireEngine pe9;
    static QdFireEngine pe10;
    static QdFireEngine pe11;
    static QdFireEngine pe12;
    static QdFireEngine pe13;
    static QdFireEngine pe14;
    static QdFireEngine pe15;
    static QdFireEngine pe16;


};
#endif //OPENGL_ASSIGNMENTS_LORENZ_H
