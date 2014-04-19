//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Hutman - a man in a hut. Full name John Hutman.
//
//=============================================================================//


//-----------------------------------------------------------------------------
// Generic NPC - purely for scripted sequence work.
//-----------------------------------------------------------------------------
#include "cbase.h"
#include "npcevent.h"
#include "ai_basenpc.h"
#include "ai_hull.h"
#include "ai_baseactor.h"
#include "ai_playerally.h"
#include "npc_playercompanion.h"
#include "sceneentity.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define HUTMAN_MODEL "models/humans/group01/male_02.mdl"
#define HUTMAN_IDLE_SCENE "scenes/hutman/idle.vcd"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CNPC_Hutman : public CNPC_PlayerCompanion
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CNPC_Hutman, CNPC_PlayerCompanion );

	void	Spawn( void );
	void	Precache( void );
	Class_T Classify ( void );
	void	SelectModel ( void );
	Activity NPC_TranslateActivity( Activity );
	virtual void InputSuppressRun(inputdata_t&);

private:
	bool m_bSuppressRun;
};


LINK_ENTITY_TO_CLASS( npc_hutman, CNPC_Hutman );
BEGIN_DATADESC( CNPC_Hutman )
	DEFINE_FIELD( m_bSuppressRun, FIELD_BOOLEAN ),
	DEFINE_INPUTFUNC( FIELD_BOOLEAN, "SuppressRun", InputSuppressRun )
END_DATADESC()

//-----------------------------------------------------------------------------
// Classify - indicates this NPC's place in the 
// relationship table.
//-----------------------------------------------------------------------------
Class_T	CNPC_Hutman::Classify ( void )
{
	return	CLASS_PLAYER_ALLY_VITAL;
}

Activity CNPC_Hutman::NPC_TranslateActivity( Activity baseAct )
{
	if ( baseAct == ACT_RUN && m_bSuppressRun )
	{
		return ACT_WALK;
	}

	return BaseClass::NPC_TranslateActivity( baseAct );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_Hutman::SelectModel()
{
	SetModelName( AllocPooledString( HUTMAN_MODEL ) );
}

void CNPC_Hutman::InputSuppressRun( inputdata_t &inputdata )
{
	m_bSuppressRun = inputdata.value.Bool();
}

//-----------------------------------------------------------------------------
// Spawn
//-----------------------------------------------------------------------------
void CNPC_Hutman::Spawn()
{
	Precache();

	m_iHealth = 80;

	m_iszCombatExpression = m_iszIdleExpression = m_iszAlertExpression = MAKE_STRING( HUTMAN_IDLE_SCENE );

	BaseClass::Spawn();

	AddEFlags( EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION );

	NPCInit();
}

//-----------------------------------------------------------------------------
// Precache - precaches all resources this NPC needs
//-----------------------------------------------------------------------------
void CNPC_Hutman::Precache()
{
	SelectModel( );
	BaseClass::Precache();

	PrecacheInstancedScene( HUTMAN_IDLE_SCENE );
}