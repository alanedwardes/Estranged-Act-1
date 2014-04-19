#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "EstrangedMenuPanel.h"

class EstrangedMenuPanelOptions : public EstrangedMenuPanel
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuPanelOptions, EstrangedMenuPanel);

	public:
		EstrangedMenuPanelOptions(vgui::Panel* parent);
		virtual void OnCommand(const char*);
};