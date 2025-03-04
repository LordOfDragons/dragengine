/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "delGame.h"
#include "delGameManager.h"
#include "delGameConfigXML.h"
#include "delGameRunParams.h"
#include "fileformat/delFileFormat.h"
#include "icon/delGameIcon.h"
#include "patch/delPatch.h"
#include "patch/delPatchManager.h"
#include "profile/delGPModule.h"
#include "profile/delGPMParameter.h"
#include "profile/delGameProfile.h"
#include "../delLauncher.h"
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



// Class delGame
//////////////////

// Constructors and Destructors
/////////////////////////////////

delGame::delGame( delLauncher &launcher ) :
pLauncher( launcher ),

pAllFormatsSupported( false ),
pScriptModuleFound( false ),
pGameUpToDate( false ),
pCanRun( false ),

pLogToConsole( false ),

pUseLatestPatch( true ){
}

delGame::~delGame(){
	SetEngineInstance( nullptr );
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

decPoint delGame::GetDisplayScaledWindowSize() const{
	const int scaleFactor = pLauncher.GetEngine().GetScaleFactor();
	return decPoint(pWindowSize.x * scaleFactor / 100, pWindowSize.y * scaleFactor / 100);
}

void delGame::SetWindowSize( const decPoint &size ){
	pWindowSize = size.Largest( decPoint() );
}



bool delGame::HasEngineInstance() const{
	return pEngineInstance;
}

void delGame::SetEngineInstance( delEngineInstance *engineInstance ){
	pEngineInstance = engineInstance;
}



void delGame::VerifyRequirements(){
	const delEngine &engine = pLauncher.GetEngine();
	const delEngineModuleList &modules = engine.GetModules();
	int f, formatCount = pFileFormats.GetCount();
	int m, moduleCount = modules.GetCount();
	delEngineModule *module;
	
	// verify custom profile if present
	if( pCustomProfile ){
		GetCustomProfile()->Verify( pLauncher );
	}
	
	// check if all file formats are supported
	pAllFormatsSupported = true;
	
	for( f=0; f<formatCount; f++ ){
		delFileFormat &format = *pFileFormats.GetAt ( f );
		const deModuleSystem::eModuleTypes formatType = format.GetType();
		
		format.SetSupported( false );
		
		if( ! deModuleSystem::IsSingleType( formatType ) ){
			const decString &formatPattern = format.GetPattern();
			
			for( m=0; m<moduleCount; m++ ){
				module = modules.GetAt ( m );
				
				if( module->GetType() == formatType
				&& module->GetStatus() == delEngineModule::emsReady
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
		module = modules.GetNamed ( pScriptModule );
		
	}else{
		module = modules.GetNamedAtLeast ( pScriptModule, pScriptModuleVersion );
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



#ifdef OS_ANDROID
void delGame::SetVFSDelgaContainer(const deVFSContainer::Ref &container){
	pVFSDelgaContainer = container;
}
#endif



bool delGame::IsRunning() const{
	return pEngineInstance;
}

void delGame::StartGame( const delGameRunParams &runParams ){
	StartGame( runParams, pLauncher.GetEngineInstanceFactory() );
}

void delGame::StartGame( const delGameRunParams &runParams, delEngineInstance::Factory &factory ){
	decPath filePath;
	
	SaveConfig(); // ensure game profile exists
	
	if( IsRunning() ){
		DETHROW_INFO( deeInvalidAction, "game is running" );
	}
	if( ! pCanRun ){
		DETHROW_INFO( deeInvalidAction, "game can not run" );
	}
	if( ! runParams.GetGameProfile() ){
		DETHROW_INFO( deeNullPointer, "runParams.gameProfile" );
	}
	if( ! runParams.GetGameProfile()->GetValid() ){
		DETHROW_INFO( deeInvalidAction, "runParams.gameProfile is not valid" );
	}
	
	deLogger &logger = *pLauncher.GetLogger();
	decString logfile;
	
	// clear the log file. a bit an ugly hack but it works
	if( ! pLogToConsole ){
		logfile.Format( "games/%s/logs/last_run.log", pIdentifier.ToHexString( false ).GetString() );
		
		// using open file for writing instead of delete file to allow applications
		// to keep file open in a text viewer on operating systems preventing file
		// deleting while the file is open in an application (windows for example)
		filePath.SetFromUnix( "/logs" );
		filePath.AddUnixPath( logfile );
		decBaseFileWriter::Ref::New( pLauncher.GetVFS()->OpenFileForWriting( filePath ) );
	}
	
	// start the game
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Starting game '%s'", pTitle.ToUTF8().GetString() );
	
	try{
		// create engine instance and start engine
		pEngineInstance.TakeOver( factory.CreateEngineInstance( pLauncher, logfile ) );
		pEngineInstance->StartEngine();
		pEngineInstance->SetCacheAppID( pIdentifier.ToHexString( false ) );
		pEngineInstance->LoadModules();
		
		// activate profile
		runParams.GetGameProfile()->Activate( pLauncher, *pEngineInstance );
		
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
#ifdef OS_ANDROID
		if(pVFSDelgaContainer){
			pEngineInstance->VFSAddDelgaFileVfs(pVFSDelgaContainer, pDelgaFile, pDataDirectory, {});
			
		}else if(pDelgaFile.IsEmpty()){
			pEngineInstance->VFSAddDiskDir("/", pathDataDir.GetPathNative(), true);
			
		}else{
			pEngineInstance->VFSAddDelgaFile(pDelgaFile, pDataDirectory);
		}
#else
		if( pDelgaFile.IsEmpty() ){
			pEngineInstance->VFSAddDiskDir( "/", pathDataDir.GetPathNative(), true );
			
		}else{
			pEngineInstance->VFSAddDelgaFile( pDelgaFile, pDataDirectory );
		}
#endif
		
		int i;
		for( i=0; i<runParams.GetPatches().GetCount(); i++ ){
			const delPatch &patch = *runParams.GetPatches().GetAt( i );
			
			if( patch.GetDelgaFile().IsEmpty() ){
				pathDataDir.SetFromNative( patch.GetPatchDirectory() );
				pathDataDir.AddUnixPath( patch.GetDataDirectory() );
				pEngineInstance->VFSAddDiskDir( "/", pathDataDir.GetPathNative(), true );
				
			}else{
				pEngineInstance->VFSAddDelgaFile( patch.GetDelgaFile(),
					patch.GetDataDirectory(), patch.GetHiddenPath() );
			}
		}
		
		pEngineInstance->VFSAddScriptSharedDataDir();
		
		pEngineInstance->ModulesAddVFSContainers( deModuleSystem::VFSStagePatches );
		pEngineInstance->ModulesAddVFSContainers( deModuleSystem::VFSStageMods );
		pEngineInstance->ModulesAddVFSContainers( deModuleSystem::VFSStageOverlay );
		
		filePath.SetFromNative( pLauncher.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "overlay" );
		pEngineInstance->VFSAddDiskDir( "/", filePath.GetPathNative(), false );
		
		pEngineInstance->SetPathOverlay( filePath.GetPathNative() );
		
		filePath.SetFromNative( pLauncher.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "config" );
		pEngineInstance->VFSAddDiskDir( pPathConfig, filePath.GetPathNative(), false );
		
		pEngineInstance->SetPathConfig( filePath.GetPathNative() );
		
		filePath.SetFromNative( pLauncher.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "capture" );
		pEngineInstance->VFSAddDiskDir( pPathCapture, filePath.GetPathNative(), false );
		
		pEngineInstance->SetPathCapture( filePath.GetPathNative() );
		
		// create render window. for the time being use the largest icon present
		delGameIcon * const icon = pIcons.GetLargest();
		
		pEngineInstance->CreateRenderWindow( runParams.GetWidth(), runParams.GetHeight(),
			runParams.GetFullScreen(), pTitle.ToUTF8(), icon ? icon->GetPath().GetString() : "" );
		
		// store information for handling parameter changes during runtime
		pCollectChangedParams.RemoveAll();
		pCollectChangedParamsProfile.TakeOver( pLauncher.CreateGameProfile( runParams.GetGameProfile() ) );
		
		// start game
		pEngineInstance->StartGame( pScriptDirectory, pScriptModuleVersion,
			pGameObject, &pCollectChangedParams );
		
	}catch( const deException &e ){
		logger.LogErrorFormat( pLauncher.GetLogSource(),
			"Game '%s' caused an exception", pTitle.ToUTF8().GetString() );
		logger.LogException( pLauncher.GetLogSource(), e );
		StopGame();
		throw;
	}
	
	// it is possible to game is now stopped. this can happen if a direct engine instance
	PulseChecking();
}

void delGame::StopGame(){
	if( ! IsRunning() ){
		DETHROW_INFO( deeInvalidAction, "game is not running" );
	}
	if( ! pEngineInstance ){
		DETHROW_INFO( deeNullPointer, "engineInstance" );
	}
	
	deLogger &logger = *pLauncher.GetLogger();
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Stopping game '%s'", pTitle.ToUTF8().GetString() );
	
	// calling IsGameRunning collects information if threaded
	if(pEngineInstance->IsGameRunning() != 0){
		pEngineInstance->StopGame();
	}
	
	// ensure information is collected. and if still running kill the process
	if(pEngineInstance->IsGameRunning() != 0){
		pEngineInstance->KillEngine();
	}
	SetEngineInstance( nullptr );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Game '%s' stopped", pTitle.ToUTF8().GetString() );
	pStoreCustomConfig();
}

void delGame::KillGame(){
	if( ! IsRunning() || ! pEngineInstance ) DETHROW( deeInvalidAction );
	
	deLogger &logger = *pLauncher.GetLogger();
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Killing game '%s'", pTitle.ToUTF8().GetString() );
	
	pEngineInstance->KillEngine();
	SetEngineInstance( nullptr );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Game '%s' killed", pTitle.ToUTF8().GetString() );
}

void delGame::PulseChecking(){
	if( ! IsRunning() ){
		return;
	}
	
	try{
		if( pEngineInstance->IsGameRunning() == 0 ){
			pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
				"Game '%s' is no longer running", pTitle.ToUTF8().GetString() );
			StopGame();
		}
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Game '%s' caused an exception", pTitle.ToUTF8().GetString() );
		pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
		KillGame(); // just to make sure the process is gone
	}
}

void delGame::LoadConfig(){
	delGameConfigXML configXML( pLauncher );
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfoFormat( pLauncher.GetLogSource(),
				"Reading game configuration file for '%s'", pTitle.ToUTF8().GetString() );
			configXML.ReadFromFile( decBaseFileReader::Ref::New( vfs.OpenFileForReading( pathFile ) ), *this );
			
		}else{
			logger.LogErrorFormat( pLauncher.GetLogSource(),
				"Game configuration file is not a regular file" );
			DETHROW_INFO( deeInvalidParam, "game configuration file is not regular file" );
		}
		
	}else{
		logger.LogInfoFormat( pLauncher.GetLogSource(),
			"Game configuration file not found, will be created upon exiting" );
	}
	
	// VR module exists since version 1.6 . if absent use module from default profile
	const delGameProfile * const defaultProfile = pLauncher.GetGameManager().GetDefaultProfile();
	if( defaultProfile && pCustomProfile && pCustomProfile->GetModuleVR().IsEmpty() ){
		pCustomProfile->SetModuleVR( defaultProfile->GetModuleVR() );
		pCustomProfile->SetModuleVRVersion( defaultProfile->GetModuleVRVersion() );
	}
}

void delGame::SaveConfig(){
	delGameConfigXML configXML( pLauncher );
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
	pathFile.AddComponent( "launcher.xml" );
	
	if( vfs.CanWriteFile( pathFile ) ){
		logger.LogInfoFormat( pLauncher.GetLogSource(),
			"Writing game configuration file for '%s'", pTitle.ToUTF8().GetString() );
		
		try{
			configXML.WriteToFile( decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( pathFile ) ), *this );
			
		}catch( const deException & ){
			logger.LogError( pLauncher.GetLogSource(),
				"Failed to write game configuration file (file permission problem)" );
			// DIALOG BOX
			// "Game configuration can not be written!\n"
			// "Make sure you have write permission for the file and parent directory.\n"
			// "Game configuration has not been saved!\n"
		}
		
	}else{
		logger.LogError( pLauncher.GetLogSource(),
			"Failed to write game configuration file (file writing problem)" );
		
		// DIALOG BOX
		// "Game configuration can not be written!\n"
		// "Make sure you have write permission for the file and parent directory.\n"
		// "Game configuration has not been saved!\n"
	}
}

delGameProfile *delGame::GetProfileToUse() const{
	if( pActiveProfile ){
		return pActiveProfile;
	}
	if( pCustomProfile ){
		return pCustomProfile;
	}
	
	const delGameManager &gameManager = pLauncher.GetGameManager();
	if( gameManager.GetActiveProfile() ){
		return gameManager.GetActiveProfile();
	}
	
	return gameManager.GetDefaultProfile();
}

void delGame::FindPatches( delPatchList &list ) const{
	delPatchList patches( pLauncher.GetPatchManager().GetPatches() );
	patches.AddAll( pLocalPatches );
	
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
	if( pCollectChangedParams.GetCount() == 0 || ! pCollectChangedParamsProfile ){
		return;
	}
	
	deLogger &logger = *pLauncher.GetLogger();
	logger.LogInfo( pLauncher.GetLogSource(), "Engine module parameters changed. Store custom profile" );
	
	// ensure custom profile exists and is initialized with profile used to run the game
	// unless this had been already the custom profile
	if( ! pCustomProfile ){
		pCustomProfile.TakeOver( pLauncher.CreateGameProfile() );
	}
	
	if( pCollectChangedParamsProfile != pCustomProfile ){
		*pCustomProfile = *pCollectChangedParamsProfile; // copy content not pointer
	}
	
	// update custom profile
	pCustomProfile->GetModules().Update( pCollectChangedParams );
	
	// if game wants window mode adjust the profile to have the same values
	if( pWindowSize != decPoint() ){
		pCustomProfile->SetFullScreen( false );
		pCustomProfile->SetWidth( pWindowSize.x );
		pCustomProfile->SetHeight( pWindowSize.y );
	}
	
	// save configuration
	SaveConfig();
	
	// verify profile and game so it is immediately usable
	pCustomProfile->Verify( pLauncher );
}
