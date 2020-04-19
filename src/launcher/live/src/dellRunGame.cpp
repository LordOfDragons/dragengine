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

#include "dellRunGame.h"
#include "dellLauncher.h"
#include "engine/dellEngine.h"
#include "engine/modules/dellEngineModule.h"
#include "game/dellGame.h"
#include "game/dellGameManager.h"
#include "game/profile/dellGameProfile.h"
#include "game/profile/dellGPModule.h"
#include "game/profile/dellGPMParameter.h"
#include "game/fileformat/dellFileFormat.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/resources/archive/deArchiveManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deLibraryModule.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deLibraryModule.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/systems/modules/ai/deBaseAIModule.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorModule.h>
#include <dragengine/systems/modules/audio/deBaseAudioModule.h>
#include <dragengine/systems/modules/crashrecovery/deBaseCrashRecoveryModule.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/network/deBaseNetworkModule.h>
#include <dragengine/systems/modules/physics/deBasePhysicsModule.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>
#include <dragengine/systems/modules/synthesizer/deBaseSynthesizerModule.h>



// Definitions
////////////////

#define LOGSOURCE "LiveLauncher"



// Class dellRunGame
//////////////////////

// Constructor, destructor
////////////////////////////

dellRunGame::dellRunGame( dellLauncher &launcher ) :
pLauncher( launcher ),
pGame( NULL ),
pProfile( NULL ),
pModuleParameters( NULL ),
pRunWidth( 0 ),
pRunHeight( 0 ),
pRunFullScreen( false ){
}

dellRunGame::~dellRunGame(){
	if( pGame ){
		pGame->FreeReference();
	}
	if( pModuleParameters ){
		delete pModuleParameters;
	}
}



// Management
///////////////

bool dellRunGame::ParseArguments(){
	const decUnicodeArgumentList &arguments = pLauncher.GetArguments();
	const int argumentCount = arguments.GetArgumentCount();
	deLogger &logger = *pLauncher.GetLogger();
	decString utf8Argument, utf8Value;
	const decUnicodeString *argument;
	int o, optionLen, option;
	int argumentIndex = 1;
	
	// first check for options. they all start with a dash.
	while( argumentIndex < argumentCount ){
		argument = arguments.GetArgumentAt( argumentIndex );
		utf8Argument = argument->ToUTF8();
		
		if( utf8Argument == "--profile" ){
			argumentIndex++;
			
			if( argumentCount - argumentIndex > 0 ){
				pProfileName = arguments.GetArgumentAt( argumentIndex )->ToUTF8();
				
			}else{
				logger.LogError( LOGSOURCE, "Missing profile name after --profile" );
				return false;
			}
			
		}else if( utf8Argument == "--mparam" ){
			argumentIndex++;
			
			if( argumentCount - argumentIndex > 0 ){
				utf8Value = arguments.GetArgumentAt( argumentIndex )->ToUTF8();
				
				if( ! ParseModuleParameter( utf8Value ) ){
					return false;
				}
				
			}else{
				logger.LogError( LOGSOURCE, "Missing value after --mparam" );
				return false;
			}
			
		}else if( utf8Argument[ 0 ] == '-' ){
			optionLen = utf8Argument.GetLength();
			
			for( o=1; o<optionLen; o++ ){
				option = utf8Argument[ o ];
				
				if( option == 'p' ){
					if( o == optionLen - 1 ){
						argumentIndex++;
						
						if( argumentCount - argumentIndex > 0 ){
							pProfileName = arguments.GetArgumentAt( argumentIndex )->ToUTF8();
							
						}else{
							logger.LogError( LOGSOURCE, "Missing profile name after -p" );
							return false;
						}
						
					}else{
						logger.LogErrorFormat( LOGSOURCE, "Invalid option '%s' (p not last character)",
							utf8Argument.GetString() );
						return false;
					}
					
				}else{
					logger.LogErrorFormat( LOGSOURCE, "Unknown option -%c", ( char )option );
					return false;
				}
			}
			
		}else{
			break;
		}
		
		argumentIndex++;
	}
	
	// the rest are arguments for the game
	while( argumentIndex < argumentCount ){
		pGameArgs.AddArgument( *arguments.GetArgumentAt( argumentIndex ) );
		argumentIndex++;
	}
	
	return true;
}

