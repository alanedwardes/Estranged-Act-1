#include "cbase.h"
#include "IOptionsPanel.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Divider.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Tooltip.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/URLLabel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Slider.h>
#include "vgui/ISurface.h"
#include "sourcevr/isourcevirtualreality.h"
#include "estranged_system_caps.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

ConVar ae_showoptionspanel("ae_showoptionspanel", "0", FCVAR_CLIENTDLL, "Sets the state of the Estranged options panel <state>");

CON_COMMAND(ToggleEstrangedOptionsPanel, "Toggle the Estranged options panel on or off")
{
	ae_showoptionspanel.SetValue(!ae_showoptionspanel.GetBool());
};
 
class COptionsPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(COptionsPanel, Frame); 
 
	COptionsPanel(VPANEL parent);
	~COptionsPanel(){};
 
	protected:
		void				SaveEstrangedSettings(void);
		void				LoadEstrangedSettings(void);
		virtual void		OnTick(void);
		virtual void		OnCommand(const char* pcCommand);
		int					GetSteamID(void);

		void				DrawOptionsItems(void);
		void				DrawWheel(void);
		void				DrawBuildLabel(void);
		void				DrawStatsCheckbutton(void);
		void				DrawHUDCheckbutton(void);
		void				DrawCrosshairCheckbutton(void);
		void				DrawGlowCheckButton(void);
		void				DrawVignetteCheckbutton(void);
		void				DrawLensFlareCheckbutton(void);
		void				DrawDoFCheckbutton(void);
		void				DrawGrainSlider(void);
		void				DrawSSAOCheckbutton(void);
		void				DrawMotionBlurCheckbutton(void);
		void				DrawFlashlightShadowsCheckbutton(void);
 
	private:
		Label *m_pLabel;
		CheckButton *m_pStatsEnabled;
		CheckButton *m_pObjectivesEnabled;
		CheckButton *m_pHUDEnabled;
		CheckButton *m_pCrosshairEnabled;
		CheckButton	*m_pGlowEnabled;
		CheckButton *m_pLensFlareEnabled;
		CheckButton *m_pDoFEnabled;
		Slider *m_pGrainIntensity;
		CheckButton *m_pSSAOEnabled;
		CheckButton *m_pVignetteEnabled;
		CheckButton *m_pMotionBlurEnabled;
		CheckButton *m_pFlashlightShadowsEnabled;
		bool m_bLastVisibilityState;
};

COptionsPanel::COptionsPanel(VPANEL parent): BaseClass(NULL, "EstrangedOptions")
{
	SetParent(parent);
	SetTitle("#Estranged_Options_Title", true);
	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);
	SetProportional(false);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(true);
	SetSizeable(false);
	SetMoveable(true);

	DrawOptionsItems();

	LoadControlSettingsAndUserConfig("resource/estrangedoptionspanel.res");

	ivgui()->AddTickSignal(GetVPanel(), 100);
}

void COptionsPanel::DrawOptionsItems()
{
	DrawStatsCheckbutton();
	DrawHUDCheckbutton();
	DrawCrosshairCheckbutton();
	DrawGlowCheckButton();
	DrawVignetteCheckbutton();
	DrawLensFlareCheckbutton();
	DrawDoFCheckbutton();
	DrawSSAOCheckbutton();
	DrawMotionBlurCheckbutton();
	DrawFlashlightShadowsCheckbutton();
	DrawGrainSlider();
}

void COptionsPanel::DrawStatsCheckbutton()
{
	m_pStatsEnabled = new CheckButton(this, "StatsEnabledCheckButton", "#Estranged_Stats_Enable");
}

void COptionsPanel::DrawHUDCheckbutton()
{
	m_pHUDEnabled = new CheckButton(this, "HudCheckButton", "#Estranged_HUD_Show");
}

void COptionsPanel::DrawCrosshairCheckbutton( void )
{
	m_pCrosshairEnabled = new CheckButton(this, "CrosshairCheckButton", "#Estranged_Crosshair_Show");
}

void COptionsPanel::DrawGlowCheckButton( void )
{
	m_pGlowEnabled = new CheckButton(this, "GlowShowCheckButton", "#Estranged_Glow_Show");
}

