/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string.h>

#include "deSteamSdk.h"
#include "deSsdkServiceSteam.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>

#ifdef OS_UNIX
	#include <unistd.h>
	#include <linux/limits.h>
#elif defined OS_W32
	#include <dragengine/app/include_windows.h>
#endif



// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *SSDKCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *SSDKCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deSteamSdk( *loadableModule );
	}catch( ... ){
		return nullptr;
	}
}


// SteamSDK requires the logger function to be extern. who had that strange idead?
extern "C" void __cdecl fSteamSdkDebugCallback( int severity, const char *debugText ) {
	deSteamSdk * const module = deSteamSdk::globalModule;
	if( ! module ){
		return;
	}
	
	switch( severity ){
	case 0:
		module->LogInfoFormat( "SteamSdkDebug: %s", debugText );
		break;
		
	case 1:
		module->LogWarnFormat( "SteamSdkDebug: %s", debugText );
		break;
		
	default:
		module->LogErrorFormat( "SteamSdkDebug: %s", debugText );
		break;
	}
}


// Class deSteamSdk
/////////////////////

deSteamSdk *deSteamSdk::globalModule = nullptr;


// Constructor, destructor
////////////////////////////

deSteamSdk::deSteamSdk( deLoadableModule& loadableModule ) :
deBaseServiceModule( loadableModule ),
pSdkInited( false ){
	globalModule = this;
}

deSteamSdk::~deSteamSdk(){
	if( pSdkInited ){
		SteamUtils()->SetWarningMessageHook( nullptr );
		LogInfo( "Shut down Steam SDK" );
		SteamAPI_Shutdown();
	}
	
	globalModule = nullptr;
}


// Management
///////////////

decStringSet deSteamSdk::GetSupportedServices(){
	decStringSet names;
	names.Add( deSsdkServiceSteam::serviceName );
	return names;
}

void deSteamSdk::InitSdk( const deServiceObject::Ref &data ){
	if( pSdkInited ){
		return;
	}
	
	if( ! SteamAPI_IsSteamRunning() ){
		LogError( "Steam is not running." );
		DETHROW_INFO( deeInvalidAction, "Steam is not running" );
	}
	
	LogInfo( "Initialize Steam SDK" );
	SteamErrMsg errorMsg;
	memset( errorMsg, 0, sizeof( errorMsg ) );
	
	ESteamAPIInitResult result = SteamAPI_InitEx( &errorMsg );
	if( result == k_ESteamAPIInitResult_OK ){
		LogInfo( "Steam SDK initialized successfully" );
		pSdkInited = true;
		SteamUtils()->SetWarningMessageHook( fSteamSdkDebugCallback );
		return;
	}
	
	LogInfoFormat( "Failed initialize Steam SDK (%d): %s", result, errorMsg );
	
	const deServiceObject::Ref objAppId( data->GetChildAt( "appId" ) );
	if( objAppId ){
		decString appId;
		appId.Format( "%d", objAppId->GetInteger() );
		
		LogInfoFormat( "Try initialize Steam SDK using AppID" );
		result = k_ESteamAPIInitResult_FailedGeneric;
		memset( errorMsg, 0, sizeof( errorMsg ) );
		
		deVirtualFileSystem &vfs = GetVFS();
		const decPath pathAppId( decPath::CreatePathUnix( "/cache/global/steam_appid.txt" ) );
		decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( pathAppId ) )->WriteString( appId );
		
		try{
			deEngine &engine = *GetGameEngine();
			decPath pathCache( decPath::CreatePathNative( engine.GetOS()->GetPathUserCache() ) );
			pathCache.AddComponent( "global" );
			pathCache.AddComponent( "modules" );
			pathCache.AddComponent( engine.GetModuleSystem()->GetTypeDirectory( deModuleSystem::emtService ) );
			pathCache.AddComponent( GetLoadableModule().GetDirectoryName() );
			
			#ifdef OS_UNIX
			char workDir[ PATH_MAX ];
			if( ! getcwd( workDir, sizeof( workDir ) ) ){
				DETHROW_INFO( deeInvalidAction, "Failed getting working directory" );
			}
			if( chdir( pathCache.GetPathNative() ) ){
				DETHROW_INFO( deeInvalidAction, "Failed changing working directory" );
			}
			
			result = SteamAPI_InitEx( &errorMsg );
			if( chdir( workDir ) ){ /* ignore */ }
			
			#elif defined OS_W32
			TCHAR workDir[ MAX_PATH ];
			if( ! GetCurrentDirectory( MAX_PATH, workDir ) ){
				DETHROW_INFO( deeInvalidAction, "Failed getting working directory" );
			}
			if( ! SetCurrentDirectory( pathCache.GetPathNative() ) ){
				DETHROW_INFO( deeInvalidAction, "Failed changing working directory" );
			}
			
			result = SteamAPI_InitEx( &errorMsg );
			SetCurrentDirectory( workDir );
			
			#else
			DETHROW_INFO( deeInvalidAction, "Function unsupported on this OS" );
			#endif
			
		}catch( const deException &e ){
			LogException( e );
			if( vfs.CanDeleteFile( pathAppId ) ){
				vfs.DeleteFile( pathAppId );
			}
			throw;
		}
		
		if( vfs.CanDeleteFile( pathAppId ) ){
			vfs.DeleteFile( pathAppId );
		}
		
		if( result == k_ESteamAPIInitResult_OK ){
			LogInfo( "Steam SDK initialized successfully" );
			pSdkInited = true;
			SteamUtils()->SetWarningMessageHook( fSteamSdkDebugCallback );
			return;
		}
		
		LogInfoFormat( "Failed initialize Steam SDK (%d): %s", result, errorMsg );
	}
	
	DETHROW_INFO( deeInvalidAction, errorMsg );
}

deBaseServiceService* deSteamSdk::CreateService( deService *service,
const char *name, const deServiceObject::Ref &data ){
	DEASSERT_NOTNULL( service )
	
	if( strcmp( name, deSsdkServiceSteam::serviceName ) == 0 ){
		return new deSsdkServiceSteam( *this, service, data );
	}
	
	return nullptr;
}
