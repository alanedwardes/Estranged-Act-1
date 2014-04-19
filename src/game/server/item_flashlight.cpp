#include "cbase.h"
#include "gamerules.h"
#include "player.h"
#include "items.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"
#include "hl2_player_shared.h"

#define TORCH_MODEL "models/ae/flashlight.mdl"

extern bool UTIL_ItemCanBeTouchedByPlayer( CBaseEntity *pItem, CBasePlayer *pPlayer );

class CItemFlashlight : public CBaseAnimating
{
public:
	DECLARE_CLASS( CItemFlashlight, CBaseAnimating );
	DECLARE_DATADESC();

	void Spawn( void )
	{
		Precache();
		BaseClass::Spawn();
		
		SetModel(TORCH_MODEL);

		SetSolid(SOLID_BBOX);
		SetBlocksLOS(false);
		AddEFlags(EFL_NO_ROTORWASH_PUSH);

		SetCollisionGroup(COLLISION_GROUP_WEAPON);

		int nSolidFlags = GetSolidFlags() | FSOLID_NOT_STANDABLE | FSOLID_NOT_STANDABLE | FSOLID_TRIGGER | FSOLID_USE_TRIGGER_BOUNDS;

		IPhysicsObject *pPhysicsObject = VPhysicsInitNormal(SOLID_VPHYSICS, nSolidFlags, false);

		if (GetSpawnFlags())
		{
			pPhysicsObject->EnableMotion(false);
		}

		CollisionProp()->UseTriggerBounds(true, 64);

		SetUse(&CItemFlashlight::Pickup);

		SetGlow(true);
	}

	void Precache( void )
	{
		PrecacheModel(TORCH_MODEL);
		PrecacheScriptSound("ItemBattery.Touch");
	}

	int ObjectCaps( void ) 
	{
		return (BaseClass::ObjectCaps() | FCAP_IMPULSE_USE);
	}

	void Pickup( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
	{
		CBasePlayer *pPlayer = ToBasePlayer(pActivator);

		if (!pPlayer) return;

		CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player*>(pPlayer);

		if (!pHL2Player) return;

		if (!pHL2Player->GetFlashlightEnabled())
		{
			pHL2Player->SetFlashlightEnabled(true);
			UTIL_HudHintText(pHL2Player, "#hint_flashlight");
			pHL2Player->FlashlightTurnOn();
			m_OnPickup.FireOutput(pActivator, pCaller);
			EmitSoundAndRemove(pPlayer);
			return;
		}
		
		if (pHL2Player->AddFlashlightBattery(100))
		{
			EmitSoundAndRemove(pPlayer);
			m_OnPickup.FireOutput(pActivator, pCaller);
			return;
		}
		
		return;
	}
	
	void EmitSoundAndRemove(CBasePlayer *pPlayer)
	{
		CSingleUserRecipientFilter user( pPlayer );
		user.MakeReliable();

		UserMessageBegin( user, "ItemPickup" );
		WRITE_STRING( GetClassname() );
		MessageEnd();

		CPASAttenuationFilter filter( pPlayer, "ItemBattery.Touch" );
		EmitSound( filter, pPlayer->entindex(), "ItemBattery.Touch" );

		UTIL_Remove(this);
	}

	COutputEvent m_OnPickup;
};

LINK_ENTITY_TO_CLASS( item_flashlight, CItemFlashlight );
PRECACHE_REGISTER( item_flashlight );

BEGIN_DATADESC( CItemFlashlight )
	DEFINE_USEFUNC( Pickup ),
	DEFINE_OUTPUT(m_OnPickup, "OnPickup"),
END_DATADESC()