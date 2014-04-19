#include "cbase.h"
#include "colorgrade_manager.h"

//Singleton access
static CColorGradingMgr s_ColorGradingMgr;
CColorGradingMgr *g_pColorGradingMgr = &s_ColorGradingMgr;

CColorGradingMgr::CColorGradingMgr() : BaseClass( "CColorGradingMgr" )
{

}

CColorGradingMgr::~CColorGradingMgr()
{

}

void CColorGradingMgr::AddGrader(C_EnvColorGrade *pC_EnvColorGrade)
{
	m_pGraders.AddToHead(pC_EnvColorGrade);
}

void CColorGradingMgr::RemoveGrader(C_EnvColorGrade *pC_EnvColorGrade)
{
	int index = m_pGraders.Find(pC_EnvColorGrade);
	if (index >= 0)
	{
		m_pGraders.Remove(index);
	}
}

// -------------------------------------------
// Purpose: Initialize
// Author: AniCator
// -------------------------------------------
bool CColorGradingMgr::Init()
{
	return true;
}

// -------------------------------------------
// Purpose: Shutdown
// Author: AniCator
// -------------------------------------------
void CColorGradingMgr::Shutdown()
{
}

// -------------------------------------------
// Purpose: Checks which grader to use for the color grading effect
// Author: AniCator
// -------------------------------------------
void CColorGradingMgr::SelectGrader()
{
	C_EnvColorGrade *pClosestGrader = NULL;
	C_EnvColorGrade *pGlobalGrader = NULL;
	for(int i=0;i<m_pGraders.Count();i++)
	{
		C_EnvColorGrade *grader = m_pGraders[i];

		if(grader->IsGlobalGrader() && !pGlobalGrader)
		{
			pGlobalGrader = grader;
		}

		C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();

		vec_t flDistanceFromPlayer = 0.0f;
		flDistanceFromPlayer = player->GetAbsOrigin().DistTo(grader->GetAbsOrigin());
		if(flDistanceFromPlayer < grader->GetInfluenceRadius())
		{
			if(pClosestGrader)
			{
				if(player->GetAbsOrigin().DistTo(pClosestGrader->GetAbsOrigin()) > flDistanceFromPlayer)
				{
					pClosestGrader = grader;
				}
			}
			else
			{
				pClosestGrader = grader;
			}
		}
	}

	ConVarRef ae_colorgrading("ae_colorgrading");
	if(pClosestGrader)
	{
		m_ColorGradingData = pClosestGrader->GetColorGradingData();
	}
	else
	{
		if(pGlobalGrader)
		{
			m_ColorGradingData = pGlobalGrader->GetColorGradingData();
		}
		else
		{
			ColorGradingData_t defaultData;
			defaultData.brightness = Vector(1.0f,1.0f,1.0f);
			defaultData.contrast = Vector(1.0f,1.0f,1.0f);

			defaultData.levels_r_min_input = 0.0;
			defaultData.levels_r_max_input = 1.0;
			defaultData.levels_r_min_output = 0.0;
			defaultData.levels_r_max_output = 1.0;

			defaultData.saturation = 1.0f;
			defaultData.gamma = 1.0f;

			m_ColorGradingData = defaultData;
		}
	}
	BlendGradingData();
}

static ConVar ae_colorgrading_fadetime("ae_colorgrading_fadetime","0.5");
int _BlendIntValue(int value_old, int value_new)
{	
	return Approach( value_new, value_old, gpGlobals->frametime * ae_colorgrading_fadetime.GetFloat() * 255 );
}

float _BlendFloatValue(float value_old, float value_new)
{
	return Approach( value_new, value_old, gpGlobals->frametime * ae_colorgrading_fadetime.GetFloat() );
}

color32 _BlendLevels(color32 levels_old, color32 levels_new)
{
	//Blend levels and output color32
	color32 result;
	result.r = _BlendIntValue(levels_old.r, levels_new.r);
	result.g = _BlendIntValue(levels_old.g, levels_new.g);
	result.b = _BlendIntValue(levels_old.b, levels_new.b);
	result.a = _BlendIntValue(levels_old.a, levels_new.a);
	
	return result;
}

