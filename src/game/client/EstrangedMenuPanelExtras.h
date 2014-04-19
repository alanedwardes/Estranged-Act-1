#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "EstrangedMenuPanel.h"
#include "igamesystem.h"

class EstrangedMenuPanelExtras : public EstrangedMenuPanel
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuPanelExtras, EstrangedMenuPanel);

	public:
		EstrangedMenuPanelExtras(vgui::Panel* parent);
		virtual void OnCommand(const char*);
		virtual void OpenAddress(wchar_t*);
};