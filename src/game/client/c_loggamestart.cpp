//
// Purpose: Send a up small startup statistic for our records.
//

#include "cbase.h"
#include "igamesystem.h"
#include "steam/steam_api.h"
#include "estrangedplayerinformation.h"

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
	}

	virtual void Shutdown() {};
};

static C_LogGameStart g_pLogGameStart;
C_LogGameStart *GetLogGameStartGameSystem()
{
	return &g_pLogGameStart;
}