/*	Ken Mazaika
	February 23, 2005
	Quagmire Particle Engine::Fire Engine

  */

#include "FireEngine.h"
#include <math.h>

//Allow the easy change of a flame color, through setting it in the constructor
QdFireEngine::QdFireEngine(float fRed, float fGreen, float fBlue, float fAlpha)
{
	m_clrParticleColor.m_fA = fAlpha;
	m_clrParticleColor.m_fB = fBlue;
	m_clrParticleColor.m_fG = fGreen;
	m_clrParticleColor.m_fR = fRed;
}

//This initializes the particles/reinitializes particles that faded
int QdFireEngine::particleDead(int nParticle)
{
	int nCount = nParticle;
	setCurrentColor(nCount, m_clrParticleColor.m_fR, m_clrParticleColor.m_fG, m_clrParticleColor.m_fB,m_clrParticleColor.m_fA);
	
	float fRand= (float)(rand()%500);
	fRand = fRand/50000;
	setFadeColor(nCount, 0, 0, 0,-fRand-.01f);
	setFadeLife(nCount, -fRand-.001f);

	float fX;
	float fY;

	fRand= (float)(rand()%500);
	fRand = fRand/500;

	fX = fRand-.5f;
	fRand= (float)(rand()%500);
	fRand = fRand/500;
	fY = fRand;

	fX = fX/1000;
	fY = fY/50;

	setDirection(nCount,0, fY, 0);
	setGravity(nCount, 0, 0, 0);
	fRand= (float)(rand()%400);
	fRand = fRand/500;
	setLR(nCount,fRand-.4f, -1, 0);
	setUL(nCount, fRand-.4f+0.3f, -0.7f, 0);
	setAge(nCount, 0);
	setFrame(nCount, 0);

	setLife(nCount, 1);

	return QD_SUCCESS;
}

//This function is called by default by init() and sets the particles with
//particleDead.
int QdFireEngine::resetParticles()
{
	int nCount=0;
	
	//Give the random number generator a seed
	srand(time(NULL));

	while(nCount != m_nParticles)
	{
		particleDead(nCount);
		nCount++;
	}
	return QD_SUCCESS;
}
