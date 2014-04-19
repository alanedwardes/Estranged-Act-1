//====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: Entity to project light, optionally with realtime shadows.
//
//=============================================================================

#include "cbase.h"
#include "shareddefs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CEnvLightProjected : public CPointEntity
{
	DECLARE_CLASS( CEnvLightProjected, CPointEntity );
public:
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	CEnvLightProjected();
	bool KeyValue( const char *szKeyName, const char *szValue );

	// Always transmit to clients
	virtual int UpdateTransmitState();
	virtual void Activate( void );

	virtual void Spawn( void );
	void InputToggle( inputdata_t &inputdata );
	void InputTurnOn( inputdata_t &inputdata );
	void InputTurnOff( inputdata_t &inputdata );
	void InputSetFOV( inputdata_t &inputdata );
	void InputSetNearZ( inputdata_t &inputdata );
	void InputSetFarZ( inputdata_t &inputdata );
	void InputSetLinearAtten( inputdata_t &inputdata );
	void InputSetFilterSize( inputdata_t &inputdata );
	void InputSetTarget( inputdata_t &inputdata );
	void InputSetCameraSpace( inputdata_t &inputdata );
	void InputSetLightOnlyTarget( inputdata_t &inputdata );
	void InputSetLightWorld( inputdata_t &inputdata );
	void InputSetEnableShadows( inputdata_t &inputdata );
	void InputSetLightColor( inputdata_t &inputdata );
	void InputSetSpotlightTexture( inputdata_t &inputdata );
	void InputSetPattern(inputdata_t &inputdata);
	void InputSetAmbient( inputdata_t &inputdata );

	void SetLinearAtten( float );

	void Think( void );
	bool IsTurnedOn( void );
	bool IsShadowMapped( void );

	void DoTurnOn( void );
	void DoTurnOff( void );

	CNetworkHandle( CBaseEntity, m_hTargetEntity );

private:
	COutputEvent		m_OnTurnOff;
	COutputEvent		m_OnTurnOn;

	CNetworkVar( bool, m_bOnlyDrawIfShadowed );
	CNetworkVar( bool, m_bState );
	CNetworkVar( int, m_iEnableShadowRadius );
	CNetworkVar( Vector, m_vRadiusOrigin );
	CNetworkVar( float, m_flLightFOV );
	CNetworkVar( float, m_fLinearAtten );
	CNetworkVar( float, m_flFilterSize );
	CNetworkVar( bool, m_bEnableShadows );
	CNetworkVar( bool, m_bLightOnlyTarget );
	CNetworkVar( bool, m_bLightWorld );
	CNetworkVar( bool, m_bCameraSpace );
	CNetworkVector( m_LinearFloatLightColor );
	CNetworkVar( float, m_flAmbient );
	CNetworkString( m_SpotlightTextureName, MAX_PATH );
	CNetworkString( m_szPattern, 64 );
	CNetworkVar( int, m_nSpotlightTextureFrame );
	CNetworkVar( float, m_flNearZ );
	CNetworkVar( float, m_flFarZ );
	CNetworkVar( int, m_nShadowQuality );
};

LINK_ENTITY_TO_CLASS( env_light_projected, CEnvLightProjected );

