#ifndef C_ENV_COLORGRADE_H
#define C_ENV_COLORGRADE_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "shareddefs_estranged.h"

class C_EnvColorGrade : public C_BaseEntity
{
	DECLARE_CLASS( C_EnvColorGrade, C_BaseEntity );
public:
	DECLARE_CLIENTCLASS();

	C_EnvColorGrade();
	~C_EnvColorGrade();

	virtual void OnDataChanged( DataUpdateType_t updateType );
	virtual void Simulate();

	virtual void ClientThink();

	int GetInfluenceRadius();
	ColorGradingData_t GetColorGradingData();

	bool IsGlobalGrader();
private:
	void	AddToManager();
	void	RemoveFromManager();

	int m_iInfluenceRadius;

	Vector m_vContrast;
	Vector m_vBrightness;
	Vector4D m_vLevels_r;
	Vector4D m_vLevels_g;
	Vector4D m_vLevels_b;
	float m_flSaturation;
	float m_flGamma;

	bool m_bAddedToManager;
	bool m_bGlobalGrader;

	ColorGradingData_t m_ColorGradingData;
};

#endif //C_ENV_COLORGRADE_H