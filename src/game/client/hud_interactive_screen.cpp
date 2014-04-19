#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "clientmode.h"
#include <vgui_controls/Frame.h>
#include <vgui_controls/HTML.h>
#include <vgui/ISurface.h>
#include <filesystem.h>
#include <vgui/IVGui.h>
#include <vgui/ISystem.h>
#include <steam/steam_api.h>
#include <steam/steamclientpublic.h>
#include "hud.h"
#include "ienginevgui.h"
#include "shareddefs_estranged.h"
#include "hud_interactive_screen_utils.h"

#define LOADING_FILENAME "./html/ui/loading.html"
#define CLOSE_MESSAGE "close"
#define OVERLAY_MESSAGE "overlay"
#define ADDFRIEND_MESSAGE "addfriend"
#define PERSIST_VALUE_MESSAGE "persist"
#define TRIGGER_MESSAGE "trigger"
#define CUSTOM_URL_MESSAGE "CustomURL"
#define STEAM_USER_MESSAGE "openuser"
#define CONSOLE_LOG "log"
#define CHAT_MESSAGE "chat"
#define BIG_PICTURE_INPUT "controllertextinput"

#define SF_ENABLE_INPUT (1<<0)

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// ae - the below code is horrible. It's designed to hack controller
//      support into our vgui::HTML implementation.
class CHudInteractiveScreenBrowser : public vgui::HTML
{
	DECLARE_CLASS_SIMPLE(CHudInteractiveScreenBrowser, vgui::HTML);
	CHudInteractiveScreenBrowser(Panel *parent, const char *name, bool allowJavaScript = false, bool bPopupWindow = false)
		: BaseClass(parent, name, allowJavaScript, bPopupWindow)
	{
	}
public:
	virtual void OnKeyCodePressed(vgui::KeyCode keyCode)
	{
		RunJavascript(VarArgs("ae.engineCallbacks.keyCodePressed(%i);", keyCode));
		if (!ShouldSwallow(keyCode)) BaseClass::OnKeyCodePressed(keyCode);
	}
	virtual void OnKeyTyped(vgui::KeyCode keyCode)
	{
		if (!ShouldSwallow(keyCode)) BaseClass::OnKeyTyped(keyCode);
	}
	virtual void OnKeyCodeReleased(vgui::KeyCode keyCode)
	{
		RunJavascript(VarArgs("ae.engineCallbacks.keyCodeReleased(%i);", keyCode));
		if (!ShouldSwallow(keyCode)) BaseClass::OnKeyCodeReleased(keyCode);
	}
private:
	// ae - ignore browser keys
	bool ShouldSwallow(vgui::KeyCode keyCode)
	{
		switch (keyCode)
		{
			case KEY_F5:
				return true;
			default:
				return false;
		}
	}
};

class CHudInteractiveScreen : public CHudElement, public vgui::Frame
{
public:
	DECLARE_CLASS_SIMPLE(CHudInteractiveScreen, vgui::Frame);

	CHudInteractiveScreen(const char*);
	virtual void Init();
	virtual void PerformLayout();
	virtual void HandleCustomCommands(const char *command);
	virtual void SetIsShowing(bool state);
	virtual void MsgFunc_HideInteractiveScreen(bf_read &msg);
	virtual void MsgFunc_ShowInteractiveScreen(bf_read &msg);
	virtual void OnMessage(const KeyValues *params, vgui::VPANEL ifromPanel);
	virtual void LevelShutdown();
	virtual bool ShouldDraw();
	STEAM_CALLBACK(CHudInteractiveScreen, OnGamepadInputReceived, GamepadTextInputDismissed_t, m_CallbackGamepadInputReceived);
	void LoadAddress();
private:
	void HandleActionParameters(const char *, CUtlVector<char*> *actionParameters);
	CHudInteractiveScreenBrowser *m_Browser;
	char m_cPersistParameters[MAX_PATH];
	char m_cFilename[MAX_PATH];
	bool m_bAcceptsUserInput;
	bool m_bCanExit;
};

DECLARE_HUDELEMENT(CHudInteractiveScreen);
DECLARE_HUD_MESSAGE(CHudInteractiveScreen, ShowInteractiveScreen);
DECLARE_HUD_MESSAGE(CHudInteractiveScreen, HideInteractiveScreen);

CHudInteractiveScreen::CHudInteractiveScreen(const char* pElementName) :
		CHudElement(pElementName),
		BaseClass(NULL, "InteractiveScreen"),
		m_CallbackGamepadInputReceived(this, &CHudInteractiveScreen::OnGamepadInputReceived)
{
	SetParentedToClientDLLRootPanel(true);
	SetParent(enginevgui->GetPanel(PANEL_CLIENTDLL));

	m_Browser = NULL;

	SetSizeable(false);
	SetMoveable(false);
	SetCloseButtonVisible(false);
	SetTitleBarVisible(false);
	SetPaintBackgroundEnabled(false);
	SetMouseInputEnabled(false);
	SetKeyBoardInputEnabled(false);
}

