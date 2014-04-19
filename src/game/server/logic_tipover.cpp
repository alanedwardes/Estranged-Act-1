//=============================================================================
//
// Purpose: Logic fallover - fires output when tipped over.
//
//=============================================================================
 
#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define THINK_TIME 0.05f
 
class CLogicTipOver : public CPointEntity
{
	DECLARE_DATADESC();

	public:
		DECLARE_CLASS(CLogicTipOver, CPointEntity);

		CLogicTipOver()
		{
			m_bTippedOver = false;
		}
 
		void Spawn( void );
		void Think( void );

		COutputEvent	m_onTippedOver;
	private:
		bool	m_bTippedOver;
		float	m_flThreshold;
};
 
LINK_ENTITY_TO_CLASS(logic_tipover, CLogicTipOver);
BEGIN_DATADESC(CLogicTipOver)
	DEFINE_FIELD(m_bTippedOver, FIELD_BOOLEAN),
	DEFINE_KEYFIELD(m_flThreshold, FIELD_FLOAT, "threshold"),
	DEFINE_THINKFUNC(Think),
	DEFINE_OUTPUT(m_onTippedOver, "OnTippedOver")
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CLogicTipOver::Spawn(void)
{
	BaseClass::Spawn();
	SetNextThink(gpGlobals->curtime + THINK_TIME);
}

//-----------------------------------------------------------------------------
// Purpose: Think function to detect when the entity falls over
//-----------------------------------------------------------------------------
void CLogicTipOver::Think(void)
{
	bool oldTippedOver = m_bTippedOver;
	CBaseEntity *parent = GetParent();

	if (parent)
	{
		QAngle m_qAngles = parent->GetLocalAngles();

		m_bTippedOver = false;

		if (m_qAngles.z > m_flThreshold || m_qAngles.z < -m_flThreshold)
		{
			m_bTippedOver = true;
		}

		if (m_qAngles.x > m_flThreshold || m_qAngles.x < -m_flThreshold)
		{
			m_bTippedOver = true;
		}

		if (oldTippedOver != m_bTippedOver && m_bTippedOver)
		{
			m_onTippedOver.FireOutput(this, this);
		}

		SetNextThink(gpGlobals->curtime + THINK_TIME);
	}
	else
	{
		DevWarning("trigger_fallover with no parent!\n");
	}
}