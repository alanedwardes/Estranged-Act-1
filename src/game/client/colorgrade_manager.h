#ifndef COLORGRADING_MGR_H
#define COLORGRADING_MGR_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "c_env_colorgrade.h"
#include "shareddefs_estranged.h"

class CColorGradingMgr : public CAutoGameSystemPerFrame
{
	typedef CAutoGameSystemPerFrame BaseClass;
public:
	CColorGradingMgr();
	~CColorGradingMgr();

	virtual bool Init();
	virtual void Shutdown();

	virtual void AddGrader(C_EnvColorGrade *pC_EnvColorGrade);
	virtual void RemoveGrader(C_EnvColorGrade *pC_EnvColorGrade);

	ColorGradingData_t GetColorGradingData();
private:
	virtual void Update( float frametime );

	void			SelectGrader();
	void			BlendGradingData();

	void			Cleanup();

	CUtlVector<C_EnvColorGrade*> m_pGraders;
	ColorGradingData_t m_ColorGradingData;
	ColorGradingData_t m_BlendedColorGradingData;
};

//Singleton access
extern CColorGradingMgr *g_pColorGradingMgr;


#endif // COLORGRADING_MGR_H
