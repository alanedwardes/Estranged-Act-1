#include "basevsshader.h"

//Ambient Occlusion Combine Shader

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_ssao_combine_ps30.inc"

BEGIN_VS_SHADER( AESSAOCombine, "Estranged SSAO Combine Shader" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM( BASETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" ) //framebuffer
	SHADER_PARAM( SSAOTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" ) //SSAO material
	END_SHADER_PARAMS

	SHADER_FALLBACK
{
	return 0;
}

SHADER_INIT
{
	LoadTexture( BASETEXTURE );
	LoadTexture( SSAOTEXTURE );
}

SHADER_DRAW
{
	SHADOW_STATE
	{
		pShaderShadow->EnableDepthWrites( false );
		pShaderShadow->EnableDepthTest( false );
		pShaderShadow->EnableAlphaWrites( false );
		pShaderShadow->EnableBlending( false );
		pShaderShadow->EnableCulling( false );

		pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

		int fmt = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( fmt, 1, NULL, 0 );

		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20  );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20  );

		DECLARE_STATIC_PIXEL_SHADER( ae_ssao_combine_ps30 );
		SET_STATIC_PIXEL_SHADER( ae_ssao_combine_ps30 );
	}
	DYNAMIC_STATE
	{
		BindTexture( SHADER_SAMPLER0, BASETEXTURE, -1 );
		BindTexture( SHADER_SAMPLER1, SSAOTEXTURE, -1 );

		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( ae_ssao_combine_ps30 );
		SET_DYNAMIC_PIXEL_SHADER( ae_ssao_combine_ps30 );
	}
	Draw();
}
END_SHADER