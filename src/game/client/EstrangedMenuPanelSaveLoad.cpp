#include "cbase.h"
#include "EstrangedMenuPanelSaveLoad.h"

EstrangedMenuPanelSaveLoad::EstrangedMenuPanelSaveLoad(vgui::Panel *parent) : EstrangedMenuPanel(parent, SAVELOAD_MENU_NAME)
{
	EstrangedMenuItem *back = AddButton("#Estranged_Menu_BackToMainMenu", MAIN_MENU_NAME, parent, "menu/back");
	SetBackButton(back);
	loadGameButton = AddButton("#Estranged_Menu_LoadGame", LOAD_COMMAND, this, "menu/load");
	saveGameButton = AddButton("#Estranged_Menu_SaveGame", SAVE_COMMAND, this, "menu/save");
	loadGameButton->SetVisible(HasSaveFiles());
}

void EstrangedMenuPanelSaveLoad::OnCommand(const char* command)
{
	if (!Q_strcmp(command, LOAD_COMMAND))		SendCommandToGameUI(LOAD_COMMAND);
	if (!Q_strcmp(command, SAVE_COMMAND))		SendCommandToGameUI(SAVE_COMMAND);
}

void EstrangedMenuPanelSaveLoad::OnEnterGame()
{
	saveGameButton->SetVisible(true);

	InvalidateLayout(true);
}

void EstrangedMenuPanelSaveLoad::OnExitGame()
{
	saveGameButton->SetVisible(false);

	InvalidateLayout(true);
}

void EstrangedMenuPanelSaveLoad::OnSave()
{
	if (!loadGameButton->IsVisible())
	{
		loadGameButton->SetVisible(true);

		InvalidateLayout(true);
	}
}