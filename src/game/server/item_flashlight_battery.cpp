//=============================================================================
//
// Purpose: Brain entity for the player to pick up
//
//=============================================================================
 
#include "cbase.h"
#include "player.h"
#include "engine/IEngineSound.h"
#include "hl2_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	ENTITY_MODEL "models/anicator/battery_d.mdl"
#define BATTERY_VALUE 40

extern bool UTIL_ItemCanBeTouchedByPlayer( CBaseEntity *pItem, CBasePlayer *pPlayer );

class CItemFlashlightBattery : public CBaseAnimating
{
		DECLARE_CLASS( CItemFlashlightBattery, CBaseAnimating );
	public:
		DECLARE_DATADESC();

		void Spawn( void )
		{
			Precache();
			BaseClass::Spawn();

			SetModel(ENTITY_MODEL);

			SetMoveType(MOVETYPE_FLYGRAVITY);
			SetSolid(SOLID_BBOX);
			SetBlocksLOS(false);
			AddEFlags(EFL_NO_ROTORWASH_PUSH);

			SetCollisionGroup(COLLISION_GROUP_WEAPON);

			int nSolidFlags = GetSolidFlags() | FSOLID_NOT_STANDABLE | FSOLID_NOT_STANDABLE | FSOLID_TRIGGER | FSOLID_USE_TRIGGER_BOUNDS;

			VPhysicsInitNormal(SOLID_VPHYSICS, nSolidFlags, false);

			CollisionProp()->UseTriggerBounds(true, 64);

			SetUse(&CItemFlashlightBattery::Pickup);

			SetGlow(true);
		};
		void Precache( void )
		{
			PrecacheModel( ENTITY_MODEL );
			PrecacheScriptSound( "HealthVial.Touch" );
			BaseClass::Precache();
		};
		int ObjectCaps( void )
		{
			return BaseClass::ObjectCaps() | FCAP_IMPULSE_USE;
		};
		void Pickup(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
		{
			CBasePlayer *pPlayer = ToBasePlayer(pActivator);
			if (pPlayer)
			{
				if (!AddFlashlightBattery(pPlayer))
				{
					pPlayer->PickupObject(this);
				}
			}
		}
	private:
		bool AddFlashlightBattery(CBasePlayer *pPlayer)
			{
				CHL2_Player *hl2_player = dynamic_cast<CHL2_Player*>(pPlayer);

				if (hl2_player && hl2_player->AddFlashlightBattery(BATTERY_VALUE))
				{
					CSingleUserRecipientFilter user(pPlayer);
					user.MakeReliable();

					UserMessageBegin(user, "ItemPickup");
						WRITE_STRING(GetClassname());
					MessageEnd();

					CPASAttenuationFilter filter(pPlayer, "ItemBattery.Touch");
					EmitSound(filter, pPlayer->entindex(), "ItemBattery.Touch");
					UTIL_Remove(this);
					return true;
				}
	
				return false;
			}
};

LINK_ENTITY_TO_CLASS( item_flashlight_battery, CItemFlashlightBattery );
PRECACHE_REGISTER( item_flashlight_battery );

BEGIN_DATADESC( CItemFlashlightBattery )
	DEFINE_USEFUNC( Pickup ),
END_DATADESC()