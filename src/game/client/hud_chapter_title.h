#include "cbase.h"
#include "hudelement.h"
#include "vgui_controls/Panel.h"

class CHudChapterTitle : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudChapterTitle, vgui::Panel);

public:
	CHudChapterTitle(const char *pElementName);
protected:
	virtual void Init();
	virtual void Paint();
	virtual void LevelInit();
	char* deblank(char* input);
private:
	vgui::HFont m_ChapterTitleFont;
	wchar_t*	m_wChapterName;
};