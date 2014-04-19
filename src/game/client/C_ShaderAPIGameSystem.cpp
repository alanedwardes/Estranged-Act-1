#include "cbase.h"

#include "../../materialsystem/stdshaders/ishaderapiext.h"
#include "worldlight.h"

class C_ShaderAPIGameSystem : public CAutoGameSystemPerFrame
{
	typedef CAutoGameSystemPerFrame BaseClass;
public:
	C_ShaderAPIGameSystem();
	~C_ShaderAPIGameSystem();

	virtual bool Init();
	virtual void Shutdown();

private:
	virtual void Update( float frametime );
};

static C_ShaderAPIGameSystem __g_shaderapigamesystem;
C_ShaderAPIGameSystem *GetShaderAPIGameSystem()
{
	return &__g_shaderapigamesystem;
}

C_ShaderAPIGameSystem::C_ShaderAPIGameSystem() : BaseClass( "CShaderAPIGameSystem" )
{

}

C_ShaderAPIGameSystem::~C_ShaderAPIGameSystem()
{

}

// -------------------------------------------
// Purpose: Initialize the Shader API extension and execute DebugPingInterface
// Author: AniCator
// -------------------------------------------
bool C_ShaderAPIGameSystem::Init()
{
	bool bGotDefShaderDll = ConnectShaderAPIExt();
	if(bGotDefShaderDll)
	{
		GetShaderAPIExt()->DebugPingInterface();
	}
	return true;
}

// -------------------------------------------
// Purpose: Shutdown the Shader API extension
// Author: AniCator
// -------------------------------------------
void C_ShaderAPIGameSystem::Shutdown()
{
	ShutdownShaderAPIExt();
}

ConVar ae_dof_focus_override("ae_dof_focus_override","0");
ConVar ae_dof_focus_override_distance("ae_dof_focus_override_distance","100.0");
ConVar ae_dof_focus_speed("ae_dof_focus_speed","300.0");
ConVar ae_dof_model_trace("ae_dof_model_trace","1");
float CalcDOF()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	// Estranged DOF Trace
	float flDofRay_Goal = 0;
	static float s_flDofRay_Last = 0;

	// TODO: Move this into its own method.
	ConVarRef ae_dof("ae_dof");
	if (ae_dof.GetBool())
	{
		if ( pPlayer )
		{
			trace_t tr;
			Vector vecAbsStart, vecAbsEnd, vecDir;
			AngleVectors( pPlayer->EyeAngles(), &vecDir );
			vecAbsStart = pPlayer->EyePosition();
			vecAbsEnd = vecAbsStart + (vecDir * MAX_TRACE_LENGTH);

			//Initially trace every object
			UTIL_TraceLine( vecAbsStart, vecAbsEnd, MASK_SOLID, pPlayer, COLLISION_GROUP_NONE, &tr );

			//In case this is a window or other transparent world object do another trace
			if(tr.m_pEnt && tr.m_pEnt->GetMoveType()==MOVETYPE_NONE)
			{
				CTraceFilterSkipTwoEntities filter(pPlayer,tr.m_pEnt,COLLISION_GROUP_NONE);
				UTIL_TraceLine( vecAbsStart, vecAbsEnd, (CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_OPAQUE), &filter, &tr );
			}

			if( ae_dof_model_trace.GetBool() )
			{
				UTIL_TraceModel(vecAbsStart, vecAbsEnd, Vector(0,0,0), Vector(0,0,0), tr.m_pEnt, COLLISION_GROUP_NONE, &tr);
			}

			Vector vecStart = tr.startpos;
			Vector vecEnd = tr.endpos;

			if( tr.fraction == 1.0 || (tr.surface.flags & SURF_SKY) )
			{
				vecEnd = vecAbsEnd;
			}

			flDofRay_Goal = (vecEnd - vecStart).Length();
		}

		if ( s_flDofRay_Last != flDofRay_Goal )
		{
			float flLerpSpeed = ae_dof_focus_speed.GetFloat();
			if(flDofRay_Goal > (MAX_TRACE_LENGTH / 2))
			{
				flDofRay_Goal = 5000; //clamp goal distance because interpolation would take ages when using the full trace length
			}
			s_flDofRay_Last = Approach( flDofRay_Goal, s_flDofRay_Last, gpGlobals->frametime * ( (!!flDofRay_Goal) ? flLerpSpeed : flLerpSpeed ) );
		}
	}

	if(ae_dof_focus_override.GetBool())
		s_flDofRay_Last = ae_dof_focus_override_distance.GetFloat();

	return s_flDofRay_Last;
}

// -------------------------------------------
// Purpose: Run the shader light update when the engine is running in-game.
// Author: AniCator
// -------------------------------------------
void C_ShaderAPIGameSystem::Update( float frametime )
{
	// ae - this is gone for now since phong is disabled
	// do not enable this without looking at threading issues,
	// they need fixing (ie asserts in utlvector).
	if( engine->IsInGame())
	{
		//g_pWorldLights->UpdateShaderLights();
		ConVarRef ae_dof("ae_dof");
		if(ae_dof.GetBool())
		{
			GetShaderAPIExt()->SetEyeFocusDistance(CalcDOF());
		}
	}
}