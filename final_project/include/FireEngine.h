#include "qd.h"
#pragma once

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