BEGIN_DATADESC( CEnvLightProjected )
	DEFINE_FIELD( m_hTargetEntity, FIELD_EHANDLE ),
	DEFINE_FIELD( m_bState, FIELD_BOOLEAN ),
	DEFINE_KEYFIELD( m_iEnableShadowRadius, FIELD_INTEGER, "radius" ),
	DEFINE_KEYFIELD( m_vRadiusOrigin, FIELD_VECTOR, "radiusorigin" ),
	DEFINE_KEYFIELD( m_flLightFOV, FIELD_FLOAT, "lightfov" ),
	DEFINE_KEYFIELD( m_flFilterSize, FIELD_FLOAT, "filtersize" ),
	DEFINE_KEYFIELD( m_fLinearAtten, FIELD_FLOAT, "linearatten" ),
	DEFINE_KEYFIELD( m_bEnableShadows, FIELD_BOOLEAN, "enableshadows" ),
	DEFINE_KEYFIELD( m_bLightOnlyTarget, FIELD_BOOLEAN, "lightonlytarget" ),
	DEFINE_KEYFIELD( m_bOnlyDrawIfShadowed, FIELD_BOOLEAN, "onlydrawifshadowed" ),
	DEFINE_KEYFIELD( m_bLightWorld, FIELD_BOOLEAN, "lightworld" ),
	DEFINE_KEYFIELD( m_bCameraSpace, FIELD_BOOLEAN, "cameraspace" ),
	DEFINE_KEYFIELD( m_flAmbient, FIELD_FLOAT, "ambient" ),
	DEFINE_AUTO_ARRAY( m_szPattern, FIELD_CHARACTER ),
	DEFINE_AUTO_ARRAY( m_SpotlightTextureName, FIELD_CHARACTER ),
	DEFINE_KEYFIELD( m_nSpotlightTextureFrame, FIELD_INTEGER, "textureframe" ),
	DEFINE_KEYFIELD( m_flNearZ, FIELD_FLOAT, "nearz" ),
	DEFINE_KEYFIELD( m_flFarZ, FIELD_FLOAT, "farz" ),
	DEFINE_KEYFIELD( m_nShadowQuality, FIELD_INTEGER, "shadowquality" ),
	DEFINE_FIELD( m_LinearFloatLightColor, FIELD_VECTOR ), 

	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
	DEFINE_INPUTFUNC( FIELD_VOID, "TurnOn", InputTurnOn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "TurnOff", InputTurnOff ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "FOV", InputSetFOV ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "NearZ", InputSetNearZ ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "FarZ", InputSetFarZ ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "LinearAtten", InputSetLinearAtten ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "FilterSize", InputSetFilterSize ),
	DEFINE_INPUTFUNC( FIELD_EHANDLE, "Target", InputSetTarget ),
	DEFINE_INPUTFUNC( FIELD_BOOLEAN, "CameraSpace", InputSetCameraSpace ),
	DEFINE_INPUTFUNC( FIELD_BOOLEAN, "LightOnlyTarget", InputSetLightOnlyTarget ),
	DEFINE_INPUTFUNC( FIELD_BOOLEAN, "LightWorld", InputSetLightWorld ),
	DEFINE_INPUTFUNC( FIELD_BOOLEAN, "EnableShadows", InputSetEnableShadows ),
	DEFINE_INPUTFUNC( FIELD_STRING, "LightColor", InputSetLightColor ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "Ambient", InputSetAmbient ),
	DEFINE_INPUTFUNC( FIELD_STRING, "SpotlightTexture", InputSetSpotlightTexture ),
	DEFINE_INPUTFUNC(FIELD_STRING, "SetPattern", InputSetPattern),
	DEFINE_OUTPUT( m_OnTurnOff, "OnTurnOff" ),
	DEFINE_OUTPUT( m_OnTurnOn, "OnTurnOn" ),
	DEFINE_THINKFUNC( Think ),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CEnvLightProjected, DT_EnvLightProjected )
	SendPropInt( SENDINFO( m_iEnableShadowRadius ) ),
	SendPropVector( SENDINFO( m_vRadiusOrigin ) ),
	SendPropEHandle( SENDINFO( m_hTargetEntity ) ),
	SendPropBool( SENDINFO( m_bState ) ),
	SendPropBool( SENDINFO( m_bOnlyDrawIfShadowed ) ),
	SendPropFloat( SENDINFO( m_flLightFOV ) ),
	SendPropFloat( SENDINFO( m_fLinearAtten ) ),
	SendPropFloat( SENDINFO( m_flFilterSize ) ),
	SendPropBool( SENDINFO( m_bEnableShadows ) ),
	SendPropBool( SENDINFO( m_bLightOnlyTarget ) ),
	SendPropBool( SENDINFO( m_bLightWorld ) ),
	SendPropBool( SENDINFO( m_bCameraSpace ) ),
	SendPropVector( SENDINFO( m_LinearFloatLightColor ) ),
	SendPropFloat( SENDINFO( m_flAmbient ) ),
	SendPropString( SENDINFO( m_SpotlightTextureName ) ),
	SendPropString( SENDINFO( m_szPattern ) ),
	SendPropInt( SENDINFO( m_nSpotlightTextureFrame ) ),
	SendPropFloat( SENDINFO( m_flNearZ ), 16, SPROP_ROUNDDOWN, 0.0f,  500.0f ),
	SendPropFloat( SENDINFO( m_flFarZ ),  18, SPROP_ROUNDDOWN, 0.0f, 1500.0f ),
	SendPropInt( SENDINFO( m_nShadowQuality ), 1, SPROP_UNSIGNED ),  // Just one bit for now
