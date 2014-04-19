#include "cbase.h" 
#include "vgui/ISurface.h" 
#include "c_baseplayer.h" 
#include "hud_hp.h" 
 
DECLARE_HUDELEMENT(CHudHP);
 
//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudHP::CHudHP(const char* pElementName) : EstrangedHUDBase(pElementName, "HudHP", 8.0f, 8.0f, 3.3f)
{
}

void CHudHP::Init()
{
	m_HPTexture = vgui::surface()->CreateNewTextureID();
	vgui::surface()->DrawSetTextureFile(m_HPTexture, "vgui/hud_hp", true, false);
}
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudHP::OnThink()
{
	BaseClass::OnThink();

	C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();

	if (local)
	{
		SetPercentFilled(local->GetHealth());
	}
}

bool CHudHP::ShouldDraw()
{
	ConVarRef hp_enabled("ae_hud_enabled");
	return BaseClass::ShouldDraw() && hp_enabled.GetBool();
}

void CHudHP::DrawCrossBackground()
{
	vgui::surface()->DrawSetColor(m_cBackgroundColour);

	// Top segment
	FillRectangle(3.0f, 1.0f, 2.0f, 2.0f);

	// Middle segment
	FillRectangle(1.0f, 3.0f, 6.0f, 2.0f);

	// Bottom segment
	FillRectangle(3.0f, 5.0f, 2.0f, 2.0f);
}

void CHudHP::DrawCross()
{
	vgui::surface()->DrawSetColor(m_cCrossColour);

	// Top segment
	FillRectangle(3.0f, 1.0f, 2.0f, 2.0f, GetSectionHeight(66.6f, 100.0f));

	// Middle segment
	FillRectangle(1.0f, 3.0f, 6.0f, 2.0f, GetSectionHeight(33.3f, 66.6f));

	// Bottom segment
	FillRectangle(3.0f, 5.0f, 2.0f, 2.0f, GetSectionHeight(0.0f, 33.3f));
}

void CHudHP::DrawCrossForeground()
{
	if (!vgui::surface()->IsTextureIDValid(m_HPTexture)) return;
	// Foreground gradient
	vgui::surface()->DrawSetColor(255, 255, 255, 255);
	vgui::surface()->DrawSetTexture(m_HPTexture);
	vgui::surface()->DrawTexturedRect(0, 0, GetWide(), GetTall());
}
 
//------------------------------------------------------------------------
// Purpose: draws the cross
//------------------------------------------------------------------------
void CHudHP::Paint()
{
	BaseClass::Paint();

	DrawCrossBackground();
	DrawCross();
	DrawCrossForeground();
}