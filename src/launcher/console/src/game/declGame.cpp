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

#include "declGame.h"
#include "declGameManager.h"
#include "declGameConfigXML.h"
#include "fileformat/declFileFormat.h"
#include "patch/declPatch.h"
#include "patch/declPatchManager.h"
#include "profile/declGameProfile.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"
#include "../engine/declEngine.h"
#include "../engine/modules/declEngineModule.h"
#include "../engine/modules/declEngineModuleList.h"

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



// Class declGame
///////////////////

// Constructors and Destructors
/////////////////////////////////

declGame::declGame( declLauncher &launcher ) :
pLauncher( launcher ),
pIconSize( 0 ),
pAllFormatsSupported( false ),
pScriptModuleFound( false ),
pGameUpToDate( false ),
pCanRun( false ),
pUseLatestPatch( true ){
}

declGame::~declGame(){
}



// Management
///////////////

void declGame::SetIdentifier( const decUuid &identifier ){
	pIdentifier = identifier;
}

void declGame::SetAliasIdentifier( const char *identifier ){
	pAliasIdentifier = identifier;
}

void declGame::SetTitle( const decUnicodeString &title ){
	pTitle = title;
}

void declGame::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void declGame::SetCreator( const decUnicodeString &creator ){
	pCreator = creator;
}

void declGame::SetHomepage( const char *homepage ){
	pHomepage = homepage;
}



void declGame::SetGameDirectory( const char *directory ){
	pGameDirectory = directory;
}

void declGame::SetDataDirectory( const char *directory ){
	pDataDirectory = directory;
}

void declGame::SetScriptDirectory( const char *directory ){
	pScriptDirectory = directory;
}

void declGame::SetGameObject( const char *gameObject ){
	pGameObject = gameObject;
}

void declGame::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void declGame::SetPathCapture( const char *path ){
	pPathCapture = path;
}

void declGame::SetWindowSize( const decPoint &size ){
	pWindowSize = size.Largest( decPoint() );
}

void declGame::SetIcon( const char* path, int size ){
	pIconPath = path;
	pIconSize = size;
}



void declGame::SetScriptModule( const char *module ){
	pScriptModule = module;
}

void declGame::SetScriptModuleVersion( const char *version ){
	pScriptModuleVersion = version;
}



void declGame::VerifyRequirements(){
	const declEngine &engine = *pLauncher.GetEngine();
	const declEngineModuleList &moduleList = engine.GetModuleList();
	int f, formatCount = pFileFormatList.GetCount();
	int m, moduleCount = moduleList.GetCount();
	declEngineModule *module;
	deModuleSystem::eModuleTypes formatType;
	
	if( GetCustomProfile() ){
		GetCustomProfile()->Verify( pLauncher );
	}
	
	// check if all file formats are supported
	pAllFormatsSupported = true;
	
	for( f=0; f<formatCount; f++ ){
		declFileFormat &format = *pFileFormatList.GetAt( f );
		formatType = format.GetType();
		
		format.SetSupported( false );
		
		if( ! deModuleSystem::IsSingleType( formatType ) ){
			const decString &formatPattern = format.GetPattern();
			
			for( m=0; m<moduleCount; m++ ){
				module = moduleList.GetAt( m );
				
				if( module->GetType() == formatType && module->GetStatus() == declEngineModule::emsReady
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
	if( pScriptModuleVersion.IsEmpty() ){
		module = moduleList.GetNamed( pScriptModule );
		
	}else{
		module = moduleList.GetNamedAtLeast( pScriptModule, pScriptModuleVersion );
	}
	
	pScriptModuleFound = module
		&& module->GetType() == deModuleSystem::emtScript
		&& module->GetStatus() == declEngineModule::emsReady;
	
	if( module ){
		pScriptModuleFoundVersion = module->GetVersion();
	}
	
	// determine if the game is up to date
	pGameUpToDate = true;
	
	// determine if the game can run
	pCanRun = pAllFormatsSupported && pScriptModuleFound;
}

void declGame::SetAllFormatsSupported( bool supported ){
	pAllFormatsSupported = supported;
}

void declGame::SetScriptModuleFound( bool found ){
	pScriptModuleFound = found;
}

void declGame::SetGameUpToDate( bool upToDate ){
	pGameUpToDate = upToDate;
}

void declGame::SetCanRun( bool canRun ){
	pCanRun = canRun;
}



void declGame::SetActiveProfile( declGameProfile *profile ){
	pActiveProfile = profile;
}

void declGame::SetCustomProfile( declGameProfile *profile ){
	pCustomProfile = profile;
}

void declGame::SetRunArguments( const char *arguments ){
	pRunArguments = arguments;
}

void declGame::SetDelgaFile( const char *file ){
	pDelgaFile = file;
}

void declGame::SetUseLatestPatch( bool useLatestPatch ){
	pUseLatestPatch = useLatestPatch;
}

void declGame::SetUseCustomPatch( const decUuid &patch ){
	pUseCustomPatch = patch;
}

void declGame::LoadConfig(){
	declGameConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
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

void declGame::SaveConfig(){
	declGameConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
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

declGameProfile *declGame::GetProfileToUse() const{
	if( GetActiveProfile() ){
		return GetActiveProfile();
	}
	if( GetCustomProfile() ){
		return GetCustomProfile();
	}
	
	declGameManager &gameManager = *pLauncher.GetGameManager();
	if( gameManager.GetActiveProfile() ){
		return gameManager.GetActiveProfile();
	}
	return gameManager.GetDefaultProfile();
}

void declGame::FindPatches( declPatchList &list ) const{
	const declPatchList &patches = pLauncher.GetPatchManager().GetPatches();
	const int count = patches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		declPatch * const patch = patches.GetAt( i );
		if( patch->GetGameID() == pIdentifier ){
			list.Add( patch );
		}
	}
}

void declGame::SortPatches( declPatchList &sorted, const declPatchList &patches ) const{
	const int count = patches.GetCount();
	bool hasAdded = true;
	int i, j;
	
	sorted.RemoveAll();
	
	while( hasAdded ){
		hasAdded = false;
		
		for( i=0; i<count; i++ ){
			declPatch * const patch = patches.GetAt( i );
			if( sorted.Has( patch ) ){
				continue;
			}
			
			const decUuidSet &requiredPatches = patch->GetRequiredPatches();
			const int requiredPatchCount = requiredPatches.GetCount();
			if( requiredPatchCount > 0 ){
				for( j=0; j<requiredPatchCount; j++ ){
					if( sorted.HasWithID( requiredPatches.GetAt( j ) ) ){
						break;
					}
				}
				if( j == requiredPatchCount ){
					continue;
				}
			}
			
			sorted.Add( patch );
			hasAdded = true;
		}
	}
}
