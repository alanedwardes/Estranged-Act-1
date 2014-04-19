#include <vgui/IVGui.h>
#include "EstrangedMenuItem.h"

class EstrangedMenuSlider : public EstrangedMenuItem
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuSlider, EstrangedMenuItem);

	public:
		EstrangedMenuSlider(Panel *parent, const char *label, const char *command, Panel *target, float value);

		virtual void PerformLayout();

	private:
		vgui::Slider	*slide;
};