bool dellRunGame::ParseModuleParameter( const decString &value ){
	deLogger &logger = *pLauncher.GetLogger();
	const int length = value.GetLength();
	decString moduleName, parameterName, parameterValue;
	dellGPMParameter *parameter;
	int separator1, separator2;
	dellGPModule *module;
	
	for( separator1=0; separator1<length; separator1++){
		if( value[ separator1 ] == ':' ){
			break;
			
		}else{
			moduleName.AppendCharacter( value[ separator1 ] );
		}
	}
	
	for( separator2=separator1+1; separator2<length; separator2++ ){
		if( value[ separator2 ] == '=' ){
			break;
			
		}else{
			parameterName.AppendCharacter( value[ separator2 ] );
		}
	}
	
	if( separator1 == length || separator2 == length || moduleName.IsEmpty() || parameterName.IsEmpty() ){
		logger.LogErrorFormat( LOGSOURCE, "Invalid module parameter value '%s'. "
			"Has to be of the format module:parameter=value", value.GetString() );
		return false;
	}
	
	parameterValue = value.GetMiddle( separator2 + 1 );
	
	if( ! pModuleParameters ){
		pModuleParameters = new dellGPModuleList;
	}
	
	module = pModuleParameters->GetNamed( moduleName );
	if( ! module ){
		module = new dellGPModule;
		module->SetName( moduleName );
		pModuleParameters->Add( module );
	}
	
	dellGPMParameterList &parameterList = module->GetParameterList();
	
	parameter = parameterList.GetNamed( parameterName );
	if( ! parameter ){
		parameter = new dellGPMParameter;
		parameter->SetName( parameterName );
		parameterList.Add( parameter );
	}
	
	parameter->SetValue( parameterValue );
	
	return true;
}

void dellRunGame::InitLauncher(){
	dellEngine &engine = pLauncher.GetEngine();
	engine.Start( pLauncher.GetLogger(), pGame->GetIdentifier().ToHexString( false ) );
	engine.PutEngineIntoVFS();
	engine.LoadModuleList();
	engine.LoadConfig();
	
	dellGameManager &gameManager = pLauncher.GetGameManager();
	gameManager.CreateDefaultProfile();
	gameManager.LoadGameConfigs();
	gameManager.Verify();
}

bool dellRunGame::LocateGame(){
	dellGameManager &gameManager = pLauncher.GetGameManager();
	deLogger &logger = *pLauncher.GetLogger();
	
	// locate the game to run
	if( gameManager.GetGameList().GetCount() == 0 ){
		logger.LogInfo( LOGSOURCE, "No valid game definition found." );
		return false;
	}
	
	pGame = gameManager.GetGameList().GetAt( 0 ); // TODO support multiple games using a choice for for example
	pGame->AddReference();
	return true;
}

bool dellRunGame::LocateProfile(){
	dellGameManager &gameManager = pLauncher.GetGameManager();
	deLogger &logger = *pLauncher.GetLogger();
	
	// locate the profile to run
	if( pProfileName.IsEmpty() ){
		pProfile = pGame->GetProfileToUse();
		
	}else{
		pProfile = gameManager.GetProfileList().GetNamed( pProfileName );
		
		if( ! pProfile ){
			logger.LogErrorFormat( LOGSOURCE, "No profile found with name '%s'",
				pProfileName.GetString() );
			return false;
		}
	}
	
	if( ! pProfile->GetValid() ){
		ShowProfileProblems();
		return false;
	}
	
	// udpate the run parameters
	if( pProfile->GetReplaceRunArguments() ){
		pRunArguments = pProfile->GetRunArguments();
		
	}else{
		pRunArguments.Format( "%s %s", pGame->GetRunArguments().GetString(),
			pProfile->GetRunArguments().GetString() );
	}
	
	pRunWidth = pProfile->GetWidth();
	pRunHeight = pProfile->GetHeight();
	pRunFullScreen = true; // full screen by default //pProfile->GetFullScreen();
	
	if( pGame->GetWindowSize() != decPoint() ){
		pRunWidth = pGame->GetWindowSize().x;
		pRunHeight = pGame->GetWindowSize().y;
		pRunFullScreen = false;
	}
	
	return true;
}