Vector _BlendVector(Vector vector_old, Vector vector_new)
{
	//Blend levels and output color32
	Vector result = Vector(0,0,0);
	result.x = _BlendFloatValue(vector_old.x, vector_new.x);
	result.y = _BlendFloatValue(vector_old.y, vector_new.y);
	result.z = _BlendFloatValue(vector_old.z, vector_new.z);

	return result;
}

//I'm Ron Burgundy?
void CColorGradingMgr::BlendGradingData()
{
	ColorGradingData_t blendedGradingData;
	blendedGradingData.brightness = _BlendVector(m_BlendedColorGradingData.brightness,m_ColorGradingData.brightness);
	blendedGradingData.contrast = _BlendVector(m_BlendedColorGradingData.contrast,m_ColorGradingData.contrast);

	blendedGradingData.levels_r_min_input = _BlendFloatValue(m_BlendedColorGradingData.levels_r_min_input,m_ColorGradingData.levels_r_min_input);
	blendedGradingData.levels_r_max_input = _BlendFloatValue(m_BlendedColorGradingData.levels_r_max_input,m_ColorGradingData.levels_r_max_input);
	blendedGradingData.levels_r_min_output = _BlendFloatValue(m_BlendedColorGradingData.levels_r_min_output,m_ColorGradingData.levels_r_min_output);
	blendedGradingData.levels_r_max_output = _BlendFloatValue(m_BlendedColorGradingData.levels_r_max_output,m_ColorGradingData.levels_r_max_output);

	blendedGradingData.levels_g_min_input = _BlendFloatValue(m_BlendedColorGradingData.levels_g_min_input,m_ColorGradingData.levels_g_min_input);
	blendedGradingData.levels_g_max_input = _BlendFloatValue(m_BlendedColorGradingData.levels_g_max_input,m_ColorGradingData.levels_g_max_input);
	blendedGradingData.levels_g_min_output = _BlendFloatValue(m_BlendedColorGradingData.levels_g_min_output,m_ColorGradingData.levels_g_min_output);
	blendedGradingData.levels_g_max_output = _BlendFloatValue(m_BlendedColorGradingData.levels_g_max_output,m_ColorGradingData.levels_g_max_output);

	blendedGradingData.levels_b_min_input = _BlendFloatValue(m_BlendedColorGradingData.levels_b_min_input,m_ColorGradingData.levels_b_min_input);
	blendedGradingData.levels_b_max_input = _BlendFloatValue(m_BlendedColorGradingData.levels_b_max_input,m_ColorGradingData.levels_b_max_input);
	blendedGradingData.levels_b_min_output = _BlendFloatValue(m_BlendedColorGradingData.levels_b_min_output,m_ColorGradingData.levels_b_min_output);
	blendedGradingData.levels_b_max_output = _BlendFloatValue(m_BlendedColorGradingData.levels_b_max_output,m_ColorGradingData.levels_b_max_output);

	blendedGradingData.saturation = _BlendFloatValue(m_BlendedColorGradingData.saturation,m_ColorGradingData.saturation);
	blendedGradingData.gamma = _BlendFloatValue(m_BlendedColorGradingData.gamma,m_ColorGradingData.gamma);

	m_BlendedColorGradingData = blendedGradingData;
}

ColorGradingData_t CColorGradingMgr::GetColorGradingData()
{
	return m_BlendedColorGradingData;
}

void CColorGradingMgr::Cleanup()
{
	m_pGraders.RemoveAll();
}

// -------------------------------------------
// Purpose: Update
// Author: AniCator
// -------------------------------------------
void CColorGradingMgr::Update( float frametime )
{
	if(!engine->IsInGame())
	{
		Cleanup();
		return;
	}

	SelectGrader();
}