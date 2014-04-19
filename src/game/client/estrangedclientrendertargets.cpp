#include "cbase.h"
#include "estrangedclientrendertargets.h"
#include "materialsystem/imaterialsystem.h"
#include "rendertexture.h"
#include "estranged_system_caps.h"

static int SSAOTextureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_NOLOD | TEXTUREFLAGS_RENDERTARGET;

#define YELLOW Color(255, 201, 14, 255)

ITexture* CEstrangedClientRenderTargets::CreateSSAODepthTexture()
{
	return m_pMaterialSystem->CreateNamedRenderTargetTextureEx(
		"_rt_SSAO_depth",
		m_iBackBufferWidth, m_iBackBufferHeight, RT_SIZE_NO_CHANGE,
		IMAGE_FORMAT_R32F,
		MATERIAL_RT_DEPTH_SHARED,
		SSAOTextureFlags,
		0);
}

ITexture* CEstrangedClientRenderTargets::CreateSSAOResultTexture()
{
	return m_pMaterialSystem->CreateNamedRenderTargetTextureEx(
		"_rt_ssao_result",
		m_iBackBufferWidth / 2, m_iBackBufferHeight / 2, RT_SIZE_NO_CHANGE,
		IMAGE_FORMAT_RGB888,
		MATERIAL_RT_DEPTH_NONE,
		SSAOTextureFlags,
		0);
}

ITexture* CEstrangedClientRenderTargets::CreateSSAOBlurYTexture()
{
	return m_pMaterialSystem->CreateNamedRenderTargetTextureEx(
		"_rt_ssao_blury",
		m_iBackBufferWidth, m_iBackBufferHeight, RT_SIZE_FULL_FRAME_BUFFER,
		IMAGE_FORMAT_RGB888,
		MATERIAL_RT_DEPTH_NONE,
		SSAOTextureFlags,
		0);
}

void CEstrangedClientRenderTargets::InitSSAORenderTargets()
{
	if (CEstrangedSystemCaps::HasCaps(CAPS_ESTRANGED_DEPTHPASS) && CEstrangedSystemCaps::HasCaps(CAPS_SHADER_POSTPROCESS))
	{
		ConDColorMsg(YELLOW, "CEstrangedClientRenderTargets::InitSSAORenderTargets()\n");

		m_SSAODepthTexture.Init(CreateSSAODepthTexture());
		m_SSAOResultTexture.Init(CreateSSAOResultTexture());
		m_SSAOBlurYTexture.Init(CreateSSAOBlurYTexture());
	}
}

void CEstrangedClientRenderTargets::ShutdownSSAORenderTargets()
{
	m_SSAODepthTexture.Shutdown();
	m_SSAOResultTexture.Shutdown();
	m_SSAOBlurYTexture.Shutdown();
}

//-----------------------------------------------------------------------------
// Purpose: Called by the engine in material system init and shutdown.
//			Clients should override this in their inherited version, but the base
//			is to init all standard render targets for use.
// Input  : pMaterialSystem - the engine's material system (our singleton is not yet inited at the time this is called)
//			pHardwareConfig - the user hardware config, useful for conditional render target setup
//-----------------------------------------------------------------------------
void CEstrangedClientRenderTargets::InitClientRenderTargets(IMaterialSystem* pMaterialSystem, IMaterialSystemHardwareConfig* pHardwareConfig)
{
	// Store a reference to this material system
	m_pMaterialSystem = pMaterialSystem;

	// Grab a copy of the backbuffer width and height
	m_pMaterialSystem->GetBackBufferDimensions(m_iBackBufferWidth, m_iBackBufferHeight);

	// Init the RTs
	InitSSAORenderTargets();

	// Water effects & camera from the base class (standard HL2 targets) 
	BaseClass::InitClientRenderTargets(pMaterialSystem, pHardwareConfig);
}

//-----------------------------------------------------------------------------
// Purpose: Shut down each CTextureReference we created in InitClientRenderTargets.
//			Called by the engine in material system shutdown.
// Input  :  - 
//-----------------------------------------------------------------------------
void CEstrangedClientRenderTargets::ShutdownClientRenderTargets()
{
	// Shutdown the RTs
	ShutdownSSAORenderTargets();

	// Unassign our pointer
	m_pMaterialSystem = NULL;

	BaseClass::ShutdownClientRenderTargets();
}

void CEstrangedClientRenderTargets::PreRender()
{
	if (!m_pMaterialSystem)
	{
		return;
	}

	// Store the old back buffer dinensions
	int iOldBackBufferWidth = m_iBackBufferWidth;
	int iOldBackBufferHeight = m_iBackBufferHeight;

	// Grab the new back buffer dimensions
	m_pMaterialSystem->GetBackBufferDimensions(m_iBackBufferWidth, m_iBackBufferHeight);

	// If there has been a change
	if (iOldBackBufferWidth != m_iBackBufferWidth || iOldBackBufferHeight != m_iBackBufferHeight)
	{
		// For debugging purposes, spew yellow message
		ConDColorMsg(YELLOW, "CEstrangedClientRenderTargets::PreRender() - Detected back buffer change from %ix%i to %ix%i\n",
			iOldBackBufferWidth, iOldBackBufferHeight, m_iBackBufferWidth, m_iBackBufferHeight);

		// Shutdown the RTs
		ShutdownSSAORenderTargets();

		// Init the RTs
		InitSSAORenderTargets();
	}
}

static CEstrangedClientRenderTargets g_EstrangedClientRenderTargets;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CEstrangedClientRenderTargets, IClientRenderTargets, CLIENTRENDERTARGETS_INTERFACE_VERSION, g_EstrangedClientRenderTargets);
CEstrangedClientRenderTargets* TNERenderTargets = &g_EstrangedClientRenderTargets;