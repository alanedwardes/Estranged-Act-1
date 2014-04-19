#include <vgui/IVGui.h>
#include <vgui_controls/Label.h>
#include "EstrangedMenuItem.h"
#include <vgui_controls/ImagePanel.h>

class EstrangedMenuButton : public EstrangedMenuItem
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuButton, EstrangedMenuItem);

	public:
		EstrangedMenuButton(Panel *parent, const char *label, const char *command, Panel *target, const char *iconPath);

		virtual void OnMouseReleased(vgui::MouseCode);
		virtual void OnKeyCodePressed(vgui::KeyCode);
		virtual void PerformLayout();

	private:
		vgui::ImagePanel	*icon;
		vgui::Panel			*signalTarget;
		const char			*signalCommand;
};