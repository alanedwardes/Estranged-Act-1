#include "EstrangedHUDBase.h"
 
class CHudHP : public EstrangedHUDBase
{
	DECLARE_CLASS_SIMPLE(CHudHP, EstrangedHUDBase);
public:
	CHudHP(const char *pElementName);
protected:
	virtual void OnThink();
	virtual bool ShouldDraw();
	virtual void Init();
	virtual void Paint();
private:
	void DrawCrossBackground();
	void DrawCross();
	void DrawCrossForeground();
	int m_HPTexture;
	vgui::HFont m_FontHPTitle;
	CPanelAnimationVar( Color, m_cBackgroundColour, "BackgroundColour", BLANK );
	CPanelAnimationVar( Color, m_cCrossColour, "CrossColour", BLANK );
};