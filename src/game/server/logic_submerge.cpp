//=============================================================================
//
// Purpose: Logic submerge - fires output when submerged in water.
//
//=============================================================================
 
#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define THINK_TIME 0.05f
 
class CLogicSubmerge : public CPointEntity
{
	DECLARE_DATADESC();

	public:
		DECLARE_CLASS(CLogicSubmerge, CPointEntity);

		CLogicSubmerge()
		{
			m_bSubmerged = false;
		}
 
		void Spawn( void );
		void Think( void );

		COutputEvent	m_onSubmerge;
	private:
		bool	m_bSubmerged;
};
 
LINK_ENTITY_TO_CLASS(logic_submerge, CLogicSubmerge);
BEGIN_DATADESC(CLogicSubmerge)
	DEFINE_FIELD(m_bSubmerged, FIELD_BOOLEAN),
	DEFINE_THINKFUNC(Think),
	DEFINE_OUTPUT(m_onSubmerge, "OnSubmerged")
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CLogicSubmerge::Spawn(void)
{
	BaseClass::Spawn();
	SetNextThink(gpGlobals->curtime + THINK_TIME);
}

//-----------------------------------------------------------------------------
// Purpose: Think function to detect when the entity is submerged
//-----------------------------------------------------------------------------
void CLogicSubmerge::Think(void)
{
	bool oldSubmerged = m_bSubmerged;
	CBaseEntity *parent = GetParent();

	if (parent)
	{
		if (parent->GetWaterLevel() > 0)
		{
			m_bSubmerged = true;
		}
		else
		{
			m_bSubmerged = false;
		}

		if (oldSubmerged != m_bSubmerged && m_bSubmerged)
		{
			m_onSubmerge.FireOutput(this, this);
		}

		SetNextThink(gpGlobals->curtime + THINK_TIME);
	}
	else
	{
		DevWarning("logic_submerge with no parent!\n");
	}
}