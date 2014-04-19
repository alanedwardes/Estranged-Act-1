#include "cbase.h"
#include "EstrangedMenuPanelNewGame.h"

#define CHAPTER1_CFG "chapter1"
#define CHAPTER2_CFG "chapter2"
#define CHAPTER3_CFG "chapter3"

EstrangedMenuPanelNewGame::EstrangedMenuPanelNewGame(vgui::Panel *parent) : EstrangedMenuPanel(parent, NEW_GAME_MENU_NAME)
{
	EstrangedMenuItem *back = AddButton("#Estranged_Menu_BackToMainMenu", MAIN_MENU_NAME, parent, "menu/back");
	SetBackButton(back);
	AddButton("#Estranged_Chapter1_Title", CHAPTER1_CFG, this, "menu/wheel");
	AddButton("#Estranged_Chapter2_Title", CHAPTER2_CFG, this, "menu/sewer");
	AddButton("#Estranged_Chapter3_Title", CHAPTER3_CFG, this, "menu/cliff");
}

void EstrangedMenuPanelNewGame::OnCommand(const char* command)
{
	char chapter_cfg[64];
	Q_snprintf(chapter_cfg, sizeof(chapter_cfg), "exec %s.cfg", command);
	engine->ClientCmd(chapter_cfg);
}