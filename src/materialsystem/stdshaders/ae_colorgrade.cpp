#include "basevsshader.h"

#include "sdk_screenspaceeffect_vs20.inc"
#include "ae_colorgrade_ps20.inc"

BEGIN_VS_SHADER( AEColorGrade, "Estranged Color Grading Shader" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
	SHADER_PARAM( CONTRAST, SHADER_PARAM_TYPE_COLOR, "[1.0 1.0 1.0]", "" )
	SHADER_PARAM( SATURATION, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	SHADER_PARAM( GAMMA_AMOUNT, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	SHADER_PARAM( BRIGHTNESS, SHADER_PARAM_TYPE_VEC4, "[1.0 1.0 1.0 1.0]", "" )

	SHADER_PARAM( levels_r_min_input, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	SHADER_PARAM( levels_r_max_input, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	SHADER_PARAM( levels_r_min_output, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	SHADER_PARAM( levels_r_max_output, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )

	SHADER_PARAM( levels_g_min_input, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	SHADER_PARAM( levels_g_max_input, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	SHADER_PARAM( levels_g_min_output, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	SHADER_PARAM( levels_g_max_output, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )

	SHADER_PARAM( levels_b_min_input, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	SHADER_PARAM( levels_b_max_input, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	SHADER_PARAM( levels_b_min_output, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	SHADER_PARAM( levels_b_max_output, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
{
}

SHADER_INIT
{
	//LoadTexture( FBTEXTURE );
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

		int vertexFlags = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat( vertexFlags, 1, NULL, 0 );

		DECLARE_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_STATIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		//Set Pixel shader
		DECLARE_STATIC_PIXEL_SHADER( ae_colorgrade_ps20 );
		SET_STATIC_PIXEL_SHADER( ae_colorgrade_ps20 );
	}
	DYNAMIC_STATE
	{
		//BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
		pShaderAPI->BindStandardTexture( SHADER_SAMPLER0, TEXTURE_FRAME_BUFFER_FULL_TEXTURE_0 );

		
		const float *contrastVector = params[CONTRAST]->GetVecValue();
		float contrastInfo[4];
		contrastInfo[0] = contrastVector[0];
		contrastInfo[1] = contrastVector[1];
		contrastInfo[2] = contrastVector[2];
		contrastInfo[3] = params[SATURATION]->GetFloatValue();

		const float *brightnessVector = params[BRIGHTNESS]->GetVecValue();
		float brightnessInfo[4];
		brightnessInfo[0] = brightnessVector[0];
		brightnessInfo[1] = brightnessVector[1];
		brightnessInfo[2] = brightnessVector[2];
		brightnessInfo[3] = params[GAMMA_AMOUNT]->GetFloatValue();
		
		const float levelsVectorR[4] = {
			params[levels_r_min_input]->GetFloatValue(),
			params[levels_r_max_input]->GetFloatValue(),
			params[levels_r_min_output]->GetFloatValue(),
			params[levels_r_max_output]->GetFloatValue(),
		};
		const float levelsVectorG[4] = {
			params[levels_g_min_input]->GetFloatValue(),
			params[levels_g_max_input]->GetFloatValue(),
			params[levels_g_min_output]->GetFloatValue(),
			params[levels_g_max_output]->GetFloatValue(),
		};
		const float levelsVectorB[4] = {
			params[levels_b_min_input]->GetFloatValue(),
			params[levels_b_max_input]->GetFloatValue(),
			params[levels_b_min_output]->GetFloatValue(),
			params[levels_b_max_output]->GetFloatValue(),
		};

		pShaderAPI->SetPixelShaderConstant( 0, contrastInfo, 1 );
		pShaderAPI->SetPixelShaderConstant( 1, brightnessInfo, 1 );
		pShaderAPI->SetPixelShaderConstant( 2, levelsVectorR, 1 );
		pShaderAPI->SetPixelShaderConstant( 3, levelsVectorG, 1 );
		pShaderAPI->SetPixelShaderConstant( 4, levelsVectorB, 1 );

		DECLARE_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );
		SET_DYNAMIC_VERTEX_SHADER( sdk_screenspaceeffect_vs20 );

		DECLARE_DYNAMIC_PIXEL_SHADER( ae_colorgrade_ps20 );
		SET_DYNAMIC_PIXEL_SHADER( ae_colorgrade_ps20 );
	}
	Draw();
}
END_SHADER