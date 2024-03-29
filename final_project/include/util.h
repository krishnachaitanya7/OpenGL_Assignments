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

    static int axes;
    static int move;
    static int proj;
    static int th;
    static int ph;
    static int fov;         
    static float spc;       
    static double asp;       
    static double dim;     
    static double scale;   
    static int zh;          
    static float Ylight;     
    static int obj;
    static float plane_z;
    static bool display_terrain;
    static unsigned int textures[2];
    static bool toggletexture;
    static bool day_night;
    static bool fog;
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

 
    static float PLANE_SPEED;
    static float plane_yaw;

 
    static float Azimuth;
    static const float timeStep;
    static float tick;
    static const float PI;   
 
    static float maxRenderDepth;
    static int gradientDepth;
    static int SmoothShading;

    static Terrain *myTerrain;

      
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
#endif   
