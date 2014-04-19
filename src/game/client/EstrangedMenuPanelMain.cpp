#include "cbase.h"
#include "EstrangedMenuPanelMain.h"
#include <vgui/ILocalize.h>

EstrangedMenuPanelMain::EstrangedMenuPanelMain(vgui::Panel *parent) : EstrangedMenuPanel(parent, MAIN_MENU_NAME)
{
	resumeGameButton = AddButton("#Estranged_Menu_ResumeGame", RESUME_COMMAND, this, "menu/back");
	saveLoadMenu = AddButton("#Estranged_Menu_SaveLoadMenu", SAVELOAD_MENU_NAME, parent, "menu/load");
	loadGameButton = AddButton("#Estranged_Menu_ContinueGame", LOAD_COMMAND, this, "menu/load");
	AddButton("#Estranged_Menu_NewGame", NEW_GAME_MENU_NAME, parent, "menu/new");
	AddButton("#Estranged_Menu_OptionsMenu", OPTIONS_MENU_NAME, parent, "menu/options");
	AddButton("#Estranged_Menu_ExtrasMenu", LINKS_MENU_NAME, parent, "menu/extras");
	AddButton("#Estranged_Menu_QuitMenu", QUIT_MENU_NAME, parent, "menu/quit");
}

void EstrangedMenuPanelMain::OnCommand(const char* command)
{
	if (!Q_strcmp(command, RESUME_COMMAND))		SendCommandToGameUI(RESUME_COMMAND);
	if (!Q_strcmp(command, LOAD_COMMAND))		SendCommandToGameUI(LOAD_COMMAND);
}

void EstrangedMenuPanelMain::OnEnterGame()
{
	resumeGameButton->SetVisible(true);
	saveLoadMenu->SetVisible(true);
	loadGameButton->SetVisible(false);

	InvalidateLayout(true);
}

void EstrangedMenuPanelMain::OnExitGame()
{
	resumeGameButton->SetVisible(false);
	saveLoadMenu->SetVisible(false);
	loadGameButton->SetVisible(HasSaveFiles());

	InvalidateLayout(true);
}