/* 
 * Drag[en]gine Console Launcher
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

#ifdef OS_W32
#include <shlobj.h>
//#include <tchar.h>
#endif

#include "declConfigXML.h"
#include "declConfiguration.h"
#include "../declLauncher.h"
#include "../engine/declEngine.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#ifdef OS_W32
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#endif
#include <dragengine/common/exceptions.h>
#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif

#ifdef OS_W32
#ifndef SHGFP_TYPE_CURRENT
#define SHGFP_TYPE_CURRENT 0
#endif
#endif


// Definitions
////////////////

#define FILE_LAUNCHER_CONFIG_SYSTEM		"/config/system/delauncherconsole.xml"
#define FILE_LAUNCHER_CONFIG_USER		"/config/user/delauncherconsole.xml"

#define LOGSOURCE "Launcher"



// Class declConfiguration
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declConfiguration::declConfiguration( declLauncher *launcher ){
	if( ! launcher ){
		DETHROW( deeInvalidParam );
	}
	
	pLauncher = launcher;
	pCanSave = false;
}

declConfiguration::~declConfiguration(){
}



// Management
///////////////

void declConfiguration::SetPathConfigSystem( const char *path ){
	pPathConfigSystem = path;
}

void declConfiguration::SetPathConfigUser( const char *path ){
	pPathConfigUser = path;
}

void declConfiguration::SetPathShares( const char *path ){
	pPathShares = path;
}

void declConfiguration::SetPathLogs( const char *path ){
	pPathLogs = path;
}



void declConfiguration::LocatePath(){
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
	
#elif defined( OS_BEOS )
	pPathConfigUser = "/boot/home/config/settings/delauncher";
	
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

void declConfiguration::InitVirtualFileSystem(){
	deVirtualFileSystem &vfs = *pLauncher->GetFileSystem();
	deVFSDiskDirectory *diskDir = NULL;
	decPath pathRootDir, pathDiskDir;
	
	// add the found path to the virtual file system. this makes it easier
	// to find the files later on without having to deal with file system
	// specific quirks.
	try{
		// add the configuration containers. the containers are added in
		// separate locations as we want to read the config files one
		// by one and mapping both containers to the same path would
		// shadow the system config files.
		if( ! pPathConfigSystem.IsEmpty() ){
			pathRootDir.SetFromUnix( "/config/system" );
			pathDiskDir.SetFromNative( pPathConfigSystem.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( true );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! pPathConfigUser.IsEmpty() ){
			pathRootDir.SetFromUnix( "/config/user" );
			pathDiskDir.SetFromNative( pPathConfigUser.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		// add the data directory. currently there exists only one which
		// is the system shares directory. a user one could be layered
		// on top of it though if required later on. the shares container
		// is set to read-write as the launcher has to potentiall install
		// new games or uninstall them.
		if( ! pPathShares.IsEmpty() ){
			pathRootDir.SetFromUnix( "/data" );
			pathDiskDir.SetFromNative( pPathShares.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		// add the logs directory. this is read-write
		if( ! pPathLogs.IsEmpty() ){
			pathRootDir.SetFromUnix( "/logs" );
			pathDiskDir.SetFromNative( pPathLogs.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
	}catch( const deException & ){
		if( diskDir ){
			diskDir->FreeReference();
		}
		throw;
	}
}

void declConfiguration::LogImportantValues(){
	deLogger &logger = *pLauncher->GetLogger();
	
	logger.LogInfoFormat( LOGSOURCE, "System config path = '%s'", pPathConfigSystem.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "User config path = '%s'", pPathConfigUser.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Games path = '%s'", pPathGames.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Shares path = '%s'", pPathShares.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Logs path = '%s'", pPathLogs.GetString() );
}



void declConfiguration::LoadConfiguration(){
	declConfigXML configXML( pLauncher->GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher->GetFileSystem();
	deLogger &logger = *pLauncher->GetLogger();
	decBaseFileReader *reader = NULL;
	decPath pathFile;
	
	// read the system wide config file if existing
	pathFile.SetFromUnix( FILE_LAUNCHER_CONFIG_SYSTEM );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			 logger.LogInfo( LOGSOURCE, "Reading system configuration file" );
			try{
				reader = vfs.OpenFileForReading( pathFile );
				configXML.ReadFromFile( *reader, *this );
				
				reader->FreeReference();
				
			}catch( const deException & ){
				if( reader ){
					reader->FreeReference();
				}
				throw;
			}
			
		}else{
			logger.LogError( LOGSOURCE, "System configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfo( LOGSOURCE, "System configuration file not found, skipped" );
	}
	
	// read the user config file if existing
	pathFile.SetFromUnix( FILE_LAUNCHER_CONFIG_USER );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfo( LOGSOURCE, "Reading user configuration file" );
			reader = NULL;
			
			try{
				reader = vfs.OpenFileForReading( pathFile );
				configXML.ReadFromFile( *reader, *this );
				
				reader->FreeReference();
				
			}catch( const deException & ){
				if( reader ){
					reader->FreeReference();
				}
				throw;
			}
			
		}else{
			logger.LogError( LOGSOURCE, "User configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfo( LOGSOURCE, "User configuration file not found, will be created upon exiting" );
	}
	
	pCanSave = true;
}

void declConfiguration::SaveConfiguration(){
	if( pCanSave ){
		declConfigXML configXML( pLauncher->GetLogger(), LOGSOURCE );
		deVirtualFileSystem &vfs = *pLauncher->GetFileSystem();
		deLogger &logger = *pLauncher->GetLogger();
		decBaseFileWriter *writer = NULL;
		decPath pathFile;
		
		pathFile.SetFromUnix( FILE_LAUNCHER_CONFIG_USER );
		if( vfs.CanWriteFile( pathFile ) ){
			logger.LogInfo( LOGSOURCE, "Writing user configuration file" );
			
			try{
				writer = vfs.OpenFileForWriting( pathFile );
				configXML.WriteToFile( *writer, *this );
				
				writer->FreeReference();
				
			}catch( const deException & ){
				if( writer ){
					writer->FreeReference();
				}
				logger.LogError( LOGSOURCE, "Failed to write user configuration file (file permission problem)" );
				// DIALOG BOX
				// "User configuration can not be written!\n"
				// "Make sure you have write permission for the file and parent directory.\n"
				// "User configuration has not been saved!\n"
			}
			
		}else{
			logger.LogError( LOGSOURCE, "Failed to write user configuration file (file writing problem)" );
			// DIALOG BOX
			// "User configuration can not be written!\n"
			// "Make sure you have write permission for the file and parent directory.\n"
			// "User configuration has not been saved!\n"
		}
	}
}
