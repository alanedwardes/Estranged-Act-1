#include "basevsshader.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_lensflare_ps20.inc"
#include "ae_lensflare_ps20b.inc"

ConVar ae_lensflare_intensity( "ae_lensflare_intensity", "1.0", FCVAR_ARCHIVE);

BEGIN_VS_SHADER( AELensFlare, "Estranged Lens Flare Shader" )

	BEGIN_SHADER_PARAMS
		//SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_ssao_depth", "SSAO Depth" )
		SHADER_PARAM( BLOOMTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_SmallFB0", "" )
		SHADER_PARAM( DIRTTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "overlays/dust01", "" )
		//SHADER_PARAM( NOISEMAP, SHADER_PARAM_TYPE_TEXTURE, "effects/haloNoise", "" )
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
	END_SHADER_PARAMS

SHADER_INIT_PARAMS()
{
}

SHADER_INIT
{
	LoadTexture( BLOOMTEXTURE );
	LoadTexture( FBTEXTURE );
	LoadTexture( DIRTTEXTURE );
	//LoadTexture( NOISEMAP );
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
		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
		//pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
		pShaderShadow->EnableTexture( SHADER_SAMPLER3, true );

		int vertexFlags = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( vertexFlags, 1, NULL, 0 );

		//Set Vertex shader
		/*DECLARE_STATIC_VERTEX_SHADER( ae_ssao_vs20 );
		SET_STATIC_VERTEX_SHADER( ae_ssao_vs20 );*/

		// No need to use ae_ssao_vs20 because sdk_screenspaceeffect_vs20 exists
		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		if ( g_pHardwareConfig->SupportsPixelShaders_2_b() )
		{
			DECLARE_STATIC_PIXEL_SHADER( ae_lensflare_ps20b );
			SET_STATIC_PIXEL_SHADER( ae_lensflare_ps20b );
		}
		else
		{
			DECLARE_STATIC_PIXEL_SHADER( ae_lensflare_ps20 );
			SET_STATIC_PIXEL_SHADER( ae_lensflare_ps20 );
		}
		

		//DefaultFog();
	}
	DYNAMIC_STATE
	{
		//Bind FBTEXTURE to texture slot s0
		BindTexture( SHADER_SAMPLER0, BLOOMTEXTURE, -1 );
		BindTexture( SHADER_SAMPLER1, DIRTTEXTURE, -1 );
		BindTexture( SHADER_SAMPLER3, FBTEXTURE, -1 );

		float flFlareIntensity[1] = { ae_lensflare_intensity.GetFloat() };
		pShaderAPI->SetPixelShaderConstant(0,flFlareIntensity,1);

		// No need to use ae_ssao_vs20 because sdk_screenspaceeffect_vs20 exists
		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		if ( g_pHardwareConfig->SupportsPixelShaders_2_b() )
		{
			DECLARE_DYNAMIC_PIXEL_SHADER( ae_lensflare_ps20b );
			SET_DYNAMIC_PIXEL_SHADER( ae_lensflare_ps20b );
		}
		else
		{
			DECLARE_DYNAMIC_PIXEL_SHADER( ae_lensflare_ps20 );
			SET_DYNAMIC_PIXEL_SHADER( ae_lensflare_ps20 );
		}
	}
	Draw();
}
END_SHADER