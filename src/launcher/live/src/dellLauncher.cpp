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
#include "engine/dellEngine.h"
#include "game/dellGameManager.h"

#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class dellLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

dellLauncher::dellLauncher() :
pEngine( *this ),
pGameManager( *this )
{
	pLogger.TakeOver( new deLoggerConsoleColor );
}

dellLauncher::~dellLauncher(){
}



// Management
///////////////

void dellLauncher::AddArgument( const decUnicodeString &argument ){
	pArguments.AddArgument( argument );
}



void dellLauncher::Init(){
	pWorkingDir.SetWorkingDirectory();
	pUpdateEnvironment();
	
	pFileSystem.TakeOver( new deVirtualFileSystem );
	pInitVFS();
	
	// init logger has to wait until the virtual file system is created
	pInitLogger();
}

void dellLauncher::Run(){
	Init();
	dellRunGame( *this ).Run();
}



// Private Functions
//////////////////////

void dellLauncher::pInitVFS(){
	deVFSContainerReference container;
	
	container.TakeOver( new deVFSDiskDirectory(
		decPath::CreatePathUnix( "/config/user" ),
		pPathConfigUser ) );
	( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( false );
	pFileSystem->AddContainer( container );
	
	container.TakeOver( new deVFSDiskDirectory(
		decPath::CreatePathUnix( "/data" ),
		pWorkingDir + decPath::CreatePathUnix( "data" ) ) );
	( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( false );
	pFileSystem->AddContainer( container );
	
	container.TakeOver( new deVFSDiskDirectory(
		decPath::CreatePathUnix( "/logs" ),
		pWorkingDir + decPath::CreatePathUnix( "logs" ) ) );
	( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( false );
	pFileSystem->AddContainer( container );
}

void dellLauncher::pInitLogger(){
	decBaseFileWriterReference fileWriter;
	fileWriter.TakeOver( pFileSystem->OpenFileForWriting( decPath::CreatePathUnix( "/logs/launcher.log" ) ) );
	pLogger.TakeOver( new deLoggerFile( fileWriter ) );
}

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
	
	#elif defined OS_W32
	const decPath pathEngineLib( pathEngine + decPath::CreatePathUnix( "data" ) );
	const decPath pathEngineShare( pathEngine + decPath::CreatePathUnix( "share" ) );
	const decPath pathEngineConfig( pathEngine + decPath::CreatePathUnix( "config" ) );
	#endif
	
	const decPath pathCache( pathBase + decPath::CreatePathUnix( "cache" ) );
	const decPath pathCapture( pathBase + decPath::CreatePathUnix( "capture" ) );
	
	pPathConfigUser = pathBase + decPath::CreatePathUnix( "config/launcher"  );
	
	pEnvParamsStore.Add( decString( "DE_ENGINE_PATH=" ) + pathEngineLib.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_SHARE_PATH=" ) + pathEngineShare.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_CONFIG_PATH=" ) + pathEngineConfig.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_CACHE_PATH=" ) + pathCache.GetPathNative() );
	pEnvParamsStore.Add( decString( "DE_CAPTURE_PATH=" ) + pathCapture.GetPathNative() );
	
	const int count = pEnvParamsStore.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( putenv( ( char* )pEnvParamsStore.GetAt( i ).GetString() ) ){
			pLogger->LogError( LOGSOURCE, "putenv failed" );
			DETHROW( deeInvalidParam );
		}
	}
}
