#include "cbase.h"

class CEstrangedInteractiveScreen : public CPointEntity
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CEstrangedInteractiveScreen, CPointEntity );

	CEstrangedInteractiveScreen(){}

	virtual bool IsActive();
	virtual void Spawn(void);
	virtual void FireTriggerNumber(int);
	virtual void InputHideScreen(inputdata_t&);
	virtual void InputShowScreen(inputdata_t&);
	virtual void PersistData(const char *key, const char *value);
	virtual bool KeyValue(const char *, const char *);
private:
	bool m_bActive;
	char m_cScreenFilename[MAX_PATH];
	char m_cPersistParameters[MAX_PATH];
	void SetActive(bool);

	COutputEvent m_OnShowScreen;
	COutputEvent m_OnHideScreen;
	COutputEvent m_OnTrigger1;
	COutputEvent m_OnTrigger2;
	COutputEvent m_OnTrigger3;
	COutputEvent m_OnTrigger4;
	COutputEvent m_OnTrigger5;
	COutputEvent m_OnTrigger6;
	COutputEvent m_OnTrigger7;
	COutputEvent m_OnTrigger8;
	COutputEvent m_OnTrigger9;
};