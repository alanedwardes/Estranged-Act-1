//
// Purpose: Send a up small startup statistic for our records.
//

#include "cbase.h"
#include "igamesystem.h"
#include "steam/steam_api.h"
#include "estrangedplayerinformation.h"

ConVar ae_sentinitialstatistic("ae_sentinitialstatistic", "0", FCVAR_ARCHIVE,
	"Whether the game has sent its initial start up statistic. No personally identifiable data is included in this report.");

class C_LogGameStart : public CAutoGameSystem
{
public:
	C_LogGameStart() {};
	~C_LogGameStart() {};
	
	virtual void PostInit()
	{
		Color color(255, 201, 14, 255);
		ConColorMsg(color, "========================================================\n");
		ConColorMsg(color, " Estranged binaries built on %s, at %s.\n", __DATE__, __TIME__);
		ConColorMsg(color, "========================================================\n");

		if (!ae_sentinitialstatistic.GetBool())
		{
			ISteamHTTP *steamHTTP = steamapicontext->SteamHTTP();
			HTTPRequestHandle httpRequest = steamHTTP->CreateHTTPRequest(k_EHTTPMethodPOST, ESTRANGED_STATISTICS_ENDPOINT);
			steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "service", "event");
			steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "event", "startup");
			AppendPlayerInformation(steamHTTP, httpRequest, steamapicontext, NULL, false);
			steamHTTP->SendHTTPRequest(httpRequest, NULL);
			DevMsg("C_LogGameStart: Pushed up initial language/startup statistic.\n");

			ae_sentinitialstatistic.SetValue(true);
		}
	}

	virtual void Shutdown() {};
};

static C_LogGameStart g_pLogGameStart;
C_LogGameStart *GetLogGameStartGameSystem()
{
	return &g_pLogGameStart;
}