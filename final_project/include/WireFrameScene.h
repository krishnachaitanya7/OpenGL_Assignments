/*
 * WireFrameScene.h
 *
 * Original Author: Samuel R. Buss
 * Adapted by Tim Wheeler
 */

#include "Terrain.h"
#include "TerrainBlock.h"

void myKeyboardFunc( unsigned char key, int x, int y );
void mySpecialKeyFunc( int key, int x, int y );

void drawScene(void);

void initRendering();
void initTerrain();
void resizeWindow(int w, int h);

void renderBlock(TerrainBlock *TB, float depth, float dispS, float dispT);
float lerp(float u, float v, float a);
void lerp(float* u, float* v, float a, int len, float* retval);
