//=============================================================================
//
// Purpose: Spawns consumables. Doink.
//
//=============================================================================

#include "cbase.h"
#include "hl2_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define THINK_TIME 0.05f

#define HEALTH "item_morphine"
#define AMMO_PISTOL "item_ammo_pistol"
#define AMMO_SMG1 "item_ammo_smg1"
#define BATTERIES "item_flashlight_battery"

class CConsumableSpawner : public CPointEntity
{
	DECLARE_DATADESC();
	public:
		DECLARE_CLASS( CConsumableSpawner, CPointEntity );

		CConsumableSpawner(){}
 
		void Spawn(void);
		void ForceSpawn(inputdata_t &inputData);
		void Think(void);
		void SpawnConsumables(void);
		void SpawnFlashlightBattery(CHL2_Player*);
		void SpawnMorphine(CHL2_Player*);
		void SpawnAmmo(CHL2_Player*);
		virtual void Precache();
	private:
		void DoSpawnItem(CBaseEntity*);
		int m_iSpawnRadius;
		int m_iThreshold;
		bool m_bIsThinking;
		enum Item_bits
		{
			SPAWN_ITEM_AMMO      = 0x00000001,
			SPAWN_ITEM_HEALTH    = 0x00000002,
			SPAWN_ITEM_BATTERIES = 0x00000004
		};
};
 
LINK_ENTITY_TO_CLASS(consumable_spawner, CConsumableSpawner);
BEGIN_DATADESC(CConsumableSpawner)
	DEFINE_FIELD(m_bIsThinking, FIELD_BOOLEAN),
	DEFINE_KEYFIELD(m_iSpawnRadius, FIELD_INTEGER, "radius"),
	DEFINE_KEYFIELD(m_iThreshold, FIELD_INTEGER, "threshold"),
	DEFINE_THINKFUNC(Think),
	DEFINE_INPUTFUNC(FIELD_STRING, "ForceSpawn", ForceSpawn)
END_DATADESC()

void CConsumableSpawner::Spawn( void )
{
	BaseClass::Spawn();

	// If someone has set a radius, think
	if (m_iSpawnRadius > 0)
	{
		m_bIsThinking = true;
		SetNextThink(gpGlobals->curtime + THINK_TIME);
	}

	Precache();
}

void CConsumableSpawner::Precache()
{
	BaseClass::Precache();

	UTIL_PrecacheOther(BATTERIES);
	UTIL_PrecacheOther(HEALTH);
	UTIL_PrecacheOther(AMMO_SMG1);
	UTIL_PrecacheOther(AMMO_PISTOL);
}

void CConsumableSpawner::Think()
{
	if (!m_bIsThinking) return;

	CBasePlayer *pPlayer = UTIL_GetLocalPlayer();

	if (pPlayer)
	{
		float flDist = (GetAbsOrigin() - pPlayer->GetAbsOrigin()).LengthSqr();

		if (flDist <= Square(m_iSpawnRadius))
		{
			DevMsg("Player within radius. CConsumableSpawner::SpawnConsumables()\n");
			m_bIsThinking = false;
			SpawnConsumables();
		}
	}

	SetNextThink(gpGlobals->curtime + THINK_TIME);
}

void CConsumableSpawner::ForceSpawn( inputdata_t &inputData )
{
	DevMsg("Input used. CConsumableSpawner::ForceSpawn()\n");
	SpawnConsumables();
}

void CConsumableSpawner::SpawnConsumables()
{
	CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player*>(UTIL_GetLocalPlayer());
	if (!pHL2Player)
	{
		return;
	}

	int flags = GetSpawnFlags();
	if (flags & SPAWN_ITEM_AMMO)
	{
		SpawnAmmo(pHL2Player);
	}

	if (flags & SPAWN_ITEM_HEALTH)
	{
		SpawnMorphine(pHL2Player);
	}

	if (flags & SPAWN_ITEM_BATTERIES)
	{
		SpawnFlashlightBattery(pHL2Player);
	}
}

void CConsumableSpawner::SpawnFlashlightBattery(CHL2_Player *pHL2Player)
{
	if (pHL2Player->GetFlashlightLevel() > m_iThreshold)
	{
		return;
	}

	// Don't spawn if the player has reserves
	if (pHL2Player->GetFlashlightBatteries() > 0)
	{
		return;
	}

	DevMsg("CConsumableSpawner::SpawnFlashlightBattery()\n");
	DoSpawnItem(CreateEntityByName(BATTERIES));
}

void CConsumableSpawner::SpawnMorphine(CHL2_Player *pHL2Player)
{
	if (pHL2Player->GetHealth() > m_iThreshold)
	{
		return;
	}

	DevMsg("CConsumableSpawner::SpawnMorphine()\n");
	DoSpawnItem(CreateEntityByName(HEALTH));
}

void CConsumableSpawner::SpawnAmmo(CHL2_Player *pHL2Player)
{
	CBaseCombatWeapon *pWeapon = pHL2Player->GetActiveWeapon();
	if (!pWeapon)
	{
		return;
	}

	int currentAmmo = pHL2Player->GetAmmoCount(pWeapon->GetPrimaryAmmoType());
	if (currentAmmo > 0 && (currentAmmo / pWeapon->GetDefaultClip1()) * 100 > m_iThreshold)
	{
		return;
	}

	DevMsg("CConsumableSpawner::SpawnAmmo()\n");

	if (!Q_strcmp(pWeapon->GetName(), "weapon_pistol"))
	{
		DoSpawnItem(CreateEntityByName(AMMO_PISTOL));
	}
	else if (!Q_strcmp(pWeapon->GetName(), "weapon_smg1"))
	{
		DoSpawnItem(CreateEntityByName(AMMO_SMG1));
	}
}

void CConsumableSpawner::DoSpawnItem(CBaseEntity *pItem)
{
	pItem->Teleport(&GetLocalOrigin(), &GetLocalAngles(), NULL );
	pItem->Spawn();
}