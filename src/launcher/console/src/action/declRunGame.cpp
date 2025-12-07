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

#include "declRunGame.h"
#include "../declLauncher.h"

#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/game/profile/delGPModule.h>
#include <delauncher/game/profile/delGPMParameter.h>
#include <delauncher/game/fileformat/delFileFormat.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deLibraryModule.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declRunGame
//////////////////////

// Constructor, destructor
////////////////////////////

declRunGame::declRunGame(declLauncher &launcher) :
pLauncher(launcher),
pUseConsole(false),
pLogAllToConsole(false),
pModuleParameters(NULL),
pHasPatchIdentifier(false){
}

declRunGame::~declRunGame(){
	if(pModuleParameters){
		delete pModuleParameters;
	}
}



// Management
///////////////

void declRunGame::PrintSyntax(){
	printf("Drag[en]gine Console Launcher.\n");
	printf("Written by Plüss Roland (roland@rptd.ch).\n");
	printf("Released under the GPL (http://www.gnu.org/licenses/gpl.html), 2011.\n");
	printf("\n");
	printf("Runs a game.\n");
	printf("\n");
	printf("Syntax:\n");
	printf("delauncherconsole run [<options>] {<game> | -f <game.delga> | --file <game.delga>>} [<game-options>]\n");
	printf("   <options> can be one or more of the following:\n");
	printf("      -c, --console           Use console and no graphic system.\n");
	printf("      -p, --profile <name>    Use named game profile instead of the default one.\n");
	printf("      -d, --debug             Display all debug information in the console not just the log file.\n");
	printf("      -P, --patch <id|alias>  Use patch with identifier instead of latest. Use empty string to run unpatched.\n");
	printf("      --mparam module:param=value     Set module parameter before running the game.\n");
	printf("   <game>                Identifier of the game to run.\n");
	printf("   -f <game.delga>       DELGA file to run.\n");
	printf("   --file <game.delga>   DELGA file to run.\n");
	printf("   \n");
	printf("   <game-options> are optional parameters specific to the game to load.\n");
	printf("   see the game documentation for a list of all available parameters.\n");
	printf("\n");
}

