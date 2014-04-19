//=============================================================================
//
// Purpose: An interactive screen.
//
//=============================================================================

#include "cbase.h"
#include "estranged_interactive_screen.h"
#include "hl2_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS(estranged_interactive_screen, CEstrangedInteractiveScreen);
BEGIN_DATADESC(CEstrangedInteractiveScreen)
	DEFINE_AUTO_ARRAY(m_cScreenFilename, FIELD_CHARACTER),
	DEFINE_AUTO_ARRAY(m_cPersistParameters, FIELD_CHARACTER),
	DEFINE_FIELD(m_bActive, FIELD_BOOLEAN),
	DEFINE_INPUTFUNC(FIELD_VOID, "ShowScreen", InputShowScreen),
	DEFINE_INPUTFUNC(FIELD_VOID, "HideScreen", InputHideScreen),
	DEFINE_OUTPUT(m_OnShowScreen, "OnShowScreen"),
	DEFINE_OUTPUT(m_OnHideScreen, "OnHideScreen"),
	DEFINE_OUTPUT(m_OnTrigger1, "OnTrigger1"),
	DEFINE_OUTPUT(m_OnTrigger2, "OnTrigger2"),
	DEFINE_OUTPUT(m_OnTrigger3, "OnTrigger3"),
	DEFINE_OUTPUT(m_OnTrigger4, "OnTrigger4"),
	DEFINE_OUTPUT(m_OnTrigger5, "OnTrigger5"),
	DEFINE_OUTPUT(m_OnTrigger6, "OnTrigger6"),
	DEFINE_OUTPUT(m_OnTrigger7, "OnTrigger7"),
	DEFINE_OUTPUT(m_OnTrigger8, "OnTrigger8"),
	DEFINE_OUTPUT(m_OnTrigger9, "OnTrigger9"),
END_DATADESC()

bool CEstrangedInteractiveScreen::IsActive(void)
{
	return m_bActive;
}

void CEstrangedInteractiveScreen::FireTriggerNumber(int output)
{
	DevMsg("CEstrangedInteractiveScreen::FireTriggerNumber(%i)\n", output);

	switch(output)
	{
	case 1:
		m_OnTrigger1.FireOutput(this, this);
		break;
	case 2:
		m_OnTrigger2.FireOutput(this, this);
		break;
	case 3:
		m_OnTrigger3.FireOutput(this, this);
		break;
	case 4:
		m_OnTrigger4.FireOutput(this, this);
		break;
	case 5:
		m_OnTrigger5.FireOutput(this, this);
		break;
	case 6:
		m_OnTrigger6.FireOutput(this, this);
		break;
	case 7:
		m_OnTrigger7.FireOutput(this, this);
		break;
	case 8:
		m_OnTrigger8.FireOutput(this, this);
		break;
	case 9:
		m_OnTrigger9.FireOutput(this, this);
		break;
	}
}

void CEstrangedInteractiveScreen::Spawn(void)
{
	BaseClass::Spawn();
}

void CEstrangedInteractiveScreen::InputHideScreen(inputdata_t &inputdata)
{
	SetActive(false);
	m_OnHideScreen.FireOutput(inputdata.pActivator, inputdata.pCaller);
}

void CEstrangedInteractiveScreen::InputShowScreen(inputdata_t &inputdata)
{
	SetActive(true);
	m_OnShowScreen.FireOutput(inputdata.pActivator, inputdata.pCaller);
}

void CEstrangedInteractiveScreen::SetActive(bool state)
{
	m_bActive = state;
	DevMsg("CEstrangedInteractiveScreen::SetActive(%i)\n", state);
	CBasePlayer *pPlayer = (CBasePlayer *)UTIL_GetLocalPlayer();

	if (pPlayer && pPlayer->IsPlayer())
	{
		CSingleUserRecipientFilter user(pPlayer);
		user.MakeReliable();
		if (state)
		{
			if (pPlayer->FlashlightIsOn())
			{
				pPlayer->FlashlightTurnOff();
			}

			UserMessageBegin(user, "ShowInteractiveScreen");
				WRITE_STRING(m_cScreenFilename);
				WRITE_STRING(m_cPersistParameters);
				WRITE_BYTE(GetSpawnFlags());
			MessageEnd();
		}
		else
		{
			UserMessageBegin(user, "HideInteractiveScreen");
				WRITE_STRING(m_cScreenFilename);
			MessageEnd();
		}
	}
}

void CEstrangedInteractiveScreen::PersistData(const char *key, const char *value)
{
	Q_strncat(m_cPersistParameters, UTIL_VarArgs("&%s=%s", key, value), sizeof(m_cPersistParameters), COPY_ALL_CHARACTERS);
}

bool CEstrangedInteractiveScreen::KeyValue(const char *szKeyName, const char *szValue)
{
	if (FStrEq(szKeyName, "filename" ))
	{
		Q_strcpy(m_cScreenFilename, szValue);
	}
	else
	{
		return BaseClass::KeyValue(szKeyName, szValue);
	}

	return true;
}