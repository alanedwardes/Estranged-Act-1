#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Label.h>
#include "MainMenuButtonPanel.h"
#include "EstrangedMenuButton.h"
#include "ienginevgui.h"

MenuMainButtonPanel::MenuMainButtonPanel(Panel *parent) : vgui::Panel(parent) {}

void MenuMainButtonPanel::AddButton(const char *text, const char *command)
{
	m_Buttons.AddToTail(new EstrangedMenuButton(this, text, command, GetParent()));
}

void MenuMainButtonPanel::PerformLayout()
{
	if (GetTall() <= 0 || GetChildCount() <= 0) return;

	int buttonHeight = GetTall() / max(5, m_Buttons.Count());

	for (int i = 0; i < m_Buttons.Count(); i++)
	{
		vgui::Panel *child = m_Buttons.Element(i);
		child->SetPos(0, i * buttonHeight);
		child->SetSize(GetWide(), buttonHeight);
	}
}

void MenuMainButtonPanel::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	for (int i = 0; i < GetChildCount(); i++)
	{
		vgui::Panel *child = GetChild(i);
		vgui::Label *button = dynamic_cast<vgui::Label *>(child);
		button->SetFont(pScheme->GetFont("EstrangedMenuButton"));
		child->SetWide(GetWide());
	}
}