#include "cbase.h"
#include <vgui_controls/Frame.h>

#ifndef ESTRANGED_FEEDBACK_H
#define ESTRANGED_FEEDBACK_H

#ifdef _WIN32
#pragma once
#endif

class CReportFeedback : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CReportFeedback, vgui::Frame);

	CReportFeedback(vgui::VPANEL parent);
	~CReportFeedback(){};

	public:
		virtual void Show(bool);

	protected:
		virtual void OnCommand(const char*);
		virtual void PerformLayout(void);
		virtual void OnMessage(const KeyValues *params, vgui::VPANEL fromPanel);

	private:
		void BuildControls(void);
		void AddTopFeedbackLabel(void);
		void AddTypeOfFeedbackControl(void);
		void AddFeedbackCommentControl(void);
		void AddCharacterLimitControl(void);
		void AddEmailControl(void);
		void AddNameControl(void);
		void UpdateCharacterLimitControl(void);
		void AddButtons(void);
		void SubmitFeedback(void);
		void SetControlsEnabled(bool);
		bool IsInputValid();
		int GetSteamID(void);
		int textureID;
		vgui::Label		*topFeedbackLabel;
		vgui::ComboBox	*typeOfFeedback;
		vgui::Label		*typeOfFeedbackLabel;
		vgui::Label		*commentLabel;
		vgui::Label		*characterLimitLabel;
		vgui::TextEntry	*commentControl;
		vgui::Label		*emailLabel;
		vgui::TextEntry	*emailControl;
		vgui::Label		*nameLabel;
		vgui::TextEntry	*nameControl;
		vgui::Button	*saveButton;
		vgui::Button	*cancelButton;
};

extern CReportFeedback *g_pReportFeedback;

#endif