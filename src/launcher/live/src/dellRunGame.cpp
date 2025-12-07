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

#include <stdlib.h>

#include "dellRunGame.h"
#include "dellLauncher.h"

#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/fileformat/delFileFormat.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deLibraryModule.h>



// Definitions
////////////////

#define LOGSOURCE "LiveLauncher"



// Class dellRunGame
//////////////////////

// Constructor, destructor
////////////////////////////

dellRunGame::dellRunGame(dellLauncher &launcher) :
pLauncher(launcher){
}

dellRunGame::~dellRunGame(){
}



// Management
///////////////

bool dellRunGame::ParseArguments(){
	const decUnicodeArgumentList &arguments = pLauncher.GetArguments();
	const int argumentCount = arguments.GetArgumentCount();
	deLogger &logger = *pLauncher.GetLauncher().GetLogger();
	decString utf8Argument, utf8Value;
	const decUnicodeString *argument;
	int o, optionLen, option;
	int argumentIndex = 1;
	
	// first check for options. they all start with a dash.
	while(argumentIndex < argumentCount){
		argument = arguments.GetArgumentAt(argumentIndex);
		utf8Argument = argument->ToUTF8();
		
		if(utf8Argument == "--profile"){
			argumentIndex++;
			
			if(argumentCount - argumentIndex > 0){
				pProfileName = arguments.GetArgumentAt(argumentIndex)->ToUTF8();
				
			}else{
				logger.LogError(LOGSOURCE, "Missing profile name after --profile");
				return false;
			}
			
		}else if(utf8Argument[0] == '-'){
			optionLen = utf8Argument.GetLength();
			
			for(o=1; o<optionLen; o++){
				option = utf8Argument[o];
				
				if(option == 'p'){
					if(o == optionLen - 1){
						argumentIndex++;
						
						if(argumentCount - argumentIndex > 0){
							pProfileName = arguments.GetArgumentAt(argumentIndex)->ToUTF8();
							
						}else{
							logger.LogError(LOGSOURCE, "Missing profile name after -p");
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
			
		}else{
			break;
		}
		
		argumentIndex++;
	}
	
	// the rest are arguments for the game
	while(argumentIndex < argumentCount){
		pGameArgs.AddArgument(*arguments.GetArgumentAt(argumentIndex));
		argumentIndex++;
	}
	
	return true;
}

bool dellRunGame::LocateGame(){
	delGameManager &gameManager = pLauncher.GetLauncher().GetGameManager();
	deLogger &logger = *pLauncher.GetLauncher().GetLogger();
	
	// locate the game to run
	if(gameManager.GetGames().GetCount() == 0){
		logger.LogInfo(LOGSOURCE, "No valid game definition found.");
		return false;
	}
	
	pGame = gameManager.GetGames().GetAt(0); // TODO support multiple games using a choice for for example
	return true;
}

bool dellRunGame::LocateProfile(){
	delGameManager &gameManager = pLauncher.GetLauncher().GetGameManager();
	deLogger &logger = *pLauncher.GetLauncher().GetLogger();
	
	// locate the profile to run
	delGameProfile *profile = nullptr;
	
	if(pProfileName.IsEmpty()){
		profile = pGame->GetProfileToUse();
		
	}else{
		profile = gameManager.GetProfiles().GetNamed(pProfileName);
		
		if(! profile){
			logger.LogErrorFormat(LOGSOURCE, "No profile found with name '%s'",
				pProfileName.GetString());
			return false;
		}
	}
	
	if(! profile->GetValid()){
		ShowProfileProblems();
		return false;
	}
	
	// udpate the run parameters
	pRunParams.SetGameProfile(profile);
	
	decString error;
	if(! pRunParams.FindPatches(pGame, pGame->GetUseLatestPatch(), pGame->GetUseCustomPatch(), error)){
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
	
	return true;
}

void dellRunGame::UpdateRunArguments(){
	const delGameProfile &profile = *pRunParams.GetGameProfile();
	decString arguments;
	
	if(profile.GetReplaceRunArguments()){
		arguments = profile.GetRunArguments();
		
	}else{
		arguments = pGame->GetRunArguments();
		if(! arguments.IsEmpty()){
			arguments.Append(" ");
		}
		arguments.Append(profile.GetRunArguments());
	}
	
	const int count = pGameArgs.GetArgumentCount();
	int i;
	for(i=0; i<count; i++){
		if(! arguments.IsEmpty()){
			arguments.Append(" ");
		}
		arguments.Append(pGameArgs.GetArgumentAt(i)->ToUTF8());
	}
	
	pRunParams.SetRunArguments(arguments);
}

void dellRunGame::ShowGameProblems(){
	const delFileFormatList &fileFormatList = pGame->GetFileFormats();
	const int fileFormatCount = fileFormatList.GetCount();
	deLogger &logger = *pLauncher.GetLauncher().GetLogger();
	int i;
	
	logger.LogErrorFormat(LOGSOURCE, "Game '%s' has the following problems:",
		pGame->GetAliasIdentifier().GetString());
	
	for(i=0; i<fileFormatCount; i++){
		const delFileFormat &fileFormat = *fileFormatList.GetAt(i);
		
		if(! fileFormat.GetSupported()){
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
	
	if(! pGame->GetScriptModuleFound()){
		ShowModuleProblem(pGame->GetScriptModule(), deModuleSystem::emtScript);
	}
}

void dellRunGame::ShowProfileProblems(){
	delGameProfile &profile = *pRunParams.GetGameProfile();
	deLogger &logger = *pLauncher.GetLauncher().GetLogger();
	
	logger.LogErrorFormat(LOGSOURCE, "Profile '%s' has the following problems:", profile.GetName().GetString());
	
	ShowModuleProblem(profile.GetModuleGraphic(), deModuleSystem::emtGraphic);
	ShowModuleProblem(profile.GetModuleInput(), deModuleSystem::emtInput);
	ShowModuleProblem(profile.GetModulePhysics(), deModuleSystem::emtPhysics);
	ShowModuleProblem(profile.GetModuleAnimator(), deModuleSystem::emtAnimator);
	ShowModuleProblem(profile.GetModuleAI(), deModuleSystem::emtAI);
	ShowModuleProblem(profile.GetModuleCrashRecovery(), deModuleSystem::emtCrashRecovery);
	ShowModuleProblem(profile.GetModuleAudio(), deModuleSystem::emtAudio);
	ShowModuleProblem(profile.GetModuleSynthesizer(), deModuleSystem::emtSynthesizer);
	ShowModuleProblem(profile.GetModuleNetwork(), deModuleSystem::emtNetwork);
	ShowModuleProblem(profile.GetModuleVR(), deModuleSystem::emtVR);
}

void dellRunGame::ShowModuleProblem(const char *moduleName, deModuleSystem::eModuleTypes moduleType){
	delEngineModule * const module = pLauncher.GetLauncher().GetEngine().GetModules().GetNamed(moduleName);
	deLogger &logger = *pLauncher.GetLauncher().GetLogger();
	
	if(! module){
		logger.LogErrorFormat(LOGSOURCE, "- %s module '%s' does not exist",
			deModuleSystem::GetTypeDirectory(moduleType), moduleName);
		
	}else if(module->GetType() != deModuleSystem::emtScript){
		logger.LogErrorFormat(LOGSOURCE, "- Module '%s' is not a %s module",
			moduleName, deModuleSystem::GetTypeDirectory(moduleType));
		
	}else if(module->GetStatus() == delEngineModule::emsReady){
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

void dellRunGame::Run(){
	if(! ParseArguments()){
		return;
	}
	
	pLauncher.GetLauncher().Prepare();
	
	if(! LocateGame()){
		return;
	}
	
	if(! pGame->GetCanRun()){
		ShowGameProblems();
		return;
	}
	
	if(! LocateProfile()){
		return;
	}
	
	// start the game
	deLogger &logger = *pLauncher.GetLauncher().GetLogger();
	logger.LogInfoFormat(LOGSOURCE, "Cache application ID = '%s'",
		pGame->GetIdentifier().ToHexString(false).GetString());
	logger.LogInfoFormat(LOGSOURCE, "Starting game '%s' using profile '%s'",
		pGame->GetTitle().ToUTF8().GetString(), pRunParams.GetGameProfile()->GetName().GetString());
	
	// run game. blocks until finished since we use a direct engine instance. this method
	// does all the heavy lifting including storing game configuration if changed
#ifdef OS_BEOS
	/** Running game or nullptr. */
	pLauncher.runningGame = pGame;
	try{
#endif
	
	pGame->StartGame(pRunParams);
	
#ifdef OS_BEOS
		pLauncher.runningGame = nullptr;
	}catch(const deException &){
		pLauncher.runningGame = nullptr;
		throw;
	}
#endif
}
