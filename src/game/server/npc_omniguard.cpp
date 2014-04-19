//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: OmniCorp Guard NPC
//
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "ai_baseactor.h"
#include "sceneentity.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define THINKTIME 0.5f

const char *pOmniguardModels[] =
{
	"models/humans/group03m/female_01.mdl",
	"models/humans/group03m/female_02.mdl",
	"models/humans/group03m/female_03.mdl",
	"models/humans/group03m/female_04.mdl",
	"models/humans/group03m/female_06.mdl",
	"models/humans/group03m/female_07.mdl",

	"models/humans/group03m/male_01.mdl",
	"models/humans/group03m/male_03.mdl",
	"models/humans/group03m/male_05.mdl",
	"models/humans/group03m/male_06.mdl",
	"models/humans/group03m/male_07.mdl",
	"models/humans/group03m/male_08.mdl",
};

ConVar sk_omniguard_health("sk_omniguard_health","20");
ConVar sk_omniguard_melee_dmg("sk_omniguard_melee_dmg","10");

class CNPC_OmniGuard : public CAI_BaseActor
{
public:
	DECLARE_DATADESC();
	DECLARE_CLASS( CNPC_OmniGuard, CAI_BaseActor );

	void	Spawn( void );
	void	Precache( void );
	Class_T Classify ( void );
	Activity NPC_TranslateActivity( Activity eNewActivity );

	void	NPCThink(void);
	float	GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo &info );
	void	Weapon_Equip( CBaseCombatWeapon *pWeapon );
	void	Event_Killed( const CTakeDamageInfo &info );
	void	HandleAnimEvent( animevent_t *pEvent );
	void	CreateGunlight();
private:
	CBaseEntity *gunLight;
	CBaseEntity *gunLightSprite;
	bool m_bHasFlashlight;
	bool m_bHasFlashlightBeenCreated;
};

BEGIN_DATADESC( CNPC_OmniGuard )
	DEFINE_KEYFIELD( m_bHasFlashlight, FIELD_BOOLEAN, "hasflashlight" ),
	DEFINE_FIELD( m_bHasFlashlightBeenCreated, FIELD_BOOLEAN )
END_DATADESC()

LINK_ENTITY_TO_CLASS( npc_omniguard, CNPC_OmniGuard );

//-----------------------------------------------------------------------------
// Classify - indicates this NPC's place in the 
// relationship table.
//-----------------------------------------------------------------------------
Class_T	CNPC_OmniGuard::Classify ( void )
{
	return CLASS_OMNIGUARD;
}

//-----------------------------------------------------------------------------
// Spawn
//-----------------------------------------------------------------------------
void CNPC_OmniGuard::Spawn()
{
	gunLight = NULL;

	Precache();

	BaseClass::Spawn();

	const char *modelName = STRING( GetModelName() );

	if ( Q_strlen(modelName) > 0 )
	{
		// If the model was set in WC
		SetModel( modelName );
	}
	else
	{
		// If it wasn't, pick a random one
		int modelIndex = random->RandomInt(0, ARRAYSIZE(pOmniguardModels) - 1);
		SetModel(pOmniguardModels[modelIndex]);
	}

	SetHullType( HULL_HUMAN );
	SetHullSizeNormal( true );

	SetSolid( SOLID_BBOX );

	SetDefaultEyeOffset();
	SetActivity( ACT_IDLE );

	SetMoveType( MOVETYPE_STEP );
	SetBloodColor( BLOOD_COLOR_RED );

	CapabilitiesAdd(
		bits_CAP_ANIMATEDFACE | 
		bits_CAP_AUTO_DOORS | 
		bits_CAP_AIM_GUN |
		bits_CAP_DOORS_GROUP | 
		bits_CAP_DUCK |
		bits_CAP_FRIENDLY_DMG_IMMUNE |
		bits_CAP_INNATE_MELEE_ATTACK1 |
		bits_CAP_MOVE_GROUND | 
		bits_CAP_MOVE_JUMP |
		bits_CAP_MOVE_SHOOT | 
		bits_CAP_NO_HIT_SQUADMATES |
		bits_CAP_OPEN_DOORS |
		bits_CAP_SQUAD | 
		bits_CAP_TURN_HEAD | 
		bits_CAP_USE_WEAPONS | 
		bits_CAP_WEAPON_RANGE_ATTACK1
	);

	m_iHealth			= sk_omniguard_health.GetFloat();
	m_flFieldOfView		= -0.2;
	m_HackedGunPos = Vector ( 0, 0, 55 ); // oh Valve...

	m_iszIdleExpression = MAKE_STRING("scenes/expressions/citizenidle.vcd");
	m_iszAlertExpression = MAKE_STRING("scenes/expressions/citizenalert_loop.vcd");
	m_iszCombatExpression = MAKE_STRING("scenes/expressions/citizencombat_loop.vcd");

	//-----------------------------------------------------------------------------

	AddEFlags( EFL_NO_DISSOLVE | EFL_NO_MEGAPHYSCANNON_RAGDOLL | EFL_NO_PHYSCANNON_INTERACTION );

	NPCInit();
}