bool declRunGame::ParseArguments(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	const int argumentCount = argumentList.GetArgumentCount();
	deLogger &logger = *pLauncher.GetLogger();
	decString utf8Argument, utf8Value;
	const decUnicodeString *argument;
	int o, optionLen, option;
	int argumentIndex = 1;
	
	// first check for options. they all start with a dash.
	while(argumentIndex < argumentCount){
		argument = argumentList.GetArgumentAt(argumentIndex);
		utf8Argument = argument->ToUTF8();
		
		if(utf8Argument == "--console"){
			pUseConsole = true;
			
		}else if(utf8Argument == "--profile"){
			argumentIndex++;
			
			if(argumentCount - argumentIndex > 0){
				pProfileName = argumentList.GetArgumentAt(argumentIndex)->ToUTF8();
				
			}else{
				logger.LogError(LOGSOURCE, "Missing profile name after --profile");
				return false;
			}
			
		}else if(utf8Argument == "--mparam"){
			argumentIndex++;
			
			if(argumentCount - argumentIndex > 0){
				utf8Value = argumentList.GetArgumentAt(argumentIndex)->ToUTF8();
				
				if(!ParseModuleParameter(utf8Value)){
					return false;
				}
				
			}else{
				logger.LogError(LOGSOURCE, "Missing value after --mparam");
				return false;
			}
			
		}else if(utf8Argument == "--debug"){
			pLogAllToConsole = true;
			
		}else if(utf8Argument == "--file"){
			argumentIndex++;
			
			if(argumentCount - argumentIndex > 0){
				pGameDefFile = argumentList.GetArgumentAt(argumentIndex)->ToUTF8();
				argumentIndex++;
				break;
				
			}else{
				logger.LogError(LOGSOURCE, "Missing game definition filename after --file");
				return false;
			}
			
		}else if(utf8Argument == "--patch"){
			argumentIndex++;
			
			if(argumentCount - argumentIndex > 0){
				pPatchIdentifier.SetFromHexString(argumentList.GetArgumentAt(argumentIndex)->ToUTF8(), false);
				pHasPatchIdentifier = true;
				
			}else{
				logger.LogError(LOGSOURCE, "Missing patch identifier after --patch");
				return false;
			}
			
		}else if(utf8Argument[0] == '-'){
			optionLen = utf8Argument.GetLength();
			
			for(o=1; o<optionLen; o++){
				option = utf8Argument[o];
				
				if(option == 'c'){
					pUseConsole = true;
					
				}else if(option == 'p'){
					if(o == optionLen - 1){
						argumentIndex++;
						
						if(argumentCount - argumentIndex > 0){
							pProfileName = argumentList.GetArgumentAt(argumentIndex)->ToUTF8();
							
						}else{
							logger.LogError(LOGSOURCE, "Missing profile name after -p");
							return false;
						}
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "Invalid option '%s' (p not last character)",
							utf8Argument.GetString());
						return false;
					}
					
				}else if(option == 'd'){
					pLogAllToConsole = true;
					
				}else if(option == 'f'){
					if(o == optionLen - 1){
						argumentIndex++;
						
						if(argumentCount - argumentIndex > 0){
							pGameDefFile = argumentList.GetArgumentAt(argumentIndex)->ToUTF8();
							argumentIndex++;
							break;
							
						}else{
							logger.LogError(LOGSOURCE, "Missing game definition filename after -f");
							return false;
						}
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "Invalid option '%s' (f not last character)",
							utf8Argument.GetString());
						return false;
					}
					
				}else if(option == 'P'){
					if(o == optionLen - 1){
						argumentIndex++;
						
						if(argumentCount - argumentIndex > 0){
							pPatchIdentifier.SetFromHexString(argumentList.GetArgumentAt(argumentIndex)->ToUTF8(), false);
							pHasPatchIdentifier = true;
							
						}else{
							logger.LogError(LOGSOURCE, "Missing patch identifier after -P");
							return false;
						}
						
					}else{
						logger.LogErrorFormat(LOGSOURCE, "Invalid option '%s' (p not last character)",
							utf8Argument.GetString());
						return false;
					}
					
				}else{
					logger.LogErrorFormat(LOGSOURCE, "Unknown option -%c", (char)option);
					return false;
				}
			}
			
			if(o < optionLen){
				break;
			}
			
		}else{
			break;
		}
		
		argumentIndex++;
	}
	
	// the next argument has to be the game identifier if a game definition file has not been specified
	if(pGameDefFile.IsEmpty()){
		if(argumentIndex == argumentCount){
			logger.LogError(LOGSOURCE, "No game specified");
			return false;
		}
		
		pGameIdentifier = argumentList.GetArgumentAt(argumentIndex++)->ToUTF8();
	}
	
	// the rest are arguments for the game
	while(argumentIndex < argumentCount){
		pGameArgs.AddArgument(*argumentList.GetArgumentAt(argumentIndex));
		argumentIndex++;
	}
	
	// if console mode is requested modify the engine instance factory
	if(pUseConsole){
		const delEngineInstanceDirect::Factory::Ref factory(
			delEngineInstanceDirect::Factory::Ref::NewWith());
		factory->SetUseConsole(true);
		pLauncher.SetEngineInstanceFactory(factory);
	}
	
	return true;
}

bool declRunGame::ParseModuleParameter(const decString &value){
	deLogger &logger = *pLauncher.GetLogger();
	const int length = value.GetLength();
	decString moduleName, parameterName, parameterValue;
	delGPMParameter *parameter;
	int separator1, separator2;
	delGPModule *module;
	
	for(separator1=0; separator1<length; separator1++){
		if(value[separator1] == ':'){
			break;
			
		}else{
			moduleName.AppendCharacter(value[separator1]);
		}
	}
	
	for(separator2=separator1+1; separator2<length; separator2++){
		if(value[separator2] == '='){
			break;
			
		}else{
			parameterName.AppendCharacter(value[separator2]);
		}
	}
	
	if(separator1 == length || separator2 == length || moduleName.IsEmpty() || parameterName.IsEmpty()){
		logger.LogErrorFormat(LOGSOURCE, "Invalid module parameter value '%s'. "
			"Has to be of the format module:parameter=value", value.GetString());
		return false;
	}
	
	parameterValue = value.GetMiddle(separator2 + 1);
	
	if(!pModuleParameters){
		pModuleParameters = new delGPModuleList;
	}
	
	module = pModuleParameters->GetNamed(moduleName);
	if(!module){
		module = new delGPModule;
		module->SetName(moduleName);
		pModuleParameters->Add(module);
	}
	
	delGPMParameterList &parameterList = module->GetParameters();
	
	parameter = parameterList.GetNamed(parameterName);
	if(!parameter){
		parameter = new delGPMParameter;
		parameter->SetName(parameterName);
		parameterList.Add(parameter);
	}
	
	parameter->SetValue(parameterValue);
	
	return true;
}

