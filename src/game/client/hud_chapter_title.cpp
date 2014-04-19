#include "cbase.h"
#include "hud_chapter_title.h"
#include "vgui/ISurface.h"
#include "vgui/IScheme.h"
#include "vgui/ILocalize.h"
#include "vgui_controls/AnimationController.h"
#include "iclientmode.h"
#include "fmtstr.h"

DECLARE_HUDELEMENT(CHudChapterTitle);

CHudChapterTitle::CHudChapterTitle(const char *pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudChapterTitle")
{
	SetParent(g_pClientMode->GetViewport());
	SetPaintBackgroundEnabled(false);
}

void CHudChapterTitle::Init()
{
	vgui::HScheme scheme = vgui::scheme()->GetScheme("ClientScheme");
	m_ChapterTitleFont = vgui::scheme()->GetIScheme(scheme)->GetFont("EstrangedChapterTitle");
}

char* CHudChapterTitle::deblank(char* str)                                         
{
	char *out = str, *put = str;

	for(; *str != '\0'; ++str)
	{
		if(*str != ' ')
			*put++ = *str;
	}
	*put = '\0';
	return out;
}

void CHudChapterTitle::LevelInit()
{
	char chapter_name[32];
	engine->GetChapterName(chapter_name, sizeof(chapter_name));
	m_wChapterName = g_pVGuiLocalize->Find(deblank(chapter_name));
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HudChapterTitle");
}

void CHudChapterTitle::Paint()
{
	return;
	if (!m_wChapterName) return;

	int textWidth, textHeight;
	vgui::surface()->GetTextSize(m_ChapterTitleFont, m_wChapterName, textWidth, textHeight);
	vgui::surface()->DrawSetTextColor(Color(255, 255, 255, 255));
	vgui::surface()->DrawSetTextFont(m_ChapterTitleFont);
	vgui::surface()->DrawSetTextPos(0, GetTall() / 2 - textHeight / 2);
	vgui::surface()->DrawPrintText(m_wChapterName, wcslen(m_wChapterName), vgui::FONT_DRAW_NONADDITIVE);
}