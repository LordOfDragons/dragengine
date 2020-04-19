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

#include "dealGame.h"
#include "dealGameManager.h"
#include "dealGameConfigXML.h"
#include "dealGameRunParams.h"
#include "fileformat/dealFileFormat.h"
#include "profile/dealGameProfile.h"
#include "../dealLauncher.h"
#include "../config/dealConfiguration.h"
#include "../engine/dealEngine.h"
#include "../engine/modules/dealEngineModule.h"
#include "../engine/modules/dealEngineModuleList.h"
#include "../../srcengine/dealIEngineInstance.h"

#include "../logger/deLogger.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../filesystem/deVFSContainer.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/file/decDiskFileWriter.h"
#include "../common/file/decPath.h"
#include "../common/exceptions.h"



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class dealGame
///////////////////

// Constructors and Destructors
/////////////////////////////////

dealGame::dealGame( dealLauncher &launcher ) :
pLauncher( launcher ){
	pAllFormatsSupported = false;
	pScriptModuleFound = false;
	pGameUpToDate = false;
	pCanRun = false;
	
	pGlobalProfile = NULL;
	pGameProfile = NULL;
}

dealGame::~dealGame(){
	SetGlobalProfile( NULL );
	SetGameProfile( NULL );
}



// Management
///////////////

void dealGame::SetIdentifier( const char *identifier ){
	pIdentifier = identifier;
}

void dealGame::SetTitle( const decUnicodeString &title ){
	pTitle = title;
}

void dealGame::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void dealGame::SetCreator( const decUnicodeString &creator ){
	pCreator = creator;
}

void dealGame::SetHomepage( const char *homepage ){
	pHomepage = homepage;
}



void dealGame::SetGameDirectory( const char *directory ){
	pGameDirectory = directory;
}

void dealGame::SetDataDirectory( const char *directory ){
	pDataDirectory = directory;
}

void dealGame::SetScriptDirectory( const char *directory ){
	pScriptDirectory = directory;
}

void dealGame::SetGameObject( const char *gameObject ){
	pGameObject = gameObject;
}

void dealGame::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void dealGame::SetPathCapture( const char *path ){
	pPathCapture = path;
}



void dealGame::SetScriptModule( const char *module ){
	pScriptModule = module;
}

void dealGame::SetWindowSize( const decPoint &size ){
	pWindowSize = size.Largest( decPoint() );
}



void dealGame::VerifyRequirements(){
	const dealEngine &engine = pLauncher.GetEngine();
	const dealEngineModuleList &moduleList = engine.GetModuleList();
	int f, formatCount = pFileFormatList.GetFormatCount();
	int m, moduleCount = moduleList.GetModuleCount();
	dealEngineModule *module;
	dealEngineModule::eModuleTypes formatType;
	
	// validate profiles
	pProfileList.ValidateProfiles( pLauncher );
	
	// check if all file formats are supported
	pAllFormatsSupported = true;
	
	for( f=0; f<formatCount; f++ ){
		dealFileFormat &format = *pFileFormatList.GetFormatAt( f );
		formatType = format.GetType();
		
		format.SetSupported( false );
		
		if( ! dealEngineModule::IsSingleType( formatType ) ){
			const decString &formatPattern = format.GetPattern();
			
			for( m=0; m<moduleCount; m++ ){
				module = moduleList.GetModuleAt( m );
				
				if( module->GetType() == formatType && module->GetStatus() == dealEngineModule::emsReady
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
	
	// check if the script module exists. we can only check if a module of any version exists.
	// the actual version check can only be done once the actual profile is known
	module = moduleList.GetModuleNamed( pScriptModule );
	
	pScriptModuleFound = module
		&& module->GetType() == dealEngineModule::emtScript
		&& module->GetStatus() == dealEngineModule::emsReady;
	
	// determine if the game is up to date
	pGameUpToDate = true;
	
	// determine if the game can run
	pCanRun = pAllFormatsSupported && pScriptModuleFound;
}

void dealGame::SetAllFormatsSupported( bool supported ){
	pAllFormatsSupported = supported;
}

void dealGame::SetScriptModuleFound( bool found ){
	pScriptModuleFound = found;
}

void dealGame::SetGameUpToDate( bool upToDate ){
	pGameUpToDate = upToDate;
}

void dealGame::SetCanRun( bool canRun ){
	pCanRun = canRun;
}



void dealGame::SetGlobalProfile( dealGameProfile *profile ){
	if( profile != pGlobalProfile ){
		if( pGlobalProfile ){
			pGlobalProfile->FreeReference();
		}
		
		pGlobalProfile = profile;
		
		if( profile ){
			profile->AddReference();
		}
	}
}

void dealGame::SetGameProfile( dealGameProfile *profile ){
	if( profile != pGameProfile ){
		if( pGameProfile ){
			pGameProfile->FreeReference();
		}
		
		pGameProfile = profile;
		
		if( profile ){
			profile->AddReference();
		}
	}
}

void dealGame::SetRunArguments( const char *arguments ){
	if( ! arguments ){
		DETHROW( deeInvalidParam );
	}
	
	pRunArguments = arguments;
}



void dealGame::LoadConfig(){
	dealGameConfigXML configXML( &pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
	decBaseFileReader *reader = NULL;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfoFormat( LOGSOURCE, "Reading game configuration file for '%s'", pTitle.ToUTF8().GetString() );
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
			logger.LogErrorFormat( LOGSOURCE, "Game configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfoFormat( LOGSOURCE, "Game configuration file not found, will be created upon exiting" );
	}
}

void dealGame::SaveConfig(){
	dealGameConfigXML configXML( &pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
	decBaseFileWriter *writer = NULL;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.CanWriteFile( pathFile ) ){
		logger.LogInfoFormat( LOGSOURCE, "Writing game configuration file for '%s'", pTitle.ToUTF8().GetString() );
		
		try{
			writer = vfs.OpenFileForWriting( pathFile );
			configXML.WriteToFile( *writer, *this );
			writer->FreeReference();
			
		}catch( const deException & ){
			if( writer ){
				writer->FreeReference();
			}
			logger.LogError( LOGSOURCE, "Failed to write game configuration file (file permission problem)" );
			// DIALOG BOX
			// "Game configuration can not be written!\n"
			// "Make sure you have write permission for the file and parent directory.\n"
			// "Game configuration has not been saved!\n"
		}
		
	}else{
		logger.LogError( LOGSOURCE, "Failed to write game configuration file (file writing problem)" );
		
		// DIALOG BOX
		// "Game configuration can not be written!\n"
		// "Make sure you have write permission for the file and parent directory.\n"
		// "Game configuration has not been saved!\n"
	}
}

void dealGame::ApplyProfileChanges(){
	if( ! pProfileList.Has( pGameProfile ) ){
		SetGlobalProfile( NULL );
		VerifyRequirements();
	}
}

dealGameProfile *dealGame::GetProfileToUse() const{
	if( pGameProfile ){
		return pGameProfile;
	}
	
	if( pGlobalProfile ){
		return pGlobalProfile;
	}
	
	dealGameManager &gameManager = pLauncher.GetGameManager();
	
	if( gameManager.GetActiveProfile() ){
		return gameManager.GetActiveProfile();
	}
	
	return gameManager.GetDefaultProfile();
}
