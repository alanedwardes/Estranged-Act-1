#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "EstrangedMenuPanel.h"
#include "igamesystem.h"

class EstrangedMenuPanelMain : public EstrangedMenuPanel
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuPanelMain, EstrangedMenuPanel);

	public:
		EstrangedMenuPanelMain(vgui::Panel* parent);
		virtual void OnEnterGame();
		virtual void OnExitGame();
		virtual void OnCommand(const char*);
 
	private:
		vgui::Panel		*resumeGameButton;
		vgui::Panel		*saveLoadMenu;
		vgui::Panel		*loadGameButton;
};