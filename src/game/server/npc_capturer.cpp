//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
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

#define CAPTURER_MODEL "models/humans/group03m/male_04.mdl"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CNPC_Capturer : public CNPC_PlayerCompanion
{
public:
	DECLARE_CLASS( CNPC_Capturer, CNPC_PlayerCompanion );

	void	Spawn( void );
	void	Precache( void );
	Class_T Classify ( void );
	void	Weapon_Equip( CBaseCombatWeapon *pWeapon );
	bool	CreateBehaviors( void );
	void	SelectModel ( void );
	void	HandleAnimEvent( animevent_t *pEvent );
	int		GetSoundInterests ( void );
};

LINK_ENTITY_TO_CLASS( npc_capturer, CNPC_Capturer );

//-----------------------------------------------------------------------------
// Classify - indicates this NPC's place in the 
// relationship table.
//-----------------------------------------------------------------------------
Class_T	CNPC_Capturer::Classify ( void )
{
	return	CLASS_PLAYER_ALLY;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_Capturer::Weapon_Equip( CBaseCombatWeapon *pWeapon )
{
	BaseClass::Weapon_Equip( pWeapon );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_Capturer::SelectModel()
{
	SetModelName( AllocPooledString( CAPTURER_MODEL ) );
}

//-----------------------------------------------------------------------------
// HandleAnimEvent - catches the NPC-specific messages
// that occur when tagged animation frames are played.
//-----------------------------------------------------------------------------
void CNPC_Capturer::HandleAnimEvent( animevent_t *pEvent )
{
	switch( pEvent->event )
	{
		case 1:
		default:
			BaseClass::HandleAnimEvent( pEvent );
			break;
	}
}

bool CNPC_Capturer::CreateBehaviors( void )
{
	BaseClass::CreateBehaviors();

	return true;
}

//-----------------------------------------------------------------------------
// GetSoundInterests - generic NPC can't hear.
//-----------------------------------------------------------------------------
int CNPC_Capturer::GetSoundInterests ( void )
{
	return	NULL;
}

//-----------------------------------------------------------------------------
// Spawn
//-----------------------------------------------------------------------------
void CNPC_Capturer::Spawn()
{
	Precache();

	m_iHealth = 80;

	m_iszIdleExpression = MAKE_STRING("scenes/Expressions/citizen_normal_idle_01.vcd");
	m_iszAlertExpression = MAKE_STRING("scenes/Expressions/citizen_normal_alert_01.vcd");
	m_iszCombatExpression = MAKE_STRING("scenes/Expressions/citizen_angry_combat_01.vcd");

	BaseClass::Spawn();

	AddEFlags( EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION );

	NPCInit();
}

//-----------------------------------------------------------------------------
// Precache - precaches all resources this NPC needs
//-----------------------------------------------------------------------------
void CNPC_Capturer::Precache()
{
	SelectModel( );
	BaseClass::Precache();

	PrecacheInstancedScene( "scenes/Expressions/citizen_normal_idle_01.vcd" );
	PrecacheInstancedScene( "scenes/Expressions/citizen_normal_alert_01.vcd" );
	PrecacheInstancedScene( "scenes/Expressions/citizen_angry_combat_01.vcd" );
}