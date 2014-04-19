#include "cbase.h"
#include "estranged_system_caps.h"
#include "sourcevr/isourcevirtualreality.h"

// These constants match up
// with the constants in common_ps_fxc.h
#define NVIDIA_PCF_POISSON	0
#define ATI_NOPCF			1
#define ATI_NO_PCF_FETCH4	2

bool CEstrangedSystemCaps::HasCaps( int testCaps )
{
	int systemCaps = CEstrangedSystemCaps::GetCaps();

	return (testCaps & systemCaps) != 0;
}

int CEstrangedSystemCaps::GetCaps( void )
{
	int systemCaps = ( CAPS_MATERIAL_POSTPROCESS | CAPS_SHADER_POSTPROCESS | CAPS_ESTRANGED_DEPTHPASS | CAPS_SHADOW_DEPTHPASS );

	// If we're in the middle of building cubemaps
	ConVarRef building_cubemaps("building_cubemaps");
	if ( building_cubemaps.GetBool() )
	{
		systemCaps &= ~CAPS_ESTRANGED_DEPTHPASS;
		systemCaps &= ~CAPS_SHADER_POSTPROCESS;
		systemCaps &= ~CAPS_MATERIAL_POSTPROCESS;
	}

	// Check for null-ness...
	if ( g_pMaterialSystemHardwareConfig )
	{
		// If we don't support shader model 3.0
		if ( !g_pMaterialSystemHardwareConfig->SupportsShaderModel_3_0() )
		{
			systemCaps &= ~CAPS_ESTRANGED_DEPTHPASS;
		}

		// If we support fetch4 (ATI)
		if ( g_pMaterialSystemHardwareConfig->SupportsFetch4() )
		{
			systemCaps &= ~CAPS_ESTRANGED_DEPTHPASS;
		}

		// If we use ATI depth formats
		int shadowFilterMode = g_pMaterialSystemHardwareConfig->GetShadowFilterMode();
		if ( shadowFilterMode != NVIDIA_PCF_POISSON && ( shadowFilterMode == ATI_NO_PCF_FETCH4 || shadowFilterMode == ATI_NOPCF ) )
		{
			systemCaps &= ~CAPS_ESTRANGED_DEPTHPASS;
		}

		// If we're not running with DirectX 9
		if ( g_pMaterialSystemHardwareConfig->GetDXSupportLevel() < 90 )
		{
			systemCaps &= ~CAPS_SHADER_POSTPROCESS;
			systemCaps &= ~CAPS_ESTRANGED_DEPTHPASS;
			systemCaps &= ~CAPS_SHADOW_DEPTHPASS;
		}

		// If we're not running with DirectX 8
		if ( g_pMaterialSystemHardwareConfig->GetDXSupportLevel() < 80 )
		{
			systemCaps &= ~CAPS_MATERIAL_POSTPROCESS;
		}
	}

	// If we're not Windows
	if ( !IsWindows() )
	{
		systemCaps &= ~CAPS_ESTRANGED_DEPTHPASS;
	}

	// ae - temporary bugfix: disable shader
	// post processing on mac
	if ( IsOSX() )
	{
		systemCaps &= ~CAPS_SHADER_POSTPROCESS;
	}

	// If we're in VR mode
	if ( UseVR() )
	{
		systemCaps &= ~CAPS_ESTRANGED_DEPTHPASS;
		systemCaps &= ~CAPS_SHADER_POSTPROCESS;
	}

	// Check null
	if ( materials )
	{
		// If we don't support shadow depth textures
		if ( !materials->SupportsShadowDepthTextures() )
		{
			systemCaps &= ~CAPS_SHADOW_DEPTHPASS;
		}
	}

	return systemCaps;
}