#include "Texture.h"
#include "CSCIx229.h"


int QdTexture::SelectTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_nTexture);
	return true;

}

int QdTexture::LoadFromBitmap(char *szPath)
{

    return LoadTexBMP(szPath);

}


QdTexture::QdTexture()
{
	m_bAutoTexture = 1;  //Set to autotexture by default
}

QdTexture::~QdTexture()
{

}

int QdTexture::SetMode(int nMode)
{
	if(nMode == QD_NO_AUTO_TEXTURE)
	{
		m_bAutoTexture = 0;
	}
	else if(nMode == QD_AUTO_TEXURE)
	{
		m_bAutoTexture = 1;
	}
	return QD_SUCCESS;
}