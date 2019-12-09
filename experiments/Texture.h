/*	Ken Mazaika
	February 23, 2005
	Texture Engine
	Copyright 2005
	http://home.comcast.net/~kjmaz
  */

#pragma once

#include "qd.h"
class QdTexture
{
public:
	QdTexture();
	~QdTexture();

	//Select this texture to be automapped to future objects
	int SelectTexture();
	//Load the texture from a bitmap file
	int LoadFromBitmap(char *szPath);
	//Set the mode for the texture
	int SetMode(int nMode);

protected:

	unsigned int m_nTexture;
	bool m_bAutoTexture;
};