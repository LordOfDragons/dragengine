/* 
 * Drag[en]gine GUI Launcher
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

#include "deglGame.h"
#include "deglGameManager.h"
#include "deglGameConfigXML.h"
#include "deglGameRunParams.h"
#include "fileformat/deglFileFormat.h"
#include "patch/deglPatch.h"
#include "patch/deglPatchManager.h"
#include "profile/deglGPModule.h"
#include "profile/deglGPMParameter.h"
#include "profile/deglGameProfile.h"
#include "../deglLauncher.h"
#include "../config/deglConfiguration.h"
#include "../engine/deglEngine.h"
#include "../engine/deglEngineInstance.h"
#include "../engine/modules/deglEngineModule.h"
#include "../engine/modules/deglEngineModuleList.h"
#include "../gui/deglSharedIcon.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decMemoryFile.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class deglGame
///////////////////

// Constructors and Destructors
/////////////////////////////////

deglGame::deglGame( deglLauncher &launcher ) :
pLauncher( launcher ),

pIconSize( 0 ),

pFoxIconMini( NULL ),
pFoxIconBig( NULL ),
pEngineInstance( NULL ),

pAllFormatsSupported( false ),
pScriptModuleFound( false ),
pGameUpToDate( false ),
pCanRun( false ),

pLogToConsole( false ),

pUseLatestPatch( true ){
}

deglGame::~deglGame(){
	SetEngineInstance( NULL );
}



// Management
///////////////

void deglGame::SetIdentifier( const decUuid &identifier ){
	pIdentifier = identifier;
}

void deglGame::SetAliasIdentifier( const char *identifier ){
	pAliasIdentifier = identifier;
}

void deglGame::SetTitle( const decUnicodeString &title ){
	pTitle = title;
}

void deglGame::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void deglGame::SetCreator( const decUnicodeString &creator ){
	pCreator = creator;
}

void deglGame::SetHomepage( const char *homepage ){
	pHomepage = homepage;
}



void deglGame::SetGameDirectory( const char *directory ){
	pGameDirectory = directory;
}

void deglGame::SetDataDirectory( const char *directory ){
	pDataDirectory = directory;
}

void deglGame::SetScriptDirectory( const char *directory ){
	pScriptDirectory = directory;
}

void deglGame::SetGameObject( const char *gameObject ){
	pGameObject = gameObject;
}

void deglGame::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void deglGame::SetPathCapture( const char *path ){
	pPathCapture = path;
}



void deglGame::SetScriptModule( const char *module ){
	pScriptModule = module;
}

void deglGame::SetScriptModuleVersion( const char *version ){
	pScriptModuleVersion = version;
}

void deglGame::SetWindowSize( const decPoint &size ){
	pWindowSize = size.Largest( decPoint() );
}

void deglGame::SetIcon( const char* path, int size ){
	pIconPath = path;
	pIconSize = size;
}

void deglGame::SetIconFileContent( decMemoryFile *content ){
	pIconFileContent = content;
}

void deglGame::CreateFoxIcons(){
	pFoxIconMini = NULL;
	pFoxIconBig = NULL;
	
	if( ! pIconFileContent ){
		return;
	}
	
	// try PNG for the time being. later on try other formats too
	FXMemoryStream stream( FXStreamLoad, ( FXuchar* )pIconFileContent->GetPointer(), pIconFileContent->GetLength() );
	const int quality = 2;
		// quality 0: fast but ugly scale
		// quality 1: slower box filtered scale
		// quality 2: slow gamma corrected scale
	
	try{
		stream.position( 0 );
		pFoxIconBig.TakeOver( new deglSharedIcon( new FXPNGIcon( FXApp::instance() ) ) );
		FXIcon &iconBig = *( ( deglSharedIcon& )( deObject& )pFoxIconBig ).GetIcon();
		iconBig.loadPixels( stream );
		iconBig.scale( 48, 48, quality );
		iconBig.create();
		
		stream.position( 0 );
		pFoxIconMini.TakeOver( new deglSharedIcon( new FXPNGIcon( FXApp::instance() ) ) );
		FXIcon &iconMini = *( ( deglSharedIcon& )( deObject& )pFoxIconMini ).GetIcon();
		iconMini.loadPixels( stream );
		iconMini.scale( 32, 32, quality );
		iconMini.create();
		
	}catch( const deException & ){
		pFoxIconMini = NULL;
		pFoxIconBig = NULL;
	}
}



bool deglGame::HasEngineInstance() const{
	return pEngineInstance != NULL;
}

void deglGame::SetEngineInstance( deglEngineInstance *engineInstance ){
	if( engineInstance == pEngineInstance ){
		return;
	}
	
	if( pEngineInstance ){
		delete pEngineInstance;
	}
	
	pEngineInstance = engineInstance;
}



void deglGame::VerifyRequirements(){
	const deglEngine &engine = *pLauncher.GetEngine();
	const deglEngineModuleList &moduleList = engine.GetModuleList();
	int f, formatCount = pFileFormatList.GetFormatCount();
	int m, moduleCount = moduleList.GetModuleCount();
	deglEngineModule *module;
	deModuleSystem::eModuleTypes formatType;
	
	// check if all file formats are supported
	pAllFormatsSupported = true;
	
	for( f=0; f<formatCount; f++ ){
		deglFileFormat &format = *pFileFormatList.GetFormatAt( f );
		formatType = format.GetType();
		
		format.SetSupported( false );
		
		if( ! deModuleSystem::IsSingleType( formatType ) ){
			const decString &formatPattern = format.GetPattern();
			
			for( m=0; m<moduleCount; m++ ){
				module = moduleList.GetModuleAt( m );
				
				if( module->GetType() == formatType && module->GetStatus() == deglEngineModule::emsReady
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
		&& module->GetStatus() == deglEngineModule::emsReady;
	
	if( module ){
		pScriptModuleFoundVersion = module->GetVersion();
	}
	
	// determine if the game is up to date
	pGameUpToDate = true;
	
	// determine if the game can run
	pCanRun = pAllFormatsSupported && pScriptModuleFound;
}

void deglGame::SetAllFormatsSupported( bool supported ){
	pAllFormatsSupported = supported;
}

void deglGame::SetScriptModuleFound( bool found ){
	pScriptModuleFound = found;
}

void deglGame::SetGameUpToDate( bool upToDate ){
	pGameUpToDate = upToDate;
}

void deglGame::SetCanRun( bool canRun ){
	pCanRun = canRun;
}



void deglGame::SetCustomProfile( deglGameProfile *profile ){
	pCustomProfile = profile;
}

void deglGame::SetActiveProfile( deglGameProfile *profile ){
	pActiveProfile = profile;
}

void deglGame::SetRunArguments( const char *arguments ){
	pRunArguments = arguments;
}



void deglGame::SetLogFile( const char *path ){
	pLogFile = path;
}

void deglGame::SetDefaultLogFile(){
	pLogFile.Format( "games/%s/logs/last_run.log", pIdentifier.ToHexString( false ).GetString() );
}

void deglGame::SetLogToConsole( bool logToConsole ){
	pLogToConsole = logToConsole;
}

void deglGame::SetDelgaFile( const char *delgaFile ){
	pDelgaFile = delgaFile;
}

void deglGame::SetUseLatestPatch( bool useLatestPatch ){
	pUseLatestPatch = useLatestPatch;
}

void deglGame::SetUseCustomPatch( const decUuid &patch ){
	pUseCustomPatch = patch;
}



bool deglGame::IsRunning() const{
	return pEngineInstance != NULL;
}

void deglGame::StartGame( const deglGameRunParams &runParams ){
	const deglConfiguration &configuration = *pLauncher.GetConfiguration();
	decPath filePath;
	
	if( IsRunning() || ! pCanRun || ! runParams.GetGameProfile()
	|| ! runParams.GetGameProfile()->GetValid() ){
		DETHROW( deeInvalidAction );
	}
	
	deLogger &logger = *pLauncher.GetLogger();
	decString logfile;
	
	// clear the log file. a bit an ugly hack but it works
	if( ! pLogToConsole ){
		filePath.SetFromUnix( "/config/user/games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "logs" );
		filePath.AddComponent( "last_run.log" );
		
		// using open file for writing instead of delete file to allow applications
		// to keep file open in a text viewer on operating systems preventing file
		// deleting while the file is open in an application (windows for example)
		{
		decBaseFileWriterReference writer;
		writer.TakeOver( pLauncher.GetFileSystem()->OpenFileForWriting( filePath ) );
		}
		
		logfile.Format( "games/%s/logs/last_run.log", pIdentifier.ToHexString( false ).GetString() );
	}
	
	// start the game
	logger.LogInfoFormat( LOGSOURCE, "Starting game '%s'", pTitle.ToUTF8().GetString() );
	
	try{
		// create engine instance and start engine
		pEngineInstance = new deglEngineInstance( pLauncher, logfile );
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
		if( pDelgaFile.IsEmpty() ){
			pEngineInstance->VFSAddDiskDir( "/", pathDataDir.GetPathNative(), true );
			
		}else{
			pEngineInstance->VFSAddDelgaFile( pDelgaFile, pDataDirectory );
		}
		
		int i;
		for( i=0; i<runParams.GetPatches().GetCount(); i++ ){
			const deglPatch &patch = *runParams.GetPatches().GetAt( i );
			
			if( patch.GetDelgaFile().IsEmpty() ){
				pathDataDir.SetFromNative( patch.GetPatchDirectory() );
				pathDataDir.AddUnixPath( patch.GetDataDirectory() );
				pEngineInstance->VFSAddDiskDir( "/", pathDataDir.GetPathNative(), true );
				
			}else{
				pEngineInstance->VFSAddDelgaFile( patch.GetDelgaFile(), patch.GetDataDirectory() );
			}
		}
		
		pEngineInstance->VFSAddScriptSharedDataDir();
		
		filePath.SetFromNative( configuration.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "overlay" );
		pEngineInstance->VFSAddDiskDir( "/", filePath.GetPathNative(), false );
		
		filePath.SetFromNative( configuration.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "config" );
		pEngineInstance->VFSAddDiskDir( pPathConfig, filePath.GetPathNative(), false );
		
		filePath.SetFromNative( configuration.GetPathConfigUser() );
		filePath.AddComponent( "games" );
		filePath.AddComponent( pIdentifier.ToHexString( false ) );
		filePath.AddComponent( "capture" );
		pEngineInstance->VFSAddDiskDir( pPathCapture, filePath.GetPathNative(), false );
		
		// create render window
		pEngineInstance->CreateRenderWindow( runParams.GetWidth(), runParams.GetHeight(),
			runParams.GetFullScreen(), pTitle.ToUTF8(), pIconPath );
		
		// store information for handling parameter cchanges during runtime
		pCollectChangedParams.RemoveAllModules();
		pCollectChangedParamsProfile.TakeOver( new deglGameProfile( *runParams.GetGameProfile() ) );
		
		// start game
		pEngineInstance->StartGame( pScriptDirectory, pGameObject, &pCollectChangedParams );
		
	}catch( const deException &e ){
		logger.LogErrorFormat( LOGSOURCE, "Game '%s' caused an exception", pTitle.ToUTF8().GetString() );
		logger.LogException( LOGSOURCE, e );
		StopGame();
		throw;
	}
}

void deglGame::StopGame(){
	if( ! IsRunning() || ! pEngineInstance ) DETHROW( deeInvalidAction );
	
	deLogger &logger = *pLauncher.GetLogger();
	
	logger.LogInfoFormat( LOGSOURCE, "Stopping game '%s'", pTitle.ToUTF8().GetString() );
	
	pEngineInstance->StopEngine();
	delete pEngineInstance;
	pEngineInstance = NULL;
	
	logger.LogInfoFormat( LOGSOURCE, "Game '%s' stopped", pTitle.ToUTF8().GetString() );
	pStoreCustomConfig();
}

void deglGame::KillGame(){
	if( ! IsRunning() || ! pEngineInstance ) DETHROW( deeInvalidAction );
	
	deLogger &logger = *pLauncher.GetLogger();
	
	logger.LogInfoFormat( LOGSOURCE, "Killing game '%s'", pTitle.ToUTF8().GetString() );
	
	pEngineInstance->KillProcess();
	delete pEngineInstance;
	pEngineInstance = NULL;
	
	logger.LogInfoFormat( LOGSOURCE, "Game '%s' killed", pTitle.ToUTF8().GetString() );
}

void deglGame::PulseChecking(){
	if( IsRunning() ){
		try{
			if( pEngineInstance->IsGameRunning() == 0 ){
				pLauncher.GetLogger()->LogInfoFormat( LOGSOURCE, "Game '%s' is no longer running", pTitle.ToUTF8().GetString() );
				StopGame();
			}
			
		}catch( const deException &e ){
			pLauncher.GetLogger()->LogInfoFormat( LOGSOURCE, "Game '%s' caused an exception", pTitle.ToUTF8().GetString() );
			pLauncher.GetLogger()->LogException( LOGSOURCE, e );
			KillGame(); // just to make sure the process is gone
		}
	}
}

void deglGame::LoadConfig(){
	deglGameConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileReader *reader = NULL;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
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

void deglGame::SaveConfig(){
	deglGameConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE, pLauncher.GetGameManager() );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileWriter *writer = NULL;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/games" );
	pathFile.AddComponent( pIdentifier.ToHexString( false ) );
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

deglGameProfile *deglGame::GetProfileToUse() const{
	if( pActiveProfile ){
		return ( deglGameProfile* )( deObject* )pActiveProfile;
	}
	if( pCustomProfile ){
		return ( deglGameProfile* )( deObject* )pCustomProfile;
	}
	
	deglGameManager &gameManager = *pLauncher.GetGameManager();
	if( gameManager.GetActiveProfile() ){
		return gameManager.GetActiveProfile();
	}
	
	return gameManager.GetDefaultProfile();
}

void deglGame::FindPatches( deglPatchList &list ) const{
	const deglPatchList &patches = pLauncher.GetPatchManager().GetPatches();
	const int count = patches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deglPatch * const patch = patches.GetAt( i );
		if( patch->GetGameID() == pIdentifier ){
			list.Add( patch );
		}
	}
}

void deglGame::SortPatches( deglPatchList &sorted, const deglPatchList &patches ) const{
	const int count = patches.GetCount();
	bool hasAdded = true;
	int i, j;
	
	sorted.RemoveAll();
	
	while( hasAdded ){
		hasAdded = false;
		
		for( i=0; i<count; i++ ){
			deglPatch * const patch = patches.GetAt( i );
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

void deglGame::pStoreCustomConfig(){
	if( pCollectChangedParams.GetModuleCount() == 0 || ! pCollectChangedParamsProfile ){
		return;
	}
	
	// ensure custom profile exists and is initialized with profile used to run the game
	// unless this had been already the custom profile
	if( ! pCustomProfile ){
		pCustomProfile.TakeOver( new deglGameProfile );
	}
	deglGameProfile &customProfile = ( deglGameProfile& )( deObject& )pCustomProfile;
	
	if( pCollectChangedParamsProfile != pCustomProfile ){
		customProfile.CopyFrom( ( deglGameProfile& )( deObject& )pCollectChangedParamsProfile );
	}
	
	// update custom profile
	deglGPModuleList &modules = customProfile.GetModuleList();
	const int moduleCount = pCollectChangedParams.GetModuleCount();
	deObjectReference module, parameter;
	int i, j;
	
	for( i=0; i<moduleCount; i++ ){
		const deglGPModule &moduleChanges = *pCollectChangedParams.GetModuleAt( i );
		
		module = modules.GetModuleNamed( moduleChanges.GetName() );
		if( module ){
			deglGPMParameterList &params = ( ( deglGPModule* )( deObject* )module )->GetParameterList();
			const deglGPMParameterList &paramsChanges = moduleChanges.GetParameterList();
			const int paramCount = paramsChanges.GetParameterCount();
			
			for( j=0; j<paramCount; j++ ){
				const deglGPMParameter &paramChanges = *paramsChanges.GetParameterAt( j );
				parameter = params.GetParameterNamed( paramChanges.GetName() );
				
				if( parameter ){
					( ( deglGPMParameter& )( deObject& )parameter ).SetValue( paramChanges.GetValue() );
					
				}else{
					parameter.TakeOver( new deglGPMParameter( paramChanges ) );
					params.AddParameter( ( deglGPMParameter* )( deObject* )parameter );
				}
			}
			
		}else{
			module.TakeOver( new deglGPModule( moduleChanges ) );
			modules.AddModule( ( deglGPModule* )( deObject* )module );
		}
	}
	
	// if game wants window mode adjust the profile to have the same values
	if( pWindowSize != decPoint() ){
		customProfile.SetFullScreen( false );
		customProfile.SetWidth( pWindowSize.x );
		customProfile.SetHeight( pWindowSize.y );
	}
	
	// save configuration
	SaveConfig();
	
	// verify profile and game so it is immediately usable
	customProfile.Verify( pLauncher );
}
