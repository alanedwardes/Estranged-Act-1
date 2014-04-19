#include "basevsshader.h"

#include "ishaderapiext.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_depthoffield_ps30.inc"

static ConVar ae_dof_radius("ae_dof_radius","5.0f");
static ConVar ae_dof_debug_vis("ae_dof_debug_vis","0");
static ConVar ae_dof_focallength("ae_dof_focallength","10");
static ConVar ae_dof_fstop("ae_dof_fstop","6");

BEGIN_VS_SHADER( AEDOF, "Estranged Depth of Field Shader" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM( BASETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB0", "" )
	SHADER_PARAM( DEPTHTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_SSAO_depth", "" )
	SHADER_PARAM( RANGE, SHADER_PARAM_TYPE_BOOL, "0", "" )
	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
{
}

SHADER_INIT
{
	LoadTexture( BASETEXTURE );
	LoadTexture( DEPTHTEXTURE );
}

SHADER_FALLBACK
{
	return 0;
}

SHADER_DRAW
{
	SHADOW_STATE
	{
		pShaderShadow->EnableDepthWrites( false );

		//Enable sampler slots
		pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

		int vertexFlags = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( vertexFlags, 1, NULL, 0 );

		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		DECLARE_STATIC_PIXEL_SHADER( ae_depthoffield_ps30 );
		SET_STATIC_PIXEL_SHADER( ae_depthoffield_ps30 );

		//DefaultFog();
	}
	DYNAMIC_STATE
	{
		//Bind FBTEXTURE to texture slot s0
		BindTexture( SHADER_SAMPLER0, BASETEXTURE, -1 );
		BindTexture( SHADER_SAMPLER1, DEPTHTEXTURE, -1 );

		int nWidth, nHeight;
		pShaderAPI->GetBackBufferDimensions( nWidth, nHeight );

		float focus_distance = GetShaderAPIExt()->GetEyeFocusDistance();
		float focusInfo[4] = {
			focus_distance,
			params[RANGE]->GetFloatValue(),
			nWidth,
			nHeight
		};
		pShaderAPI->SetPixelShaderConstant( 0, focusInfo, 1 );
		
		float f_length	= ae_dof_focallength.GetFloat();
		float f_stop		= ae_dof_fstop.GetFloat();

		float subject_magnification = (f_length / (GetShaderAPIExt()->GetEyeFocusDistance() - f_length));
		float blur_coefficient = ( f_length * subject_magnification / f_stop );
		float bokehInfo[2] = { 
			ae_dof_radius.GetFloat(),
			blur_coefficient
		};
		pShaderAPI->SetPixelShaderConstant( 1, bokehInfo, 1 );

		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( ae_depthoffield_ps30 );
		SET_DYNAMIC_PIXEL_SHADER_COMBO( DEBUG_VIS, ae_dof_debug_vis.GetBool() );
		SET_DYNAMIC_PIXEL_SHADER( ae_depthoffield_ps30 );
	}
	Draw();
}
END_SHADER