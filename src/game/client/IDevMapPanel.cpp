#include "cbase.h"
#include "IDevMapPanel.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/ListPanel.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Label.h>
#include "vgui/ISurface.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define WIDTH 300
#define HEIGHT 400
#define BUTTON_WIDTH 80

ConVar ae_showdevmappanel("ae_showdevmappanel", "1", FCVAR_CLIENTDLL, "Sets the state of the Estranged dev map panel <state>");

class DoubleClickListPanel : public vgui::ListPanel
{
	DECLARE_CLASS_SIMPLE(DoubleClickListPanel, vgui::ListPanel);

	explicit DoubleClickListPanel(Panel *parent, const char *panelName) : vgui::ListPanel(parent, panelName) { }

	virtual void OnMouseDoublePressed(MouseCode code)
	{
		PostMessage(GetParent(), new KeyValues("command", "command", "DoubleClick"));
	}

	virtual void OnKeyCodePressed(KeyCode code)
	{
		if (code == KEY_ENTER)
		{
			PostMessage(GetParent(), new KeyValues("command", "command", "PressedEnter"));
		}
	}
};

class CDevMapPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CDevMapPanel, vgui::Frame);

	CDevMapPanel(vgui::VPANEL parent);
	~CDevMapPanel(){};

	protected:
		void			AddMapsToPanel(void);
		virtual void	OnTick(void);
		virtual void	OnCommand(const char* pcCommand);
		virtual void	PerformLayout(void);

	private:
		bool			m_bLastVisibilityState;
		DoubleClickListPanel *m_pListPanel;
		vgui::Button	*m_bLoadButton;
		vgui::Button	*m_bRefreshButton;
};

CDevMapPanel::CDevMapPanel(vgui::VPANEL parent): BaseClass(NULL, "Estranged Map Select")
{
	SetParent(parent);

	SetTitle("ESTRANGED MAP SELECT", true);
	SetSize(WIDTH, HEIGHT);

	m_pListPanel = new DoubleClickListPanel(this, "UL");
	m_pListPanel->AddColumnHeader(0, "Map", "Map", 100, ListPanel::COLUMN_RESIZEWITHWINDOW);
	m_pListPanel->AddColumnHeader(1, "Size", "Size", 20, ListPanel::COLUMN_RESIZEWITHWINDOW);
	m_pListPanel->SetMultiselectEnabled(false);

	m_bLoadButton = new vgui::Button(this, "Load", "Load", this, "Load");
	m_bLoadButton->SetWide(BUTTON_WIDTH);

	m_bRefreshButton = new vgui::Button(this, "Refresh", "Refresh", this, "Refresh");
	m_bRefreshButton->SetWide(BUTTON_WIDTH);
 
	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);
 
	SetProportional(false);
	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetVisible(true);
	Activate();

	AddMapsToPanel();

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
}

void CDevMapPanel::PerformLayout(void)
{
	BaseClass::PerformLayout();

	const int GUTTER_TOP = 40;
	const int GUTTER_BOTTOM = 20;
	const int GUTTERLR = 10;

	int w, h;
	GetSize(w, h);
	m_pListPanel->SetBounds(GUTTERLR, GUTTER_TOP, w - GUTTERLR * 2, h - 90);

	m_bLoadButton->SetPos(w - GUTTERLR - BUTTON_WIDTH, h - GUTTER_BOTTOM - m_bLoadButton->GetTall());
	m_bRefreshButton->SetPos(GUTTERLR, h - GUTTER_BOTTOM - m_bRefreshButton->GetTall());
}

void CDevMapPanel::AddMapsToPanel()
{
	const char *BSP_PATH = "./maps/";

	FileFindHandle_t fh;

	char path[512];
	Q_snprintf(path, sizeof(path), "%s*.bsp", BSP_PATH);

	char const *fn = g_pFullFileSystem->FindFirstEx(path, "MOD", &fh);
	if (!fn) return;

	m_pListPanel->RemoveAll();

	do
	{
		if (fn[0] != '.' && !Q_strstr(fn, "_background"))
		{
			char map_name[64];
			Q_StripExtension(fn, map_name, sizeof(map_name));

			char map_path[64];
			Q_snprintf(map_path, sizeof(map_path), "%s%s", BSP_PATH, fn);
			float size = g_pFullFileSystem->Size(map_path, "MOD");

			size /= 1024.0f * 1024.0f;

			char map_size[64];
			Q_snprintf(map_size, sizeof(map_size), "%.2f MB", size);

			KeyValues *kv = new KeyValues("LI");
			kv->SetString("Map", map_name);
			kv->SetString("Size", map_size);
			m_pListPanel->AddItem(kv, 0, false, false);
			kv->deleteThis();
		}

		fn = g_pFullFileSystem->FindNext(fh);
	}
	while(fn);

	m_pListPanel->SetSingleSelectedItem(0);

	g_pFullFileSystem->FindClose(fh);
}

void CDevMapPanel::OnCommand(const char *command)
{
	if (!Q_strcmp(command, "Load") || !Q_strcmp(command, "DoubleClick") || !Q_strcmp(command, "PressedEnter"))
	{
		int itemId = m_pListPanel->GetSelectedItem(0);
		KeyValues *kv = m_pListPanel->GetItem(itemId);
		if (kv)
		{
			const char *map = kv->GetString("Map");
			char map_command[64];
			Q_snprintf(map_command, sizeof(map_command), "map %s", map);
			engine->ClientCmd("scene_flush");
			engine->ClientCmd("disconnect");
			engine->ClientCmd("mat_fullbright 0");
			engine->ClientCmd("progress_enable");
			engine->ClientCmd("clear");
			engine->ClientCmd(map_command);
		}
	}
	else if (!Q_strcmp(command, "Refresh"))
	{
		AddMapsToPanel();
	}

	BaseClass::OnCommand(command);
}

void CDevMapPanel::OnTick()
{
	ConVarRef developer("developer");
	if (developer.GetInt() < 1) return;

	bool visibleState = ae_showdevmappanel.GetBool();
	if (visibleState)
	{
		if (!m_bLastVisibilityState)
		{
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

class CDevMapPanelInterface : public IDevMapPanel
{
	private:
		CDevMapPanel *DevMapPanel;
	public:
		CDevMapPanelInterface()
		{
			DevMapPanel = NULL;
		}
		void Create(vgui::VPANEL parent)
		{
			DevMapPanel = new CDevMapPanel(parent);
		}
		void Destroy()
		{
			if (DevMapPanel)
			{
				DevMapPanel->SetParent( (vgui::Panel *)NULL);
				delete DevMapPanel;
			}
		}
};

static CDevMapPanelInterface g_DevMapPanel;
IDevMapPanel* devmappanel = (IDevMapPanel*)&g_DevMapPanel;