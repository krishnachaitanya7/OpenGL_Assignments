/*	Ken Mazaika
	February 23, 2005
	Quagmire Particle Engine::Fire Engine

  */


#include "qd.h"

class QdFireEngine: public QdParticleEngine
{
public:

	QdFireEngine(float fRed, float fGreen, float fBlue, float fAlpha);
	virtual int resetParticles();
	virtual int particleDead(int nParticle);
	int m_nParticlesDead;

protected:

	QdColor m_clrParticleColor;
};