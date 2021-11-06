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

#include "declRunGame.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"
#include "../engine/declEngine.h"
#include "../engine/modules/declEngineModule.h"
#include "../game/declGame.h"
#include "../game/declGameManager.h"
#include "../game/profile/declGameProfile.h"
#include "../game/profile/declGPModule.h"
#include "../game/profile/declGPMParameter.h"
#include "../game/fileformat/declFileFormat.h"
#include "../game/patch/declPatch.h"
#include "../game/patch/declPatchManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveReference.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/resources/archive/deArchiveManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>
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
#include <dragengine/systems/deVRSystem.h>
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
#include <dragengine/systems/modules/vr/deBaseVRModule.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declRunGame
//////////////////////

// Constructor, destructor
////////////////////////////

declRunGame::declRunGame( declLauncher *launcher ) :
pLauncher( launcher ),
pUseConsole( false ),
pLogAllToConsole( false ),
pGame( NULL ),
pProfile( NULL ),
pModuleParameters( NULL ),
pHasPatchIdentifier( false ),
pRunWidth( 0 ),
pRunHeight( 0 ),
pRunFullScreen( false )
{
	if( ! launcher ){
		DETHROW( deeInvalidParam );
	}
}

declRunGame::~declRunGame(){
	if( pGame ){
		pGame->FreeReference();
	}
	if( pModuleParameters ){
		delete pModuleParameters;
	}
}



// Management
///////////////

void declRunGame::PrintSyntax(){
	printf( "Drag[en]gine Console Launcher.\n" );
	printf( "Written by Plüss Roland ( roland@rptd.ch ).\n" );
	printf( "Released under the GPL ( http://www.gnu.org/licenses/gpl.html ), 2011.\n" );
	printf( "\n" );
	printf( "Runs a game.\n" );
	printf( "\n" );
	printf( "Syntax:\n" );
	printf( "delauncherconsole run [ <options> ] { <game> | -f <game.delga> | --file <game.delga>> } [ <game-options> ]\n" );
	printf( "   <options> can be one or more of the following:\n" );
	printf( "      -c, --console           Use console and no graphic system.\n" );
	printf( "      -p, --profile <name>    Use named game profile instead of the default one.\n" );
	printf( "      -d, --debug             Display all debug information in the console not just the log file.\n" );
	printf( "      -P, --patch <id|alias>  Use patch with identifier instead of latest. Use empty string to run unpatched.\n" );
	printf( "      --mparam module:param=value     Set module parameter before running the game.\n" );
	printf( "   <game>                Identifier of the game to run.\n" );
	printf( "   -f <game.delga>       DELGA file to run.\n" );
	printf( "   --file <game.delga>   DELGA file to run.\n" );
	printf( "   \n" );
	printf( "   <game-options> are optional parameters specific to the game to load.\n" );
	printf( "   see the game documentation for a list of all available parameters.\n" );
	printf( "\n" );
}

