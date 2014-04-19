#include "cbase.h"
#include "estrangedplayerinformation.h"
#include "basecombatweapon_shared.h"
#include <time.h>
#include "platform.h"
#include "checksum_sha1.h"
#include "steam/isteamuser.h"

static ConVar ae_uniqueplayerid("ae_uniqueplayerid", "", FCVAR_REPLICATED | FCVAR_HIDDEN | FCVAR_ARCHIVE | FCVAR_NOTIFY);

void AppendPlayerInformation(ISteamHTTP *pSteamHttp, HTTPRequestHandle httpRequest, CSteamAPIContext *pSteamAPIContext, CBasePlayer *pPlayer, bool bAnonymous)
{
	// Local DS
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	char* local_ds = asctime(timeinfo);

	char* newLine = strstr(local_ds, "\n");
	if (newLine)
	{
		*newLine = 0;
	}

	pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "localdatetime", local_ds);

	// Build DS
	char build_ds[64];
	Q_snprintf(build_ds, sizeof(build_ds), "%s,%s", __DATE__, __TIME__);
	pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "builddatetime", build_ds);

	// DxLevel
	pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "dxlevel", ConVarRef("mat_dxlevel").GetString());

	if (pPlayer)
	{
		// Position
		char player_position[32];
		Vector player_pos = pPlayer->GetLocalOrigin();
		Q_snprintf(player_position, sizeof(player_position), "%.2f,%.2f,%.2f", player_pos.x, player_pos.y, player_pos.z);
		pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "position", player_position);

		// Angles
		char player_angles[32];
		QAngle player_ang = pPlayer->GetLocalAngles();
		Q_snprintf(player_angles, sizeof(player_angles), "%.2f,%.2f,%.2f", player_ang.x, player_ang.y, player_ang.z);
		pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "angles", player_angles);

		// Health
		char player_health[16];
		Q_snprintf(player_health, sizeof(player_health), "%i", pPlayer->GetHealth());
		pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "health", player_health);

		// Weapon
		CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
		if (pWeapon)
		{
			pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "weapon", pWeapon->GetClassname());
		}
	}

	// Cheats
	char cheats_enabled[8];
	Q_snprintf(cheats_enabled, sizeof(cheats_enabled), "%i", (ConVarRef("sv_cheats").GetBool() || ConVarRef("developer").GetBool()));
	pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "cheats", cheats_enabled);

	// Map
#ifdef CLIENT_DLL
	char mapname[256];
	V_FileBase(engine->GetLevelName(), mapname, sizeof(mapname));
	pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "level", mapname);
#else
	pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "level", gpGlobals->mapname.ToCStr());
#endif

	if (pSteamAPIContext)
	{
		// Language
		pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "language", pSteamAPIContext->SteamUtils()->GetSteamUILanguage());
	}

	// Platform
	char *			platform = "unknown";
	if (IsPC())		platform = "pc";
	if (IsOSX())	platform = "osx";
	if (IsLinux())	platform = "linux";
	if (IsX360())	platform = "360";
	if (IsPS3())	platform = "ps3";
	pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "platform", platform);

	// Player ID
	if (bAnonymous == false)
	{
		if (ae_uniqueplayerid.GetInt() == 0)
			GenerateUniquePlayerId(pSteamAPIContext);
		pSteamHttp->SetHTTPRequestGetOrPostParameter(httpRequest, "playerid", ae_uniqueplayerid.GetString());
	}
}

void GenerateUniquePlayerId(CSteamAPIContext *pSteamAPIContext)
{
	CSHA1 sha1;

	// Add the steam name
	char szSteamId[256] = "\0";
	CSteamID steamid = pSteamAPIContext->SteamUser()->GetSteamID();
	V_sprintf_safe(szSteamId, "%u%u%u%u", steamid.GetEUniverse(), steamid.GetEAccountType(), steamid.GetAccountID(), steamid.GetUnAccountInstance());
	sha1.Update((unsigned char *)szSteamId, strlen(szSteamId));
	
	// Add some random numbers
	char randomNumbers[256] = "\0";
	Q_snprintf(randomNumbers, sizeof(randomNumbers), "%i%i%i%i%i%i",
		RandomInt(0, 1000), RandomInt(0, 1000), RandomInt(0, 1000), RandomInt(0, 1000), RandomInt(0, 1000), RandomInt(0, 1000));
	sha1.Update((unsigned char *)randomNumbers, strlen(randomNumbers));

	// Generate the hash
	sha1.Final();

	// Compile SHA1 Report
	char szReport[1024] = "\0";
	sha1.ReportHash(szReport);

	// Remove the spaces and make it lowercase
	char playerId[1024] = "\0";
	Q_StrSubst(szReport, " ", "", playerId, sizeof(playerId));
	Q_strlower(playerId);

	DevMsg("Generated unique player ID: %s\n", playerId);
	ae_uniqueplayerid.SetValue(playerId);
}