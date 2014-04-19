#include <vgui/IVGui.h>
#include "EstrangedMenuItem.h"

class EstrangedMenuCheckButton : public EstrangedMenuItem
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuCheckButton, EstrangedMenuItem);

	public:
		EstrangedMenuCheckButton(Panel *parent, const char *label, const char *command, Panel *target, bool isChecked);
		
		virtual void PerformLayout();
		virtual void OnMousePressed(vgui::MouseCode);

	private:
		vgui::CheckButton	*check;
};