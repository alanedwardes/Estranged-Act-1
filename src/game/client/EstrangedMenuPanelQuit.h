#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "EstrangedMenuPanel.h"

class EstrangedMenuPanelQuit : public EstrangedMenuPanel
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuPanelQuit, EstrangedMenuPanel);

	public:
		EstrangedMenuPanelQuit(vgui::Panel* parent);
		virtual void OnEnterGame();
		virtual void OnExitGame();
		virtual void OnCommand(const char*);
 
	private:
		vgui::Panel		*quitSaveConfirm;
};