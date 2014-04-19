#include "cbase.h"
#include "EstrangedMenuCheckButton.h"
#include <vgui_controls/CheckButton.h>
#include <vgui/IVGui.h>
#include <vgui/ISurface.h>

#define TRANSPARENT Color(0, 0, 0, 0)

EstrangedMenuCheckButton::EstrangedMenuCheckButton(Panel *parent, const char *label, const char *command, Panel *target, bool checked):
	EstrangedMenuItem(parent, label, command, target)
{
	check = new vgui::CheckButton(this, "CheckButton", "");
	check->SetSelected(checked);
	check->SetKeyBoardInputEnabled(false);
	check->SetMouseInputEnabled(false);
}

void EstrangedMenuCheckButton::PerformLayout()
{
	BaseClass::PerformLayout();

	check->SetSize(GetTall(), GetTall());
	SetTextInset(GetTall() / 1.5, 0);
}

void EstrangedMenuCheckButton::OnMousePressed(vgui::MouseCode code)
{
	BaseClass::OnMousePressed(code);

	check->SetSelected(!check->IsSelected());
}