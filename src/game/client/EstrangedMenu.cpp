#include "cbase.h"
#include "EstrangedMenu.h"
#include "EstrangedMenuPanelMain.h"
#include "EstrangedMenuPanelOptions.h"
#include "EstrangedMenuPanelQuit.h"
#include "EstrangedMenuPanelExtras.h"
#include "EstrangedMenuPanelSaveLoad.h"
#include "EstrangedMenuPanelNewGame.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Label.h>
#include "ienginevgui.h"
#include "EstrangedMenuUtil.h"
#include <vgui/ISurface.h>

// See interface.h/.cpp for specifics:  basically this ensures that we actually Sys_UnloadModule the dll and that we don't call Sys_LoadModule 
//  over and over again.
static CDllDemandLoader g_GameUI( "GameUI" );

EstrangedMenu::EstrangedMenu(vgui::VPANEL parent): BaseClass(NULL, "EstrangedMenu")
{
	SetParent(parent);

	SetCloseButtonVisible(false);
	SetDragEnabled(false);
	SetShowDragHelper(false);
	SetVisible(false);
	SetTitleBarVisible(false);
	SetSizeable(false);

	AddMenuPanels();

	CreateInterfaceFn gameUIFactory = g_GameUI.GetFactory();
	if (gameUIFactory)
	{
		m_pGameUI = (IGameUI*) gameUIFactory(GAMEUI_INTERFACE_VERSION, NULL);
	}

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
}

void EstrangedMenu::AddMenuPanels()
{
	logo = new vgui::Label(this, "Logo", "#Game_Title");
	tag = new vgui::Label(this, "Tag", "#Game_Tag");
	darkBackground = new vgui::Panel(this);

	menuPanels.AddToTail(new EstrangedMenuPanelMain(this));

	menuPanels.AddToTail(new EstrangedMenuPanelOptions(this));
	menuPanels.AddToTail(new EstrangedMenuPanelQuit(this));
	menuPanels.AddToTail(new EstrangedMenuPanelSaveLoad(this));
	menuPanels.AddToTail(new EstrangedMenuPanelExtras(this));

	menuPanels.AddToTail(new EstrangedMenuPanelNewGame(this));

	SetActiveMenu(MAIN_MENU_NAME);
}

void EstrangedMenu::SetActiveMenu(const char* newMenu)
{
	for (int i = 0; i < menuPanels.Count(); i++)
	{
		vgui::Panel *menuPanel = menuPanels.Element(i);
		if (!Q_strcmp(menuPanel->GetName(), newMenu))
		{
			menuPanel->SetVisible(true);
			menuPanel->RequestFocus();
		}
		else
		{
			menuPanel->SetVisible(false);
		}
	}

	Activate();
}

void EstrangedMenu::OnKeyCodePressed(vgui::KeyCode code)
{
	BaseClass::OnKeyCodePressed(code);
}

void EstrangedMenu::PerformLayout()
{
	BaseClass::PerformLayout();

	SetPos(ScreenWidth() / 10, 0);

	float relationalWidthBasedonScreenWidth = float(ScreenWidth()) / 3.5f;
	float relationalWidthBasedonScreenHeight = float(ScreenHeight()) / 2.0f;

	float menuWidth = max(relationalWidthBasedonScreenWidth, relationalWidthBasedonScreenHeight);

	SetWide(menuWidth);
	SetTall(ScreenHeight());

	const float menuDivision = 2.5f;
	const float menuPadding = menuWidth / 30.0f;

	int height = GetTall() / menuDivision;

	logo->SizeToContents();
	logo->SetPos(menuPadding, 0);
	logo->SetContentAlignment(vgui::Label::a_south);
	logo->SetTall(height);

	tag->SizeToContents();
	tag->SetPos(menuPadding + logo->GetWide() + (menuPadding / 2.0f), 0);
	tag->SetContentAlignment(vgui::Label::a_south);
	tag->SetTall(height);

	int y = GetTall() / menuDivision;
	darkBackground->SetPos(0, y);
	darkBackground->SetSize(GetWide(), GetTall());
	int menuTall = GetTall() / menuDivision;

	for (int i = 0; i < menuPanels.Count(); i++)
	{
		vgui::Panel *menuPanel = menuPanels.Element(i);
		menuPanel->SetPos(0, y);
		menuPanel->SetSize(GetWide(), menuTall);
	}
}

void EstrangedMenu::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	logo->SetFont(pScheme->GetFont("EstrangedMenuHeader"));
	tag->SetFont(pScheme->GetFont("EstrangedMenuHeader"));
	tag->SetFgColor(pScheme->GetColor("LightSlateGray", BLANK));
	SetAlpha(0);

	m_pScheme = pScheme;
}

void EstrangedMenu::OnCommand(const char *command)
{
	if (!Q_strcmp(command, "Close")) return;

	SetActiveMenu(command);

	EstrangedMenuUtil::CloseGameFrames(GetVPanel());
}

void EstrangedMenu::OnThink()
{
	BaseClass::OnThink();

	if (engine->IsDrawingLoadingImage() && IsVisible())
	{
		SetActiveMenu(MAIN_MENU_NAME);
		SetVisible(false);
	}

	if (IsVisible() && GetAlpha() < 200)
	{
		SetAlpha(GetAlpha() + 1);
	}

	//vgui::surface()->MovePopupToBack(GetVPanel());
	darkBackground->SetBgColor(m_pScheme->GetColor("DarkSlateGray", BLANK));
	
	SetBgColor(m_pScheme->GetColor("SlateGray", BLANK));
}

void EstrangedMenu::OnTick()
{	
	BaseClass::OnTick();

	if (!engine->IsDrawingLoadingImage() && !IsVisible())
	{
		Activate();
		SetVisible(true);
	}
}