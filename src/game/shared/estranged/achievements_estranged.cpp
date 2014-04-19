#include "cbase.h"

#if GAME_DLL

#include "achievementmgr.h"
#include "baseachievement.h"
#include "hl2_player.h"

CAchievementMgr g_AchievementMgrEstranged;

class CAchievementEstrangedFinishGameWithoutPickingUpFlashlight : public CFailableAchievement
{
	DECLARE_CLASS(CAchievementEstrangedFinishGameWithoutPickingUpFlashlight, CFailableAchievement);

	void Init()
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
	}

	// This achievement is always active
	virtual const char *GetActivationEventName() { return ""; }

	// Listen for this map event - in this case, the endgame achievement
	virtual const char *GetEvaluationEventName() { return "EA1_ESTRANGED_IN_THE_DARK_EVALUATE"; }

	virtual void OnEvaluationEvent()
	{
		CBasePlayer *pPlayer = UTIL_GetLocalPlayer();
		CHL2_Player *pHL2Player = assert_cast<CHL2_Player*>(pPlayer);

		bool bHasFlashlightBeenEnabled = pHL2Player->GetFlashlightHasBeenEnabled();
		if (bHasFlashlightBeenEnabled)
		{
			// If the player has the flashlight, achievement fails
			SetFailed();
		}

		BaseClass::OnEvaluationEvent();
	}
};
DECLARE_ACHIEVEMENT(CAchievementEstrangedFinishGameWithoutPickingUpFlashlight, ACHIEVEMENT_EA1_ESTRANGED_IN_THE_DARK, "EA1_ESTRANGED_IN_THE_DARK", 15);

class CAchievementEstrangedEmptyVendingMachines : public CBaseAchievement
{
	void Init()
	{
		static const char *szComponents[] =
		{
			"EA1_VEND_RANCH", "EA1_VEND_LUNCHROOM1", "EA1_VEND_LUNCHROOM2", "EA1_VEND_SECONDFLOOR1", "EA1_VEND_SECONDFLOOR2"
		};
		SetFlags(ACH_HAS_COMPONENTS | ACH_LISTEN_COMPONENT_EVENTS | ACH_SAVE_GLOBAL);
		m_pszComponentNames = szComponents;
		m_iNumComponents = ARRAYSIZE(szComponents);
		SetComponentPrefix("EA1_VEND_");
		SetStoreProgressInSteam(true);
		SetGoal(m_iNumComponents);
	}
};
DECLARE_ACHIEVEMENT(CAchievementEstrangedEmptyVendingMachines, ACHIEVEMENT_EA1_VENDAHOLIC, "EA1_VENDAHOLIC", 15);

DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_EA1_MORE_THAN_SHIPWRECKED, "EA1_MORE_THAN_SHIPWRECKED", 25);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_EA1_MARKSMAN, "EA1_MARKSMAN", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_EA1_POWER_CONSERVER, "EA1_POWER_CONSERVER", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_EA1_CAT_LOVER, "EA1_CAT_LOVER", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_EA1_WATER_COLLECTOR, "EA1_WATER_COLLECTOR", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_EA1_HOUDINI, "EA1_HOUDINI", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_EA1_EVENING_NEWS, "EA1_EVENING_NEWS", 15);

#endif