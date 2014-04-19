#include "cbase.h"
#include <vgui/IVGui.h>
#include "EstrangedMenuPanelExtras.h"
#include <vgui/ILocalize.h>
#include <vgui/ISystem.h>
#include <vgui/ISurface.h>
#include <steam/steam_api.h>

#define ESTRANGED_SITE_COMMAND "EstrangedSite"
#define MODDB_COMMAND "ModDB"
#define ACHIEVEMENTS_COMMAND "Achievements"

EstrangedMenuPanelExtras::EstrangedMenuPanelExtras(vgui::Panel *parent) : EstrangedMenuPanel(parent, LINKS_MENU_NAME)
{
	EstrangedMenuItem *back = AddButton("#Estranged_Menu_BackToMainMenu", MAIN_MENU_NAME, parent, "menu/back");
	SetBackButton(back);
	AddButton("#Estranged_Menu_Estranged", ESTRANGED_SITE_COMMAND, this, "menu/wheel");
	AddButton("#Estranged_Menu_ModDB", MODDB_COMMAND, this, "menu/moddb");
	AddButton("#Estranged_Menu_Achievements", ACHIEVEMENTS_COMMAND, this, "menu/achievements");
}

void EstrangedMenuPanelExtras::OnCommand(const char* command)
{
	if (!Q_strcmp(command, ESTRANGED_SITE_COMMAND))		OpenAddress(g_pVGuiLocalize->Find("#Estranged_Menu_EstrangedAddress"));
	if (!Q_strcmp(command, MODDB_COMMAND))				OpenAddress(g_pVGuiLocalize->Find("#Estranged_Menu_ModDBAddress"));
	if (!Q_strcmp(command, ACHIEVEMENTS_COMMAND))		steamapicontext->SteamFriends()->ActivateGameOverlay("Achievements");
}

void EstrangedMenuPanelExtras::OpenAddress(wchar_t* address)
{
	char char_address[128];
	g_pVGuiLocalize->ConvertUnicodeToANSI(address, char_address, sizeof(char_address));

	steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage(char_address);
}