void COptionsPanel::DrawVignetteCheckbutton()
{
	m_pVignetteEnabled = new CheckButton(this, "VignetteCheckButton", "#Estranged_Vignette_Label");
}

void COptionsPanel::DrawLensFlareCheckbutton()
{
	m_pLensFlareEnabled = new CheckButton(this, "LensflareCheckButton", "#Estranged_Lensflare_Label");
}

void COptionsPanel::DrawDoFCheckbutton()
{
	m_pDoFEnabled = new CheckButton(this, "DoFCheckButton", "#Estranged_DoF_Label");
}

void COptionsPanel::DrawGrainSlider()
{
	m_pGrainIntensity = new Slider(this, "GrainSlider");
	m_pGrainIntensity->SetRange(0, 30);
}

void COptionsPanel::DrawSSAOCheckbutton()
{
	m_pSSAOEnabled = new CheckButton(this, "SSAOCheckButton", "#Estranged_SSAO_Label");
	m_pSSAOEnabled->GetTooltip()->SetText("#Estranged_SSAO_Tooltip");
}

void COptionsPanel::DrawMotionBlurCheckbutton()
{
	m_pMotionBlurEnabled = new CheckButton(this, "MotionBlurCheckButton", "#Estranged_MotionBlur_Label");
}

void COptionsPanel::DrawFlashlightShadowsCheckbutton()
{
	m_pFlashlightShadowsEnabled = new CheckButton(this, "FlashlightShadowsCheckButton", "#Estranged_FlashlightShadows_Label");
}

void COptionsPanel::LoadEstrangedSettings()
{
	ConVarRef ae_stats_enabled("ae_stats_enabled");
	m_pStatsEnabled->SetSelected(ae_stats_enabled.GetBool());

	ConVarRef ae_hud_enabled("ae_hud_enabled");
	m_pHUDEnabled->SetSelected(ae_hud_enabled.GetBool());

	ConVarRef crosshair_enabled("crosshair");
	m_pCrosshairEnabled->SetSelected(crosshair_enabled.GetBool());

	ConVarRef ae_glow("ae_glow");
	m_pGlowEnabled->SetSelected(ae_glow.GetBool());

	ConVarRef ae_vignette("ae_vignette");
	m_pVignetteEnabled->SetEnabled(CEstrangedSystemCaps::HasCaps(CAPS_MATERIAL_POSTPROCESS));
	m_pVignetteEnabled->SetSelected(m_pVignetteEnabled->IsEnabled() && ae_vignette.GetBool());

	ConVarRef ae_lensflare("ae_lensflare");
	m_pLensFlareEnabled->SetEnabled(CEstrangedSystemCaps::HasCaps(CAPS_SHADER_POSTPROCESS));
	m_pLensFlareEnabled->SetSelected(m_pLensFlareEnabled->IsEnabled() && ae_lensflare.GetBool());

	ConVarRef ae_dof("ae_dof");
	m_pDoFEnabled->SetEnabled(CEstrangedSystemCaps::HasCaps(CAPS_SHADER_POSTPROCESS) && CEstrangedSystemCaps::HasCaps(CAPS_ESTRANGED_DEPTHPASS));
	m_pDoFEnabled->SetSelected(m_pDoFEnabled->IsEnabled() && ae_dof.GetBool());

	ConVarRef ae_grain_intensity("ae_grain_intensity");
	m_pGrainIntensity->SetEnabled(CEstrangedSystemCaps::HasCaps(CAPS_SHADER_POSTPROCESS));
	m_pGrainIntensity->SetValue(m_pGrainIntensity->IsEnabled() ? ae_grain_intensity.GetFloat() * 10.0f : 0);

	ConVarRef ae_ssao("ae_ssao");
	m_pSSAOEnabled->SetEnabled(CEstrangedSystemCaps::HasCaps(CAPS_SHADER_POSTPROCESS) && CEstrangedSystemCaps::HasCaps(CAPS_ESTRANGED_DEPTHPASS));
	m_pSSAOEnabled->SetSelected(m_pSSAOEnabled->IsEnabled() && ae_ssao.GetBool());

	ConVarRef mat_motion_blur_enabled("mat_motion_blur_enabled");
	m_pMotionBlurEnabled->SetEnabled(CEstrangedSystemCaps::HasCaps(CAPS_SHADER_POSTPROCESS));
	m_pMotionBlurEnabled->SetSelected(m_pMotionBlurEnabled->IsEnabled() && mat_motion_blur_enabled.GetBool());

	ConVarRef ae_flashlightshadow("ae_flashlightshadow");
	m_pFlashlightShadowsEnabled->SetEnabled(CEstrangedSystemCaps::HasCaps(CAPS_SHADOW_DEPTHPASS));
	m_pFlashlightShadowsEnabled->SetSelected(m_pFlashlightShadowsEnabled->IsEnabled() && ae_flashlightshadow.GetBool());
}

