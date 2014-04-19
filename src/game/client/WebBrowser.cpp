#include "cbase.h"
#include "WebBrowser.h"
#include <vgui_controls/Frame.h>
#include <vgui_controls/HTML.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CWebBrowser : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CWebBrowser, vgui::Frame);

	CWebBrowser(vgui::VPANEL parent);
	~CWebBrowser(){};

	public:
		void OpenURL(const char*);
	
	protected:
		virtual void PerformLayout();

	private:
		vgui::HTML *m_pBrowser;
};

static CWebBrowser *g_pWebBrowser = NULL;

CWebBrowser *GetWebBrowserInstance()
{
	Assert(g_pWebBrowser);
	return g_pWebBrowser;
}

CON_COMMAND(ae_browse, "Browse to <url>")
{
	CWebBrowser *browser = GetWebBrowserInstance();
	browser->OpenURL(args[1]);
	browser->SetVisible(true);
}

CWebBrowser::CWebBrowser(vgui::VPANEL parent): BaseClass(NULL, "WebBrowser")
{
	g_pWebBrowser = this;

	SetParent(parent);

	m_pBrowser = new vgui::HTML(this, "WebBrowser", true, false);

	SetTitle("Web Browser", true);
	SetSize(640, 480);
	m_pBrowser->SetSize(GetWide() - 20, GetTall() - 50);
}

void CWebBrowser::OpenURL(const char* url)
{
	m_pBrowser->OpenURL(url, "");
}

void CWebBrowser::PerformLayout()
{
	BaseClass::PerformLayout();

	m_pBrowser->SetSize(GetWide() - 20, GetTall() - 50);
	m_pBrowser->SetPos(10, 40);
}

class CWebBrowserInterface : public IWebBrowser
{
	private:
		CWebBrowser *WebBrowser;
	public:
		CWebBrowserInterface()
		{
			WebBrowser = NULL;
		}
		void Create(vgui::VPANEL parent)
		{
			WebBrowser = new CWebBrowser(parent);
		}
		void Destroy()
		{
			if (WebBrowser)
			{
				WebBrowser->SetParent((vgui::Panel*)NULL);
				delete WebBrowser;
			}
		}
};

static CWebBrowserInterface g_WebBrowser;
IWebBrowser* webbrowser = (IWebBrowser*)&g_WebBrowser;