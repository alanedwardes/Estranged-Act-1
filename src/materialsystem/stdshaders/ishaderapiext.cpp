#include "commandbuilder.h"
#include "convar.h"

#include "ishaderapiext.h"

CShaderAPIExtension __g_shaderExt;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CShaderAPIExtension, IShaderAPIExtension, SHADERAPI_EXTENSION_VERSION, __g_shaderExt );

CShaderAPIExtension::CShaderAPIExtension()
{
}

CShaderAPIExtension::~CShaderAPIExtension()
{

}

#ifdef _DEBUG
void CShaderAPIExtension::DebugPingInterface()
{
	DevMsg("CShaderAPIExtension is alive.\n");
}
#else
void CShaderAPIExtension::DebugPingInterface()
{
	
}
#endif

int CShaderAPIExtension::GetNumberWorldLights()
{
	return m_nWorldLights;
}

void CShaderAPIExtension::CommitWorldLightsSize(int size)
{
	m_nWorldLights = size;
}

CUtlVector<d_ae_shaderlight_t> &CShaderAPIExtension::GetLights()
{
	return shaderLights;
}

void CShaderAPIExtension::SetLights(CUtlVector<d_ae_shaderlight_t> &lightList)
{
	shaderLights = lightList;
}

void CShaderAPIExtension::SetEyeFocusDistance(float FocusDistance)
{
	m_flFocusDistance = FocusDistance;
}

float CShaderAPIExtension::GetEyeFocusDistance()
{
	return m_flFocusDistance;
}