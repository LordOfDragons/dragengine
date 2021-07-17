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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "delGame.h"
#include "delGameManager.h"
#include "delGameConfigXML.h"
#include "delGameRunParams.h"
#include "fileformat/delFileFormat.h"
#include "patch/delPatch.h"
#include "patch/delPatchManager.h"
#include "profile/delGPModule.h"
#include "profile/delGPMParameter.h"
#include "profile/delGameProfile.h"
#include "../delLauncherSupport.h"
#include "../engine/delEngine.h"
#include "../engine/delEngineInstance.h"
#include "../engine/modules/delEngineModule.h"
#include "../engine/modules/delEngineModuleList.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decMemoryFile.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class delGame
///////////////////

// Constructors and Destructors
/////////////////////////////////

delGame::delGame( delLauncherSupport &support ) :
pSupport( support ),

pEngineInstance( NULL ),

pAllFormatsSupported( false ),
pScriptModuleFound( false ),
pGameUpToDate( false ),
pCanRun( false ),

pLogToConsole( false ),

pUseLatestPatch( true ){
}

delGame::~delGame(){
	SetEngineInstance( NULL );
}



// Management
///////////////

void delGame::SetIdentifier( const decUuid &identifier ){
	pIdentifier = identifier;
}

void delGame::SetAliasIdentifier( const char *identifier ){
	pAliasIdentifier = identifier;
}

void delGame::SetTitle( const decUnicodeString &title ){
	pTitle = title;
}

void delGame::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void delGame::SetCreator( const decUnicodeString &creator ){
	pCreator = creator;
}

void delGame::SetHomepage( const char *homepage ){
	pHomepage = homepage;
}



void delGame::SetGameDirectory( const char *directory ){
	pGameDirectory = directory;
}

void delGame::SetDataDirectory( const char *directory ){
	pDataDirectory = directory;
}

void delGame::SetScriptDirectory( const char *directory ){
	pScriptDirectory = directory;
}

void delGame::SetGameObject( const char *gameObject ){
	pGameObject = gameObject;
}

void delGame::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void delGame::SetPathCapture( const char *path ){
	pPathCapture = path;
}



void delGame::SetScriptModule( const char *module ){
	pScriptModule = module;
}

void delGame::SetScriptModuleVersion( const char *version ){
	pScriptModuleVersion = version;
}

void delGame::SetWindowSize( const decPoint &size ){
	pWindowSize = size.Largest( decPoint() );
}



bool delGame::HasEngineInstance() const{
	return pEngineInstance != NULL;
}

void delGame::SetEngineInstance( delEngineInstance *engineInstance ){
	if( engineInstance == pEngineInstance ){
		return;
	}
	
	if( pEngineInstance ){
		delete pEngineInstance;
	}
	
	pEngineInstance = engineInstance;
}



