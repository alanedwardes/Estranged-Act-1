#include "common_ps_fxc.h"
#include "interface_typedefs_fxc.h"

// -------------------------------------------
// Purpose: Calculate specularity according to Blinn-Phong model
// Author: AniCator
// -------------------------------------------
float3 CalculatePhongSpecular_Lightmapped(float3 eyePos, float3 vNormal, float3 worldPos, float projPosZ, float m_fPhongExponent, float m_fFresnelIntensity, d_ae_shaderlight_t lightInfo)
{
	float3 viewVector = normalize(eyePos - worldPos);
	float3 normalVector = normalize(vNormal);

	float3 lightVector = normalize(lightInfo.pos.xyz - worldPos);
	float3 halfVector = normalize(lightVector + viewVector);
	
	// Cosines
	float NdotL = saturate(dot( normalVector, lightVector ));
	float NdotH = saturate(dot( normalVector, halfVector ));
	float LdotH = saturate(dot( lightVector,  halfVector  ));

	// Conserve energy along our specular curve
	float3 lightSpecular = ((m_fPhongExponent + 2.0f) / (8.0f * 3.14159265f)) *  pow( NdotH, m_fPhongExponent );
	lightSpecular *= lightInfo.color.rgb;

	// m_fFresnelIntensity actually controls the brightness of the phong highlight, it should be clamped between 0 and 1.0
	float f0 = saturate( m_fFresnelIntensity );
	
	// Spherical Gaussian ( faster )
	float fresnel = f0 + (1.0 - f0) * exp2((-5.55473 * LdotH - 6.98316) * LdotH);
	
	lightSpecular *= fresnel;
	lightSpecular *= NdotL;

	// Get light distance.
	float lightDist = distance(lightInfo.pos.xyz, worldPos);
	lightDist += 1.e-6f;

	//Calculate attenuation (fixed distance)
	float lightAtten = 1000.0f - lightDist;
	lightAtten /= 1000.0f;
	lightAtten = saturate(lightAtten);

	lightSpecular *= lightAtten;
	lightSpecular *= 0.005f;

	return lightSpecular;
}