#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <GL/gl.h>
#pragma once



  
#define PSI_FONT_ENGINE_VERSION 2.57
#define PSI_FONT_BOLD 1
#define PSI_FONT_ITALIC 2
#define PSI_FONT_UNDERLINE 4
#define PSI_FONT_STRIKETHROUGH 8
#define PSI_FONT_3D 1
#define PSI_FONT_BITMAP 0
#define PSI_MAX_CHARS	256									
#define PSI_DEFAULT_FONT_EXTRUDE 0.40f
#define PSI_LINE_FONT 1
#define PSI_POLYGON_FONT 0

  
#define QD_NO_PATH_GIVEN 207
#define QD_INVALID_RESOURCE 208
#define QD_SUCCESS 1
#define QD_NO_AUTO_TEXTURE 571
#define QD_AUTO_TEXURE 572

  

class Qd3dPoint
{
public:
	float m_fX;
	float m_fY;
	float m_fZ;
};

class QdColor
{
public:
	float m_fR;
	float m_fG;
	float m_fB;
	float m_fA;
};

#include "Texture.h"
#include "Particle.h"

