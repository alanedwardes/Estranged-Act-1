#include "cbase.h"
#include "EstrangedMenuPanelQuit.h"

#define SAVE_QUIT_COMMAND "SaveQuit"
#define QUIT_COMMAND "Quit"

EstrangedMenuPanelQuit::EstrangedMenuPanelQuit(vgui::Panel *parent) : EstrangedMenuPanel(parent, QUIT_MENU_NAME)
{
	EstrangedMenuItem *back = AddButton("#Estranged_Menu_BackToMainMenu", MAIN_MENU_NAME, parent, "menu/back");
	SetBackButton(back);
	quitSaveConfirm = AddButton("#Estranged_Menu_QuitSave", SAVE_QUIT_COMMAND, this, "menu/save");
	AddButton("#Estranged_Menu_Quit", QUIT_COMMAND, this, "menu/quit");
}

void EstrangedMenuPanelQuit::OnCommand(const char* command)
{
	if (!Q_strcmp(command, SAVE_QUIT_COMMAND))	return engine->ExecuteClientCmd("save autosave\n exit");
	if (!Q_strcmp(command, QUIT_COMMAND))		return engine->ExecuteClientCmd("quit");
}

void EstrangedMenuPanelQuit::OnEnterGame()
{
	quitSaveConfirm->SetVisible(true);

	InvalidateLayout(true);
}

void EstrangedMenuPanelQuit::OnExitGame()
{
	quitSaveConfirm->SetVisible(false);

	InvalidateLayout(true);
}