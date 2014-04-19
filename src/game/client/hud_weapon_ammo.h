#include "EstrangedHUDBase.h"

class CHudWeaponAmmo : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudWeaponAmmo, vgui::Panel);

	public:
		CHudWeaponAmmo(const char * pElementName);
		virtual void OnThink();
	protected:
		virtual void Init();
		virtual void Paint();
		virtual bool ShouldDraw();
	private:
		int		m_WeaponAmmoTexture;
		bool	m_bHasGun;
		void	DrawBullet();
		void	DrawBulletBackground();
		void	DrawBulletForeground();
		CPanelAnimationVar( Color, m_cBackgroundColour, "BackgroundColour", BLANK );
		CPanelAnimationVar( Color, m_cBulletColour, "BulletColour", BLANK );
		vgui::HFont m_FontBullets;
		vgui::HFont m_FontClips;
		wchar_t m_wBullets[8];
		wchar_t m_wClips[8];
		int m_BulletTexture;
		vgui::ImagePanel *m_BulletPanel;
};