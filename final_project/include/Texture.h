#pragma once

#include "qd.h"
class QdTexture
{
public:
	QdTexture();
	~QdTexture();

	  
	int SelectTexture();
	  
	int LoadFromBitmap(char *szPath);
	  
	int SetMode(int nMode);

protected:

	unsigned int m_nTexture;
	bool m_bAutoTexture;
};