void CHudInteractiveScreen::LevelShutdown()
{
	SetIsShowing(false);
}

bool CHudInteractiveScreen::ShouldDraw()
{
	return true; // Show no matter what
}

void CHudInteractiveScreen::LoadAddress()
{
	const char *hashLang = "#language=";
	const char *hashDebug = "&debug=true";
	const char *hashUseKey = "&use=";
	const char *defaultL = "english";

	if (!m_Browser) return;

	char localURL[MAX_PATH + 7];
	Q_strncpy(localURL, "file://", sizeof(localURL));

	// Resolve the local path
	char pPathData[MAX_PATH];
	g_pFullFileSystem->GetLocalPath(m_cFilename, pPathData, sizeof(pPathData));

	// Encode the path
	char pPathDataEncoded[MAX_PATH];
	encodeUrl(pPathDataEncoded, MAX_PATH, pPathData, Q_strlen(pPathData));

	// Add encoded path to HTML file
	Q_strncat(localURL, pPathDataEncoded, sizeof(localURL), COPY_ALL_CHARACTERS);

	// Append language
	Q_strncat(localURL, hashLang, sizeof(localURL), COPY_ALL_CHARACTERS);
	ISteamUtils *steamUtils = steamapicontext->SteamUtils();
	if (steamUtils)
	{
		Q_strncat(localURL, steamUtils->GetSteamUILanguage(), sizeof(localURL), COPY_ALL_CHARACTERS);
	}
	else
	{
		Q_strncat(localURL, defaultL, sizeof(localURL), COPY_ALL_CHARACTERS);
	}

	// Append any other persist parameters
	Q_strncat(localURL, m_cPersistParameters, sizeof(m_cPersistParameters), COPY_ALL_CHARACTERS);

	if (developer.GetInt() > 0)
	{
		// If we're debugging
		Q_strncat(localURL, hashDebug, sizeof(localURL), COPY_ALL_CHARACTERS);
	}

	// Get the binding for +use
	const char *useBinding = engine->Key_LookupBinding("+use");
	Q_strncat(localURL, hashUseKey, sizeof(localURL), COPY_ALL_CHARACTERS);
	Q_strncat(localURL, useBinding, sizeof(localURL), COPY_ALL_CHARACTERS);

	DevMsg(2, "Interactive screen navigating to %s\n", localURL);
	m_Browser->OpenURL(localURL, "");
}

void CHudInteractiveScreen::Init()
{
	HOOK_HUD_MESSAGE(CHudInteractiveScreen, ShowInteractiveScreen);
	HOOK_HUD_MESSAGE(CHudInteractiveScreen, HideInteractiveScreen);
}

void CHudInteractiveScreen::HandleActionParameters(const char *fullCommand, CUtlVector<char*> *actionParameters)
{
	char* action = actionParameters->Element(0);
	char remainder[256];
	Q_StrSlice(fullCommand, Q_strlen(action) + 1, -1, remainder, 256);

	if (!Q_strcmp(action, CLOSE_MESSAGE))
	{
		SetIsShowing(false);
		return;
	}

	// Below this point we need more parameters
	if (actionParameters->Count() == 1)
		return;

	if (!Q_strcmp(action, CONSOLE_LOG))
	{
		DevMsg("%s\n", actionParameters->Element(1));
		return;
	}

	if (!Q_strcmp(action, BIG_PICTURE_INPUT))
	{
		steamapicontext->SteamUtils()->ShowGamepadTextInput(k_EGamepadTextInputModeNormal, k_EGamepadTextInputLineModeSingleLine, actionParameters->Element(1), 32);
		return;
	}

	if (!Q_strcmp(action, OVERLAY_MESSAGE))
	{
		steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage(remainder);
		return;
	}

	if (!Q_strcmp(action, STEAM_USER_MESSAGE))
	{
		steamapicontext->SteamFriends()->ActivateGameOverlayToUser("steamid", CSteamID(atoi(actionParameters->Element(1)), k_EUniversePublic, k_EAccountTypeIndividual));
		return;
	}

	if (!Q_strcmp(action, CHAT_MESSAGE))
	{
		steamapicontext->SteamFriends()->ActivateGameOverlayToUser("chat", CSteamID(atoi(actionParameters->Element(1)), k_EUniversePublic, k_EAccountTypeIndividual));
		return;
	}

	if (!Q_strcmp(action, ADDFRIEND_MESSAGE))
	{
		steamapicontext->SteamFriends()->ActivateGameOverlayToUser("addfriend", CSteamID(atoi(actionParameters->Element(1)), k_EUniversePublic, k_EAccountTypeIndividual));
		return;
	}

	if (!Q_strcmp(action, TRIGGER_MESSAGE))
	{
		engine->ServerCmd(VarArgs("%s %s", ESTRANGED_SCREEN_TRIGGER_OUTPUT, actionParameters->Element(1)));
		return;
	}

	if (!Q_strcmp(action, PERSIST_VALUE_MESSAGE))
	{
		if (actionParameters->Count() != 3)
		{
			Warning("Warning: persist parameters should be in the format estranged://persist/key/value. Ignoring.");
			return;
		}

		DevMsg("Persisting %s = %s\n", actionParameters->Element(1), actionParameters->Element(2));
		engine->ServerCmd(VarArgs("%s %s %s", ESTRANGED_SCREEN_PERSIST_PARAMETERS, actionParameters->Element(1), actionParameters->Element(2)));
		return;
	}
}