END_SEND_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEnvLightProjected::CEnvLightProjected( void )
{
	m_bState = false;
	m_flLightFOV = 70.0f;
	m_bEnableShadows = true;
	m_bLightOnlyTarget = false;
	m_bLightWorld = true;
	m_bCameraSpace = false;

	m_iEnableShadowRadius = 1024;
	m_flFilterSize = 0.8f;

	Q_strcpy( m_SpotlightTextureName.GetForModify(), "effects/flashlight001" );

	m_nSpotlightTextureFrame = 0;
	m_LinearFloatLightColor.Init( 1.0f, 1.0f, 1.0f );
	m_flAmbient = 0.0f;
	m_flNearZ = 4.0f;
	m_flFarZ = 750.0f;
	m_nShadowQuality = 1;
}

bool CEnvLightProjected::KeyValue( const char *szKeyName, const char *szValue )
{
	if ( FStrEq( szKeyName, "lightcolor" ) )
	{
		Vector tmp;
		UTIL_ColorStringToLinearFloatColor( tmp, szValue );
		m_LinearFloatLightColor = tmp;
	}
	else if ( FStrEq(szKeyName, "texturename" ) )
	{
		Q_strcpy( m_SpotlightTextureName.GetForModify(), szValue );
	}
	else if ( FStrEq ( szKeyName, "radiusorigin" ) )
	{
		Vector tmp;
		UTIL_StringToVector(tmp.Base(), szValue);
		m_vRadiusOrigin = tmp - GetAbsOrigin();
	}
	else if ( FStrEq( szKeyName, "pattern" ) )
	{
		Q_strcpy( m_szPattern.GetForModify(), szValue );
	}
	else
	{
		return BaseClass::KeyValue( szKeyName, szValue );
	}
 
	return true;
}

void CEnvLightProjected::InputToggle( inputdata_t &inputdata )
{
	if (m_bState)
	{
		DoTurnOff();
	}
	else
	{
		DoTurnOn();
	}
}

void CEnvLightProjected::InputTurnOn( inputdata_t &inputdata )
{
	DoTurnOn();
}

bool CEnvLightProjected::IsShadowMapped()
{
	return m_bEnableShadows;
}

bool CEnvLightProjected::IsTurnedOn()
{
	return m_bState;
}

void CEnvLightProjected::DoTurnOn()
{
	if (m_bEnableShadows)
	{
		if (gEntList.FindEntityByClassname(NULL, "env_light_projected_manager") == NULL)
		{
			CEnvLightProjected *entity = NULL;
			while ((entity = (CEnvLightProjected*) gEntList.FindEntityByClassname(entity, "env_light_projected")) != NULL)
			{
				if (entity->IsTurnedOn() && entity->IsShadowMapped() && entity != this)
				{
					entity->DoTurnOff();
					DevMsg(2, "env_light_projected: Too many shadow maps enabled. Turned off: %s.\n", STRING(entity->GetEntityName()));
				}
			}
		}
		else
		{
			DevMsg(2, "env_light_projected_manager detected.\n");
		}
	}

	m_bState = true;
	m_OnTurnOn.FireOutput(this, this);
}

