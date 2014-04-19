#include "cbase.h"
#include "EstrangedMenuScroller.h"

#define GUTTER 50
#define INNER_GUTTER_X 10
#define SCROLL_BUTTON_HEIGHT 30
#define SCROLL_UP_COMMAND "OliverLikesPonies"
#define SCROLL_DOWN_COMMAND "ChrisIsANaySayer"
#define SCROLL_AMOUNT 100

EstrangedMenuScroller::EstrangedMenuScroller(vgui::VPANEL parent, const char *title) : BaseClass(NULL, "EstrangedMenuSlideyScrollPanel")
{
	SetTitle(title, true);
	SetParent(parent);

	SetMenuButtonResponsive(false);
	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(false);
	SetSizeable(false);
	SetMoveable(false);
	SetVisible(true);

	Activate();
	RequestFocus();

	m_pScrollControl = new vgui::Panel(this, "EstrangedMenuSlideyScrollPanelScroller");

	m_pUpButton = new vgui::Button(this, "EstrangedMenuSlideyScrollPanelUp", "UP", this, SCROLL_UP_COMMAND);
	m_pUpButton->SetContentAlignment(vgui::Label::a_center);

	m_pDownButton = new vgui::Button(this, "EstrangedMenuSlideyScrollPanelDown", "DOWN", this, SCROLL_DOWN_COMMAND);
	m_pDownButton->SetContentAlignment(vgui::Label::a_center);

	m_flScrollerInnerHeight = 0.0f;
	m_flScrollerPosition = 0.0f;
	m_flScrollerAcceleration = 0.0f;
	m_flScrollerTargetPosition = 0.0f;
}

void EstrangedMenuScroller::LayoutItems()
{
	int runningHeight = m_flScrollerPosition;
	int totalHeight = 0;

	for (int i = 0; i < m_pItems.Count(); i++)
	{
		vgui::Panel *item = m_pItems.Element(i);
		item->SetPos(0, runningHeight);
		runningHeight += item->GetTall();
		totalHeight += item->GetTall();
	}

	m_flScrollerInnerHeight = totalHeight;
}

void EstrangedMenuScroller::ClearItems()
{
	for (int i = 0; i < m_pItems.Count(); i++)
	{
		vgui::Panel *item = m_pItems.Element(i);
		item->DeletePanel();
	}

	m_pItems.RemoveAll();
}

void EstrangedMenuScroller::AddItem(vgui::Panel *item)
{
	item->SetParent(m_pScrollControl);
	m_pItems.AddToTail(item);
	LayoutItems();
}

void EstrangedMenuScroller::OnThink()
{
	if (m_flScrollerAcceleration > 0)
	{
		m_flScrollerAcceleration = max(m_flScrollerAcceleration - 0.1f, 0);
		m_flScrollerPosition = Lerp(m_flScrollerAcceleration, m_flScrollerTargetPosition, m_flScrollerPosition);
		LayoutItems();
	}

	m_pScrollControl->SetBgColor(Color(100, 100, 100, 255));
}

void EstrangedMenuScroller::PerformLayout()
{
	BaseClass::PerformLayout();

	const int qScreen = max(ScreenWidth() / 4, 300);

	const int lBuffer = ScreenWidth() / 10;

	SetWide(qScreen);
	SetTall(ScreenHeight() - (GUTTER * 2));
	SetPos(lBuffer + qScreen + GUTTER, GUTTER);

	const int itemWidth = GetWide() - (INNER_GUTTER_X * 2);
	const int topGutter = 40;

	m_pUpButton->SetPos(INNER_GUTTER_X, topGutter);
	m_pUpButton->SetSize(itemWidth, SCROLL_BUTTON_HEIGHT);

	m_pDownButton->SetPos(INNER_GUTTER_X, GetTall() - (INNER_GUTTER_X + SCROLL_BUTTON_HEIGHT));
	m_pDownButton->SetSize(itemWidth, SCROLL_BUTTON_HEIGHT);

	m_pScrollControl->SetSize(itemWidth, GetTall() - (topGutter + SCROLL_BUTTON_HEIGHT * 2));
	m_pScrollControl->SetPos(INNER_GUTTER_X, topGutter + SCROLL_BUTTON_HEIGHT);

	for (int i = 0; i < m_pItems.Count(); i++)
	{
		vgui::Panel *item = m_pItems.Element(i);
		item->SetWide(itemWidth);
	}
}

void EstrangedMenuScroller::AddToScroller(int amount)
{
	m_flScrollerAcceleration = 1.0f;

	float newTarget = min(amount + m_flScrollerTargetPosition, 0.0f);

	newTarget = max(newTarget, -m_flScrollerInnerHeight);

	m_flScrollerTargetPosition = newTarget;
}

void EstrangedMenuScroller::OnCommand(const char *command)
{
	BaseClass::OnCommand(command);

	if (!Q_strcmp(command, SCROLL_UP_COMMAND))
	{
		AddToScroller(SCROLL_AMOUNT);
	}
	else if (!Q_strcmp(command, SCROLL_DOWN_COMMAND))
	{
		AddToScroller(-SCROLL_AMOUNT);
	}
}

void EstrangedMenuScroller::OnMouseWheeled(int delta)
{
	AddToScroller(delta * SCROLL_AMOUNT);
}