Activity CNPC_OmniGuard::NPC_TranslateActivity( Activity activity )
{
	if ( activity == ACT_MELEE_ATTACK1 )
		return ACT_MELEE_ATTACK_SWING;
	
	// Fake activities for shotguns, stolen from npc_citizen17.cpp
	if ( activity == ACT_RUN_AIM_SHOTGUN )
		return ACT_RUN_AIM_RIFLE;
	if ( activity == ACT_WALK_AIM_SHOTGUN )
		return ACT_WALK_AIM_RIFLE;
	if ( activity == ACT_IDLE_ANGRY_SHOTGUN )
		return ACT_IDLE_ANGRY_SMG1;
	if ( activity == ACT_RANGE_ATTACK_SHOTGUN_LOW )
		return ACT_RANGE_ATTACK_SMG1_LOW;

	// Hack - the models don't support our pistol really, so fake this activity
	if (m_NPCState == NPC_STATE_COMBAT || m_NPCState == NPC_STATE_ALERT)
	{
		if ( activity == ACT_IDLE_PISTOL )
			return ACT_RANGE_ATTACK_PISTOL;
	}

	if ( activity == ACT_COWER )
		return ACT_COVER_LOW;

	if ( activity == ACT_RUN && ( IsCurSchedule( SCHED_TAKE_COVER_FROM_BEST_SOUND ) || IsCurSchedule( SCHED_FLEE_FROM_BEST_SOUND ) ) )
	{
		if ( random->RandomInt( 0, 1 ) && HaveSequenceForActivity( ACT_RUN_PROTECTED ) )
			activity = ACT_RUN_PROTECTED;
	}

	// Only do the below if we're *not* the pistol
	if (GetActiveWeapon() != NULL && !FStrEq(GetActiveWeapon()->GetClassname(), "weapon_pistol"))
	{
		activity = BaseClass::NPC_TranslateActivity( activity );

		if ( activity == ACT_IDLE  )
		{
			if ( (m_NPCState == NPC_STATE_COMBAT || m_NPCState == NPC_STATE_ALERT) && gpGlobals->curtime - m_flLastAttackTime < 3)
			{
				activity = ACT_IDLE_ANGRY;
			}
		}
	}

	return BaseClass::NPC_TranslateActivity( activity );
}

//-----------------------------------------------------------------------------
// Precache - precaches all resources this NPC needs
//-----------------------------------------------------------------------------
void CNPC_OmniGuard::Precache()
{
	BaseClass::Precache();

	for (int i = 0; i < ARRAYSIZE(pOmniguardModels); i++)
	{
		PrecacheModel( pOmniguardModels[i] );
	}

	PrecacheScriptSound("Omniguard.Punch");
	PrecacheInstancedScene( "scenes/expressions/citizenidle.vcd" );
	PrecacheInstancedScene( "scenes/expressions/citizenalert_loop.vcd" );
	PrecacheInstancedScene( "scenes/expressions/citizencombat_loop.vcd" );
}

void CNPC_OmniGuard::NPCThink()
{
	BaseClass::NPCThink();

	if ( GetSleepState() == AISS_AWAKE )
	{
		// Create us a flashlight
		if ( m_bHasFlashlight && !m_bHasFlashlightBeenCreated )
		{
			CreateGunlight();
		}
	}
}

