#include <vgui_controls/Frame.h>
#include "EstrangedMenuPanel.h"
#include "GameUI/IGameUI.h"

#define BLANK Color(0, 0, 0, 0)

class EstrangedMenu : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(EstrangedMenu, vgui::Frame);
 
	EstrangedMenu(vgui::VPANEL parent);
	~EstrangedMenu(){};
 
	protected:
		virtual void OnKeyCodePressed(vgui::KeyCode code);
		virtual void OnTick();
		virtual void OnThink();
		virtual void OnCommand(const char*);
		virtual void PerformLayout();
		virtual void ApplySchemeSettings(vgui::IScheme*);
 
	private:
		void							SetActiveMenu(const char* newMenu);
		void							AddMenuPanels();
		vgui::Label						*logo;
		vgui::Label						*tag;
		vgui::Panel						*darkBackground;
		IGameUI							*m_pGameUI;
		CUtlVector<vgui::Panel*>		menuPanels;
		vgui::IScheme					*m_pScheme;
		int buttonWidth;
};