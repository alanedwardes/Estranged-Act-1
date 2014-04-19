#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "EstrangedMenuButton.h"
#include "EstrangedMenuCheckButton.h"
#include "EstrangedMenuSlider.h"

#define MAIN_MENU_NAME "EstrangedMenuPanelMain"
#define LINKS_MENU_NAME "EstrangedMenuPanelLinks"
#define SAVELOAD_MENU_NAME "EstrangedMenuPanelSaveLoad"
#define OPTIONS_MENU_NAME "EstrangedMenuPanelOptions"
#define QUIT_MENU_NAME "EstrangedMenuPanelQuit"
#define NEW_GAME_MENU_NAME "EstrangedMenuPanelNewGame"

#define LOAD_COMMAND "OpenLoadGameDialog"
#define SAVE_COMMAND "OpenSaveGameDialog"
#define RESUME_COMMAND "ResumeGame"

#pragma once

abstract_class EstrangedMenuPanel : public vgui::Panel, public CAutoGameSystem
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuPanel, vgui::Panel);

	public:
		EstrangedMenuPanel(vgui::Panel* parent, const char* name);
		EstrangedMenuItem*		AddButton(const char*, const char*, vgui::Panel*, const char* iconPath);
		EstrangedMenuItem*		AddCheckButton(const char*, const char*, vgui::Panel*, bool);
		EstrangedMenuItem*		AddSlider(const char*, const char*, vgui::Panel*, float);
		virtual void			SetVisible(bool visible);
		virtual void			SetAllItemsInactive();
		virtual int				GetNextVisibleItem(int);
		virtual int				GetLastVisibleItem(int);
		virtual int				GetActiveItemIndex();
		virtual void			SetBackButton(EstrangedMenuItem*);
		virtual void			RemoveButtons();
		virtual void			OnKeyCodePressed(vgui::KeyCode code);
		virtual void			OnThink();
		virtual void			OnEnterGame(){};
		virtual void			OnExitGame(){};
		virtual void			OnHideMenu(){};
		virtual void			OnShowMenu(){};
		virtual void			PerformLayout();
		virtual void			SendCommandToGameUI(const char*);
		virtual bool			HasSaveFiles();
		virtual void			OnTick();
 
	private:
		CUtlVector<EstrangedMenuItem*>	m_Buttons;
		EstrangedMenuItem*				m_backItem;
		short							m_iGameState;
		bool							m_bWasVisible;
};