bool declRunGame::ParseArguments(){
	const decUnicodeArgumentList &argumentList = pLauncher->GetArgumentList();
	const int argumentCount = argumentList.GetArgumentCount();
	deLogger &logger = *pLauncher->GetLogger();
	decString utf8Argument, utf8Value;
	const decUnicodeString *argument;
	int o, optionLen, option;
	int argumentIndex = 1;
	
	// first check for options. they all start with a dash.
	while( argumentIndex < argumentCount ){
		argument = argumentList.GetArgumentAt( argumentIndex );
		utf8Argument = argument->ToUTF8();
		
		if( utf8Argument == "--console" ){
			pUseConsole = true;
			
		}else if( utf8Argument == "--profile" ){
			argumentIndex++;
			
			if( argumentCount - argumentIndex > 0 ){
				pProfileName = argumentList.GetArgumentAt( argumentIndex )->ToUTF8();
				
			}else{
				logger.LogError( LOGSOURCE, "Missing profile name after --profile" );
				return false;
			}
			
		}else if( utf8Argument == "--mparam" ){
			argumentIndex++;
			
			if( argumentCount - argumentIndex > 0 ){
				utf8Value = argumentList.GetArgumentAt( argumentIndex )->ToUTF8();
				
				if( ! ParseModuleParameter( utf8Value ) ){
					return false;
				}
				
			}else{
				logger.LogError( LOGSOURCE, "Missing value after --mparam" );
				return false;
			}
			
		}else if( utf8Argument == "--debug" ){
			pLogAllToConsole = true;
			
		}else if( utf8Argument == "--file" ){
			argumentIndex++;
			
			if( argumentCount - argumentIndex > 0 ){
				pGameDefFile = argumentList.GetArgumentAt( argumentIndex )->ToUTF8();
				break;
				
			}else{
				logger.LogError( LOGSOURCE, "Missing game definition filename after --file" );
				return false;
			}
			
		}else if( utf8Argument == "--patch" ){
			argumentIndex++;
			
			if( argumentCount - argumentIndex > 0 ){
				pPatchIdentifier.SetFromHexString( argumentList.GetArgumentAt( argumentIndex )->ToUTF8(), false );
				pHasPatchIdentifier = true;
				
			}else{
				logger.LogError( LOGSOURCE, "Missing patch identifier after --patch" );
				return false;
			}
			
		}else if( utf8Argument[ 0 ] == '-' ){
			optionLen = utf8Argument.GetLength();
			
			for( o=1; o<optionLen; o++ ){
				option = utf8Argument[ o ];
				
				if( option == 'c' ){
					pUseConsole = true;
					
				}else if( option == 'p' ){
					if( o == optionLen - 1 ){
						argumentIndex++;
						
						if( argumentCount - argumentIndex > 0 ){
							pProfileName = argumentList.GetArgumentAt( argumentIndex )->ToUTF8();
							
						}else{
							logger.LogError( LOGSOURCE, "Missing profile name after -p" );
							return false;
						}
						
					}else{
						logger.LogErrorFormat( LOGSOURCE, "Invalid option '%s' (p not last character)",
							utf8Argument.GetString() );
						return false;
					}
					
				}else if( option == 'd' ){
					pLogAllToConsole = true;
					
				}else if( option == 'f' ){
					if( o == optionLen - 1 ){
						argumentIndex++;
						
						if( argumentCount - argumentIndex > 0 ){
							pGameDefFile = argumentList.GetArgumentAt( argumentIndex )->ToUTF8();
							break;
							
						}else{
							logger.LogError( LOGSOURCE, "Missing game definition filename after -f" );
							return false;
						}
						
					}else{
						logger.LogErrorFormat( LOGSOURCE, "Invalid option '%s' (f not last character)",
							utf8Argument.GetString() );
						return false;
					}
					
				}else if( option == 'P' ){
					if( o == optionLen - 1 ){
						argumentIndex++;
						
						if( argumentCount - argumentIndex > 0 ){
							pPatchIdentifier.SetFromHexString( argumentList.GetArgumentAt( argumentIndex )->ToUTF8(), false );
							pHasPatchIdentifier = true;
							
						}else{
							logger.LogError( LOGSOURCE, "Missing patch identifier after -P" );
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
			
			if( o < optionLen ){
				break;
			}
			
		}else{
			break;
		}
		
		argumentIndex++;
	}
	
	// the next argument has to be the game identifier if a game definition file has not been specified
	if( pGameDefFile.IsEmpty() ){
		if( argumentIndex == argumentCount ){
			logger.LogError( LOGSOURCE, "No game specified" );
			return false;
		}
		
		pGameIdentifier = argumentList.GetArgumentAt( argumentIndex++ )->ToUTF8();
	}
	
	// the rest are arguments for the game
	while( argumentIndex < argumentCount ){
		pGameArgs.AddArgument( *argumentList.GetArgumentAt( argumentIndex ) );
		argumentIndex++;
	}
	
	return true;
}

bool declRunGame::ParseModuleParameter( const decString &value ){
	deLogger &logger = *pLauncher->GetLogger();
	const int length = value.GetLength();
	decString moduleName, parameterName, parameterValue;
	declGPMParameter *parameter;
	int separator1, separator2;
	declGPModule *module;
	
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
		pModuleParameters = new declGPModuleList;
	}
	
	module = pModuleParameters->GetNamed( moduleName );
	if( ! module ){
		module = new declGPModule;
		module->SetName( moduleName );
		pModuleParameters->Add( module );
	}
	
	declGPMParameterList &parameterList = module->GetParameterList();
	
	parameter = parameterList.GetNamed( parameterName );
	if( ! parameter ){
		parameter = new declGPMParameter;
		parameter->SetName( parameterName );
		parameterList.Add( parameter );
	}
	
	parameter->SetValue( parameterValue );
	
	return true;
}

void declRunGame::InitLauncher(){
	declGameManager &gameManager = *pLauncher->GetGameManager();
	declEngine &engine = *pLauncher->GetEngine();
	deLogger &logger = *pLauncher->GetLogger();
	
	deLogger *engineLogger = pLauncher->GetEngineLogger();
	if( pLogAllToConsole ){
		engineLogger = pLauncher->GetEngineLoggerDebug();
	}
	
	engine.Start( engineLogger, pGame->GetIdentifier().ToHexString( false ) );
	engine.PutEngineIntoVFS();
	
	logger.LogInfoFormat( LOGSOURCE, "Cache application ID = '%s'", pGame->GetIdentifier().ToHexString( false ).GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Engine config path = '%s'", engine.GetPathConfig().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Engine share path = '%s'", engine.GetPathShare().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Engine lib path = '%s'", engine.GetPathLib().GetString() );
	
	engine.LoadModuleList();
	engine.LoadConfig();
	
	gameManager.CreateDefaultProfile();
	gameManager.LoadGameConfigs();
	gameManager.Verify();
	
	// if game file has been specified manual verification is required
	if( ! gameManager.GetGameList().Has( pGame ) ){
		pGame->LoadConfig();
		pGame->VerifyRequirements();
	}
}

bool declRunGame::LocateGame(){
	declGameManager &gameManager = *pLauncher->GetGameManager();
	deLogger &logger = *pLauncher->GetLogger();
	
	// locate the game to run
	if( pGameDefFile.IsEmpty() ){
		try{
			pGame = gameManager.GetGameList().GetWithID( decUuid( pGameIdentifier, false ) );
		}catch( const deException &){
			// ignore. could be identifier
		}
		
		if( ! pGame ){
			const declGameList matching( gameManager.GetGameList().GetWithAlias( pGameIdentifier ) );
			if( matching.GetCount() == 1 ){
				pGame = matching.GetAt( 0 );
				pGame->AddReference();
				
			}else if( matching.GetCount() > 1 ){
				logger.LogErrorFormat( LOGSOURCE, "Multiple games found with same alias identifier '%s'",
					pGameIdentifier.GetString() );
				return false;
			}
		}
		
		if( pGame ){
			pGame->AddReference();
		}
		
	}else{
		// the game definition file is not required to be installed if defined by file. It is
		// always used even if a game with the same identifier is already installed. running a
		// game by explicit game file overrides the installed one. otherwise it is difficult
		// for the user to understand why something else happens than he indented
		declGameList list;
		
		pLauncher->GetEngine()->Start( pLauncher->GetEngineLogger(), "" );
		try{
			gameManager.LoadGameFromDisk( pGameDefFile, list );
			
		}catch( const deException &e ){
			logger.LogException( LOGSOURCE, e );
			pLauncher->GetEngine()->Stop();
			return false;
		}
		pLauncher->GetEngine()->Stop();
		
		if( list.GetCount() == 0 ){
			logger.LogInfo( LOGSOURCE, "No valid game definition found." );
			return false;
		}
		
		pGame = list.GetAt( 0 ); // TODO support multiple games using a choice for for example
		pGame->AddReference();
	}
	
	if( ! pGame ){
		logger.LogErrorFormat( LOGSOURCE, "No game found with identifier '%s'",
			pGameIdentifier.GetString() );
		return false;
	}
	
	return true;
}

bool declRunGame::LocateProfile(){
	declGameManager &gameManager = *pLauncher->GetGameManager();
	deLogger &logger = *pLauncher->GetLogger();
	
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
		PrintProfileProblems();
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
	pRunFullScreen = pProfile->GetFullScreen();
	
	if( pGame->GetWindowSize() != decPoint() ){
		pRunWidth = pGame->GetWindowSize().x;
		pRunHeight = pGame->GetWindowSize().y;
		pRunFullScreen = false;
	}
	
	return true;
}

bool declRunGame::LocatePatches(){
	const declPatchManager &patchManager = pLauncher->GetPatchManager();
	deLogger &logger = *pLauncher->GetLogger();
	
	// find all patches for this game
	declPatchList patches;
	int i;
	for( i=0; i<patchManager.GetPatches().GetCount(); i++ ){
		declPatch * const patch = patchManager.GetPatches().GetAt( i );
		if( patch->GetGameID() == pGame->GetIdentifier() ){
			patches.Add( patch );
		}
	}
	
	// if there is no patch requested by the user use game set one if present
	if( ! pHasPatchIdentifier ){
		if( pGame->GetUseCustomPatch() ){
			pHasPatchIdentifier = true;
			pPatchIdentifier = pGame->GetUseCustomPatch();
			
		}else if( ! pGame->GetUseLatestPatch() ){
			pHasPatchIdentifier = true;
			pPatchIdentifier.Clear();
		}
	}
	
	// if there is a patch requested by the user use it and fill in all dependencies
	if( pHasPatchIdentifier ){
		const declPatchList allPatches( patches );
		
		patches.RemoveAll();
		
		if( pPatchIdentifier ){
			declPatch *usePatch = allPatches.GetWithID( pPatchIdentifier );
			if( ! usePatch ){
				logger.LogErrorFormat( LOGSOURCE, "No patch found with identifier '%s'",
					pPatchIdentifier.ToHexString( false ).GetString() );
				return false;
			}
			
			patches.Add( usePatch );
			
			while( usePatch && usePatch->GetRequiredPatches().GetCount() > 0 ){
				const declPatch &verifyPatch = *usePatch;
				
				for( i=0; i<verifyPatch.GetRequiredPatches().GetCount(); i++ ){
					declPatch * const findPatch = allPatches.GetWithID( verifyPatch.GetRequiredPatches().GetAt( i ) );
					if( findPatch ){
						patches.Add( findPatch );
						usePatch = findPatch;
						break;
					}
				}
				if( i == verifyPatch.GetRequiredPatches().GetCount() ){
					decStringList names;
					decString name;
					for( i=0; i<verifyPatch.GetRequiredPatches().GetCount(); i++ ){
						name.Format( "'%s'", verifyPatch.GetRequiredPatches().GetAt( i ).ToHexString( false ).GetString() );
						names.Add( name );
					}
					name = names.Join( ", " );
					
					logger.LogErrorFormat( LOGSOURCE, "Required patches not found for patch with identifier '%s'."
						" Requires one of: %s", verifyPatch.GetIdentifier().ToHexString( false ).GetString(), name.GetString() );
					return false;
				}
			}
		}
	}
	
	// find all patches to apply in the right order
	pPatches.RemoveAll();
	
	bool hasAdded = true;
	int j;
	
	while( hasAdded ){
		hasAdded = false;
		
		for( i=0; i<patches.GetCount(); i++ ){
			declPatch * const patch = patches.GetAt( i );
			if( pPatches.Has( patch ) ){
				continue;
			}
			
			if( patch->GetRequiredPatches().GetCount() > 0 ){
				for( j=0; j<patch->GetRequiredPatches().GetCount(); j++ ){
					if( pPatches.HasWithID( patch->GetRequiredPatches().GetAt( j ) ) ){
						break;
					}
				}
				if( j == patch->GetRequiredPatches().GetCount() ){
					continue;
				}
			}
			
			pPatches.Add( patch );
			hasAdded = true;
		}
	}
	
	// print out the result
	for( i=0; i<pPatches.GetCount(); i++ ){
		logger.LogInfoFormat( LOGSOURCE, "Found patch: %s",
			pPatches.GetAt( i )->GetIdentifier().ToHexString( false ).GetString() );
	}
	
	return true;
}

void declRunGame::PrintGameProblems(){
	deModuleSystem &modsys = *pLauncher->GetEngine()->GetEngine()->GetModuleSystem();
	const declFileFormatList &fileFormatList = pGame->GetFileFormatList();
	const int fileFormatCount = fileFormatList.GetCount();
	deLogger &logger = *pLauncher->GetLogger();
	int i;
	
	logger.LogErrorFormat( LOGSOURCE, "Game '%s'(%s) has the following problems:",
		pGame->GetAliasIdentifier().GetString(), pGame->GetIdentifier().ToHexString( false ).GetString() );
	
	for( i=0; i<fileFormatCount; i++ ){
		const declFileFormat &fileFormat = *fileFormatList.GetAt( i );
		
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
		PrintModuleProblem( pGame->GetScriptModule(), deModuleSystem::emtScript );
	}
}

void declRunGame::PrintProfileProblems(){
	deLogger &logger = *pLauncher->GetLogger();
	
	logger.LogErrorFormat( LOGSOURCE, "Profile '%s' has the following problems:",
		pProfile->GetName().GetString() );
	
	PrintModuleProblem( pProfile->GetModuleGraphic(), deModuleSystem::emtGraphic );
	PrintModuleProblem( pProfile->GetModuleInput(), deModuleSystem::emtInput );
	PrintModuleProblem( pProfile->GetModulePhysics(), deModuleSystem::emtPhysics );
	PrintModuleProblem( pProfile->GetModuleAnimator(), deModuleSystem::emtAnimator );
	PrintModuleProblem( pProfile->GetModuleAI(), deModuleSystem::emtAI );
	PrintModuleProblem( pProfile->GetModuleCrashRecovery(), deModuleSystem::emtCrashRecovery );
	PrintModuleProblem( pProfile->GetModuleAudio(), deModuleSystem::emtAudio );
	PrintModuleProblem( pProfile->GetModuleSynthesizer(), deModuleSystem::emtSynthesizer );
	PrintModuleProblem( pProfile->GetModuleNetwork(), deModuleSystem::emtNetwork );
	PrintModuleProblem( pProfile->GetModuleVR(), deModuleSystem::emtVR );
}

void declRunGame::PrintModuleProblem( const char *moduleName, deModuleSystem::eModuleTypes moduleType ){
	declEngineModule * const module = pLauncher->GetEngine()->GetModuleList().GetNamed( moduleName );
	deModuleSystem &modsys = *pLauncher->GetEngine()->GetEngine()->GetModuleSystem();
	deLogger &logger = *pLauncher->GetLogger();
	
	if( ! module ){
		logger.LogErrorFormat( LOGSOURCE, "- %s module '%s' does not exist", modsys.GetTypeDirectory( moduleType ), moduleName );
		
	}else if( module->GetType() != moduleType ){
		logger.LogErrorFormat( LOGSOURCE, "- Module '%s' is not a %s module", moduleName, modsys.GetTypeDirectory( moduleType ) );
		
	}else if( module->GetStatus() != declEngineModule::emsReady ){
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
		
		logger.LogErrorFormat( LOGSOURCE, "- %s module '%s' is not working (%s)", modsys.GetTypeDirectory( moduleType ), moduleName, reason );
	}
}

void declRunGame::CopyGameArguments(){
	deCmdLineArgs &argumentList = *pLauncher->GetEngine()->GetEngine()->GetArguments();
	const int count = pGameArgs.GetArgumentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		argumentList.AddArgument( pGameArgs.GetArgumentAt( i )->ToUTF8() );
	}
}

void declRunGame::ActivateScriptModule(){
	const declEngineModuleList &moduleList = pLauncher->GetEngine()->GetModuleList();
	declEngineModule *module;
	
	module = moduleList.GetNamed( pGame->GetScriptModule(), pGame->GetScriptModuleFoundVersion() );
	if( ! module || ! module->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	pLauncher->GetEngine()->GetEngine()->GetScriptingSystem()->SetActiveModule( module->GetLoadableModule() );
}

void declRunGame::VFSAddDiskDir( const char *pathRoot, const char *pathDisk, bool readOnly ){
	deVirtualFileSystem &vfs = *pLauncher->GetEngine()->GetEngine()->GetVirtualFileSystem();
	deLogger &logger = *pLauncher->GetLogger();
	deVFSDiskDirectory *container = NULL;
	decPath pathRoot2, pathDisk2;
	
	logger.LogInfoFormat( LOGSOURCE, "VFS: Adding disk container '%s' => '%s'%s", pathDisk, pathRoot, readOnly ? " (ro)" : "" );
	
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

void declRunGame::InitVFS(){
	deArchiveManager &amgr = *pLauncher->GetEngine()->GetEngine()->GetArchiveManager();
	const declConfiguration &configuration = *pLauncher->GetConfiguration();
	deVFSContainerReference delgaVfsDelgaDir;
	deVFSContainerReference delgaContainer;
	deVirtualFileSystemReference delgaVfs;
	deArchiveReference delgaArchive;
	decPath filePath;
	
	// add data directory as root directory
	if( pGame->GetDelgaFile().IsEmpty() ){
		filePath.SetFromNative( pGame->GetGameDirectory() );
		filePath.AddUnixPath( pGame->GetDataDirectory() );
		VFSAddDiskDir( "/", filePath.GetPathNative(), true );
		
	}else{
		decPath pathDelgaDir( decPath::CreatePathNative( pGame->GetDelgaFile() ) );
		const decString delgaFilename( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		
		delgaVfs.TakeOver( new deVirtualFileSystem );
		
		delgaVfsDelgaDir.TakeOver( new deVFSDiskDirectory( pathDelgaDir ) );
		delgaVfs->AddContainer( delgaVfsDelgaDir );
		
		delgaArchive.TakeOver( amgr.OpenArchive( delgaVfs, delgaFilename, "/" ) );
		
		delgaContainer.TakeOver( amgr.CreateContainer( decPath::CreatePathUnix( "/" ),
			delgaArchive, decPath::CreatePathUnix( pGame->GetDataDirectory() ) ) );
		pLauncher->GetEngine()->GetEngine()->GetVirtualFileSystem()->AddContainer( delgaContainer );
	}
	
	// add patch data directories
	int i;
	for( i=0; i<pPatches.GetCount(); i++ ){
		const declPatch &patch = *pPatches.GetAt( i );
		
		if( patch.GetDelgaFile().IsEmpty() ){
			filePath.SetFromNative( patch.GetPatchDirectory() );
			filePath.AddUnixPath( patch.GetDataDirectory() );
			VFSAddDiskDir( "/", filePath.GetPathNative(), true );
			
		}else{
			decPath pathDelgaDir( decPath::CreatePathNative( patch.GetDelgaFile() ) );
			const decString delgaFilename( pathDelgaDir.GetLastComponent() );
			pathDelgaDir.RemoveLastComponent();
			
			delgaVfs.TakeOver( new deVirtualFileSystem );
			
			delgaVfsDelgaDir.TakeOver( new deVFSDiskDirectory( pathDelgaDir ) );
			delgaVfs->AddContainer( delgaVfsDelgaDir );
			
			delgaArchive.TakeOver( amgr.OpenArchive( delgaVfs, delgaFilename, "/" ) );
			
			delgaContainer.TakeOver( amgr.CreateContainer( decPath::CreatePathUnix( "/" ),
				delgaArchive, decPath::CreatePathUnix( patch.GetDataDirectory() ) ) );
			pLauncher->GetEngine()->GetEngine()->GetVirtualFileSystem()->AddContainer( delgaContainer );
		}
	}
	
	// add script module shared data if existing
	deEngine &engine = *pLauncher->GetEngine()->GetEngine();
	engine.GetScriptingSystem()->AddVFSSharedDataDir( *engine.GetVirtualFileSystem() );
	
	// add the user game overlay directory(writeable)
	filePath.SetFromNative( configuration.GetPathConfigUser() );
	filePath.AddComponent( "games" );
	filePath.AddComponent( pGame->GetIdentifier().ToHexString( false ) );
	filePath.AddComponent( "overlay" );
	VFSAddDiskDir( "/", filePath.GetPathNative(), false );
	
	engine.SetPathOverlay( filePath.GetPathNative() );
	
	// add the user game configuration directory (writeable)
	filePath.SetFromNative( configuration.GetPathConfigUser() );
	filePath.AddComponent( "games" );
	filePath.AddComponent( pGame->GetIdentifier().ToHexString( false ) );
	filePath.AddComponent( "config" );
	VFSAddDiskDir( pGame->GetPathConfig(), filePath.GetPathNative(), false );
	
	engine.SetPathConfig( filePath.GetPathNative() );
	
	// add the user game capture directory (writeable)
	filePath.SetFromNative( configuration.GetPathConfigUser() );
	filePath.AddComponent( "games" );
	filePath.AddComponent( pGame->GetIdentifier().ToHexString( false ) );
	filePath.AddComponent( "capture" );
	VFSAddDiskDir( pGame->GetPathCapture(), filePath.GetPathNative(), false );
	
	engine.SetPathCapture( filePath.GetPathNative() );
}



void declRunGame::Run(){
	deLogger &logger = *pLauncher->GetLogger();
	
	// parse the arguments
	if( ! ParseArguments() ){
		return;
	}
	
	// locate the game to run
	pLauncher->GetEngine()->Start( pLauncher->GetEngineLogger(), "" );
	try{
		pLauncher->GetGameManager()->LoadGameList();
		pLauncher->GetPatchManager().LoadPatchList();
		
	}catch( const deException & ){
		pLauncher->GetEngine()->Stop();
		throw;
	}
	pLauncher->GetEngine()->Stop();
	
	if( ! LocateGame() ){
		return;
	}
	
	// init launcher. has to be done after game is found to use the right cache app id
	InitLauncher();
	
	if( ! LocatePatches() ){
		return;
	}
	
	// check if the game can run
	if( ! pGame->GetCanRun() ){
		PrintGameProblems();
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
	deEngine &engine = *pLauncher->GetEngine()->GetEngine();
	
	struct sModuleParamState{
		const deBaseModule *module;
		decStringDictionary parameters;
	};
	sModuleParamState moduleState[ 11 ];
	deModuleParameter moduleParameter;
	int i, j;
	
	try{
		pProfile->Activate( *pLauncher );
		if( pModuleParameters ){
			logger.LogInfo( LOGSOURCE, "Apply command line module parameters" );
			pModuleParameters->Apply( *pLauncher );
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
		
		int i;
		for( i=0; i<pPatches.GetCount(); i++ ){
			logger.LogInfoFormat( LOGSOURCE, "Using patch: %s",
				pPatches.GetAt( i )->GetIdentifier().ToHexString( false ).GetString() );
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
			pRunFullScreen, pWindowTitle.GetString(), icon );
		
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
		moduleState[ 10 ].module = engine.GetVRSystem()->GetActiveModule();
		
		for( i=0; i<11; i++ ){
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
		logger.LogErrorFormat( LOGSOURCE, "Game '%s' caused an exception",
			pGameIdentifier.GetString() );
		logger.LogException( LOGSOURCE, e );
		throw;
	}
	
	// compare module parameters against stored ones and process result
	deObjectReference module, parameter;
	declGPModuleList changedModules;
	
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
				module.TakeOver( new declGPModule( moduleName ) );
				changedModules.Add( ( declGPModule* )( deObject* )module );
			}
			
			parameter.TakeOver( new declGPMParameter( name, value ) );
			( ( declGPModule& )( deObject& )module ).GetParameterList().
				Add( ( declGPMParameter* )( deObject* )parameter );
		}
	}
	
	StoreCustomConfig( changedModules );
}

void declRunGame::StoreCustomConfig( const declGPModuleList &changedModules ){
	if( changedModules.GetCount() == 0 ){
		return;
	}
	
	// ensure custom profile exists and is initialized with profile used to run the game
	// unless this had been already the custom profile
	if( ! pGame->GetCustomProfile() ){
		deObjectReference profile;
		profile.TakeOver( new declGameProfile );
		pGame->SetCustomProfile( ( declGameProfile* )( deObject* )profile );
	}
	declGameProfile &customProfile = *pGame->GetCustomProfile();
	
	if( pProfile != &customProfile ){
		customProfile.CopyFrom( *pProfile );
	}
	
	// update custom profile
	declGPModuleList &modules = customProfile.GetModuleList();
	const int moduleCount = changedModules.GetCount();
	deObjectReference module, parameter;
	int i, j;
	
	for( i=0; i<moduleCount; i++ ){
		const declGPModule &moduleChanges = *changedModules.GetAt( i );
		
		module = modules.GetNamed( moduleChanges.GetName() );
		if( module ){
			declGPMParameterList &params = ( ( declGPModule* )( deObject* )module )->GetParameterList();
			const declGPMParameterList &paramsChanges = moduleChanges.GetParameterList();
			const int paramCount = paramsChanges.GetCount();
			
			for( j=0; j<paramCount; j++ ){
				const declGPMParameter &paramChanges = *paramsChanges.GetAt( j );
				parameter = params.GetNamed( paramChanges.GetName() );
				
				if( parameter ){
					( ( declGPMParameter& )( deObject& )parameter ).SetValue( paramChanges.GetValue() );
					
				}else{
					parameter.TakeOver( new declGPMParameter( paramChanges ) );
					params.Add( ( declGPMParameter* )( deObject* )parameter );
				}
			}
			
		}else{
			module.TakeOver( new declGPModule( moduleChanges ) );
			modules.Add( ( declGPModule* )( deObject* )module );
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
