#include "cbase.h"
#include "EstrangedMenuItem.h"
#include <vgui/IVGui.h>
#include <vgui/ISurface.h>

#define TRANSPARENT Color(0, 0, 0, 0)

EstrangedMenuItem::EstrangedMenuItem(Panel *parent, const char *label, const char *command, Panel *target):
	vgui::Label(parent, "EstrangedMenuItem", label)
{
	isItemActive = false;
	hoverBorder = new vgui::Panel(this);
	hoverBorder->SetMouseInputEnabled(false);
	hoverBorder->SetKeyBoardInputEnabled(false);
}

void EstrangedMenuItem::SetItemActive(bool state)
{
	isItemActive = state;
}

bool EstrangedMenuItem::IsItemActive()
{
	return isItemActive;
}

void EstrangedMenuItem::PaintBackground()
{
	if (IsItemActive())
	{
		hoverBorder->SetBgColor(lightRed);
		SetBgColor(lightRedTrans);
	}
	else
	{
		hoverBorder->SetBgColor(TRANSPARENT);
		SetBgColor(TRANSPARENT);
	}

	BaseClass::PaintBackground();
}

void EstrangedMenuItem::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	SetFont(pScheme->GetFont("EstrangedMenuItem"));
	lightRed = pScheme->GetColor("LightRed", TRANSPARENT);
	lightRedTrans = pScheme->GetColor("LightRedTrans", TRANSPARENT);

	BaseClass::ApplySchemeSettings(pScheme);
}

void EstrangedMenuItem::PerformLayout()
{
	// Size to the contents first - fixes
	// any alignment issues if we just
	// switched resolution to a different
	// font size
	SizeToContents();

	// If we have a parent, scale to it
	if (GetParent())
	{
		SetWide(GetParent()->GetWide());
		SetTall(GetParent()->GetTall() / 6);
	}

	BaseClass::PerformLayout();

	hoverBorder->SetPos(0, 0);
	hoverBorder->SetSize(GetWide(), GetTall() / 16);
}

void EstrangedMenuItem::OnCursorEntered()
{
	vgui::surface()->PlaySound( "ui/buttonrollover.wav" );
	SetItemActive(true);
}

void EstrangedMenuItem::OnCursorExited()
{
	SetItemActive(false);
}