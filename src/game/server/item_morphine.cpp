//=============================================================================
//
// Purpose: Brain entity for the player to pick up
//
//=============================================================================
 
#include "cbase.h"
#include "player.h"
#include "engine/IEngineSound.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	ENTITY_MODEL "models/ae/morphine.mdl"
#define HEALTH_VALUE 20

extern bool UTIL_ItemCanBeTouchedByPlayer( CBaseEntity *pItem, CBasePlayer *pPlayer );

class CItemMorphine : public CBaseAnimating
{
		DECLARE_CLASS( CItemMorphine, CBaseAnimating );
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

			CollisionProp()->UseTriggerBounds(true, 36);

			SetUse(&CItemMorphine::Pickup);

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
				if (!HealPlayer(pPlayer))
				{
					pPlayer->PickupObject(this);
				}
			}
		};
	private:
		bool HealPlayer(CBasePlayer *pPlayer)
		{
			if (pPlayer->TakeHealth(HEALTH_VALUE, DMG_GENERIC))
			{
				CSingleUserRecipientFilter user(pPlayer);
				user.MakeReliable();

				UserMessageBegin(user, "ItemPickup");
					WRITE_STRING(GetClassname());
				MessageEnd();

				CPASAttenuationFilter filter(pPlayer, "HealthKit.Touch");
				EmitSound(filter, pPlayer->entindex(), "HealthKit.Touch");
				UTIL_Remove(this);
				return true;
			}
	
			return false;
		};
};
 
LINK_ENTITY_TO_CLASS( item_morphine, CItemMorphine );
PRECACHE_REGISTER( item_morphine );

BEGIN_DATADESC( CItemMorphine )
	DEFINE_USEFUNC( Pickup ),
END_DATADESC()