#include "basevsshader.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_ssao_ps30.inc"
//#include "ae_ssao_vs20.inc"

BEGIN_VS_SHADER( AESSAO, "Estranged SSAO Shader" )

	BEGIN_SHADER_PARAMS
		//SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( SSAOTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_SSAO_depth", "" )
		SHADER_PARAM( NOISETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "texture_samples\noise_ssao", "" )
		SHADER_PARAM( OCCLUSIONRADIUS, SHADER_PARAM_TYPE_FLOAT, "5.0", "" )
		SHADER_PARAM( ANGLEBIAS, SHADER_PARAM_TYPE_FLOAT, "5.0", "" )
		SHADER_PARAM( OCCLUSIONINTENSITY, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	END_SHADER_PARAMS

SHADER_INIT_PARAMS()
{
	SET_PARAM_FLOAT_IF_NOT_DEFINED(OCCLUSIONRADIUS,5.0f);
	SET_PARAM_FLOAT_IF_NOT_DEFINED(ANGLEBIAS,5.0f);
	SET_PARAM_FLOAT_IF_NOT_DEFINED(OCCLUSIONINTENSITY,1.0f);
}

SHADER_INIT
{
	LoadTexture( SSAOTEXTURE );
	LoadTexture( NOISETEXTURE );
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

		//Enable texture slot s0
		pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
		//Enable texture slot s1
		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

		int vertexFlags = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( vertexFlags, 1, NULL, 0 );

		//Set Vertex shader
		/*DECLARE_STATIC_VERTEX_SHADER( ae_ssao_vs20 );
		SET_STATIC_VERTEX_SHADER( ae_ssao_vs20 );*/

		// No need to use ae_ssao_vs20 because sdk_screenspaceeffect_vs20 exists
		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		/*if ( g_pHardwareConfig->SupportsPixelShaders_2_b() )
		{
			DECLARE_STATIC_PIXEL_SHADER( ae_ssao_ps20b );
			SET_STATIC_PIXEL_SHADER( ae_ssao_ps20b );
		}*/
		if ( g_pHardwareConfig->SupportsShaderModel_3_0() )
		{
			DECLARE_STATIC_PIXEL_SHADER( ae_ssao_ps30 );
			SET_STATIC_PIXEL_SHADER( ae_ssao_ps30 );
		}
		//DefaultFog();
	}
	DYNAMIC_STATE
	{
		//Bind SSAOTEXTURE to texture slot s0
		BindTexture( SHADER_SAMPLER0, SSAOTEXTURE, -1 );
		//Bind NOISETEXTURE to texture slot s1
		BindTexture( SHADER_SAMPLER1, NOISETEXTURE, -1 );

		int nScreenWidth, nScreenHeight;
		pShaderAPI->GetBackBufferDimensions( nScreenWidth, nScreenHeight );
		float vScreenSize[4] = { 1.0f / (float) nScreenWidth, 1.0f / (float) nScreenHeight, (float)nScreenWidth, (float)nScreenHeight };
		pShaderAPI->SetPixelShaderConstant( 2, vScreenSize, 1 );

		float occlusionRadius = params[OCCLUSIONRADIUS]->GetFloatValue();
		float vSettings[3] = { occlusionRadius, params[ANGLEBIAS]->GetFloatValue(), params[OCCLUSIONINTENSITY]->GetFloatValue() };
		pShaderAPI->SetPixelShaderConstant( 1, vSettings, 1 );

		/*DECLARE_DYNAMIC_VERTEX_SHADER( ae_ssao_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( ae_ssao_vs20 );*/

		// No need to use ae_ssao_vs20 because sdk_screenspaceeffect_vs20 exists
		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		//if ( g_pHardwareConfig->SupportsPixelShaders_2_b() )
		//{
		//	DECLARE_DYNAMIC_PIXEL_SHADER( ae_ssao_ps20b );
		//	SET_DYNAMIC_PIXEL_SHADER( ae_ssao_ps20b );
		//}
		//if ( g_pHardwareConfig->SupportsShaderModel_3_0() )
		//{
			DECLARE_DYNAMIC_PIXEL_SHADER( ae_ssao_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( ae_ssao_ps30 );
		//}
	}
	Draw();
}
END_SHADER