void dellRunGame::ShowGameProblems(){
	deModuleSystem &modsys = *pLauncher.GetEngine().GetEngine()->GetModuleSystem();
	const dellFileFormatList &fileFormatList = pGame->GetFileFormatList();
	const int fileFormatCount = fileFormatList.GetCount();
	deLogger &logger = *pLauncher.GetLogger();
	int i;
	
	logger.LogErrorFormat( LOGSOURCE, "Game '%s' has the following problems:",
		pGame->GetAliasIdentifier().GetString() );
	
	for( i=0; i<fileFormatCount; i++ ){
		const dellFileFormat &fileFormat = *fileFormatList.GetAt( i );
		
		if( ! fileFormat.GetSupported() ){
			if( deModuleSystem::IsSingleType( fileFormat.GetType() ) ){
				logger.LogErrorFormat( LOGSOURCE, "- File Format '%s' defines single type %s",
					fileFormat.GetPattern().GetString(),
					modsys.GetTypeDirectory( fileFormat.GetType() ) );
				
			}else{
				logger.LogErrorFormat( LOGSOURCE,
					"- File Format '%s' is not supported by any loaded modules",
					fileFormat.GetPattern().GetString() );
			}
		}
	}
	
	if( ! pGame->GetScriptModuleFound() ){
		ShowModuleProblem( pGame->GetScriptModule(), deModuleSystem::emtScript );
	}
}

void dellRunGame::ShowProfileProblems(){
	deLogger &logger = *pLauncher.GetLogger();
	
	logger.LogErrorFormat( LOGSOURCE, "Profile '%s' has the following problems:",
		pProfile->GetName().GetString() );
	
	ShowModuleProblem( pProfile->GetModuleGraphic(), deModuleSystem::emtGraphic );
	ShowModuleProblem( pProfile->GetModuleInput(), deModuleSystem::emtInput );
	ShowModuleProblem( pProfile->GetModulePhysics(), deModuleSystem::emtPhysics );
	ShowModuleProblem( pProfile->GetModuleAnimator(), deModuleSystem::emtAnimator );
	ShowModuleProblem( pProfile->GetModuleAI(), deModuleSystem::emtAI );
	ShowModuleProblem( pProfile->GetModuleCrashRecovery(), deModuleSystem::emtCrashRecovery );
	ShowModuleProblem( pProfile->GetModuleAudio(), deModuleSystem::emtAudio );
	ShowModuleProblem( pProfile->GetModuleSynthesizer(), deModuleSystem::emtSynthesizer );
	ShowModuleProblem( pProfile->GetModuleNetwork(), deModuleSystem::emtNetwork );
}