void CHudInteractiveScreen::HandleCustomCommands(const char *command)
{
	CUtlVector<char*> commandSegments;
	Q_SplitString(command, "/", commandSegments);

	if (commandSegments.Count() > 0)
	{
		HandleActionParameters(command, &commandSegments);
	}

	commandSegments.PurgeAndDeleteElements();
}

void CHudInteractiveScreen::OnGamepadInputReceived(GamepadTextInputDismissed_t *gamepadTextInputDismissed)
{
	if (gamepadTextInputDismissed->m_bSubmitted)
	{
		char input[256];
		steamapicontext->SteamUtils()->GetEnteredGamepadTextInput(input, sizeof(input));
		m_Browser->RunJavascript(VarArgs("ae.engineCallbacks.enteredGamepadText('%s');", input));
	}
}

void CHudInteractiveScreen::OnMessage(const KeyValues *params, vgui::VPANEL ifromPanel)
{
	BaseClass::OnMessage(params, ifromPanel);

	KeyValues *kv = params->MakeCopy();

	if (!Q_strcmp(kv->GetName(), CUSTOM_URL_MESSAGE))
	{
		HandleCustomCommands(kv->GetString("url"));
	}

	kv->deleteThis();
}

void CHudInteractiveScreen::PerformLayout()
{
	BaseClass::PerformLayout();

	SetSize(ScreenWidth(), ScreenHeight());

	if (m_Browser)
	{
		m_Browser->SetSize(GetWide(), GetTall());
	}

	SetPos(0, 0);
}

void CHudInteractiveScreen::SetIsShowing(bool state)
{
	SetVisible(state);

	if (state)
	{
		if (!m_Browser)
		{
			m_Browser = new CHudInteractiveScreenBrowser(this, "InteractiveScreenHTML", true);
			m_Browser->AddCustomURLHandler("estranged", this);
			m_Browser->SetSize(GetWide(), GetTall());
			m_Browser->SetContextMenuEnabled(false);
		}

		m_Browser->SetVisible(true);
		m_Browser->SetPaintEnabled(true);

		if (m_bAcceptsUserInput)
		{
			SetMouseInputEnabled(true);
			SetKeyBoardInputEnabled(true);
			RequestFocus();
			m_Browser->SetKeyBoardInputEnabled(true);
			m_Browser->SetMouseInputEnabled(true);
			m_Browser->RequestFocus();
		}
	}
	else
	{
		Q_strcpy(m_cFilename, LOADING_FILENAME);
		Q_strcpy(m_cPersistParameters, "");
		LoadAddress();
		SetMouseInputEnabled(false);
		SetKeyBoardInputEnabled(false);

		if (m_Browser)
		{
			engine->ServerCmd(ESTRANGED_SCREEN_EXIT_OUTPUT);
			m_Browser->SetMouseInputEnabled(false);
			m_Browser->SetKeyBoardInputEnabled(false);
			m_Browser->SetVisible(false);
		}

		SetVisible(false);
	}
}

void CHudInteractiveScreen::MsgFunc_HideInteractiveScreen(bf_read &msg)
{
	SetIsShowing(false);
}

void CHudInteractiveScreen::MsgFunc_ShowInteractiveScreen(bf_read &msg)
{
	msg.ReadString(m_cFilename, sizeof(m_cFilename));
	msg.ReadString(m_cPersistParameters, sizeof(m_cPersistParameters));
	int spawnFlags = msg.ReadByte();
	
	m_bAcceptsUserInput = (spawnFlags & SF_ENABLE_INPUT);

	SetIsShowing(true);

	LoadAddress();
}