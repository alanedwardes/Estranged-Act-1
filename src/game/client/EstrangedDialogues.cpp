#include "cbase.h"
#include "EstrangedDialogues.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Frame.h>
#include <vgui/IInput.h>
#include "IEstrangedDialogues.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define WIDTH 350
#define HEIGHT 140
#define GUTTER 10

CEstrangedDialogues *g_pEstrangedDialouges = NULL;

CON_COMMAND(ae_alert, "Show an alert with the specified <title> and <text>")
{
	CEstrangedDialogues *dialoues = g_pEstrangedDialouges;
	dialoues->ShowAlert(args[1], args[2]);
}

CEstrangedDialogues::CEstrangedDialogues(vgui::VPANEL parent) : vgui::Frame(NULL, "EstrangedDialouges")
{
	g_pEstrangedDialouges = this;
	SetSizeable(false);
	SetMoveable(false);

	m_pTextLabel = new vgui::Label(this, "DialougeLabel", "");
	m_pTextLabel->SetCenterWrap(true);

	m_pOKButton = new vgui::Button(this, "DialougeButton", "#GameUI_OK", this, "Close");
	m_pOKButton->SetCenterWrap(true);
}

void CEstrangedDialogues::PerformLayout()
{
	BaseClass::PerformLayout();

	SetSize(WIDTH, HEIGHT);
	MoveToCenterOfScreen();

	int runningY = 40;

	m_pTextLabel->SetSize(WIDTH - GUTTER * 2, 50);
	m_pTextLabel->SetPos(GUTTER, runningY);

	runningY += GUTTER + m_pTextLabel->GetTall();

	m_pOKButton->SetWide(WIDTH / 3);
	m_pOKButton->SetPos(WIDTH / 2 - m_pOKButton->GetWide() / 2, runningY);
	m_pOKButton->SetTextInset(0, 0);
}

void CEstrangedDialogues::Show()
{
	PerformLayout();
	SetVisible(true);
	Activate();
	MoveToFront();

	vgui::input()->SetAppModalSurface(GetVPanel());
}

void CEstrangedDialogues::ShowAlert(const char *title, const char *text)
{
	SetTitle(title, false);
	m_pTextLabel->SetText(text);
	Show();
}

class CEstrangedDialoguesInterface : public IEstrangedDialogues
{
	private:
		CEstrangedDialogues *EstrangedDialogues;
	public:
		CEstrangedDialoguesInterface()
		{
			EstrangedDialogues = NULL;
		}
		void Create(vgui::VPANEL parent)
		{
			EstrangedDialogues = new CEstrangedDialogues(parent);
		}
		void Destroy()
		{
			if (EstrangedDialogues)
			{
				EstrangedDialogues->SetParent( (vgui::Panel *)NULL);
				delete EstrangedDialogues;
			}
		}
};

static CEstrangedDialoguesInterface g_estrangeddialogues;
IEstrangedDialogues* estrangeddialogues = (IEstrangedDialogues*)&g_estrangeddialogues;