void dellRunGame::ShowModuleProblem( const char *moduleName, deModuleSystem::eModuleTypes moduleType ){
	dellEngineModule * const module = pLauncher.GetEngine().GetModuleList().GetNamed( moduleName );
	deModuleSystem &modsys = *pLauncher.GetEngine().GetEngine()->GetModuleSystem();
	deLogger &logger = *pLauncher.GetLogger();
	
	if( ! module ){
		logger.LogErrorFormat( LOGSOURCE, "- %s module '%s' does not exist",
			modsys.GetTypeDirectory( moduleType ), moduleName );
		
	}else if( module->GetType() != deModuleSystem::emtScript ){
		logger.LogErrorFormat( LOGSOURCE, "- Module '%s' is not a %s module",
			moduleName, modsys.GetTypeDirectory( moduleType ) );
		
	}else if( module->GetStatus() == dellEngineModule::emsReady ){
		const char *reason = "Unknown problem";
		
		if( module->GetErrorCode() == deLoadableModule::eecCreateModuleFailed ){
			reason = "Creating module failed";
			
		}else if( module->GetStatus() == deLibraryModule::eecLibFileNotFound ){
			reason = "Module library file could not be found";
			
		}else if( module->GetStatus() == deLibraryModule::eecLibFileNotRegularFile ){
			reason = "Module library file is not a regular library file";
			
		}else if( module->GetStatus() == deLibraryModule::eecLibFileSizeMismatch ){
			reason = "Size of the module library file does not match the size in the module definition";
			
		}else if( module->GetStatus() == deLibraryModule::eecLibFileCheckSumMismatch ){
			reason = "Checksum of the module library file does not match the checksum in the module definition";
			
		}else if( module->GetStatus() == deLibraryModule::eecLibFileOpenFailed ){
			reason = "Module library file could not be opened";
			
		}else if( module->GetStatus() == deLibraryModule::eecLibFileEntryPointNotFound ){
			reason = "Entry point function specified in the module definition could not be found in the library file";
			
		}else if( module->GetStatus() == deLibraryModule::eecLibFileCreateModuleFailed ){
			reason = "Creating the module using the entry point function failed";
		}
		
		logger.LogErrorFormat( LOGSOURCE, "- %s module '%s' is not working (%s)",
			modsys.GetTypeDirectory( moduleType ), moduleName, reason );
	}
}

void dellRunGame::CopyGameArguments(){
	deCmdLineArgs &arguments = *pLauncher.GetEngine().GetEngine()->GetArguments();
	const int count = pGameArgs.GetArgumentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		arguments.AddArgument( pGameArgs.GetArgumentAt( i )->ToUTF8() );
	}
}

