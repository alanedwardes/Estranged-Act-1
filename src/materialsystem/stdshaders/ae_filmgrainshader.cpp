#include "basevsshader.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_filmgrain_ps20.inc"

BEGIN_VS_SHADER( AEFilmGrain, "Estranged Film Grain Shader" )

	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( NOISETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_SmallFB0", "" )
		SHADER_PARAM( TIME, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
		SHADER_PARAM( NOISEAMOUNT, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( NOISEFALLOFF, SHADER_PARAM_TYPE_FLOAT, "60.0", "" )
	END_SHADER_PARAMS

SHADER_INIT_PARAMS()
{
	SET_PARAM_FLOAT_IF_NOT_DEFINED( TIME, 0.0f );
	SET_PARAM_FLOAT_IF_NOT_DEFINED( NOISEAMOUNT, 1.0f );
	SET_PARAM_FLOAT_IF_NOT_DEFINED( NOISEFALLOFF, 60.0f);
}

SHADER_INIT
{
	LoadTexture( FBTEXTURE );
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
		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

		int vertexFlags = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( vertexFlags, 1, NULL, 0 );

		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		DECLARE_STATIC_PIXEL_SHADER( ae_filmgrain_ps20 );
		SET_STATIC_PIXEL_SHADER( ae_filmgrain_ps20 );

		//DefaultFog();
	}
	DYNAMIC_STATE
	{
		//Bind FBTEXTURE to texture slot s0
		BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
		BindTexture( SHADER_SAMPLER1, NOISETEXTURE, -1 );

		params[TIME]->SetFloatValue(pShaderAPI->CurrentTime());

		float timeInput[1] = { params[TIME]->GetFloatValue() };
		pShaderAPI->SetPixelShaderConstant( 0, timeInput, 1 );

		float noise[1] = { params[NOISEAMOUNT]->GetFloatValue() };
		pShaderAPI->SetPixelShaderConstant( 1, noise, 1 );

		float noisefalloff[1] = { params[NOISEFALLOFF]->GetFloatValue() };
		pShaderAPI->SetPixelShaderConstant( 2, noisefalloff, 1);

		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( ae_filmgrain_ps20 );
		SET_DYNAMIC_PIXEL_SHADER( ae_filmgrain_ps20 );
	}
	Draw();
}
END_SHADER