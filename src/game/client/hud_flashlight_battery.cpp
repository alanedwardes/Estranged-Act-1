#include "cbase.h" 
#include "c_baseplayer.h" 
#include "hud_flashlight_battery.h" 
#include "vgui/ISurface.h"
#include "hl2_player_shared.h"

DECLARE_HUDELEMENT(CHudFlashlightBattery);

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudFlashlightBattery::CHudFlashlightBattery(const char* pElementName) : EstrangedHUDBase(pElementName, "HudFlashlightBattery", 16, 16, 5)
{
}

void CHudFlashlightBattery::Init()
{
	m_FlashlightTexture = vgui::surface()->CreateNewTextureID();
	vgui::surface()->DrawSetTextureFile(m_FlashlightTexture, "vgui/hud_flashlight_battery", true, false);
}

void CHudFlashlightBattery::DrawBatteryForeground()
{
	if (!vgui::surface()->IsTextureIDValid(m_FlashlightTexture)) return;
	// Foreground gradient
	vgui::surface()->DrawSetColor(255, 255, 255, 255);
	vgui::surface()->DrawSetTexture(m_FlashlightTexture);
	vgui::surface()->DrawTexturedRect(0, 0, GetWide(), GetTall());
}

//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudFlashlightBattery::OnThink()
{
	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	
	if (!pPlayer) return;

	m_bPlayerHasFlashlight = pPlayer->m_HL2Local.m_bHasFlashlight;
	m_bFlashlightIsActive = pPlayer->IsEffectActive(EF_DIMLIGHT);

	SetMultiplier(pPlayer->m_HL2Local.m_iFlashlightBatteries);
	SetPercentFilled(pPlayer->m_HL2Local.m_flFlashBattery);
}

bool CHudFlashlightBattery::ShouldDraw()
{
	ConVarRef battery_enabled("ae_hud_enabled");
	return BaseClass::ShouldDraw() && battery_enabled.GetBool();
}

void CHudFlashlightBattery::DrawBatteryBackground()
{
	vgui::surface()->DrawSetColor(m_cBackgroundColour);

	// Top segment
	FillRectangle(5.0f, 2.0f, 6.0f, 2.0f);

	// Bottom segment
	FillRectangle(6.0f, 6.0f, 4.0f, 8.0f);
}

void CHudFlashlightBattery::DrawBattery()
{
	if (m_bFlashlightIsActive)
	{
		vgui::surface()->DrawSetColor(m_cBatteryColourActive);
	}
	else
	{
		vgui::surface()->DrawSetColor(m_cBatteryColourInactive);
	}

	// Top segment
	FillRectangle(5.0f, 2.0f, 6.0f, 2.0f, GetSectionHeight(80.0f, 100.0f));

	// Bottom segments
	FillRectangle(6.0f, 6.0f, 4.0f, 2.0f, GetSectionHeight(60.0f, 80.0f));
	FillRectangle(6.0f, 8.0f, 4.0f, 2.0f, GetSectionHeight(40.0f, 60.0f));
	FillRectangle(6.0f, 10.0f, 4.0f, 2.0f, GetSectionHeight(20.0f, 40.0f));
	FillRectangle(6.0f, 12.0f, 4.0f, 2.0f, GetSectionHeight(0.0f, 20.0f));
}

//------------------------------------------------------------------------
// Purpose: draws the objectives
//------------------------------------------------------------------------
void CHudFlashlightBattery::Paint()
{
	if (!m_bPlayerHasFlashlight) return;

	BaseClass::Paint();

	DrawBatteryBackground();
	DrawBattery();
	DrawBatteryForeground();
	DrawMultiplier();
}