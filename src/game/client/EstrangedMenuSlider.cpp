#include "cbase.h"
#include "EstrangedMenuSlider.h"
#include <vgui/IVGui.h>
#include <vgui/ISurface.h>
#include <vgui_controls/Slider.h>

EstrangedMenuSlider::EstrangedMenuSlider(Panel *parent, const char *label, const char *command, Panel *target, float value):
	EstrangedMenuItem(parent, label, command, target)
{
	SetText("");
	slide = new vgui::Slider(this, "EstrangedMenuSlider");
	slide->SetRange(0, 100);
	slide->SetValue(value);
	slide->SetTickCaptions("Hi", "Lo");
}

void EstrangedMenuSlider::PerformLayout()
{
	slide->SetSize(GetWide(), GetTall());
}