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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dellLauncher.h"
#include "dellRunGame.h"

#include <delauncher/engine/delEngineInstanceDirect.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#ifdef OS_UNIX
#include <dlfcn.h>
#endif


// Definitions
////////////////

#define LOGSOURCE "Launcher"


// Class dellLauncher::Launcher
/////////////////////////////////

dellLauncher::Launcher::Launcher() :
delLauncher( "LauncherLive", "launcher" ){
	AddFileLogger( "launcher" );
	SetEngineInstanceFactory( delEngineInstanceDirect::Factory::Ref::New(
		new delEngineInstanceDirect::Factory ) );
}

dellLauncher::Launcher::~Launcher(){
}


// Class dellLauncher::PreloadLibrary
///////////////////////////////////////

#ifdef OS_UNIX

dellLauncher::PreloadLibrary::PreloadLibrary( const decPath &basePath, const char *filename ) :
pHandle( dlopen( ( basePath + decPath::CreatePathUnix( filename ) ).GetPathNative(), RTLD_LAZY ) ){
}

dellLauncher::PreloadLibrary::~PreloadLibrary(){
	if( pHandle ){
		dlclose( pHandle );
	}
}

#endif


// Class dellLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

dellLauncher::dellLauncher() :
pLauncher( nullptr ){
}

dellLauncher::~dellLauncher(){
	if( pLauncher ){
		delete pLauncher;
	}
}



// Management
///////////////

void dellLauncher::AddArgument( const decUnicodeString &argument ){
	pArguments.AddArgument( argument );
}

void dellLauncher::Run(){
	pWorkingDir.SetWorkingDirectory();
	pUpdateEnvironment();
	pLauncher = new Launcher;
	pLauncher->SetEngineInstanceFactory( delEngineInstance::Factory::Ref::NewWith() );
	
	dellRunGame( *this ).Run();
}



// Private Functions
//////////////////////

void dellLauncher::pUpdateEnvironment(){
	#ifdef OS_UNIX
	const char * const platformDir = "linux64";
	#elif defined OS_W32
	const char * const platformDir = "windows64";
	#else
	#error "Unsupported"
	#endif
	
	const decPath pathBase( pWorkingDir + decPath::CreatePathUnix( platformDir ) );
	const decPath pathEngine( pathBase );
	
	#ifdef OS_UNIX
	const decPath pathEngineLib( pathEngine + decPath::CreatePathUnix( "lib/dragengine" ) );
	const decPath pathEngineShare( pathEngine + decPath::CreatePathUnix( "share/dragengine" ) );
	const decPath pathEngineConfig( pathEngine + decPath::CreatePathUnix( "etc/dragengine" ) );
	const decPath pathLauncherConfig( pathBase + decPath::CreatePathUnix( "etc/delauncher" ) );
	const decPath pathLauncherShares( pathBase + decPath::CreatePathUnix( "share/delauncher" ) );
	
	#elif defined OS_W32
	const decPath pathEngineLib( pathEngine + decPath::CreatePathUnix( "data" ) );
	const decPath pathEngineShare( pathEngine + decPath::CreatePathUnix( "share" ) );
	const decPath pathEngineConfig( pathEngine + decPath::CreatePathUnix( "config/system/dragengine" ) );
	const decPath pathLauncherConfig( pathBase + decPath::CreatePathUnix( "config/system/delauncher" ) );
	const decPath pathLauncherShares( pathBase + decPath::CreatePathUnix( "data/delauncher" ) );
	#endif
	
	const decPath pathCache( pathBase + decPath::CreatePathUnix( "cache" ) );
	const decPath pathCapture( pathBase + decPath::CreatePathUnix( "capture" ) );
	
	const decPath pathGames( pWorkingDir ); //+ decPath::CreatePathUnix( "games" ) );
	const decPath pathLogs( pWorkingDir + decPath::CreatePathUnix( "logs" ) );
	
	pPathConfigUser = pathBase + decPath::CreatePathUnix( "config/user/delauncher" );
	
	pEnvParamsStore.Add( decString( "DE_ENGINE_PATH=" ) + pathEngineLib.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_SHARE_PATH=" ) + pathEngineShare.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_CONFIG_PATH=" ) + pathEngineConfig.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_CACHE_PATH=" ) + pathCache.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_CAPTURE_PATH=" ) + pathCapture.GetPathNative() );
	
	pEnvParamsStore.Add( decString( "DELAUNCHER_SYS_CONFIG=" ) + pathLauncherConfig.GetPathNative() );
	pEnvParamsStore.Add( decString( "DELAUNCHER_USER_CONFIG=" ) + pPathConfigUser.GetPathNative() );
	pEnvParamsStore.Add( decString( "DELAUNCHER_SHARES=" ) + pathLauncherShares.GetPathNative() );
	pEnvParamsStore.Add( decString( "DELAUNCHER_GAMES=" ) + pathGames.GetPathNative() );
	pEnvParamsStore.Add( decString( "DELAUNCHER_LOGS=" ) + pathLogs.GetPathNative() );
	
	#ifdef OS_UNIX
	pPreloadLibraries.Add( deObject::Ref::NewWith(pathBase, "lib/libDEFOX-1.7.so") );
	#endif
	
	const int count = pEnvParamsStore.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const decString &envParam = pEnvParamsStore.GetAt( i );
// 		printf( "setenv: '%s'\n", envParam.GetString() );
#ifdef OS_W32_VS
		const int separator = envParam.Find( '=' );
		if( ! SetEnvironmentVariableA( envParam.GetMiddle( 0, separator ).GetString(),
		envParam.GetMiddle( separator + 1 ).GetString() ) ){
			DETHROW( deeInvalidParam );
		}
#else
		if( putenv( ( char* )envParam.GetString() ) ){
			DETHROW( deeInvalidParam );
		}
#endif
	}
}
