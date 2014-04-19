#include "basevsshader.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_screentension_ps20b.inc"

BEGIN_VS_SHADER( AEScreenTension, "Estranged Screen Tension Shader" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
	SHADER_PARAM( SMALLFB, SHADER_PARAM_TYPE_TEXTURE, "_rt_SmallFB0", "" )
	SHADER_PARAM( TIME, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
{
	SET_PARAM_FLOAT_IF_NOT_DEFINED( TIME, 0.0f );
}

SHADER_INIT
{
	LoadTexture( FBTEXTURE );
	LoadTexture( SMALLFB );
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

		pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

		int vertexFlags = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( vertexFlags, 1, NULL, 0 );

		SET_FLAGS2(MATERIAL_VAR2_NEEDS_FULL_FRAME_BUFFER_TEXTURE);

		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		DECLARE_STATIC_PIXEL_SHADER( ae_screentension_ps20b );
		SET_STATIC_PIXEL_SHADER( ae_screentension_ps20b );

		//DefaultFog();
	}
	DYNAMIC_STATE
	{
		BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
		BindTexture( SHADER_SAMPLER1, SMALLFB, -1 );

		params[TIME]->SetFloatValue(pShaderAPI->CurrentTime());

		float shaderData[1];
		shaderData[0] = params[TIME]->GetFloatValue();
		pShaderAPI->SetPixelShaderConstant( 0, shaderData, 1 );

		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( ae_screentension_ps20b );
		SET_DYNAMIC_PIXEL_SHADER( ae_screentension_ps20b );
	}
	Draw();
}
END_SHADER