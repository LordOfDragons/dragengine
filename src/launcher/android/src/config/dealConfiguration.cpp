/* 
 * Drag[en]gine Android Launcher
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

#include "dealConfigXML.h"
#include "dealConfiguration.h"
#include "../dealLauncher.h"

#include "../logger/deLogger.h"
#include "../logger/deLoggerConsole.h"
#include "../filesystem/deVFSDiskDirectory.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../common/file/decPath.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/exceptions.h"


// Definitions
////////////////

#define FILE_LAUNCHER_CONFIG_SYSTEM		"/config/system/delauncherandroid.xml"
#define FILE_LAUNCHER_CONFIG_USER		"/config/user/delauncherandroid.xml"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealConfiguration
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealConfiguration::dealConfiguration( dealLauncher &launcher ) :
pLauncher( launcher ),
pClrValidBack( 0 ), //( 87.0f / 255.0f, 217.0f / 255.0f, 87.0f / 255.0f ),
pClrValidText( 0 ), //( 0.0f, 0.0f, 0.0f ),
pClrProblemBack( 0 ), //( 1.0f, 0.5f, 0.5f ),
pClrProblemText( 0 ){ //( 0.0f, 0.0f, 0.0f ){
}

dealConfiguration::~dealConfiguration(){
}



// Management
///////////////

void dealConfiguration::SetPathConfigSystem( const char *path ){
	pPathConfigSystem = path;
}

void dealConfiguration::SetPathConfigUser( const char *path ){
	pPathConfigUser = path;
}

void dealConfiguration::SetPathShares( const char *path ){
	pPathShares = path;
}

void dealConfiguration::SetPathLogs( const char *path ){
	pPathLogs = path;
}



void dealConfiguration::LocatePath(){
	// the system wide configuration directory. figure out a way to place this properly
	// on android. for the time being this is placed in the internal storage of the
	// game engine installation
	pPathConfigSystem = pLauncher.GetInternalPath();
	pPathConfigSystem += "/system_etc";
	
	// the user configuration directory is located under the external storage so the user
	// can access it and back it up if desired
	pPathConfigUser = pLauncher.GetExternalPath();
	pPathConfigUser += "/etc";
	
	// the shares directory is located is located in the game engine internal storage
	pPathShares = pLauncher.GetInternalPath();
	pPathShares.AppendFormat( "/%s/share", ANDROID_JNIDIR );
	
	// the logs directory is located on the external storage so it can be retrieved
	pPathLogs = pPathConfigUser + "/logs";
}

void dealConfiguration::InitVirtualFileSystem(){
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
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

void dealConfiguration::LogImportantValues(){
	deLogger &logger = pLauncher.GetLogger();
	
	logger.LogInfoFormat( LOGSOURCE, "Android internal path = %s", pLauncher.GetInternalPath() );
	logger.LogInfoFormat( LOGSOURCE, "Android external path = %s", pLauncher.GetExternalPath() );
	
	logger.LogInfoFormat( LOGSOURCE, "System config path = '%s'", pPathConfigSystem.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "User config path = '%s'", pPathConfigUser.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Shares path = '%s'", pPathShares.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Logs path = '%s'", pPathLogs.GetString() );
}



void dealConfiguration::LoadConfiguration(){
	dealConfigXML configXML( &pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
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
}

void dealConfiguration::SaveConfiguration(){
	dealConfigXML configXML( &pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
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



// Private Functions
//////////////////////