bool declRunGame::LocateGame(){
	delGameManager &gameManager = pLauncher.GetGameManager();
	deLogger &logger = *pLauncher.GetLogger();
	
	// locate the game to run
	if(pGameDefFile.IsEmpty()){
		try{
			pGame = gameManager.GetGames().GetWithID(decUuid(pGameIdentifier, false));
		}catch(const deException &){
			// ignore. could be identifier
		}
		
		if(!pGame){
			const delGameList matching(gameManager.GetGames().GetWithAlias(pGameIdentifier));
			if(matching.GetCount() == 1){
				pGame = matching.GetAt(0);
				
			}else if(matching.GetCount() > 1){
				logger.LogErrorFormat(LOGSOURCE, "Multiple games found with same alias identifier '%s'",
					pGameIdentifier.GetString());
				return false;
			}
		}
		
	}else{
		// the game definition file is not required to be installed if defined by file. It is
		// always used even if a game with the same identifier is already installed. running a
		// game by explicit game file overrides the installed one. otherwise it is difficult
		// for the user to understand why something else happens than he indented
		delGameList list;
		
		{
		const delEngineInstance::Ref instance(delEngineInstance::Ref::New(
			pLauncher.GetEngineInstanceFactory().CreateEngineInstance(
				pLauncher, pLauncher.GetEngine().GetLogFile())));
		
		instance->StartEngine();
		instance->LoadModules();
		
		gameManager.LoadGameFromDisk(instance, pGameDefFile, list);
		}
		
		if(list.GetCount() == 0){
			logger.LogInfo(LOGSOURCE, "No valid game definition found.");
			return false;
		}
		
		pGame = list.GetAt(0); // TODO support multiple games using a choice for for example
		
		// load configuration if the game is not installed. this allows to keep the parameter
		// changes alive done by the player inside the game
		if(!gameManager.GetGames().HasWithID(pGame->GetIdentifier())){
			pGame->LoadConfig();
		}
		
		pGame->VerifyRequirements();
	}
	
	if(!pGame){
		logger.LogErrorFormat(LOGSOURCE, "No game found with identifier '%s'",
			pGameIdentifier.GetString());
		return false;
	}
	
	return true;
}

bool declRunGame::LocateProfile(){
	delGameManager &gameManager = pLauncher.GetGameManager();
	deLogger &logger = *pLauncher.GetLogger();
	
	// locate the profile to run
	delGameProfile *profile = nullptr;
	
	if(pProfileName.IsEmpty()){
		profile = pGame->GetProfileToUse();
		
	}else{
		profile = gameManager.GetProfiles().GetNamed(pProfileName);
		if(!profile){
			logger.LogErrorFormat(LOGSOURCE, "No profile found with name '%s'", pProfileName.GetString());
			return false;
		}
	}
	
	if(!profile->GetValid()){
		PrintProfileProblems(*profile);
		return false;
	}
	
	// determine patch to use
	if(!pHasPatchIdentifier){
		if(pGame->GetUseCustomPatch()){
			pHasPatchIdentifier = true;
			pPatchIdentifier = pGame->GetUseCustomPatch();
			
		}else if(!pGame->GetUseLatestPatch()){
			pHasPatchIdentifier = true;
			pPatchIdentifier.Clear();
		}
	}
	
	// udpate the run parameters
	pRunParams.SetGameProfile(profile);
	
	decString error;
	if(!pRunParams.FindPatches(pGame, pGame->GetUseLatestPatch(), pPatchIdentifier, error)){
		logger.LogError(LOGSOURCE, error.GetString());
		return false;
	}
	
	UpdateRunArguments();
	pRunParams.SetWidth(profile->GetWidth());
	pRunParams.SetHeight(profile->GetHeight());
	pRunParams.SetFullScreen(profile->GetFullScreen());
	
	const decPoint windowSize(pGame->GetDisplayScaledWindowSize());
	if(windowSize != decPoint()){
		pRunParams.SetWidth(windowSize.x);
		pRunParams.SetHeight(windowSize.y);
		pRunParams.SetFullScreen(false);
	}
	
	ApplyCustomModuleParameters(); // potentially changes game profile set in run parameters
	return true;
}

