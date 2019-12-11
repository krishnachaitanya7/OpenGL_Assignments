#pragma once

#include "qd.h"

class QdParticle
{
protected:
	Qd3dPoint m_ptUL;		  
	Qd3dPoint m_ptLR;		  

	QdColor m_clrCurrent;	  
	QdColor m_clrFade;		  

	Qd3dPoint m_ptDirection;   
	Qd3dPoint m_ptGravity;	  

	int m_nFrames;			  
	float m_fAge;			  

	float m_fLife;			  
	float m_fGravityFactor;
	float m_fFadeLife;
	int m_nParticle;
public:
	QdParticle();
	void draw();
	void advance();

	friend class QdParticleEngine;
};

class QdParticleEngine
{
protected:
	bool m_bLimit;
	QdParticle *m_pParticles;
	bool m_bEngineActive;
	int m_nParticles;
	int m_nAge;

	float m_fLimitR;
	float m_fLimitL;
	float m_fLimitT;
	float m_fLimitB;

	char *m_szImgPath;
	char m_szImgPathA[128];
public:
	int setLimit(float fLimitL, float fLimitR, float fLimitT, float fLimitB);
	int setLimit(bool bLimit);
	int setCurrentColor(int nParticle, float fRed, float fGreen, float fBlue, float fAlpha);
	int setFadeColor(int nParticle, float fRed, float fGreen, float fBlue, float fAlpha);
	int setDirection(int nParticle, float fX, float fY, float fZ);
	int setGravity(int nParticle, float fX, float fY, float fZ);
	int setLR(int nParticle, float fX, float fY, float fZ);
	int setUL(int nParticle, float fX, float fY, float fZ);
	int setAge(int nParticle, float fAge);
	int setFrame(int nParticle, int nFrame);
	int setLife(int nParticle, float fLife);
	int setFadeLife(int nParticle, float fFadeLife);
	int draw();
	QdParticleEngine();
	~QdParticleEngine();
	int init(int nParticles);
	int virtual resetParticles();

	  
	virtual int particleDead(int nParticle);
	int destroy();
};