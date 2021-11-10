#include "cbase.h" 
#include "vgui/ISurface.h" 
#include "c_baseplayer.h" 
#include "c_basehlplayer.h"
#include "hud_stamina.h" 


 
DECLARE_HUDELEMENT(CHudStamina);
 
//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudStamina::CHudStamina(const char* pElementName) : EstrangedHUDBase(pElementName, "HudStamina", 8.0f, 8.0f, 3.3f)
{
}

void CHudStamina::Init()
{
	m_StaminaTexture = vgui::surface()->CreateNewTextureID();
	vgui::surface()->DrawSetTextureFile(m_StaminaTexture, "vgui/hud_stamina", true, false);
}
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudStamina::OnThink()
{
	BaseClass::OnThink();

	C_BaseHLPlayer* local = (C_BaseHLPlayer*)C_BasePlayer::GetLocalPlayer();

	if (local)
	{
		SetPercentFilled(local->m_HL2Local.m_flSuitPower);
	}
}

bool CHudStamina::ShouldDraw()
{
	ConVarRef stamina_enabled("ae_hud_enabled");
	return BaseClass::ShouldDraw() && stamina_enabled.GetBool();
}

void CHudStamina::DrawLineBackground()
{
	vgui::surface()->DrawSetColor(m_cBackgroundColour);

	FillRectangle(3.0f, 1.0f, 2.0f, 6.0f);
}

void CHudStamina::DrawLine()
{
	vgui::surface()->DrawSetColor(m_cStaminaColour);

	FillRectangle(3.0f, 1.0f, 2.0f, 6.0f, GetPercentFilled() / 100);

	//FillRectangle(3.0f, 1.0f, 2.0f, 6.0f, GetSectionHeight(0.0f, 100.0f));
}

void CHudStamina::DrawLineForeground()
{
	if (!vgui::surface()->IsTextureIDValid(m_StaminaTexture)) return;
	// Foreground gradient
	vgui::surface()->DrawSetColor(255, 255, 255, 255);
	vgui::surface()->DrawSetTexture(m_StaminaTexture);
	vgui::surface()->DrawTexturedRect(0, 0, GetWide(), GetTall());
}
 
//------------------------------------------------------------------------
// Purpose: draws the Line
//------------------------------------------------------------------------
void CHudStamina::Paint()
{
	BaseClass::Paint();

	DrawLineBackground();
	DrawLine();
	DrawLineForeground();
}