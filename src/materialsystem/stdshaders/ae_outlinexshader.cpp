#include "basevsshader.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_outlinex_ps20.inc"

BEGIN_VS_SHADER( AEOutlineX, "Estranged Outline Shader" )

	BEGIN_SHADER_PARAMS
		SHADER_PARAM( BASETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( GLOWTINT, SHADER_PARAM_TYPE_COLOR, "", "" )
	END_SHADER_PARAMS

SHADER_INIT_PARAMS()
{
}

SHADER_INIT
{
	LoadTexture( BASETEXTURE );
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

		int vertexFlags = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( vertexFlags, 1, NULL, 0 );

		// No need to use ae_ssao_vs20 because sdk_screenspaceeffect_vs20 exists
		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		DECLARE_STATIC_PIXEL_SHADER( ae_outlinex_ps20 );
		SET_STATIC_PIXEL_SHADER( ae_outlinex_ps20 );
	}
	DYNAMIC_STATE
	{
		// Bind FBTEXTURE to texture slot s0
		BindTexture( SHADER_SAMPLER0, BASETEXTURE);

		// Bind the glow tint
		float glowtint[4] = { 1, 1, 1, 1 };
		params[GLOWTINT]->GetVecValue(glowtint, 4);

		// Set the glow tint constant
		pShaderAPI->SetPixelShaderConstant(0, glowtint);

		// No need to use ae_ssao_vs20 because sdk_screenspaceeffect_vs20 exists
		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( ae_outlinex_ps20 );
		SET_DYNAMIC_PIXEL_SHADER( ae_outlinex_ps20 );
	}
	Draw();
}
END_SHADER