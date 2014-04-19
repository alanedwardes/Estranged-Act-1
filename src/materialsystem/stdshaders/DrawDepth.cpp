#include "basevsshader.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "DrawDepth_ps20.inc"

ConVar ae_experimental_depth_range("ae_experimental_depth_range","1.0");

BEGIN_VS_SHADER( AEDrawDepth, "Estranged Depth Shader" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM( BASETEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameDepth", "" )
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

		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		DECLARE_STATIC_PIXEL_SHADER( drawdepth_ps20 );
		SET_STATIC_PIXEL_SHADER( drawdepth_ps20 );

		//DefaultFog();
	}
	DYNAMIC_STATE
	{
		//Bind FBTEXTURE to texture slot s0
		BindTexture( SHADER_SAMPLER0, BASETEXTURE, -1 );

		float divVal = ae_experimental_depth_range.GetFloat();
		if (divVal < 1.0f)
			divVal = 1.0f;
		float division[1] = { divVal };
		pShaderAPI->SetPixelShaderConstant(0,division,1);

		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( drawdepth_ps20 );
		SET_DYNAMIC_PIXEL_SHADER( drawdepth_ps20 );
	}
	Draw();
}
END_SHADER