#include "cbase.h"
#include "vgui_controls/Frame.h"
#include "vgui_controls/Button.h"

class EstrangedMenuScroller : public vgui::Frame, public CAutoGameSystem
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuScroller, vgui::Frame);

	EstrangedMenuScroller(vgui::VPANEL parent, const char *title);

public:
	virtual void AddItem(vgui::Panel *item);
	virtual void ClearItems();
protected:
	virtual void PerformLayout();
	virtual void OnThink();
	virtual void LayoutItems();
	virtual void OnCommand(const char *command);
	virtual void OnMouseWheeled(int delta);
private:
	void							AddToScroller(int amount);
	vgui::Panel						*m_pScrollControl;
	float							m_flScrollerInnerHeight;
	float							m_flScrollerPosition;
	float							m_flScrollerTargetPosition;
	float							m_flScrollerAcceleration;
	CUtlVector<vgui::Panel*>		m_pItems;
	vgui::Button					*m_pDownButton;
	vgui::Button					*m_pUpButton;
};