void declRunGame::UpdateRunArguments(){
	const delGameProfile &profile = *pRunParams.GetGameProfile();
	decString arguments;
	
	if(profile.GetReplaceRunArguments()){
		arguments = profile.GetRunArguments();
		
	}else{
		arguments = pGame->GetRunArguments();
		if(!arguments.IsEmpty()){
			arguments.Append(" ");
		}
		arguments.Append(profile.GetRunArguments());
	}
	
	const int count = pGameArgs.GetArgumentCount();
	int i;
	for(i=0; i<count; i++){
		if(!arguments.IsEmpty()){
			arguments.Append(" ");
		}
		arguments.Append(pGameArgs.GetArgumentAt(i)->ToUTF8());
	}
	
	pRunParams.SetRunArguments(arguments);
}

void declRunGame::ApplyCustomModuleParameters(){
	if(!pModuleParameters){
		return;
	}
	
	// ensure custom profile exists and is initialized with profile used to run the game
	delGameProfile::Ref profile = pGame->GetCustomProfile();
	
	if(!profile){
		profile.TakeOver(pLauncher.CreateGameProfile());
		*profile = *pRunParams.GetGameProfile(); // copy content not pointer
		pGame->SetCustomProfile(profile);
	}
	
	// update custom profile
	profile->GetModules().Update(*pModuleParameters);
	
	// use custom profile
	pRunParams.SetGameProfile(profile);
}

void declRunGame::PrintGameProblems(){
	const delFileFormatList &fileFormatList = pGame->GetFileFormats();
	const int fileFormatCount = fileFormatList.GetCount();
	deLogger &logger = *pLauncher.GetLogger();
	int i;
	
	logger.LogErrorFormat(LOGSOURCE, "Game '%s'(%s) has the following problems:",
		pGame->GetAliasIdentifier().GetString(), pGame->GetIdentifier().ToHexString(false).GetString());
	
	for(i=0; i<fileFormatCount; i++){
		const delFileFormat &fileFormat = *fileFormatList.GetAt(i);
		
		if(!fileFormat.GetSupported()){
			if(deModuleSystem::IsSingleType(fileFormat.GetType())){
				logger.LogErrorFormat(LOGSOURCE, "- File Format '%s' defines single type %s",
					fileFormat.GetPattern().GetString(),
					deModuleSystem::GetTypeDirectory(fileFormat.GetType()));
				
			}else{
				logger.LogErrorFormat(LOGSOURCE,
					"- File Format '%s' is not supported by any loaded modules",
					fileFormat.GetPattern().GetString());
			}
		}
	}
	
	if(!pGame->GetScriptModuleFound()){
		PrintModuleProblem(pGame->GetScriptModule(), deModuleSystem::emtScript);
	}
}

void declRunGame::PrintProfileProblems(const delGameProfile &profile){
	pLauncher.GetLogger()->LogErrorFormat(LOGSOURCE,
		"Profile '%s' has the following problems:", profile.GetName().GetString());
	
	PrintModuleProblem(profile.GetModuleGraphic(), deModuleSystem::emtGraphic);
	PrintModuleProblem(profile.GetModuleInput(), deModuleSystem::emtInput);
	PrintModuleProblem(profile.GetModulePhysics(), deModuleSystem::emtPhysics);
	PrintModuleProblem(profile.GetModuleAnimator(), deModuleSystem::emtAnimator);
	PrintModuleProblem(profile.GetModuleAI(), deModuleSystem::emtAI);
	PrintModuleProblem(profile.GetModuleCrashRecovery(), deModuleSystem::emtCrashRecovery);
	PrintModuleProblem(profile.GetModuleAudio(), deModuleSystem::emtAudio);
	PrintModuleProblem(profile.GetModuleSynthesizer(), deModuleSystem::emtSynthesizer);
	PrintModuleProblem(profile.GetModuleNetwork(), deModuleSystem::emtNetwork);
	PrintModuleProblem(profile.GetModuleVR(), deModuleSystem::emtVR);
}