void delGame::VerifyRequirements(){
	const delEngine &engine = *pSupport.GetEngine();
	const delEngineModuleList &moduleList = engine.GetModuleList();
	int f, formatCount = pFileFormats.GetFormatCount();
	int m, moduleCount = moduleList.GetModuleCount();
	delEngineModule *module;
	deModuleSystem::eModuleTypes formatType;
	
	// verify custom profile if present
	if( pCustomProfile ){
		GetCustomProfile()->Verify( pSupport );
	}
	
	// check if all file formats are supported
	pAllFormatsSupported = true;
	
	for( f=0; f<formatCount; f++ ){
		delFileFormat &format = *pFileFormats.GetFormatAt( f );
		formatType = format.GetType();
		
		format.SetSupported( false );
		
		if( ! deModuleSystem::IsSingleType( formatType ) ){
			const decString &formatPattern = format.GetPattern();
			
			for( m=0; m<moduleCount; m++ ){
				module = moduleList.GetModuleAt( m );
				
				if( module->GetType() == formatType && module->GetStatus() == delEngineModule::emsReady
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
	if( pScriptModuleVersion.IsEmpty() ){
		module = moduleList.GetModuleNamed( pScriptModule );
		
	}else{
		module = moduleList.GetModuleNamedAtLeast( pScriptModule, pScriptModuleVersion );
	}
	
	pScriptModuleFound = module
		&& module->GetType() == deModuleSystem::emtScript
		&& module->GetStatus() == delEngineModule::emsReady;
	
	if( module ){
		pScriptModuleFoundVersion = module->GetVersion();
	}
	
	// determine if the game is up to date
	pGameUpToDate = true;
	
	// determine if the game can run
	pCanRun = pAllFormatsSupported && pScriptModuleFound;
}

void delGame::SetAllFormatsSupported( bool supported ){
	pAllFormatsSupported = supported;
}

void delGame::SetScriptModuleFound( bool found ){
	pScriptModuleFound = found;
}

void delGame::SetGameUpToDate( bool upToDate ){
	pGameUpToDate = upToDate;
}

void delGame::SetCanRun( bool canRun ){
	pCanRun = canRun;
}



void delGame::SetCustomProfile( delGameProfile *profile ){
	pCustomProfile = profile;
}

void delGame::SetActiveProfile( delGameProfile *profile ){
	pActiveProfile = profile;
}

void delGame::SetRunArguments( const char *arguments ){
	pRunArguments = arguments;
}



void delGame::SetLogFile( const char *path ){
	pLogFile = path;
}

void delGame::SetDefaultLogFile(){
	pLogFile.Format( "games/%s/logs/last_run.log", pIdentifier.ToHexString( false ).GetString() );
}

void delGame::SetLogToConsole( bool logToConsole ){
	pLogToConsole = logToConsole;
}

void delGame::SetDelgaFile( const char *delgaFile ){
	pDelgaFile = delgaFile;
}

void delGame::SetUseLatestPatch( bool useLatestPatch ){
	pUseLatestPatch = useLatestPatch;
}

void delGame::SetUseCustomPatch( const decUuid &patch ){
	pUseCustomPatch = patch;
}



bool delGame::IsRunning() const{
	return pEngineInstance != NULL;
}

void delGame::StartGame( const delGameRunParams &runParams ){
	decPath filePath;
	
	if( IsRunning() || ! pCanRun || ! runParams.GetGameProfile()
	|| ! runParams.GetGameProfile()->GetValid() ){
		DETHROW( deeInvalidAction );
	}
	
	deLogger &logger = *pSupport.GetLogger();
	decString logfile;
	
	// clear the log file. a bit an ugly hack but it works
	if( ! pLogToConsole ){
		logfile.Format( "games/%s/logs/last_run.log", pIdentifier.ToHexString( false ).GetString() );
		
		// using open file for writing instead of delete file to allow applications
		// to keep file open in a text viewer on operating systems preventing file
		// deleting while the file is open in an application (windows for example)
		filePath.SetFromUnix( "/logs" );
		filePath.AddUnixPath( logfile );
		decBaseFileWriter::Ref::With( pSupport.GetVFS()->OpenFileForWriting( filePath ) );
	}
	
	// start the game
	logger.LogInfoFormat( LOGSOURCE, "Starting game '%s'", pTitle.ToUTF8().GetString() );
	
	try{
		// create engine instance and start engine
		pEngineInstance = new delEngineInstance( pSupport, logfile );
		pEngineInstance->StartEngine();
		pEngineInstance->SetCacheAppID( pIdentifier.ToHexString( false ) );
		pEngineInstance->LoadModules();
		
		// activate profile
		runParams.GetGameProfile()->Activate( pSupport, *pEngineInstance );
		
		// activate script module and set the path.
		pEngineInstance->ActivateModule( pScriptModule, pScriptModuleFoundVersion );
		
		// set data directory. the engine expects an absolute native path. it is composed of the
		// data directory placed underneath the game directory
		decPath pathDataDir;
		pathDataDir.SetFromNative( pGameDirectory );
		pathDataDir.AddUnixPath( pDataDirectory );
		pEngineInstance->SetDataDirectory( pathDataDir.GetPathNative() );
		
		// set command line arguments
		pEngineInstance->SetCmdLineArgs( runParams.GetRunArguments() );
		
		// set up virtual file system
		if( pDelgaFile.IsEmpty() ){
			pEngineInstance->VFSAddDiskDir( "/", pathDataDir.GetPathNative(), true );
			
		}else{
			pEngineInstance->VFSAddDelgaFile( pDelgaFile, pDataDirectory );
		}
		
		int i;
		for( i=0; i<runParams.GetPatches().GetCount(); i++ ){
			const delPatch &patch = *runParams.GetPatches().GetAt( i );
			
			if( patch.GetDelgaFile().IsEmpty() ){
				pathDataDir.SetFromNative( patch.GetPatchDirectory() );
				pathDataDir.AddUnixPath( patch.GetDataDirectory() );
				pEngineInstance->VFSAddDiskDir( "/", pathDataDir.GetPathNative(), true );
				
			}else{
				pEngineInstance->VFSAddDelgaFile( patch.GetDelgaFile(), patch.GetDataDirectory() );
			}
		}
		
		pEngineInstance->VFSAddScriptSharedDataDir();
		
		filePath.SetFromNative( pSupport.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "overlay" );
		pEngineInstance->VFSAddDiskDir( "/", filePath.GetPathNative(), false );
		
		filePath.SetFromNative( pSupport.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "config" );
		pEngineInstance->VFSAddDiskDir( pPathConfig, filePath.GetPathNative(), false );
		
		filePath.SetFromNative( pSupport.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "capture" );
		pEngineInstance->VFSAddDiskDir( pPathCapture, filePath.GetPathNative(), false );
		
		// create render window
		pEngineInstance->CreateRenderWindow( runParams.GetWidth(), runParams.GetHeight(),
			runParams.GetFullScreen(), pTitle.ToUTF8(), pIconPath );
		
		// store information for handling parameter cchanges during runtime
		pCollectChangedParams.RemoveAllModules();
		pCollectChangedParamsProfile.TakeOver( new delGameProfile( *runParams.GetGameProfile() ) );
		
		// start game
		pEngineInstance->StartGame( pScriptDirectory, pGameObject, &pCollectChangedParams );
		
	}catch( const deException &e ){
		logger.LogErrorFormat( LOGSOURCE, "Game '%s' caused an exception", pTitle.ToUTF8().GetString() );
		logger.LogException( LOGSOURCE, e );
		StopGame();
		throw;
	}
}

void delGame::StopGame(){
	if( ! IsRunning() || ! pEngineInstance ) DETHROW( deeInvalidAction );
	
	deLogger &logger = *pSupport.GetLogger();
	
	logger.LogInfoFormat( LOGSOURCE, "Stopping game '%s'", pTitle.ToUTF8().GetString() );
	
	pEngineInstance->StopEngine();
	delete pEngineInstance;
	pEngineInstance = NULL;
	
	logger.LogInfoFormat( LOGSOURCE, "Game '%s' stopped", pTitle.ToUTF8().GetString() );
	pStoreCustomConfig();
}

void delGame::KillGame(){
	if( ! IsRunning() || ! pEngineInstance ) DETHROW( deeInvalidAction );
	
	deLogger &logger = *pSupport.GetLogger();
	
	logger.LogInfoFormat( LOGSOURCE, "Killing game '%s'", pTitle.ToUTF8().GetString() );
	
	pEngineInstance->KillProcess();
	delete pEngineInstance;
	pEngineInstance = NULL;
	
	logger.LogInfoFormat( LOGSOURCE, "Game '%s' killed", pTitle.ToUTF8().GetString() );
}

void delGame::PulseChecking(){
	if( IsRunning() ){
		try{
			if( pEngineInstance->IsGameRunning() == 0 ){
				pSupport.GetLogger()->LogInfoFormat( LOGSOURCE, "Game '%s' is no longer running", pTitle.ToUTF8().GetString() );
				StopGame();
			}
			
		}catch( const deException &e ){
			pSupport.GetLogger()->LogInfoFormat( LOGSOURCE, "Game '%s' caused an exception", pTitle.ToUTF8().GetString() );
			pSupport.GetLogger()->LogException( LOGSOURCE, e );
			KillGame(); // just to make sure the process is gone
		}
	}
}

void delGame::LoadConfig(){
	delGameConfigXML configXML( pSupport.GetLogger(), LOGSOURCE, pSupport.GetGameManager() );
	deVirtualFileSystem &vfs = *pSupport.GetVFS();
	deLogger &logger = *pSupport.GetLogger();
	decBaseFileReader::Ref reader;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfoFormat( LOGSOURCE, "Reading game configuration file for '%s'", pTitle.ToUTF8().GetString() );
			
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

void delGame::SaveConfig(){
	delGameConfigXML configXML( pSupport.GetLogger(), LOGSOURCE, pSupport.GetGameManager() );
	deVirtualFileSystem &vfs = *pSupport.GetVFS();
	deLogger &logger = *pSupport.GetLogger();
	decBaseFileWriter::Ref writer;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.CanWriteFile( pathFile ) ){
		logger.LogInfoFormat( LOGSOURCE, "Writing game configuration file for '%s'", pTitle.ToUTF8().GetString() );
		
		try{
			writer.TakeOver( vfs.OpenFileForWriting( pathFile ) );
			configXML.WriteToFile( writer, *this );
			
		}catch( const deException & ){
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

delGameProfile *delGame::GetProfileToUse() const{
	if( pActiveProfile ){
		return ( delGameProfile* )( deObject* )pActiveProfile;
	}
	if( pCustomProfile ){
		return ( delGameProfile* )( deObject* )pCustomProfile;
	}
	
	delGameManager &gameManager = *pSupport.GetGameManager();
	if( gameManager.GetActiveProfile() ){
		return gameManager.GetActiveProfile();
	}
	
	return gameManager.GetDefaultProfile();
}

void delGame::FindPatches( delPatchList &list ) const{
	const delPatchList &patches = pSupport.GetPatchManager().GetPatches();
	const int count = patches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delPatch * const patch = patches.GetAt( i );
		if( patch->GetGameID() == pIdentifier ){
			list.Add( patch );
		}
	}
}

void delGame::SortPatches( delPatchList &sorted, const delPatchList &patches ) const{
	const int count = patches.GetCount();
	bool hasAdded = true;
	int i, j;
	
	sorted.RemoveAll();
	
	while( hasAdded ){
		hasAdded = false;
		
		for( i=0; i<count; i++ ){
			delPatch * const patch = patches.GetAt( i );
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



// Private Functions
//////////////////////

void delGame::pStoreCustomConfig(){
	if( pCollectChangedParams.GetModuleCount() == 0 || ! pCollectChangedParamsProfile ){
		return;
	}
	
	// ensure custom profile exists and is initialized with profile used to run the game
	// unless this had been already the custom profile
	if( ! pCustomProfile ){
		pCustomProfile.TakeOver( new delGameProfile );
	}
	
	if( pCollectChangedParamsProfile != pCustomProfile ){
		*pCustomProfile = *pCollectChangedParamsProfile; // copy content not pointer
	}
	
	// update custom profile
	delGPModuleList &modules = pCustomProfile->GetModuleList();
	const int moduleCount = pCollectChangedParams.GetModuleCount();
	int i, j;
	
	for( i=0; i<moduleCount; i++ ){
		const delGPModule &moduleChanges = *pCollectChangedParams.GetModuleAt( i );
		delGPModule * const module = modules.GetModuleNamed( moduleChanges.GetName() );
		
		if( module ){
			delGPMParameterList &params = module->GetParameterList();
			const delGPMParameterList &paramsChanges = moduleChanges.GetParameterList();
			const int paramCount = paramsChanges.GetParameterCount();
			
			for( j=0; j<paramCount; j++ ){
				const delGPMParameter &paramChanges = *paramsChanges.GetParameterAt( j );
				delGPMParameter * const parameter = params.GetParameterNamed( paramChanges.GetName() );
				
				if( parameter ){
					parameter->SetValue( paramChanges.GetValue() );
					
				}else{
					params.AddParameter( delGPMParameter::Ref::With( new delGPMParameter( paramChanges ) ) );
				}
			}
			
		}else{
			modules.AddModule( delGPModule::Ref::With( new delGPModule( moduleChanges ) ) );
		}
	}
	
	// if game wants window mode adjust the profile to have the same values
	if( pWindowSize != decPoint() ){
		pCustomProfile->SetFullScreen( false );
		pCustomProfile->SetWidth( pWindowSize.x );
		pCustomProfile->SetHeight( pWindowSize.y );
	}
	
	// save configuration
	SaveConfig();
	
	// verify profile and game so it is immediately usable
	pCustomProfile->Verify( pSupport );
}
