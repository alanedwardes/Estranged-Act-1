#include "cbase.h"
#include "hudelement.h"
#include "vgui_controls/Panel.h"

#define BLANK "0 0 0 0"

class EstrangedHUDBase : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(EstrangedHUDBase, vgui::Panel);

	EstrangedHUDBase(const char *pElementName, const char *pHudElementName, float pUnitWide, float pUnitTall, int pNumSections);

protected:
	virtual	void	FillRectangle(float x, float y, float w, float h, float fillHeight = 1.0f);
	virtual	void	SetPercentFilled(int pPercent);
	virtual	void	SetMultiplier(int multiplier);
	virtual	float	GetSectionHeight(float lower, float upper);
	virtual	void	DrawMultiplier();
private:
	CPanelAnimationVar(int, m_iMultiplierX, "muliplierxpos", "0");
	CPanelAnimationVar(int, m_iMultiplierY, "muliplierypos", "0");
	bool				m_bLowPercentageTriggered;
	int					m_iPctFilled;
	int					m_iMultiplier;
	int					m_iNumSectons;
	float				m_flUnitWide;
	float				m_flUnitTall;
	vgui::HFont			m_MultiplierFont;
	wchar_t				m_wMultiplierText[10];
	const char			*m_cHudElementName;
};