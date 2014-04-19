#include "cbase.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>

class MenuMainButtonPanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(MenuMainButtonPanel, vgui::Panel);

	public:
		MenuMainButtonPanel(vgui::Panel* parent);
 
		virtual void PerformLayout();
		virtual void ApplySchemeSettings(vgui::IScheme*);
		void AddButton(const char*, const char*);
 
	private:
		CUtlVector<vgui::Panel*>	m_Buttons;
};