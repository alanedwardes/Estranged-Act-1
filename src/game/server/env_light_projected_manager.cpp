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
class CEnvLightProjectedManager : public CPointEntity
{
	DECLARE_CLASS( CEnvLightProjectedManager, CPointEntity );
public:
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	CEnvLightProjectedManager();

	// Always transmit to clients
	virtual int UpdateTransmitState();
};

LINK_ENTITY_TO_CLASS(env_light_projected_manager, CEnvLightProjectedManager);

BEGIN_DATADESC( CEnvLightProjectedManager )
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CEnvLightProjectedManager, DT_EnvLightProjectedManager )
END_SEND_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEnvLightProjectedManager::CEnvLightProjectedManager(void)
{
}

int CEnvLightProjectedManager::UpdateTransmitState()
{
	return SetTransmitState(FL_EDICT_ALWAYS);
}