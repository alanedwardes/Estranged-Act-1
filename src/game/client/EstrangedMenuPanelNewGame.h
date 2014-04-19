#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "EstrangedMenuPanel.h"
#include "igamesystem.h"

class EstrangedMenuPanelNewGame : public EstrangedMenuPanel
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuPanelNewGame, EstrangedMenuPanel);

	public:
		EstrangedMenuPanelNewGame(vgui::Panel* parent);
		virtual void OnCommand(const char*);
};