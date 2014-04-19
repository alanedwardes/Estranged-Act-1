#include "c_env_light_projected.h"

#ifndef C_ENV_LIGHT_PROJECTED_MANAGER_H
#define C_ENV_LIGHT_PROJECTED_MANAGER_H
#ifdef _WIN32
#pragma once
#endif

class C_EnvLightProjectedManager : public C_BaseEntity
{
	DECLARE_CLASS(C_EnvLightProjectedManager, C_BaseEntity);
public:
	DECLARE_CLIENTCLASS();

	virtual void AddLight(C_EnvLightProjected *light);
	virtual void RemoveLight(C_EnvLightProjected *light);
	virtual void ClientThink();
	virtual void Spawn();

	C_EnvLightProjectedManager();
	~C_EnvLightProjectedManager();

private:
	virtual void ReSort();
	virtual int GetBestCandidateForShadows(CUtlVector<int> *candidates, CUtlVector<int> *rejects);

	CUtlVector<C_EnvLightProjected*> m_lights;
	C_EnvLightProjectedManager* m_instance;
};

extern C_EnvLightProjectedManager *g_pEnvLightProjectedManager;

#endif