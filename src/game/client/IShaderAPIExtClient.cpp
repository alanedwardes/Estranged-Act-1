#include "cbase.h"
#include "../../materialsystem/stdshaders/ishaderapiext.h"

// -------------------------------------------
// Purpose: Setup globally accessible instance of the Shader API extension interface
// Author: AniCator
// -------------------------------------------
CSysModule *__g_pShaderAPIShaderModule = NULL;
static IShaderAPIExtension *__g_shaderExt = NULL;
IShaderAPIExtension *GetShaderAPIExt()
{
	return __g_shaderExt;
}

// -------------------------------------------
// Purpose: Hook up the Shader API so we can interface with it.
// Author: AniCator
// -------------------------------------------
bool ConnectShaderAPIExt()
{
	char modulePath[MAX_PATH*4];
	Q_snprintf( modulePath, sizeof( modulePath ), "%s/bin/game_shader_dx9.dll", engine->GetGameDirectory() );
	__g_pShaderAPIShaderModule = Sys_LoadModule( modulePath );

	if ( __g_pShaderAPIShaderModule )
	{
		CreateInterfaceFn shaderShaderAPIDLLFactory = Sys_GetFactory( __g_pShaderAPIShaderModule );
		__g_shaderExt = shaderShaderAPIDLLFactory ? ((IShaderAPIExtension *) shaderShaderAPIDLLFactory( SHADERAPI_EXTENSION_VERSION, NULL )) : NULL;

		if ( !__g_shaderExt )
			Warning( "Unable to pull IShaderAPIExtension interface.\n" );
	}
	else
		Warning( "Cannot load game_shader_dx9.dll from %s!\n", modulePath );

	return __g_shaderExt != NULL;
}

// -------------------------------------------
// Purpose: Unhook from the Shader API ( I think lol hehe go away! Why are you even here!? )
// Author: AniCator
// -------------------------------------------
void ShutdownShaderAPIExt()
{
	if ( !__g_shaderExt )
		return;

	//delete [] __g_defExt->CommitLightData_Common( NULL, 0, 0, 0, 0, 0 );

	__g_shaderExt = NULL;
}