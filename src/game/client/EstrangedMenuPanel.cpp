#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Label.h>
#include "EstrangedMenuPanel.h"
#include "ienginevgui.h"
#include "filesystem.h"
#include "EstrangedMenuUtil.h"

EstrangedMenuPanel::EstrangedMenuPanel(Panel *parent, const char* name) : vgui::Panel(parent, name)
{
	SetKeyBoardInputEnabled(true);
	m_iGameState = -1;
	m_backItem = NULL;

	m_bWasVisible = false;

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
}

EstrangedMenuItem* EstrangedMenuPanel::AddButton(const char *text, const char *command, vgui::Panel *signalTarget, const char *iconPath)
{
	EstrangedMenuButton *button = new EstrangedMenuButton(this, text, command, signalTarget, iconPath);
	m_Buttons.AddToTail(button);
	return button;
}

EstrangedMenuItem* EstrangedMenuPanel::AddCheckButton(const char *text, const char *command, vgui::Panel *signalTarget, bool checked)
{
	EstrangedMenuCheckButton *checkButton = new EstrangedMenuCheckButton(this, text, command, signalTarget, checked);
	m_Buttons.AddToTail(checkButton);
	return checkButton;
}

EstrangedMenuItem* EstrangedMenuPanel::AddSlider(const char *text, const char *command, vgui::Panel *signalTarget, float value)
{
	EstrangedMenuSlider *slider = new EstrangedMenuSlider(this, text, command, signalTarget, value);
	m_Buttons.AddToTail(slider);
	return slider;
}

void EstrangedMenuPanel::OnTick()
{
	BaseClass::OnTick();

	if (!EstrangedMenuUtil::IsGameUIVisible() && m_bWasVisible)
	{
		OnHideMenu();
		m_bWasVisible = false;
	}
}

void EstrangedMenuPanel::RemoveButtons()
{
	for (int i = 0; i < m_Buttons.Count(); i++)
	{
		m_Buttons.Element(i)->DeletePanel();
	}

	m_Buttons.RemoveAll();
}

bool EstrangedMenuPanel::HasSaveFiles()
{
	const char *SAVE_PATH = "./SAVE/";
	FileFindHandle_t fh;
	char path[256];
	Q_snprintf(path, sizeof(path), "%s*.sav", SAVE_PATH);

	char const *fn = g_pFullFileSystem->FindFirstEx(path, "MOD", &fh);
	g_pFullFileSystem->FindClose(fh);
	
	return fn;
}

void EstrangedMenuPanel::OnKeyCodePressed(vgui::KeyCode code)
{
	int activeItem = GetActiveItemIndex();
	SetAllItemsInactive();

	int index = 0;

	switch (code)
	{
		case KEY_DOWN:
		case KEY_XBUTTON_DOWN:
		case KEY_XSTICK1_DOWN:
			index = GetNextVisibleItem(activeItem);
			m_Buttons.Element(index)->SetItemActive(true);
			break;
		case KEY_UP:
		case KEY_XBUTTON_UP:
		case KEY_XSTICK1_UP:
			index = GetLastVisibleItem(activeItem);
			m_Buttons.Element(index)->SetItemActive(true);
			break;
		case KEY_XBUTTON_B:
			if (m_backItem != NULL)
			{
				m_backItem->SetItemActive(true);
			}
			break;
		default:
			if (activeItem > -1) m_Buttons.Element(activeItem)->OnKeyCodePressed(code);
	}
}

void EstrangedMenuPanel::SetBackButton(EstrangedMenuItem* menuItem)
{
	m_backItem = menuItem;
}

int EstrangedMenuPanel::GetLastVisibleItem(int start)
{
	for (int i = start - 1; i > -1; i--)
	{
		if (m_Buttons.Element(i)->IsVisible()) return i;
	}

	return GetLastVisibleItem(m_Buttons.Count());
 }

int EstrangedMenuPanel::GetNextVisibleItem(int start)
{
	for (int i = start + 1; i < m_Buttons.Count(); i++)
	{
		if (m_Buttons.Element(i)->IsVisible()) return i;
	}

	return GetNextVisibleItem(-1);
}

void EstrangedMenuPanel::SetAllItemsInactive()
{
	for (int i = 0; i < m_Buttons.Count(); i++)
	{
		m_Buttons.Element(i)->SetItemActive(false);
	}
}

int EstrangedMenuPanel::GetActiveItemIndex()
{
	for (int i = 0; i < m_Buttons.Count(); i++)
	{
		EstrangedMenuItem *child = m_Buttons.Element(i);
		if (child->IsItemActive() && child->IsVisible()) return i;
	}

	return -1;
}

void EstrangedMenuPanel::OnThink()
{
	short gameState = engine->IsLevelMainMenuBackground() + engine->IsInGame();

	if (gameState != m_iGameState || m_iGameState < 0)
	{
		if (gameState == 1)
		{
			OnEnterGame();
		}
		else
		{
			OnExitGame();
		}
	}

	if (!m_bWasVisible)
	{
		OnShowMenu();
		m_bWasVisible = true;
	}

	m_iGameState = gameState;
}

void EstrangedMenuPanel::SendCommandToGameUI(const char *command)
{
	EstrangedMenuUtil::CloseGameFrames(GetVPanel());

	vgui::ivgui()->PostMessage(EstrangedMenuUtil::GetBaseGameUIPanel(), new KeyValues("command", "command", command), GetVPanel());
}

void EstrangedMenuPanel::SetVisible(bool visible)
{
	BaseClass::SetVisible(visible);

	int index = GetNextVisibleItem(-1);
	m_Buttons.Element(index)->SetItemActive(true);
}

void EstrangedMenuPanel::PerformLayout()
{
	if (GetTall() <= 0 || GetChildCount() <= 0) return;

	int buttonCount = 0;
	int buttonHeight = GetTall() / 6;
	for (int i = 0; i < m_Buttons.Count(); i++)
	{
		vgui::Panel *child = m_Buttons.Element(i);
		if (!child->IsVisible()) continue;
		child->SetPos(0, buttonCount * buttonHeight);
		buttonCount++;
	}
}