#include "cbase.h"
#include "EstrangedMenuPanelOptions.h"

#define OPTIONS_COMMAND "OpenOptionsDialog"
#define ESTRANGED_OPTIONS_COMMAND "engine ToggleEstrangedOptionsPanel"

EstrangedMenuPanelOptions::EstrangedMenuPanelOptions(vgui::Panel *parent) : EstrangedMenuPanel(parent, OPTIONS_MENU_NAME)
{
	EstrangedMenuItem *back = AddButton("#Estranged_Menu_BackToMainMenu", MAIN_MENU_NAME, parent, "menu/back");
	SetBackButton(back);
	AddButton("#Estranged_Menu_GameOptions", OPTIONS_COMMAND, this, "menu/options");
	AddButton("#Estranged_Menu_EstrangedOptions", ESTRANGED_OPTIONS_COMMAND, this, "menu/estrangedoptions");
}

void EstrangedMenuPanelOptions::OnCommand(const char* command)
{
	if (!Q_strcmp(command, OPTIONS_COMMAND))				SendCommandToGameUI(OPTIONS_COMMAND);
	if (!Q_strcmp(command, ESTRANGED_OPTIONS_COMMAND))		SendCommandToGameUI(ESTRANGED_OPTIONS_COMMAND);
}