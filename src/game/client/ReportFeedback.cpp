#include "cbase.h"
#include "ReportFeedback.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/Button.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include "steam/steam_api.h"
#include "EstrangedMenuUtil.h"
#include "c_baseplayer.h"
#include "IEstrangedDialogues.h"
#include "IReportFeedback.h"
#include "estrangedplayerinformation.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define COMMENT_MAX_LEN 2048
#define EMAIL_MAX_LEN 128
#define NAME_MAX_LEN 128
#define TALL 400
#define WIDE 500
#define FEEDBACK_GENERAL "GENERAL"
#define FEEDBACK_STUCK "STUCK"
#define FEEDBACK_UNREALISTIC "UNREALISTIC"
#define FEEDBACK_CONFUSED "CONFUSED"
#define FEEDBACK_BUG "BUG"

CReportFeedback *g_pReportFeedback = NULL;

CON_COMMAND(ae_showfeedbackpanel, "Set the feedback panel's visibility to <bool>")
{
	CReportFeedback *feedback = g_pReportFeedback;
	bool visible = atoi(args[1]);
	feedback->Show(visible);
}

CReportFeedback::CReportFeedback(vgui::VPANEL parent): BaseClass(NULL, "ReportFeedback")
{
	g_pReportFeedback = this;

	BuildControls();

	SetTitle("#Estranged_ReportFeedbackTitle", true);
	SetSize(WIDE, TALL);
}

void CReportFeedback::Show(bool visible)
{
	SetVisible(visible);

	if (visible)
	{
		// Add default values
		commentControl->SetText("#Estranged_ReportFeedbackCommentExample");
		typeOfFeedback->ActivateItem(typeOfFeedback->GetItemIDFromRow(0));
		Activate();
		MoveToCenterOfScreen();
	}
}

int CReportFeedback::GetSteamID(void)
{
	ISteamUser* steam_user = steamapicontext->SteamUser();
	if (steam_user > 0 && steam_user->BLoggedOn())
	{
		return steam_user->GetSteamID().ConvertToUint64() & 0x00000000ffffffff;
	}

	return 0;
}

void CReportFeedback::BuildControls()
{
	AddTopFeedbackLabel();
	AddNameControl();
	AddEmailControl();
	AddTypeOfFeedbackControl();
	AddFeedbackCommentControl();
	AddCharacterLimitControl();
	AddButtons();
}

void CReportFeedback::AddNameControl()
{
	nameLabel = new vgui::Label(this, "NameLabel", "#Estranged_ReportFeedbackNameLabel");
	nameControl = new vgui::TextEntry(this, "NameControl");
	nameControl->SetTabPosition(1);
	nameControl->SetMaximumCharCount(NAME_MAX_LEN);
}

void CReportFeedback::AddEmailControl()
{
	emailLabel = new vgui::Label(this, "EmailLabel", "#Estranged_ReportFeedbackEmailLabel");
	emailControl = new vgui::TextEntry(this, "EmailControl");
	emailControl->SetTabPosition(2);
	emailControl->SetMaximumCharCount(EMAIL_MAX_LEN);
}

void CReportFeedback::AddTopFeedbackLabel()
{
	topFeedbackLabel = new vgui::Label(this, "TypeOfFeedbackLabel", "#Estranged_ReportFeedbackTopLabel");
	topFeedbackLabel->SetWrap(true);
}

void CReportFeedback::AddTypeOfFeedbackControl()
{
	typeOfFeedback = new vgui::ComboBox(this, "TypeOfFeedback", 0, false);

	// This is very (memory) inefficient use of KeyValues but it's relatively small amounts of data, so it doesn't matter.
	typeOfFeedback->AddItem("#GameUI_Select", new KeyValues("type", "type", ""));
	typeOfFeedback->AddItem("#Estranged_ReportFeedbackGeneral", new KeyValues("type", "type", FEEDBACK_GENERAL));
	typeOfFeedback->AddItem("#Estranged_ReportFeedbackStuck", new KeyValues("type", "type", FEEDBACK_STUCK));
	typeOfFeedback->AddItem("#Estranged_ReportFeedbackUnrealistic", new KeyValues("type", "type", FEEDBACK_UNREALISTIC));
	typeOfFeedback->AddItem("#Estranged_ReportFeedbackConfused", new KeyValues("type", "type", FEEDBACK_CONFUSED));
	typeOfFeedback->AddItem("#Estranged_ReportFeedbackBug", new KeyValues("type", "type", FEEDBACK_BUG));
	
	typeOfFeedback->ActivateItem(typeOfFeedback->GetItemIDFromRow(0));
	typeOfFeedback->SetTabPosition(3);
	typeOfFeedbackLabel = new vgui::Label(this, "TypeOfFeedbackLabel", "#Estranged_ReportFeedbackType");
}

