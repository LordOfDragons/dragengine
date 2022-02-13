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

#include "declConfigXML.h"
#include "declConfiguration.h"
#include "../declLauncher.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>


// Definitions
////////////////

#define FILE_LAUNCHER_CONFIG_SYSTEM		"/config/system/delauncherconsole.xml"
#define FILE_LAUNCHER_CONFIG_USER		"/config/user/delauncherconsole.xml"

#define LOGSOURCE "Launcher"



// Class declConfiguration
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declConfiguration::declConfiguration( declLauncher &launcher ) :
pLauncher( launcher ),
pCanSave( false ){
}

declConfiguration::~declConfiguration(){
}



// Management
///////////////

void declConfiguration::LoadConfiguration(){
	declConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	decPath pathFile;
	
	// read the system wide config file if existing
	pathFile.SetFromUnix( FILE_LAUNCHER_CONFIG_SYSTEM );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfo( LOGSOURCE, "Reading system configuration file" );
			configXML.ReadFromFile( decBaseFileReader::Ref::New( vfs.OpenFileForReading( pathFile ) ), *this );
			
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
			configXML.ReadFromFile( decBaseFileReader::Ref::New( vfs.OpenFileForReading( pathFile ) ), *this );
			
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
	if( ! pCanSave ){
		return;
	}
	
	declConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	decPath pathFile;
	
	pathFile.SetFromUnix( FILE_LAUNCHER_CONFIG_USER );
	if( vfs.CanWriteFile( pathFile ) ){
		logger.LogInfo( LOGSOURCE, "Writing user configuration file" );
		
		try{
			configXML.WriteToFile( decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( pathFile ) ), *this );
			
		}catch( const deException & ){
			logger.LogError( LOGSOURCE, "Failed to write user configuration file (file permission problem)" );
		}
		
	}else{
		logger.LogError( LOGSOURCE, "Failed to write user configuration file (file writing problem)" );
	}
}
