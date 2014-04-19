#include "steam/steam_api.h"
#include "baseplayer_shared.h"

#ifndef ESTRANGED_PLAYER_INFORMATION_H
#define ESTRANGED_PLAYER_INFORMATION_H

#include "shareddefs_estranged.h"

void AppendPlayerInformation(ISteamHTTP *pSteamHttp, HTTPRequestHandle httpRequest, CSteamAPIContext *pSteamAPIContext, CBasePlayer *pPlayer, bool bAnonymous);
void GenerateUniquePlayerId(CSteamAPIContext *pSteamAPIContext);

#endif