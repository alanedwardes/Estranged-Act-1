#include "EstrangedHUDBase.h"

class CHudFlashlightBattery : public EstrangedHUDBase
{
	DECLARE_CLASS_SIMPLE(CHudFlashlightBattery, EstrangedHUDBase);

public:
	CHudFlashlightBattery(const char *pElementName);
	virtual void OnThink();
protected:
	virtual void Init();
	virtual void Paint();
	virtual bool ShouldDraw();
private:
	bool m_bPlayerHasFlashlight;
	bool m_bFlashlightIsActive;
	int m_FlashlightTexture;
	CPanelAnimationVar( Color, m_cBackgroundColour, "BackgroundColour", BLANK );
	CPanelAnimationVar( Color, m_cBatteryColourInactive, "BatteryColourInactive", BLANK );
	CPanelAnimationVar( Color, m_cBatteryColourActive, "BatteryColourActive", BLANK );
	void DrawBattery();
	void DrawBatteryBackground();
	void DrawBatteryForeground();
};