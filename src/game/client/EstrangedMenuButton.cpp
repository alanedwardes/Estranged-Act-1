#include "cbase.h"
#include "EstrangedMenuButton.h"
#include <vgui/IVGui.h>
#include <vgui/ISurface.h>
#include <vgui_controls/Label.h>

EstrangedMenuButton::EstrangedMenuButton(Panel *parent, const char *label, const char *command, Panel *target, const char *iconPath):
	EstrangedMenuItem(parent, label, command, target)
{
	icon = new vgui::ImagePanel(this, "EstrangedMenuItemIcon");
	icon->SetImage(iconPath);
	icon->SetShouldScaleImage(true);
	icon->SetAlpha(128);
	icon->SetMouseInputEnabled(false);
	icon->SetKeyBoardInputEnabled(false);

	signalTarget = target;
	signalCommand = command;
}

void EstrangedMenuButton::OnMouseReleased(vgui::MouseCode code)
{
	BaseClass::OnKeyCodePressed(code);
	
	vgui::surface()->PlaySound("ui/buttonclick.wav");

	PostMessage(signalTarget, new KeyValues("command", "command", signalCommand));
}

void EstrangedMenuButton::OnKeyCodePressed(vgui::KeyCode code)
{
	BaseClass::OnKeyCodePressed(code);

	if (code == KEY_ENTER || code == JOYSTICK_FIRST || code == KEY_XBUTTON_B)
	{
		vgui::surface()->PlaySound("ui/buttonclick.wav");

		PostMessage(signalTarget, new KeyValues("command", "command", signalCommand));
	}
}

void EstrangedMenuButton::PerformLayout()
{
	BaseClass::PerformLayout();

	SetTextInset(GetTall(), 0);

	icon->SetPos(GetTall() / 4, GetTall() / 4);

	icon->SetSize(GetTall() / 2, GetTall() / 2);
}