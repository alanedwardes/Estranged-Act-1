#include "basevsshader.h"

//Ambient Occlusion Blurring Shader (based on depth, screenspace X-axis)
//Did you know that the Ambient Occlusion shader is secretly HBAO and not SSAO?
//Renaming files is a bitch... so yeah... HBAO.. but shhhh
// - written by dat catormeister guy he s0 cewl
// special thanks to the awesome shader god that provided us with the Fire of Knowledge
// sir mister Drew Watts

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_ssao_blurx_ps30.inc"

//Allows us to define how sharp the blur will be
ConVar ae_ssao_blursharpness("ae_ssao_blursharpness","10");

BEGIN_VS_SHADER( AESSAOBlurX, "Estranged SSAO Depth Blur X Shader" )

	BEGIN_SHADER_PARAMS
		SHADER_PARAM( BASETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" ) //SSAO depth info
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

		DECLARE_STATIC_PIXEL_SHADER( ae_ssao_blurx_ps30 );
		SET_STATIC_PIXEL_SHADER( ae_ssao_blurx_ps30 );
	}
	DYNAMIC_STATE
	{
		BindTexture( SHADER_SAMPLER0, BASETEXTURE, -1 );
		BindTexture( SHADER_SAMPLER1, SSAOTEXTURE, -1 );

		int nScreenWidth, nScreenHeight;
		pShaderAPI->GetBackBufferDimensions( nScreenWidth, nScreenHeight );
		float vScreenSize[4] = { 1.0f / (float) nScreenWidth, 1.0f / (float) nScreenHeight, (float)nScreenWidth, (float)nScreenHeight };
		pShaderAPI->SetPixelShaderConstant( 0, vScreenSize, 1 );

		float vBlurSharpness[1] = {ae_ssao_blursharpness.GetFloat()};
		pShaderAPI->SetPixelShaderConstant( 1,  vBlurSharpness,1 );

		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( ae_ssao_blurx_ps30 );
		SET_DYNAMIC_PIXEL_SHADER( ae_ssao_blurx_ps30 );
	}
	Draw();
}
END_SHADER