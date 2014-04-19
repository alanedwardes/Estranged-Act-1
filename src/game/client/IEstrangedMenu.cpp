#include "cbase.h"
#include "IEstrangedMenu.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/ImagePanel.h>
#include "vgui/ISurface.h"
#include <vgui/ISystem.h>
#include "EstrangedMenu.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class EstrangedMenuInterface : public IEstrangedMenu
{
	private:
		EstrangedMenu *estrangedMenu;
	public:
		EstrangedMenuInterface()
		{
			estrangedMenu = NULL;
		}
		void Create(vgui::VPANEL parent)
		{
			estrangedMenu = new EstrangedMenu(parent);
		}
		void Destroy()
		{
			if (estrangedMenu)
			{
				estrangedMenu->SetParent((vgui::Panel*)NULL);
				delete estrangedMenu;
			}
		}
};

static EstrangedMenuInterface g_EstrangedMenu;
IEstrangedMenu* estrangedmenu = &g_EstrangedMenu;