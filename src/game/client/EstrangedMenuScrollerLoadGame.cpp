#include "cbase.h"
#include "EstrangedMenuScrollerLoadGame.h"
#include "filesystem.h"
#include "fmtstr.h"

EstrangedMenuScrollerLoadGame::EstrangedMenuScrollerLoadGame(vgui::VPANEL parent) : BaseClass(parent, "LOAD GAME")
{
	AddSavedGames();
}

void EstrangedMenuScrollerLoadGame::OnSave()
{
	AddSavedGames();
}

void EstrangedMenuScrollerLoadGame::AddSavedGames()
{
	ClearItems();

	const char *SAVE_PATH = "./SAVE/";
	FileFindHandle_t fh;
	char path[256];
	Q_snprintf(path, sizeof(path), "%s*.sav", SAVE_PATH);

	char const *fn = g_pFullFileSystem->FindFirstEx(path, "MOD", &fh);
	if (!fn) return;

	char save_name[64];
	do
	{
		Q_StripExtension(fn, save_name, sizeof(save_name));

		AddItem(new vgui::Button(NULL, "", save_name, this, save_name));

		fn = g_pFullFileSystem->FindNext(fh);
	}
	while(fn);

	g_pFullFileSystem->FindClose(fh);
}

void EstrangedMenuScrollerLoadGame::OnCommand(const char *command)
{
	BaseClass::OnCommand(command);

	CFmtStr console_command;
	console_command.sprintf("progress_enable \n load %s", command);

	engine->ClientCmd(console_command.Access());
}