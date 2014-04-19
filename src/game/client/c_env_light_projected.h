#include "cbase.h"

#ifndef C_ENV_LIGHT_PROJECTED_H
#define C_ENV_LIGHT_PROJECTED_H
#ifdef _WIN32
#pragma once
#endif

class C_EnvLightProjected : public C_BaseEntity
{
	DECLARE_CLASS( C_EnvLightProjected, C_BaseEntity );
public:
	DECLARE_CLIENTCLASS();

	virtual void OnDataChanged( DataUpdateType_t updateType );
	void	ShutDownLightHandle( void );

	virtual void Simulate();

	virtual void ClientThink();

	void	UpdateLightSettings( const CCommand &args );

	void	UpdateLight( bool bForceUpdate );

	Vector  GetLightOrigin();

	int		GetEnableRadius();
	bool	IsFineWithoutShadows();
	bool	IsLightOn();

	void	SetManagerShadowState(bool state);

	C_EnvLightProjected();
	~C_EnvLightProjected();

private:
	void	AddToManager();
	void	RemoveFromManager();

	ClientShadowHandle_t m_LightHandle;

	EHANDLE	m_hTargetEntity;

	bool	m_bAddedToManager;
	bool	m_bShadowsOnClient;
	bool	m_bState;
	bool	m_bOnlyDrawIfShadowed;
	float	m_flLightFOV;
	float	m_fLinearAtten;
	float	m_flFilterSize;
	bool	m_bEnableShadows;
	bool	m_bLightOnlyTarget;
	bool	m_bLightWorld;
	bool	m_bCameraSpace;
	int		m_iEnableShadowRadius;
	Vector  m_vRadiusOrigin;
	Vector	m_LinearFloatLightColor;
	float	m_flAmbient;
	float	m_flNearZ;
	float	m_flFarZ;
	float	m_flPatternBrightnessModifier;
	int		m_iPatternIndex;
	CTextureReference m_SpotlightTexture;
	char	m_SpotlightTextureName[ MAX_PATH ];
	char	m_szPattern[64];
	int		m_nSpotlightTextureFrame;
	int		m_nShadowQuality;
};

#endif