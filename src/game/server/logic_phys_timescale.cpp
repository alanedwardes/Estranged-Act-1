//=============================================================================
//
// Purpose: Physics timescale logical entity
//
//=============================================================================
 
#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
 
class CLogicPhysTimescale : public CPointEntity
{
	public:
		DECLARE_CLASS( CLogicPhysTimescale, CPointEntity );
		DECLARE_DATADESC();

		CLogicPhysTimescale(){};
 
		void Spawn( void );

		void InputSetPhysTimescale( inputdata_t &inputdata );
};
 
LINK_ENTITY_TO_CLASS( logic_phys_timescale, CLogicPhysTimescale );

BEGIN_DATADESC( CLogicPhysTimescale )
	DEFINE_INPUTFUNC( FIELD_FLOAT, "SetPhysTimescale", InputSetPhysTimescale ),
END_DATADESC()

void CLogicPhysTimescale::Spawn( void )
{
	BaseClass::Spawn();
}

void CLogicPhysTimescale::InputSetPhysTimescale( inputdata_t &inputdata )
{
	ConVarRef phys_timescale("phys_timescale");
	DevMsg("Setting phys_timescale to %f\n", inputdata.value.Float());
	phys_timescale.SetValue(inputdata.value.Float());
}