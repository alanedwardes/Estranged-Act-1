/*
 *	This interface allows us to communicate between the client and the shader API
 *	Written by: Christiaan Bakker (AniCator)
 *	Based on: Biohazard's deferred extension interface
*/

#ifndef I_SHADERAPI_EXT_H
#define I_SHADERAPI_EXT_H

#ifdef CLIENT_DLL
#include "interface.h"
#include "../../materialsystem/stdshaders/interface_typedefs.h"
#else
#include "../public/tier1/interface.h"
#include "interface_typedefs.h"
#endif

class IShaderAPIExtension : public IBaseInterface
{
public:
	virtual void DebugPingInterface() = 0;
	virtual int GetNumberWorldLights() = 0;
	virtual void CommitWorldLightsSize(int size) = 0;

	virtual CUtlVector< d_ae_shaderlight_t > &GetLights() = 0;
	virtual void SetLights( CUtlVector< d_ae_shaderlight_t > &lightList ) = 0;

	virtual void SetEyeFocusDistance( float FocusDistance) = 0;
	virtual float GetEyeFocusDistance() = 0;
private:
	int m_nWorldLights;
	float m_flFocusDistance;
	CUtlVector< d_ae_shaderlight_t > shaderLights;
};

#define SHADERAPI_EXTENSION_VERSION "ShaderApiExtensionVersion001"

#ifdef STDSHADER_DX9_DLL_EXPORT

class CShaderAPIExtension : public IShaderAPIExtension
{
public:
	CShaderAPIExtension();
	~CShaderAPIExtension();

	virtual void DebugPingInterface();
	virtual int GetNumberWorldLights();
	virtual void CommitWorldLightsSize(int size);

	virtual CUtlVector< d_ae_shaderlight_t > &GetLights();
	virtual void SetLights( CUtlVector< d_ae_shaderlight_t > &lightList );

	virtual void SetEyeFocusDistance( float FocusDistance);
	virtual float GetEyeFocusDistance();
private:
	int m_nWorldLights;
	float m_flFocusDistance;
	CUtlVector< d_ae_shaderlight_t > shaderLights;
};

#endif // #ifdef STDSHADER_DX9_DLL_EXPORT

#ifdef CLIENT_DLL
bool ConnectShaderAPIExt();
void ShutdownShaderAPIExt();

extern IShaderAPIExtension *GetShaderAPIExt();
#else
extern CShaderAPIExtension __g_shaderExt;
FORCEINLINE CShaderAPIExtension *GetShaderAPIExt()
{
	return &__g_shaderExt;
}
#endif

#endif // #ifndef I_SHADERAPI_EXT_H