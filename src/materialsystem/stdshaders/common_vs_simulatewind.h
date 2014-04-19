#ifndef COMMON_VS_SIMULATEWIND_H
#define COMMON_VS_SIMULATEWIND_H

const float cShaderTime : register( c217 );
const float cSimulateWindAmount : register( c218 );

// If adding parameters etc, method used in:
// - sdk_vertexlit_and_unlit_generic_bump_vs20.fxc
// - sdk_vertexlit_and_unlit_generic_vs20.fxc
float3 SimulateWind(float3 worldPos)
{
	worldPos.x += sin(cShaderTime + worldPos.y) * cSimulateWindAmount;
	worldPos.y += cos((cShaderTime + worldPos.x + 1.2f) / 2) * cSimulateWindAmount;
	return worldPos;
}

#endif