void CReportFeedback::AddFeedbackCommentControl()
{
	commentLabel = new vgui::Label(this, "CommentLabel", "#Estranged_ReportFeedbackComment");

	commentControl = new vgui::TextEntry(this, "CommentControl");
	commentControl->SetMultiline(true);
	commentControl->SetVerticalScrollbar(true);
	commentControl->SetCatchEnterKey(true);
	commentControl->SetMaximumCharCount(COMMENT_MAX_LEN);
	commentControl->SetTabPosition(4);
}

void CReportFeedback::UpdateCharacterLimitControl()
{
	char characterLimitLabelBuffer[32];
	Q_snprintf(characterLimitLabelBuffer, sizeof(characterLimitLabelBuffer), "%i / %i", commentControl->GetTextLength(), COMMENT_MAX_LEN);
	characterLimitLabel->SetText(characterLimitLabelBuffer);
}

void CReportFeedback::AddCharacterLimitControl()
{
	characterLimitLabel = new vgui::Label(this, "CharacterLimitLabel", "");
}

void CReportFeedback::AddButtons()
{
	saveButton = new vgui::Button(this, "SaveButton", "#GameUI_Submit", this, "save");
	cancelButton = new vgui::Button(this, "CancelButton", "#GameUI_Cancel", this, "cancel");
}

void CReportFeedback::PerformLayout()
{
	BaseClass::PerformLayout();

	SetTall(min(ScreenHeight(), GetTall()));
	SetWide(min(ScreenWidth(), GetWide()));

	int top = 40;
	int gutter = 10;
	int wide, tall;
	int labelWidth = 150;

	topFeedbackLabel->SetPos(gutter, top);
	topFeedbackLabel->SetWide(GetWide() - gutter * 2);
	topFeedbackLabel->InvalidateLayout(true);
	// Invalidate again, for some reason the first didn't quite take...
	topFeedbackLabel->InvalidateLayout(true);
	topFeedbackLabel->GetContentSize(wide, tall);
	topFeedbackLabel->SetTall(tall);

	top += tall + gutter;

	nameLabel->SetPos(gutter, top);
	nameLabel->SizeToContents();

	nameControl->SetPos(labelWidth, top);
	nameControl->SetWide(GetWide() - labelWidth - gutter);
	nameLabel->SetTall(typeOfFeedback->GetTall());

	top += nameLabel->GetTall() + gutter;

	emailLabel->SetPos(gutter, top);
	emailLabel->SizeToContents();

	emailControl->SetPos(labelWidth, top);
	emailControl->SetWide(GetWide() - labelWidth - gutter);
	emailLabel->SetTall(emailControl->GetTall());

	top += nameLabel->GetTall() + gutter;

	typeOfFeedbackLabel->SetPos(gutter, top);
	typeOfFeedbackLabel->SizeToContents();

	typeOfFeedback->SetPos(labelWidth, top);
	typeOfFeedback->SetWide(GetWide() - labelWidth - gutter);
	typeOfFeedbackLabel->SetTall(typeOfFeedback->GetTall());

	top += typeOfFeedback->GetTall() + gutter;

	commentLabel->SetPos(gutter, top);
	commentLabel->SizeToContents();

	top += commentLabel->GetTall() + gutter;

	commentControl->SetPos(gutter, top);
	commentControl->SetWide(GetWide() - gutter * 2);

	cancelButton->SetWide(75);
	cancelButton->SetPos(GetWide() - gutter - cancelButton->GetWide(), GetTall() - gutter * 2 - cancelButton->GetTall());

	saveButton->SetWide(75);
	// sorry person who has to read this
	saveButton->SetPos(GetWide() - gutter - cancelButton->GetWide() - saveButton->GetWide() - gutter, GetTall() - gutter * 2 - saveButton->GetTall());

	characterLimitLabel->SizeToContents();
	characterLimitLabel->SetWide(100);
	characterLimitLabel->SetPos(gutter, GetTall() - gutter * 2 - saveButton->GetTall());

	commentControl->SetTall(GetTall() - cancelButton->GetTall() - gutter * 3 - top);
}

