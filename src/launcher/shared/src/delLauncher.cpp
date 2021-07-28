/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "delLauncher.h"
#include "engine/delEngineInstance.h"
#include "game/delGame.h"
#include "game/icon/delGameIcon.h"
#include "game/profile/delGameProfile.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/logger/deLoggerFile.h>

#ifdef OS_W32
#include <shlobj.h>
#include <dragengine/app/deOSWindows.h>
#endif


// Class delLauncher
/////////////////////////////

delLauncher::delLauncher( const char *loggerSource ) :
pLoggerHistory( delLoggerHistory::Ref::New( new delLoggerHistory ) ),
pLogSource ( loggerSource ),
pEngine( *this ),
pGameManager( *this ),
pPatchManager( *this )
{
	try{
		pLogger.TakeOver( new deLoggerChain );
		pLogger->AddLogger( deLoggerConsoleColor::Ref::New( new deLoggerConsoleColor ) );
		pLogger->AddLogger( pLoggerHistory );
		
		pLocatePath();
		pInitVFS();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

delLauncher::~delLauncher(){
	pCleanUp();
}



// Management
///////////////

void delLauncher::AddFileLogger( const char *filetitle ){
	if( ! filetitle ){
		DETHROW_INFO( deeNullPointer, "filetitle" );
	}
	if( strlen( filetitle ) == 0 ){
		DETHROW_INFO( deeInvalidParam, "filetitle is empty" );
	}
	
	decString filename;
	filename.Format( "/logs/%s.log", filetitle );
	
	pLogger->AddLogger( deLoggerFile::Ref::New( new deLoggerFile( decBaseFileWriter::Ref::New(
		pVFS->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) ) ) ) );
}

void delLauncher::Prepare(){
	pLogger->LogInfoFormat( pLogSource, "System config path: %s", pPathConfigSystem.GetString() );
	pLogger->LogInfoFormat( pLogSource, "User config path: %s", pPathConfigUser.GetString() );
	pLogger->LogInfoFormat( pLogSource, "Shares path: %s", pPathShares.GetString() );
	pLogger->LogInfoFormat( pLogSource, "Games path: %s", pPathGames.GetString() );
	pLogger->LogInfoFormat( pLogSource, "Logs path: %s", pPathLogs.GetString() );
	
	{
	delEngineInstance instance( *this, pEngine.GetLogFile() );
	instance.StartEngine();
	instance.LoadModules();
	
	pEngine.PutEngineIntoVFS( instance );
	
	pLogger->LogInfoFormat( pLogSource, "Engine config path = '%s'", pEngine.GetPathConfig().GetString() );
	pLogger->LogInfoFormat( pLogSource, "Engine share path = '%s'", pEngine.GetPathShare().GetString() );
	pLogger->LogInfoFormat( pLogSource, "Engine lib path = '%s'", pEngine.GetPathLib().GetString() );
	
	pEngine.UpdateResolutions( instance );
	pEngine.ReloadModules();
	pEngine.CheckModules( instance );
	pEngine.LoadConfig();
	
	pGameManager.CreateDefaultProfile();
	
	pGameManager.LoadGames( instance );
	pPatchManager.LoadPatches( instance );
	}
	
	pGameManager.LoadGameConfigs();
	pGameManager.Verify();
}



delGame * delLauncher::CreateGame(){
	return new delGame( *this );
}

delGameProfile *delLauncher::CreateGameProfile( const delGameProfile *copyFrom ){
	return copyFrom ? new delGameProfile( *copyFrom ) : new delGameProfile;
}

delGameIcon *delLauncher::CreateGameIcon( int size, const char *path ){
	return new delGameIcon( size, path );
}



// Private Functions
//////////////////////

void delLauncher::pCleanUp(){
	pPatchManager.Clear();
	pGameManager.Clear();
	pEngine.Clear();
	
	pLogger = nullptr;
	pVFS = nullptr;
}

void delLauncher::pLocatePath(){
	const char *value;
	decPath path;
	
	// the system wide configuration directory is hard coded at compile time.
	// can be changed at runtime using an environment parameter.
	pPathConfigSystem = LAUNCHER_CONFIG_PATH;
#ifdef OS_W32
	const decString pathEngineBase( deOSWindows().GetPathEngineBase() );
	
	//pPathConfigSystem = deOSWindows::GetRegistryValue( "SOFTWARE\\Drag[en]gine",
	//	"PathLauncherConfig", pPathConfigSystem );
	pPathConfigSystem = pathEngineBase + "\\Launchers\\Config";
#endif
	
	value = getenv( "DELAUNCHER_SYS_CONFIG" );
	if( value ){
		pPathConfigSystem = value;
	}
#ifdef OS_W32
	pPathConfigSystem = deOSWindows::ParseNativePath( pPathConfigSystem );
#endif
	
#ifdef OS_W32
	pPathConfigUser = "@RoamingAppData\\DELaunchers\\Config";
	
#else
	// the user configuration directory is located under the user home directory.
	// can be changed at runtime using an environment parameter.
	value = getenv( "HOME" );
	if( value ){
		path.SetFromNative( value );
		
	}else{
		value = getenv( "USER" );
		
		if( value ){
			path.SetFromNative( "/home" );
			path.AddComponent( value );
			
		}else{
			value = getenv( "LOGUSER" );
			
			if( value ){
				path.SetFromNative( "/home" );
				path.AddComponent( value );
			}
		}
	}
	
	if( path.GetComponentCount() > 0 ){
		path.AddComponent( ".config" );
		path.AddComponent( "delauncher" );
		pPathConfigUser = path.GetPathNative();
	}
#endif
	
	value = getenv( "DELAUNCHER_USER_CONFIG" );
	if( value ){
		pPathConfigUser = value;
	}
#ifdef OS_W32
	pPathConfigUser = deOSWindows::ParseNativePath( pPathConfigUser );
#endif
	
	// the shares directory is hard coded at compile time. can be changed at runtime
	// using an environment parameter.
	pPathShares = LAUNCHER_SHARE_PATH;
#ifdef OS_W32
	pPathShares = pathEngineBase + "\\Launchers\\Share";
	
	//pPathShares = deOSWindows::GetRegistryValue( "SOFTWARE\\Drag[en]gine",
	//	"PathLauncherShares", pPathShares );
#endif
	
	value = getenv( "DELAUNCHER_SHARES" );
	if( value ){
		pPathShares = value;
	}
#ifdef OS_W32
	pPathShares = deOSWindows::ParseNativePath( pPathShares );
#endif
	
	// the games directory is hard coded at compile time. can be changed at runtime
	// using an environment parameter.
	pPathGames = LAUNCHER_GAMES_PATH;
#ifdef OS_W32
	pPathGames = pathEngineBase + "\\Launchers\\Games";
	
	pPathGames = deOSWindows::GetRegistryValue( "SOFTWARE\\Drag[en]gine",
		"PathLauncherGames", pPathGames );
#endif
	
	value = getenv( "DELAUNCHER_GAMES" );
	if( value ){
		pPathGames = value;
	}
#ifdef OS_W32
	pPathGames = deOSWindows::ParseNativePath( pPathGames );
#endif
	
	// the logs directory is located right under the user configuration directory.
	// can be changed at runtime using an environment parameter
	path.SetFromNative( pPathConfigUser );
	path.AddComponent( "logs" );
	pPathLogs = path.GetPathNative();
#ifdef OS_W32
	pPathLogs = "@LocalAppData\\DELaunchers\\Logs";
#endif
	
	value = getenv( "DELAUNCHER_LOGS" );
	if( value ){
		pPathLogs = value;
	}
#ifdef OS_W32
	pPathLogs = deOSWindows::ParseNativePath( pPathLogs );
#endif
}

void delLauncher::pInitVFS(){
	pVFS.TakeOver( new deVirtualFileSystem );
	
	// add the found path to the virtual file system. this makes it easier to find the
	// files later on without having to deal with file system specific quirks
	
	// add the configuration containers. the containers are added in separate locations
	// as we want to read the config files one by one and mapping both containers to
	// the same path would shadow the system config files.
	if( ! pPathConfigSystem.IsEmpty() ){
		pVFS->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
			decPath::CreatePathUnix( "/config/system" ),
			decPath::CreatePathNative( pPathConfigSystem ), true ) ) );
	}
	
	if( ! pPathConfigUser.IsEmpty() ){
		pVFS->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
			decPath::CreatePathUnix( "/config/user" ),
			decPath::CreatePathNative( pPathConfigUser ), false ) ) );
	}
	
	// add the data directory. currently there exists only one which is the system shares
	// directory. a user one could be layered on top of it though if required later on.
	// the shares container is set to read-write as the launcher has to potentiall install
	// new games or uninstall them
	if( ! pPathShares.IsEmpty() ){
		pVFS->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
			decPath::CreatePathUnix( "/data" ),
			decPath::CreatePathNative( pPathShares ), false ) ) );
	}
	
	// add the logs directory. this is read-write
	if( ! pPathLogs.IsEmpty() ){
		pVFS->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
			decPath::CreatePathUnix( "/logs" ),
			decPath::CreatePathNative( pPathLogs ), false ) ) );
	}
}