void CNPC_OmniGuard::HandleAnimEvent( animevent_t *pEvent )
{
	switch (pEvent->event)
	{
		case EVENT_WEAPON_MELEE_HIT:
		{
			CBaseEntity *pHurt = CheckTraceHullAttack( 70, -Vector(16,16,18), Vector(16,16,18), 0, DMG_CLUB );
			CBaseCombatCharacter* pBCC = ToBaseCombatCharacter( pHurt );
			if (pBCC)
			{
				Vector forward, up;
				AngleVectors( GetLocalAngles(), &forward, NULL, &up );

				if ( pBCC->IsPlayer() )
				{
					pBCC->ViewPunch( QAngle(-12,-7,0) );
					pHurt->ApplyAbsVelocityImpulse( forward * 100 + up * 50 );
				}

				CTakeDamageInfo info( this, this, sk_omniguard_melee_dmg.GetInt(), DMG_CLUB );
				CalculateMeleeDamageForce( &info, forward, pBCC->GetAbsOrigin() );
				pBCC->TakeDamage( info );

				EmitSound("Omniguard.Punch");
			}
			break;
		}
		default:
		{
			BaseClass::HandleAnimEvent(pEvent);
			break;
		}
	}
	
}

float CNPC_OmniGuard::GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo &info )
{
	switch( iHitGroup )
	{
		case HITGROUP_HEAD:
		{
			// Soldiers take double headshot damage
			return 2.0f;
		}
	}

	return BaseClass::GetHitgroupDamageMultiplier( iHitGroup, info );
}

void CNPC_OmniGuard::Weapon_Equip( CBaseCombatWeapon *pWeapon )
{
	BaseClass::Weapon_Equip( pWeapon );
}

void CNPC_OmniGuard::CreateGunlight()
{
	m_bHasFlashlightBeenCreated = true;

	if (GetActiveWeapon() == NULL)
	{
		Warning("npc_omniguard: Attempted to give flashlight without a weapon.\n");
		return;
	}

	if (gunLight == NULL)
	{
		// Create the projected part
		gunLight = CreateEntityByName("env_light_projected");
		gunLight->KeyValue("lightcolor", "163 190 203 1000");
		gunLight->KeyValue("enableshadows", 1);
		gunLight->KeyValue("radius", 512);
		gunLight->KeyValue("lightfov", 70);
		gunLight->KeyValue("nearz", 30);
		gunLight->KeyValue("farz", 400);
		gunLight->KeyValue("linearatten", 200);
		gunLight->KeyValue("texturename", "effects/flashlight001");
		gunLight->AddSpawnFlags(1); // Start on
		gunLight->SetParent(this);
		gunLight->SetParentAttachment("npc_omniguard", "anim_attachment_RH", false);
		DispatchSpawn(gunLight);
		gunLight->Activate();
	}

	if (gunLightSprite == NULL)
	{
		// Create the sprite part
		gunLightSprite = CreateEntityByName("point_spotlight");
		gunLightSprite->KeyValue("spotlightwidth", 20);
		gunLightSprite->KeyValue("spotlightlength", 100);
		gunLightSprite->KeyValue("rendercolor", "75 100 110"); // rendercolor is gunLight colour / 1.5
		gunLightSprite->AddSpawnFlags(3); // Start on, no dynamic light
		gunLightSprite->SetParent(this);
		gunLightSprite->SetParentAttachment("npc_omniguard", "anim_attachment_RH", false);
		DispatchSpawn(gunLightSprite);
		gunLightSprite->Activate();
	}
}

void CNPC_OmniGuard::Event_Killed( const CTakeDamageInfo &info )
{
	// Remove the light
	if (gunLight != NULL)
	{
		gunLight->Remove();
		gunLight = NULL;
	}

	// Remove the sprite
	if (gunLightSprite != NULL)
	{
		variant_t sVariant;
		gunLightSprite->AcceptInput("LightOff", this, this, sVariant, 0);
		gunLightSprite->Remove();
		gunLightSprite = NULL;
	}

	BaseClass::Event_Killed(info);
}