void CReportFeedback::OnMessage(const KeyValues *params, vgui::VPANEL fromPanel)
{
	BaseClass::OnMessage(params, fromPanel);

	UpdateCharacterLimitControl();
}

void CReportFeedback::OnCommand(const char *command)
{
	BaseClass::OnCommand(command);

	if (!Q_strcmp(command, "save"))
	{
		if (IsInputValid())
		{
			SubmitFeedback();
		}
	}

	if (!Q_strcmp(command, "cancel") || !Q_strcmp(command, "Close"))
	{
		Show(false);
	}
}

void CReportFeedback::SetControlsEnabled(bool state)
{
	saveButton->SetEnabled(state);
	cancelButton->SetEnabled(state);
	commentControl->SetEnabled(state);
	typeOfFeedback->SetEnabled(state);
	SetCloseButtonVisible(state);
}

bool CReportFeedback::IsInputValid()
{
	char commentBuffer[COMMENT_MAX_LEN];
	commentControl->GetText(commentBuffer, sizeof(commentBuffer));

	char exampleCommentBuffer[64];
	g_pVGuiLocalize->ConvertUnicodeToANSI(g_pVGuiLocalize->Find("#Estranged_ReportFeedbackCommentExample"), exampleCommentBuffer, sizeof(exampleCommentBuffer));

	KeyValues *kv = typeOfFeedback->GetActiveItemUserData();

	// If you didn't pick a type
	if (!Q_strcmp(kv->GetString("type"), ""))
	{
		g_pEstrangedDialouges->ShowAlert("#Estranged_ReportFeedbackError", "#Estranged_ReportFeedbackTypeError");
		return false;
	}
	// Or enter a comment...
	if (!Q_strcmp(commentBuffer, "") || !Q_strcmp(commentBuffer, exampleCommentBuffer))
	{
		g_pEstrangedDialouges->ShowAlert("#Estranged_ReportFeedbackError", "#Estranged_ReportFeedbackCommentError");
		return false;
	}

	return true;
}

void CReportFeedback::SubmitFeedback()
{
	char nameBuffer[NAME_MAX_LEN];
	nameControl->GetText(nameBuffer, sizeof(nameBuffer));

	char emailBuffer[EMAIL_MAX_LEN];
	emailControl->GetText(emailBuffer, sizeof(emailBuffer));

	char commentBuffer[COMMENT_MAX_LEN];
	commentControl->GetText(commentBuffer, sizeof(commentBuffer));

	KeyValues *kv = typeOfFeedback->GetActiveItemUserData();
	const char *typeBuffer = kv->GetString("type");

	ISteamHTTP *steamHTTP = steamapicontext->SteamHTTP();
	HTTPRequestHandle httpRequest = steamHTTP->CreateHTTPRequest(k_EHTTPMethodPOST, ESTRANGED_STATISTICS_ENDPOINT);
	AppendPlayerInformation(steamHTTP, httpRequest, steamapicontext, C_BasePlayer::GetLocalPlayer(), true);
	steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "name", nameBuffer);
	steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "email", emailBuffer);
	steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "comment", commentBuffer);
	steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "type", typeBuffer);
	steamHTTP->SetHTTPRequestGetOrPostParameter(httpRequest, "service", "feedback");
	steamHTTP->SendHTTPRequest(httpRequest, NULL);

	g_pEstrangedDialouges->ShowAlert("#EstrangedFeedbackThanksTitle", "#EstrangedFeedbackThanksBody");

	SetControlsEnabled(true);
	Show(false);
	kv->deleteThis();
	EstrangedMenuUtil::CloseGameFrames(GetVPanel());
}

class CReportFeedbackInterface : public IReportFeedback
{
	private:
		CReportFeedback *ReportFeedback;
	public:
		CReportFeedbackInterface()
		{
			ReportFeedback = NULL;
		}
		void Create(vgui::VPANEL parent)
		{
			ReportFeedback = new CReportFeedback(parent);
		}
		void Destroy()
		{
			if (ReportFeedback)
			{
				ReportFeedback->SetParent( (vgui::Panel *)NULL);
				delete ReportFeedback;
			}
		}
};

static CReportFeedbackInterface g_ReportFeedback;
IReportFeedback* reportfeedback = (IReportFeedback*)&g_ReportFeedback;