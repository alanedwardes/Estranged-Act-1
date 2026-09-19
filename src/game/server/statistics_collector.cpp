//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ========
//
// Purpose: Simple entity to log and send gameplay statistics
//
//=============================================================================

#include "cbase.h"
#include "steam/steam_api.h"
#include "view_shared.h"
#include "GameEventListener.h"
#include "estrangedplayerinformation.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
class CStatisticsCollector : public CLogicalEntity, public CGameEventListener
{
	public:
		DECLARE_CLASS( CStatisticsCollector, CLogicalEntity );
		DECLARE_DATADESC();
 
		CStatisticsCollector()
		{
			ListenForGameEvent("player_hurt");
		}

		void		FireGameEvent(IGameEvent* e);
		void		Init(void);
		void		GameEvent(void);
		void		SendStatistic(const tchar *p_sEvent);
		void		InputSendStatistic( inputdata_t &inputData);
};
 
LINK_ENTITY_TO_CLASS(statistics_collector, CStatisticsCollector);
 
BEGIN_DATADESC(CStatisticsCollector)
	DEFINE_INPUTFUNC(FIELD_STRING, "Event", InputSendStatistic)
END_DATADESC()

void CStatisticsCollector::FireGameEvent(IGameEvent* e)
{
}

void CStatisticsCollector::SendStatistic(const char *p_sEvent)
{
}

void CStatisticsCollector::InputSendStatistic(inputdata_t &inputData)
{
}