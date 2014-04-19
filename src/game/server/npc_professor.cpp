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

#define PROFESSOR_MODEL "models/kleiner.mdl"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CNPC_Professor : public CNPC_PlayerCompanion
{
public:
	DECLARE_CLASS( CNPC_Professor, CNPC_PlayerCompanion );

	void	Spawn( void );
	void	Precache( void );
	Class_T Classify ( void );
	void	Weapon_Equip( CBaseCombatWeapon *pWeapon );
	bool	CreateBehaviors( void );
	void	SelectModel ( void );
	void	HandleAnimEvent( animevent_t *pEvent );
	int		GetSoundInterests ( void );
};

LINK_ENTITY_TO_CLASS( npc_professor, CNPC_Professor );

//-----------------------------------------------------------------------------
// Classify - indicates this NPC's place in the 
// relationship table.
//-----------------------------------------------------------------------------
Class_T	CNPC_Professor::Classify ( void )
{
	return	CLASS_PLAYER_ALLY_VITAL;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_Professor::Weapon_Equip( CBaseCombatWeapon *pWeapon )
{
	BaseClass::Weapon_Equip( pWeapon );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_Professor::SelectModel()
{
	SetModelName( AllocPooledString( PROFESSOR_MODEL ) );
}

//-----------------------------------------------------------------------------
// HandleAnimEvent - catches the NPC-specific messages
// that occur when tagged animation frames are played.
//-----------------------------------------------------------------------------
void CNPC_Professor::HandleAnimEvent( animevent_t *pEvent )
{
	switch( pEvent->event )
	{
		case 1:
		default:
			BaseClass::HandleAnimEvent( pEvent );
			break;
	}
}

bool CNPC_Professor::CreateBehaviors( void )
{
	BaseClass::CreateBehaviors();

	return true;
}

//-----------------------------------------------------------------------------
// GetSoundInterests - generic NPC can't hear.
//-----------------------------------------------------------------------------
int CNPC_Professor::GetSoundInterests ( void )
{
	return	NULL;
}

//-----------------------------------------------------------------------------
// Spawn
//-----------------------------------------------------------------------------
void CNPC_Professor::Spawn()
{
	Precache();

	BaseClass::Spawn();

	SetHullType(HULL_HUMAN);
	SetHullSizeNormal();

	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );
	SetBloodColor( BLOOD_COLOR_RED );

	CapabilitiesAdd( bits_CAP_MOVE_GROUND | bits_CAP_OPEN_DOORS | bits_CAP_ANIMATEDFACE | bits_CAP_TURN_HEAD );
	CapabilitiesAdd( bits_CAP_FRIENDLY_DMG_IMMUNE );

	m_iHealth = 80;
	m_flFieldOfView		= 0.5;// indicates the width of this NPC's forward view cone ( as a dotproduct result )
	m_NPCState			= NPC_STATE_NONE;

	AddEFlags( EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION );

	NPCInit();
}

//-----------------------------------------------------------------------------
// Precache - precaches all resources this NPC needs
//-----------------------------------------------------------------------------
void CNPC_Professor::Precache()
{
	SelectModel( );
	BaseClass::Precache();

	PrecacheInstancedScene( "scenes/Expressions/citizen_normal_idle_01.vcd" );
	PrecacheInstancedScene( "scenes/Expressions/citizen_normal_alert_01.vcd" );
	PrecacheInstancedScene( "scenes/Expressions/citizen_angry_combat_01.vcd" );
}