void COptionsPanel::SaveEstrangedSettings()
{
	ConVarRef ae_stats_enabled("ae_stats_enabled");
	ae_stats_enabled.SetValue(m_pStatsEnabled->IsSelected());

	ConVarRef ae_hud_enabled("ae_hud_enabled");
	ae_hud_enabled.SetValue(m_pHUDEnabled->IsSelected());

	ConVarRef crosshair_enabled("crosshair");
	crosshair_enabled.SetValue(m_pCrosshairEnabled->IsSelected());

	ConVarRef ae_glow("ae_glow");
	ae_glow.SetValue(m_pGlowEnabled->IsSelected());

	ConVarRef ae_vignette("ae_vignette");
	ae_vignette.SetValue(m_pVignetteEnabled->IsSelected());

	ConVarRef ae_lensflare("ae_lensflare");
	ae_lensflare.SetValue(m_pLensFlareEnabled->IsSelected());

	ConVarRef ae_dof("ae_dof");
	ae_dof.SetValue(m_pDoFEnabled->IsSelected());

	ConVarRef ae_grain_intensity("ae_grain_intensity");
	ae_grain_intensity.SetValue(float(m_pGrainIntensity->GetValue()) / 10);

	ConVarRef ae_ssao("ae_ssao");
	ae_ssao.SetValue(m_pSSAOEnabled->IsSelected());

	ConVarRef mat_motion_blur_enabled("mat_motion_blur_enabled");
	mat_motion_blur_enabled.SetValue(m_pMotionBlurEnabled->IsSelected());

	ConVarRef ae_flashlightshadow("ae_flashlightshadow");
	ae_flashlightshadow.SetValue(m_pFlashlightShadowsEnabled->IsSelected());
}

void COptionsPanel::OnCommand( const char *command)
{
	if (!Q_strcmp(command, "cancel") || !Q_strcmp(command, "Close"))
	{
		SetVisible(false);
		ae_showoptionspanel.SetValue("0");
	}
	else if (!Q_strcmp(command, "save"))
	{
		SaveEstrangedSettings();
		SetVisible(false);
		ae_showoptionspanel.SetValue("0");
	}
	else if (!Q_strcmp(command, "apply"))
	{
		SaveEstrangedSettings();
	}

	BaseClass::OnCommand(command);
}

void COptionsPanel::OnTick()
{
	bool visibleState = ae_showoptionspanel.GetBool();
	if (visibleState)
	{
		if (!m_bLastVisibilityState)
		{
			LoadEstrangedSettings();
			SetPos((ScreenWidth() / 2) - (GetWide() / 2), (ScreenHeight() / 2) - (GetTall() / 2));
			SetVisible(visibleState);
			Activate();
			m_bLastVisibilityState = true;
		}
	}
	else
	{
		SetVisible(false);
		m_bLastVisibilityState = false;
	}
	
	BaseClass::OnTick();
}

class COptionsPanelInterface : public IOptionsPanel
{
	private:
		COptionsPanel *OptionsPanel;
	public:
		COptionsPanelInterface()
		{
			OptionsPanel = NULL;
		}
		void Create(VPANEL parent)
		{
			OptionsPanel = new COptionsPanel(parent);
		}
		void Destroy()
		{
			if (OptionsPanel)
			{
				OptionsPanel->SetParent( (Panel *)NULL);
				delete OptionsPanel;
			}
		}
};

static COptionsPanelInterface g_OptionsPanel;
IOptionsPanel* optionspanel = (IOptionsPanel*)&g_OptionsPanel;