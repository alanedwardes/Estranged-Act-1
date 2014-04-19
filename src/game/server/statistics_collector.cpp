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

ConVar stats_enabled( "ae_stats_enabled", "1", FCVAR_ARCHIVE);
 
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
		void		SendEvent(const char *p_sEvent);
};
 
LINK_ENTITY_TO_CLASS(statistics_collector, CStatisticsCollector);
 
BEGIN_DATADESC(CStatisticsCollector)
	DEFINE_INPUTFUNC(FIELD_STRING, "Event", InputSendStatistic)
END_DATADESC()

void CStatisticsCollector::FireGameEvent(IGameEvent* e)
{
	if (!Q_strcmp("player_hurt", e->GetName()))
	{
		CBasePlayer *player = UTIL_GetLocalPlayer();
		if (player && player->GetHealth() <= 0)
		{
			// The player has unfortunately passed away
			SendStatistic("player_death");
		}
	}
}

void CStatisticsCollector::SendEvent(const char *p_sEvent)
{
	DevMsg("Statistics Collectior: Sending event '%s'\n", p_sEvent);

	CBasePlayer *pPlayer = UTIL_GetLocalPlayer();

	ISteamHTTP *steamHTTP = steamapicontext->SteamHTTP();
	if (!steamHTTP) return;
	HTTPRequestHandle httpRequest = steamHTTP->CreateHTTPRequest(k_EHTTPMethodPOST, ESTRANGED_STATISTICS_ENDPOINT);
	AppendPlayerInformation(steamHTTP, httpRequest, steamapicontext, pPlayer, false);
	steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "service", "event");
	steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "event", p_sEvent);
	steamHTTP->SendHTTPRequest(httpRequest, NULL);

	DevMsg("Statistics Collectior: Event sent.\n");
}

void CStatisticsCollector::SendStatistic(const char *p_sEvent)
{
	if (stats_enabled.GetInt() != 0)
	{
		SendEvent(p_sEvent);
	}
	else
	{
		DevMsg("Statistics Collectior: Events disabled, ignoring.\n");
	}
}

void CStatisticsCollector::InputSendStatistic(inputdata_t &inputData)
{
	SendStatistic(inputData.value.String());
}