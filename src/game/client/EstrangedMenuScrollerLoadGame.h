#include "EstrangedMenuScroller.h"

class EstrangedMenuScrollerLoadGame : public EstrangedMenuScroller
{
	DECLARE_CLASS_SIMPLE(EstrangedMenuScrollerLoadGame, EstrangedMenuScroller); 

	EstrangedMenuScrollerLoadGame(vgui::VPANEL parent);

	protected:
		virtual void OnSave();
		virtual void OnCommand(const char *command);

	private:
		void AddSavedGames();
};