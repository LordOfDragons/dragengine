/* 
 * Drag[en]gine Live Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	SetEngineInstanceFactory( new delEngineInstanceDirect::Factory );
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
	
	pPathConfigUser = pathBase + decPath::CreatePathUnix( "config/user/delauncher"  );
	
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
	pPreloadLibraries.Add( deObject::Ref::New( new PreloadLibrary( pathBase, "lib/libDEFOX-1.7.so" ) ) );
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
