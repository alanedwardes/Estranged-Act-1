#include "cbase.h" 
#include "hud.h" 
#include "hud_macros.h" 
#include "c_baseplayer.h" 
#include "hud_weapon_ammo.h" 
#include "iclientmode.h" 
#include "vgui/ISurface.h"
#include "vgui_controls/ImagePanel.h"
#include <vgui/ILocalize.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
 
DECLARE_HUDELEMENT(CHudWeaponAmmo);

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudWeaponAmmo::CHudWeaponAmmo(const char* pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudWeaponAmmo")
{
	vgui::Panel* pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	SetPaintBackgroundEnabled(false);

	SetHiddenBits(HIDEHUD_INVEHICLE);
}

void CHudWeaponAmmo::Init()
{
	vgui::HScheme scheme = vgui::scheme()->GetScheme("ClientScheme");
	m_FontBullets = vgui::scheme()->GetIScheme(scheme)->GetFont("EstrangedHUDAmmoBullets");
	m_FontClips = vgui::scheme()->GetIScheme(scheme)->GetFont("EstrangedHUDAmmoClip");

	//m_BulletTexture = vgui::surface()->CreateNewTextureID();
	//vgui::surface()->DrawSetTextureFile(m_BulletTexture, "vgui/bullet", true, false);

	//m_BulletPanel = new vgui::ImagePanel(this, "BulletImage");
	//m_BulletPanel->SetImage("bullet");
	//m_BulletPanel->SetShouldScaleImage(false);
}
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudWeaponAmmo::OnThink()
{
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if (!player)
	{
		m_bHasGun = false;
		return;
	}

	C_BaseCombatWeapon *weapon = player->GetActiveWeapon();
	if (!weapon)
	{
		m_bHasGun = false;
		return;
	}

	if (!weapon->UsesPrimaryAmmo() && !weapon->UsesSecondaryAmmo())
	{
		m_bHasGun = false;
		return;
	}

	m_bHasGun = true;

	int ammo1 = weapon->Clip1();
	int ammo2;
	if (ammo1 < 0)
	{
		// we don't use clip ammo, just use the total ammo count
		ammo1 = player->GetAmmoCount(weapon->GetPrimaryAmmoType());
		ammo2 = 0;
	}
	else
	{
		// we use clip ammo, so the second ammo is the total ammo
		ammo2 = player->GetAmmoCount(weapon->GetPrimaryAmmoType());
	}
	
	V_swprintf_safe(m_wBullets, L"%i", ammo1);
	V_swprintf_safe(m_wClips, L"%i", ammo2);
}

bool CHudWeaponAmmo::ShouldDraw()
{
	ConVarRef ammo_enabled("ae_hud_enabled");
	return CHudElement::ShouldDraw() && ammo_enabled.GetBool();
}
 
//------------------------------------------------------------------------
// Purpose: draws the objectives
//------------------------------------------------------------------------
void CHudWeaponAmmo::Paint()
{
	if (!m_bHasGun) return;

	vgui::surface()->DrawSetTextColor(Color(255, 255, 255, 255));

	int bulletWidth, bulletHeight;

	// Draw bullets
	vgui::surface()->DrawSetTextFont(m_FontBullets);
	vgui::surface()->GetTextSize(m_FontBullets, m_wBullets, bulletWidth, bulletHeight);
	vgui::surface()->DrawSetTextPos(GetWide() - bulletWidth, 0);
	vgui::surface()->DrawPrintText(m_wBullets, wcslen(m_wBullets), vgui::FONT_DRAW_NONADDITIVE);

	//int bulletImageWidth = 16;
	//int bulletImageHeight = 64;
	//int bulletImageX = GetWide() - bulletWidth - bulletImageWidth;

	//m_BulletPanel->SetPos(GetWide() - bulletWidth - bulletImageWidth, 0);
	//m_BulletPanel->SetSize(bulletImageWidth, bulletImageHeight);

	//vgui::surface()->DrawSetTexture(m_BulletTexture);
	//vgui::surface()->DrawTexturedRect(bulletImageX, 0, bulletImageX + bulletImageWidth, bulletImageHeight);
	
	int clipWidth, clipHeight;

	// Draw clips
	vgui::surface()->DrawSetTextFont(m_FontClips);
	vgui::surface()->GetTextSize(m_FontClips, m_wClips, clipWidth, clipHeight);
	vgui::surface()->DrawSetTextPos(GetWide() - clipWidth, 50);
	vgui::surface()->DrawPrintText(m_wClips, wcslen(m_wClips), vgui::FONT_DRAW_NONADDITIVE);
}