#include "cbase.h"

#ifndef ESTRANGED_DIALOGUES_H
#define ESTRANGED_DIALOGUES_H

#ifdef _WIN32
#pragma once
#endif

class CEstrangedDialogues : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CEstrangedDialogues, vgui::Frame);

	CEstrangedDialogues(vgui::VPANEL parent);
	~CEstrangedDialogues(){};

	public:
		virtual void	ShowAlert(const char *title, const char* text);
		virtual void	PerformLayout();

	private:
		virtual void	Show();
		vgui::Label		*m_pTextLabel;
		vgui::Button	*m_pOKButton;
};

extern CEstrangedDialogues *g_pEstrangedDialouges;

#endif