void CEnvLightProjected::DoTurnOff()
{
	m_bState = false;
	m_OnTurnOff.FireOutput(this, this);
}

void CEnvLightProjected::InputTurnOff( inputdata_t &inputdata )
{
	DoTurnOff();
}

void CEnvLightProjected::InputSetFOV( inputdata_t &inputdata )
{
	m_flLightFOV = inputdata.value.Float();
}

void CEnvLightProjected::InputSetFarZ( inputdata_t &inputdata )
{
	m_flFarZ = inputdata.value.Float();
}

void CEnvLightProjected::InputSetNearZ( inputdata_t &inputdata )
{
	m_flNearZ = inputdata.value.Float();
}

void CEnvLightProjected::SetLinearAtten( float linearAtten)
{
	m_fLinearAtten = linearAtten;
}

void CEnvLightProjected::InputSetLinearAtten( inputdata_t &inputdata )
{
	SetLinearAtten(inputdata.value.Float());
}

void CEnvLightProjected::InputSetFilterSize( inputdata_t &inputdata )
{
	m_flFilterSize = inputdata.value.Float();
}

void CEnvLightProjected::InputSetTarget( inputdata_t &inputdata )
{
	m_hTargetEntity = inputdata.value.Entity();
}

void CEnvLightProjected::InputSetCameraSpace( inputdata_t &inputdata )
{
	m_bCameraSpace = inputdata.value.Bool();
}

void CEnvLightProjected::InputSetLightOnlyTarget( inputdata_t &inputdata )
{
	m_bLightOnlyTarget = inputdata.value.Bool();
}

void CEnvLightProjected::InputSetLightWorld( inputdata_t &inputdata )
{
	m_bLightWorld = inputdata.value.Bool();
}

void CEnvLightProjected::InputSetEnableShadows( inputdata_t &inputdata )
{
	m_bEnableShadows  = inputdata.value.Bool();
}

void CEnvLightProjected::InputSetLightColor( inputdata_t &inputdata )
{
	Vector tmp;
	UTIL_ColorStringToLinearFloatColor( tmp, inputdata.value.String() );
	m_LinearFloatLightColor = tmp;
}

void CEnvLightProjected::InputSetAmbient( inputdata_t &inputdata )
{
	m_flAmbient = inputdata.value.Float();
}

void CEnvLightProjected::InputSetSpotlightTexture( inputdata_t &inputdata )
{
	Q_strcpy( m_SpotlightTextureName.GetForModify(), inputdata.value.String() );
}

void CEnvLightProjected::InputSetPattern(inputdata_t &inputdata)
{
	Q_strcpy(m_szPattern.GetForModify(), inputdata.value.String());
}

void CEnvLightProjected::Spawn( void )
{
	if ( GetSpawnFlags() )
	{
		m_bState = true;
	}
	else
	{
		m_bState = false;
	}
}

void CEnvLightProjected::Activate( void )
{
	SetThink( &CEnvLightProjected::Think );
	SetNextThink( gpGlobals->curtime + 0.1f );

	BaseClass::Activate();
}

void CEnvLightProjected::Think( void )
{
	if ( m_hTargetEntity == NULL && m_target != NULL_STRING )
	{
		m_hTargetEntity = gEntList.FindEntityByName( NULL, m_target );
	}

	if ( m_hTargetEntity != NULL )
	{
		Vector vecToTarget = (m_hTargetEntity->GetAbsOrigin() - GetAbsOrigin());
		QAngle vecAngles;
		VectorAngles( vecToTarget, vecAngles );
		SetAbsAngles( vecAngles );
	}

	SetNextThink( gpGlobals->curtime + 0.1 );
}

int CEnvLightProjected::UpdateTransmitState()
{
	return SetTransmitState( FL_EDICT_ALWAYS );
}