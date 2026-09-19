#include "cbase.h"
#include <vgui/IVGui.h>
#include "EstrangedMenuPanelExtras.h"
#include <vgui/ILocalize.h>
#include <vgui/ISystem.h>
#include <vgui/ISurface.h>
#include <steam/steam_api.h>

#define WIKI_COMMAND "Wiki"
#define DISCORD_COMMAND "Discord"
#define ACHIEVEMENTS_COMMAND "Achievements"

EstrangedMenuPanelExtras::EstrangedMenuPanelExtras(vgui::Panel *parent) : EstrangedMenuPanel(parent, LINKS_MENU_NAME)
{
	EstrangedMenuItem *back = AddButton("#Estranged_Menu_BackToMainMenu", MAIN_MENU_NAME, parent, "menu/back");
	SetBackButton(back);
	AddButton("wiki.iamestranged.com", WIKI_COMMAND, this, "menu/wheel");
	AddButton("discord.gg/estranged", DISCORD_COMMAND, this, "menu/discord");
	AddButton("#Estranged_Menu_Achievements", ACHIEVEMENTS_COMMAND, this, "menu/achievements");
}

void EstrangedMenuPanelExtras::OnCommand(const char* command)
{
	if (!Q_strcmp(command, WIKI_COMMAND))		        steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage("https://wiki.iamestranged.com/");
	if (!Q_strcmp(command, DISCORD_COMMAND))	        steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage("https://discord.gg/estranged");
	if (!Q_strcmp(command, ACHIEVEMENTS_COMMAND))       steamapicontext->SteamFriends()->ActivateGameOverlay("Achievements");
}
