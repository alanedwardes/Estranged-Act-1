#include "cbase.h"
#include "EstrangedHUDBase.h"
#include "vgui/ISurface.h"
#include "vgui/IScheme.h"
#include "vgui_controls/AnimationController.h"
#include "iclientmode.h"
#include "fmtstr.h"

EstrangedHUDBase::EstrangedHUDBase(const char *pElementName, const char *pHudElementName, float pUnitWide, float pUnitTall, int pNumSections) :
		CHudElement(pElementName), BaseClass(NULL, pHudElementName)
{
	SetParent(g_pClientMode->GetViewport());
	
	SetPaintBackgroundEnabled(false);

	vgui::HScheme scheme = vgui::scheme()->GetScheme("ClientScheme");
	m_MultiplierFont = vgui::scheme()->GetIScheme(scheme)->GetFont("EstrangedHUDMultiplier");

	m_flUnitWide = pUnitWide;
	m_flUnitTall = pUnitTall;
	m_iNumSectons = pNumSections;
	m_cHudElementName = pHudElementName;

	SetHiddenBits(HIDEHUD_INVEHICLE);
}

void EstrangedHUDBase::DrawMultiplier()
{
	if (m_iMultiplier <= 0) return;

	if (m_iMultiplier < 10)
	{
		V_swprintf_safe(m_wMultiplierText, L"x%i", m_iMultiplier);
	}
	else
	{
		V_swprintf_safe(m_wMultiplierText, L"%i", m_iMultiplier);
	}

	int textWidth, textHeight;
	vgui::surface()->GetTextSize(m_MultiplierFont, m_wMultiplierText, textWidth, textHeight);
	vgui::surface()->DrawSetTextColor(Color(255, 255, 255, 200));
	vgui::surface()->DrawSetTextFont(m_MultiplierFont);
	vgui::surface()->DrawSetTextPos(vgui::scheme()->GetProportionalScaledValue(m_iMultiplierX) - textWidth / 2, vgui::scheme()->GetProportionalScaledValue(m_iMultiplierY) - textHeight / 2);
	vgui::surface()->DrawPrintText(m_wMultiplierText, wcslen(m_wMultiplierText), vgui::FONT_DRAW_NONADDITIVE);
}

void EstrangedHUDBase::FillRectangle(float x, float y, float w, float h, float fillHeight)
{
	const float sw = GetWide() / m_flUnitWide;
	const float sh = GetTall() / m_flUnitTall;

	fillHeight = min(fillHeight, 1.0f);

	if (fillHeight <= 0.0f) return;

	float x1 = floor(x * sw);
	float y1 = floor(y * sh);

	w *= sw;
	h *= sh;

	if (fillHeight < 1.0f)
	{
		float newHeight = h * fillHeight;
		y1 += h - newHeight;
		h = newHeight;
	}

	vgui::surface()->DrawFilledRect(x1, y1, ceil(x1 + w), ceil(y1 + h));
}

float EstrangedHUDBase::GetSectionHeight(float lower, float upper)
{
	return ((min(max(lower, m_iPctFilled), upper) - lower) * float(m_iNumSectons)) / 100.0f;
}

void EstrangedHUDBase::SetMultiplier(int multipler)
{
	m_iMultiplier = multipler;
}

void EstrangedHUDBase::SetPercentFilled( int pPercent )
{
	vgui::AnimationController *animationController = g_pClientMode->GetViewportAnimationController();

	CFmtStr animationName = CFmtStr();
	if (pPercent < 30)
	{
		if (!m_bLowPercentageTriggered)
		{
			animationName.sprintf("%s%s", m_cHudElementName, "Low");
			m_bLowPercentageTriggered = true;
		}
	}
	else if (m_iPctFilled > pPercent || m_iPctFilled < pPercent)
	{
		animationName.sprintf("%s%s", m_cHudElementName, "Change");
	}
	else
	{
		m_bLowPercentageTriggered = false;
	}

	if (strlen(animationName) > 0 && animationController->GetAnimationSequenceLength(animationName.Access()) > 0)
	{
		animationController->StartAnimationSequence(animationName.Access());
	}

	m_iPctFilled = pPercent;
}