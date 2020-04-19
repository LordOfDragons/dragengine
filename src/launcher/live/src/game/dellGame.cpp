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

#include "dellGame.h"
#include "dellGameManager.h"
#include "dellGameConfigXML.h"
#include "fileformat/dellFileFormat.h"
#include "profile/dellGameProfile.h"
#include "../dellLauncher.h"
#include "../engine/dellEngine.h"
#include "../engine/modules/dellEngineModule.h"
#include "../engine/modules/dellEngineModuleList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class dellGame
///////////////////

// Constructors and Destructors
/////////////////////////////////

dellGame::dellGame( dellLauncher &launcher ) :
pLauncher( launcher ),
pIconSize( 0 ),
pAllFormatsSupported( false ),
pScriptModuleFound( false ),
pGameUpToDate( false ),
pCanRun( false ),
pUseLatestPatch( true ){
}

dellGame::~dellGame(){
}



// Management
///////////////

void dellGame::SetIdentifier( const decUuid &identifier ){
	pIdentifier = identifier;
}

void dellGame::SetAliasIdentifier( const char *identifier ){
	pAliasIdentifier = identifier;
}

void dellGame::SetTitle( const decUnicodeString &title ){
	pTitle = title;
}

void dellGame::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void dellGame::SetCreator( const decUnicodeString &creator ){
	pCreator = creator;
}

void dellGame::SetHomepage( const char *homepage ){
	pHomepage = homepage;
}



void dellGame::SetGameDirectory( const char *directory ){
	pGameDirectory = directory;
}

void dellGame::SetDataDirectory( const char *directory ){
	pDataDirectory = directory;
}

void dellGame::SetScriptDirectory( const char *directory ){
	pScriptDirectory = directory;
}

void dellGame::SetGameObject( const char *gameObject ){
	pGameObject = gameObject;
}

void dellGame::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void dellGame::SetPathCapture( const char *path ){
	pPathCapture = path;
}

void dellGame::SetWindowSize( const decPoint &size ){
	pWindowSize = size.Largest( decPoint() );
}

void dellGame::SetIcon( const char* path, int size ){
	pIconPath = path;
	pIconSize = size;
}



void dellGame::SetScriptModule( const char *module ){
	pScriptModule = module;
}



void dellGame::VerifyRequirements(){
	const dellEngine &engine = pLauncher.GetEngine();
	const dellEngineModuleList &moduleList = engine.GetModuleList();
	int f, formatCount = pFileFormatList.GetCount();
	int m, moduleCount = moduleList.GetCount();
	dellEngineModule *module;
	deModuleSystem::eModuleTypes formatType;
	
	// validate profiles
	if( GetCustomProfile() ){
		GetCustomProfile()->Verify( pLauncher );
	}
	
	// check if all file formats are supported
	pAllFormatsSupported = true;
	
	for( f=0; f<formatCount; f++ ){
		dellFileFormat &format = *pFileFormatList.GetAt( f );
		formatType = format.GetType();
		
		format.SetSupported( false );
		
		if( ! deModuleSystem::IsSingleType( formatType ) ){
			const decString &formatPattern = format.GetPattern();
			
			for( m=0; m<moduleCount; m++ ){
				module = moduleList.GetAt( m );
				
				if( module->GetType() == formatType && module->GetStatus() == dellEngineModule::emsReady
				&& formatPattern.MatchesPattern( module->GetPattern() ) ){
					format.SetSupported( true );
					break;
				}
			}
		}
		
		if( ! format.GetSupported() ){
			pAllFormatsSupported = false;
		}
	}
	
	// check if the script module exists
	module = moduleList.GetNamed( pScriptModule.GetString() );
	
	pScriptModuleFound = module
		&& module->GetType() == deModuleSystem::emtScript
		&& module->GetStatus() == dellEngineModule::emsReady;
	
	// determine if the game is up to date
	pGameUpToDate = true;
	
	// determine if the game can run
	pCanRun = pAllFormatsSupported && pScriptModuleFound;
}

void dellGame::SetAllFormatsSupported( bool supported ){
	pAllFormatsSupported = supported;
}

void dellGame::SetScriptModuleFound( bool found ){
	pScriptModuleFound = found;
}

void dellGame::SetGameUpToDate( bool upToDate ){
	pGameUpToDate = upToDate;
}

void dellGame::SetCanRun( bool canRun ){
	pCanRun = canRun;
}



void dellGame::SetActiveProfile( dellGameProfile *profile ){
	pActiveProfile = profile;
}

void dellGame::SetCustomProfile( dellGameProfile *profile ){
	pCustomProfile = profile;
}

void dellGame::SetRunArguments( const char *arguments ){
	pRunArguments = arguments;
}

void dellGame::SetDelgaFile( const char *file ){
	pDelgaFile = file;
}

void dellGame::SetUseLatestPatch( bool useLatestPatch ){
	pUseLatestPatch = useLatestPatch;
}

void dellGame::SetUseCustomPatch( const decUuid &patch ){
	pUseCustomPatch = patch;
}

void dellGame::LoadConfig(){
	dellGameConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	
	decPath pathFile;
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfoFormat( LOGSOURCE, "Reading game configuration file for '%s'", pTitle.ToUTF8().GetString() );
			decBaseFileReaderReference reader;
			reader.TakeOver( vfs.OpenFileForReading( pathFile ) );
			configXML.ReadFromFile( reader, *this );
			
		}else{
			logger.LogErrorFormat( LOGSOURCE, "Game configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfoFormat( LOGSOURCE, "Game configuration file not found, will be created upon exiting" );
	}
}

void dellGame::SaveConfig(){
	dellGameConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	
	decPath pathFile;
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.CanWriteFile( pathFile ) ){
		logger.LogInfoFormat( LOGSOURCE, "Writing game configuration file for '%s'", pTitle.ToUTF8().GetString() );
		decBaseFileWriterReference writer;
		writer.TakeOver( vfs.OpenFileForWriting( pathFile ) );
		configXML.WriteToFile( writer, *this );
		
	}else{
		logger.LogError( LOGSOURCE, "Failed to write game configuration file (file writing problem)" );
	}
}

dellGameProfile *dellGame::GetProfileToUse() const{
	if( GetActiveProfile() ){
		return GetActiveProfile();
	}
	if( GetCustomProfile() ){
		return GetCustomProfile();
	}
	
	dellGameManager &gameManager = pLauncher.GetGameManager();
	
	if( gameManager.GetActiveProfile() ){
		return gameManager.GetActiveProfile();
	}
	
	return gameManager.GetDefaultProfile();
}
