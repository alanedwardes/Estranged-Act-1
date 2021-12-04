#include "EstrangedHUDBase.h"
 
class CHudStamina : public EstrangedHUDBase
{
	DECLARE_CLASS_SIMPLE(CHudStamina, EstrangedHUDBase);
public:
	CHudStamina(const char *pElementName);
protected:
	virtual void OnThink();
	virtual bool ShouldDraw();
	virtual void Init();
	virtual void Paint();
private:
	void DrawLineBackground();
	void DrawLine();
	void DrawLineForeground();
	int m_StaminaTexture;
	vgui::HFont m_FontStaminaTitle;
	CPanelAnimationVar( Color, m_cBackgroundColour, "BackgroundColour", BLANK );
	CPanelAnimationVar( Color, m_cStaminaColour, "StaminaColour", BLANK );
};