#ifndef ESTRANGED_SYSTEM_CAPS_H
#define ESTRANGED_SYSTEM_CAPS_H

enum EstrangedSystemCapsFlags_t
{
	CAPS_MATERIAL_POSTPROCESS	= 0x1,
	CAPS_SHADER_POSTPROCESS		= 0x2,
	CAPS_ESTRANGED_DEPTHPASS	= 0x4,
	CAPS_SHADOW_DEPTHPASS		= 0x8
};

class CEstrangedSystemCaps
{
public:
	static bool HasCaps( int );
	static int GetCaps( void );
};

#endif // ESTRANGED_SYSTEM_CAPS_H