void declRunGame::PrintModuleProblem(const char *moduleName, deModuleSystem::eModuleTypes moduleType){
	delEngineModule * const module = pLauncher.GetEngine().GetModules().GetNamed(moduleName);
	deLogger &logger = *pLauncher.GetLogger();
	
	if(!module){
		logger.LogErrorFormat(LOGSOURCE, "- %s module '%s' does not exist",
			deModuleSystem::GetTypeDirectory(moduleType), moduleName);
		
	}else if(module->GetType() != moduleType){
		logger.LogErrorFormat(LOGSOURCE, "- Module '%s' is not a %s module",
			moduleName, deModuleSystem::GetTypeDirectory(moduleType));
		
	}else if(module->GetStatus() != delEngineModule::emsReady){
		const char *reason;
		
		switch(module->GetErrorCode()){
		case deLoadableModule::eecCreateModuleFailed:
			reason = "Creating module failed";
			break;
			
		case deLibraryModule::eecLibFileNotFound:
			reason = "Module library file could not be found";
			break;
			
		case deLibraryModule::eecLibFileNotRegularFile:
			reason = "Module library file is not a regular library file";
			break;
			
		case deLibraryModule::eecLibFileSizeMismatch:
			reason = "Size of the module library file does not match the size in the module definition";
			break;
			
		case deLibraryModule::eecLibFileCheckSumMismatch:
			reason = "Checksum of the module library file does not match the checksum in the module definition";
			break;
			
		case deLibraryModule::eecLibFileOpenFailed:
			reason = "Module library file could not be opened";
			break;
			
		case deLibraryModule::eecLibFileEntryPointNotFound:
			reason = "Entry point function specified in the module definition could not be found in the library file";
			break;
			
		case deLibraryModule::eecLibFileCreateModuleFailed:
			reason = "Creating the module using the entry point function failed";
			break;
			
		default:
			reason = "Unknown problem";
		}
		
		logger.LogErrorFormat(LOGSOURCE, "- %s module '%s' is not working (%s)",
			deModuleSystem::GetTypeDirectory(moduleType), moduleName, reason);
	}
}

void declRunGame::Run(){
	if(!ParseArguments()){
		return;
	}
	
	pLauncher.Prepare();
	
	if(!LocateGame()){
		return;
	}
	
	if(!pGame->GetCanRun()){
		PrintGameProblems();
		return;
	}
	
	if(!LocateProfile()){
		return;
	}
	
	// start the game
	deLogger &logger = *pLauncher.GetLogger();
	logger.LogInfoFormat(LOGSOURCE, "Cache application ID = '%s'",
		pGame->GetIdentifier().ToHexString(false).GetString());
	logger.LogInfoFormat(LOGSOURCE, "Starting game '%s' using profile '%s'",
		pGame->GetTitle().ToUTF8().GetString(), pRunParams.GetGameProfile()->GetName().GetString());
	
	deLogger *engineLogger = pLauncher.GetEngineLogger();
	if(pLogAllToConsole){
		engineLogger = pLauncher.GetEngineLoggerDebug();
	}
	
	const delEngineInstanceDirect::Factory::Ref factory(
		delEngineInstanceDirect::Factory::Ref::NewWith(engineLogger));
	factory->SetUseConsole(pUseConsole);
	
	// run game. blocks until finished since we use a direct engine instance. this method
	// does all the heavy lifting including storing game configuration if changed
#ifdef OS_BEOS
	/** Running game or nullptr. */
	pLauncher.runningGame = pGame;
	try{
#endif
	
	pGame->StartGame(pRunParams, factory);
	
#ifdef OS_BEOS
		pLauncher.runningGame = nullptr;
	}catch(const deException &){
		pLauncher.runningGame = nullptr;
		throw;
	}
#endif
}
