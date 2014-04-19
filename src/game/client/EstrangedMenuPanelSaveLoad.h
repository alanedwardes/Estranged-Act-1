#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "EstrangedMenuPanel.h"

class EstrangedMenuPanelSaveLoad : public EstrangedMenuPanel
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuPanelSaveLoad, EstrangedMenuPanel);

	public:
		EstrangedMenuPanelSaveLoad(vgui::Panel* parent);
		virtual void OnEnterGame();
		virtual void OnExitGame();
		virtual void OnSave();
		virtual void OnCommand(const char*);
 
	private:
		vgui::Panel		*saveGameButton;
		vgui::Panel		*loadGameButton;
};