void dellRunGame::ActivateScriptModule(){
	const dellEngineModuleList &moduleList = pLauncher.GetEngine().GetModuleList();
	dellEngineModule *module;
	
	module = moduleList.GetNamed( pGame->GetScriptModule() );
	if( ! module || ! module->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	pLauncher.GetEngine().GetEngine()->GetScriptingSystem()->SetActiveModule( module->GetLoadableModule() );
}

void dellRunGame::VFSAddDiskDir( const char *pathRoot, const char *pathDisk, bool readOnly ){
	deVirtualFileSystem &vfs = *pLauncher.GetEngine().GetEngine()->GetVirtualFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	deVFSDiskDirectory *container = NULL;
	decPath pathRoot2, pathDisk2;
	
	logger.LogInfoFormat( LOGSOURCE, "VFS: Adding disk container '%s' => '%s'%s",
		pathDisk, pathRoot, readOnly ? " (ro)" : "" );
	
	try{
		pathRoot2.SetFromUnix( pathRoot );
		pathDisk2.SetFromNative( pathDisk );
		
		container = new deVFSDiskDirectory( pathRoot2, pathDisk2 );
		container->SetReadOnly( readOnly );
		vfs.AddContainer( container );
		container->FreeReference();
		container = NULL;
		
	}catch( const deException &e ){
		if( container ){
			container->FreeReference();
		}
		
		logger.LogErrorFormat( LOGSOURCE, "Failed to add disk container to VFS (root=%s,disk=%s,ro=%c):",
			pathRoot, pathDisk, readOnly ? 't' : 'n' );
		logger.LogException( LOGSOURCE, e );
		throw;
	}
}

void dellRunGame::InitVFS(){
	decPath filePath;
	
	// add data directory as root directory
	if( pGame->GetDelgaFile().IsEmpty() ){
		filePath.SetFromNative( pGame->GetGameDirectory() );
		filePath.AddUnixPath( pGame->GetDataDirectory() );
		VFSAddDiskDir( "/", filePath.GetPathNative(), true );
		
	}else{
		deArchiveManager &amgr = *pLauncher.GetEngine().GetEngine()->GetArchiveManager();
		deVFSDiskDirectory *delgaVfsDelgaDir = NULL;
		deArchiveContainer *delgaContainer = NULL;
		deVirtualFileSystem *delgaVfs = NULL;
		deArchive *delgaArchive = NULL;
		
		try{
			decPath pathDelgaDir( decPath::CreatePathNative( pGame->GetDelgaFile() ) );
			const decString delgaFilename( pathDelgaDir.GetLastComponent() );
			pathDelgaDir.RemoveLastComponent();
			
			delgaVfs = new deVirtualFileSystem;
			
			delgaVfsDelgaDir = new deVFSDiskDirectory( pathDelgaDir );
			delgaVfs->AddContainer( delgaVfsDelgaDir );
			delgaVfsDelgaDir->FreeReference();
			delgaVfsDelgaDir = NULL;
			
			delgaArchive = amgr.OpenArchive( delgaVfs, delgaFilename, "/" );
			delgaVfs->FreeReference();
			delgaVfs = NULL;
			
			delgaContainer = amgr.CreateContainer( decPath::CreatePathUnix( "/" ),
				delgaArchive, decPath::CreatePathUnix( pGame->GetDataDirectory() ) );
			delgaArchive->FreeReference();
			delgaArchive = NULL;
			
			pLauncher.GetEngine().GetEngine()->GetVirtualFileSystem()->AddContainer( delgaContainer );
			delgaContainer->FreeReference();
			
		}catch( const deException & ){
			if( delgaContainer ){
				delgaContainer->FreeReference();
			}
			if( delgaArchive ){
				delgaArchive->FreeReference();
			}
			if( delgaVfsDelgaDir ){
				delgaVfsDelgaDir->FreeReference();
			}
			if( delgaVfs ){
				delgaVfs->FreeReference();
			}
			throw;
		}
	}
	
	// add script module shared data if existing
	deEngine &engine = *pLauncher.GetEngine().GetEngine();
	engine.GetScriptingSystem()->AddVFSSharedDataDir( *engine.GetVirtualFileSystem() );
	
	// add the user game overlay directory(writeable)
	filePath = pLauncher.GetWorkingDirectory();
	filePath.AddComponent( "overlay" );
	filePath.AddComponent( pGame->GetIdentifier().ToHexString( false ) );
	VFSAddDiskDir( "/", filePath.GetPathNative(), false );
	
	// add the user game configuration directory (writeable)
	filePath = pLauncher.GetWorkingDirectory();
	filePath.AddComponent( "config" );
	filePath.AddComponent( pGame->GetIdentifier().ToHexString( false ) );
	VFSAddDiskDir( pGame->GetPathConfig(), filePath.GetPathNative(), false );
	
	// add the user game capture directory (writeable)
	filePath = pLauncher.GetWorkingDirectory();
	filePath.AddComponent( "capture" );
	filePath.AddComponent( pGame->GetIdentifier().ToHexString( false ) );
	VFSAddDiskDir( pGame->GetPathCapture(), filePath.GetPathNative(), false );
}



void dellRunGame::Run(){
	deLogger &logger = *pLauncher.GetLogger();
	
	// parse the arguments
	if( ! ParseArguments() ){
		return;
	}
	
	// locate the game to run
	pLauncher.GetGameManager().LoadGameList();
	if( ! LocateGame() ){
		return;
	}
	
	// init launcher. has to be done after game is found to use the right cache app id
	InitLauncher();
	
	// check if the game can run
	if( ! pGame->GetCanRun() ){
		ShowGameProblems();
		return;
	}
	
	// locate the profile and set the run parameters
	if( ! LocateProfile() ){
		return;
	}
	
	// set the window title
	pWindowTitle = pGame->GetTitle().ToUTF8();
	
	// copy the game arguments to the game engine argument list
	CopyGameArguments();
	
	// start the game
	logger.LogInfoFormat( LOGSOURCE, "Starting game '%s' using profile '%s'",
		pGame->GetTitle().ToUTF8().GetString(), pProfile->GetName().GetString() );
	
	// start the game
	deEngine &engine = *pLauncher.GetEngine().GetEngine();
	
	struct sModuleParamState{
		const deBaseModule *module;
		decStringDictionary parameters;
	};
	sModuleParamState moduleState[ 10 ];
	deModuleParameter moduleParameter;
	int i, j;
	
	try{
		pProfile->Activate( pLauncher );
		if( pModuleParameters ){
			logger.LogInfo( LOGSOURCE, "Apply command line module parameters" );
			pModuleParameters->Apply( pLauncher );
		}
		
		ActivateScriptModule();
		
		decPath pathDataDir;
		pathDataDir.SetFromNative( pGame->GetGameDirectory() );
		pathDataDir.AddUnixPath( pGame->GetDataDirectory() );
		engine.SetDataDir( pathDataDir.GetPathNative() );
		
		if( pGame->GetDelgaFile().IsEmpty() ){
			logger.LogInfoFormat( LOGSOURCE, "Using data directory '%s'",
				pathDataDir.GetPathNative().GetString() );
			
		}else{
			logger.LogInfoFormat( LOGSOURCE, "Using DELGA archive '%s'",
				pGame->GetDelgaFile().GetString() );
		}
		
		logger.LogInfoFormat( LOGSOURCE, "Passing game arguments '%s'",
			pRunArguments.GetString() );
		engine.GetArguments()->AddArgsSplit( pRunArguments );
		InitVFS();
		
		deImageReference icon;
		if( ! pGame->GetIconPath().IsEmpty() ){
			logger.LogInfoFormat( LOGSOURCE, "Loading icon %s...", pGame->GetIconPath().GetString() );
			try{
				icon.TakeOver( engine.GetImageManager()->LoadImage( pGame->GetIconPath(), "/" ) );
				
			}catch( const deException &e ){
				icon = NULL;
				logger.LogException( LOGSOURCE, e );
			}
		}
		
		logger.LogInfoFormat( LOGSOURCE, "Creating window %i x %i", pRunWidth, pRunHeight );
		engine.GetGraphicSystem()->CreateAndSetRenderWindow( pRunWidth, pRunHeight,
			pRunFullScreen, pWindowTitle.GetString(), NULL );
		
		// store single type module parameters to compare after engine exits if user changed them
		moduleState[ 0 ].module = engine.GetAISystem()->GetActiveModule();
		moduleState[ 1 ].module = engine.GetAnimatorSystem()->GetActiveModule();
		moduleState[ 2 ].module = engine.GetAudioSystem()->GetActiveModule();
		moduleState[ 3 ].module = engine.GetCrashRecoverySystem()->GetActiveModule();
		moduleState[ 4 ].module = engine.GetGraphicSystem()->GetActiveModule();
		moduleState[ 5 ].module = engine.GetInputSystem()->GetActiveModule();
		moduleState[ 6 ].module = engine.GetNetworkSystem()->GetActiveModule();
		moduleState[ 7 ].module = engine.GetPhysicsSystem()->GetActiveModule();
		moduleState[ 8 ].module = engine.GetScriptingSystem()->GetActiveModule();
		moduleState[ 9 ].module = engine.GetSynthesizerSystem()->GetActiveModule();
		
		for( i=0; i<10; i++ ){
			if( moduleState[ i ].module ){
				const int count = moduleState[ i ].module->GetParameterCount();
				for( j=0; j<count; j++ ){
					moduleState[ i ].module->GetParameterInfo( j, moduleParameter );
					moduleState[ i ].parameters.SetAt( moduleParameter.GetName(),
						moduleState[ i ].module->GetParameterValue( moduleParameter.GetName() ) );
				}
			}
		}
		
		// run game
		logger.LogInfo( LOGSOURCE, "Launching Game ( now I'm no more responsible ;=) )" );
		engine.Run( pGame->GetScriptDirectory(), pGame->GetGameObject() );
		
		logger.LogInfo( LOGSOURCE, "Game finished. Cleaning up" );
		
	}catch( const deException &e ){
		logger.LogException( LOGSOURCE, e );
		throw;
	}
	
	// compare module parameters against stored ones and process result
	deObjectReference module, parameter;
	dellGPModuleList changedModules;
	
	for( i=0; i<10; i++ ){
		if( ! moduleState[ i ].module ){
			continue;
		}
		
		const decStringList keys( moduleState[ i ].parameters.GetKeys() );
		const int count = keys.GetCount();
		for( j=0; j<count; j++ ){
			const decString &name = keys.GetAt( j );
			const decString value( moduleState[ i ].module->GetParameterValue( name ) );
			if( value == moduleState[ i ].parameters.GetAt( name ) ){
				continue;
			}
			
			const decString &moduleName = moduleState[ i ].module->GetLoadableModule().GetName();
			module = changedModules.GetNamed( moduleName );
			if( ! module ){
				module.TakeOver( new dellGPModule( moduleName ) );
				changedModules.Add( ( dellGPModule* )( deObject* )module );
			}
			
			parameter.TakeOver( new dellGPMParameter( name, value ) );
			( ( dellGPModule& )( deObject& )module ).GetParameterList().
				Add( ( dellGPMParameter* )( deObject* )parameter );
		}
	}
	
	StoreCustomConfig( changedModules );
}

void dellRunGame::StoreCustomConfig( const dellGPModuleList &changedModules ){
	if( changedModules.GetCount() == 0 ){
		return;
	}
	
	// ensure custom profile exists and is initialized with profile used to run the game
	// unless this had been already the custom profile
	if( ! pGame->GetCustomProfile() ){
		deObjectReference profile;
		profile.TakeOver( new dellGameProfile );
		pGame->SetCustomProfile( ( dellGameProfile* )( deObject* )profile );
	}
	dellGameProfile &customProfile = *pGame->GetCustomProfile();
	
	if( pProfile != &customProfile ){
		customProfile.CopyFrom( *pProfile );
	}
	
	// update custom profile
	dellGPModuleList &modules = customProfile.GetModuleList();
	const int moduleCount = changedModules.GetCount();
	deObjectReference module, parameter;
	int i, j;
	
	for( i=0; i<moduleCount; i++ ){
		const dellGPModule &moduleChanges = *changedModules.GetAt( i );
		
		module = modules.GetNamed( moduleChanges.GetName() );
		if( module ){
			dellGPMParameterList &params = ( ( dellGPModule* )( deObject* )module )->GetParameterList();
			const dellGPMParameterList &paramsChanges = moduleChanges.GetParameterList();
			const int paramCount = paramsChanges.GetCount();
			
			for( j=0; j<paramCount; j++ ){
				const dellGPMParameter &paramChanges = *paramsChanges.GetAt( j );
				parameter = params.GetNamed( paramChanges.GetName() );
				
				if( parameter ){
					( ( dellGPMParameter& )( deObject& )parameter ).SetValue( paramChanges.GetValue() );
					
				}else{
					parameter.TakeOver( new dellGPMParameter( paramChanges ) );
					params.Add( ( dellGPMParameter* )( deObject* )parameter );
				}
			}
			
		}else{
			module.TakeOver( new dellGPModule( moduleChanges ) );
			modules.Add( ( dellGPModule* )( deObject* )module );
		}
	}
	
	// if game wants window mode adjust the profile to have the same values
	if( pGame->GetWindowSize() != decPoint() ){
		customProfile.SetFullScreen( false );
		customProfile.SetWidth( pGame->GetWindowSize().x );
		customProfile.SetHeight( pGame->GetWindowSize().y );
	}
	
	// save configuration
	pGame->SaveConfig();
}
