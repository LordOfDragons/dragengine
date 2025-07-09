/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <emscripten.h>

#include "dewlLauncher.h"

#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/game/fileformat/delFileFormat.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deLibraryModule.h>
#include <dragengine/threading/deMutexGuard.h>


// Definitions
////////////////

#define LOGSOURCE "WebLauncher"


// Class dewlLauncher::RunGameThread
//////////////////////////////////////

dewlLauncher::RunGameThread::RunGameThread(dewlLauncher &launcher) :
pLauncher(launcher){
}

dewlLauncher::RunGameThread::~RunGameThread(){
}

void dewlLauncher::RunGameThread::Run(){
	pLauncher.pRunGameThreadFunc();
}


// Class dewlLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

dewlLauncher::dewlLauncher() :
pModuleParameters(nullptr),
pHasPatchIdentifier(false),
pRunGameThread(nullptr)
{
	SetEngineInstanceFactory(delEngineInstanceDirect::Factory::Ref::New(
		new delEngineInstanceDirect::Factory));
}

dewlLauncher::~dewlLauncher(){
	CleanUp();
}


// Management
///////////////

void dewlLauncher::Init(){
	try{
		DEASSERT_NULL(pRunGameThread)
		
		pInitLogger();
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
		EM_ASM(throw 'Launcher.Init failed');
	}
}

void dewlLauncher::Prepare(){
	try{
		DEASSERT_NULL(pRunGameThread)
		
		delLauncher::Prepare();
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
		EM_ASM(throw 'Launcher.Prepare failed');
	}
}

void dewlLauncher::LocateGame(){
	try{
		DEASSERT_NULL(pRunGameThread)
		
		delGameManager &gameManager = GetGameManager();
		deLogger &logger = *GetLogger();
		
		delGameList list;
		
		{
		const delEngineInstance::Ref instance(delEngineInstance::Ref::New(
			GetEngineInstanceFactory().CreateEngineInstance(*this, GetEngine().GetLogFile())));
		
		instance->StartEngine();
		instance->LoadModules();
		
		gameManager.LoadGameFromDisk(instance, pDelgaPath, list);
		}
		
		if(list.GetCount() == 0){
			logger.LogInfo(LOGSOURCE, "No valid game definition found.");
			DETHROW_INFO(deeInvalidParam, "No valid game definition found.");
		}
		
		pGame = list.GetAt(0); // TODO support multiple games using a choice for for example
		
		// load configuration if the game is not installed. this allows to keep the parameter
		// changes alive done by the player inside the game
		if(!gameManager.GetGames().HasWithID(pGame->GetIdentifier())){
			pGame->LoadConfig();
		}
		
		pGame->VerifyRequirements();
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
		EM_ASM(throw 'Launcher.LocateGame failed');
	}
}

std::vector<std::string> dewlLauncher::GetGameProblems(){
	if((pGame && pGame->GetCanRun()) || pRunGameThread){
		return {};
	}
	
	std::vector<std::string> problems;
	pGameProblems(problems);
	if(problems.empty()){
		problems.push_back("Unknown problem, see logs");
	}
	return problems;
}

void dewlLauncher::LocateProfile(){
	try{
		DEASSERT_NULL(pRunGameThread)
		
		delGameManager &gameManager = GetGameManager();
		deLogger &logger = *GetLogger();
		
		if(pProfileName.IsEmpty()){
			pProfile = pGame->GetProfileToUse();
			
		}else{
			pProfile = gameManager.GetProfiles().GetNamed(pProfileName);
			if(!pProfile){
				decString message;
				message.Format("No profile found with name '%s'", pProfileName.GetString());
				logger.LogError(LOGSOURCE, message.GetString());
				DETHROW_INFO(deeInvalidParam, message);
			}
		}
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
	}
}

std::string dewlLauncher::GetLocatedProfileName(){
	return pProfile ? pProfile->GetName().GetString() : "";
}

std::vector<std::string> dewlLauncher::GetProfileProblems(){
	if((pProfile && pProfile->GetValid()) || pRunGameThread){
		return {};
	}
	
	std::vector<std::string> problems;
	pProfileProblems(problems, pProfile);
	if(problems.empty()){
		problems.push_back("Unknown problem, see logs");
	}
	return problems;
}

void dewlLauncher::LocatePatches(){
	try{
		DEASSERT_NULL(pRunGameThread)
		
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
		
		// update the run parameters
		pRunParams.SetGameProfile(pProfile);
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
	}
}

std::vector<std::string> dewlLauncher::GetPatchProblems(){
	if(!pGame || pRunGameThread){
		return {};
	}
	
	std::vector<std::string> problems;
	
	decString error;
	if(!pRunParams.FindPatches(pGame, pGame->GetUseLatestPatch(), pPatchIdentifier, error)){
		problems.push_back(error.GetString());
		GetLogger()->LogError(LOGSOURCE, error.GetString());
	}
	
	return problems;
}

void dewlLauncher::PrepareRunParameters(){
	try{
		DEASSERT_NULL(pRunGameThread)
		
		pUpdateRunArguments();
		pRunParams.SetWidth(pProfile->GetWidth());
		pRunParams.SetHeight(pProfile->GetHeight());
		pRunParams.SetFullScreen(pProfile->GetFullScreen());
		
		const decPoint windowSize(pGame->GetDisplayScaledWindowSize());
		if(windowSize != decPoint()){
			pRunParams.SetWidth(windowSize.x);
			pRunParams.SetHeight(windowSize.y);
			pRunParams.SetFullScreen(false);
		}
		
		pApplyCustomModuleParameters(); // potentially changes game profile set in run parameters
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
	}
}

void dewlLauncher::StartGame(){
	try{
		DEASSERT_NULL(pRunGameThread)
		
		deLogger &logger = *GetLogger();
		logger.LogInfoFormat(LOGSOURCE, "Cache application ID = '%s'",
			pGame->GetIdentifier().ToHexString(false).GetString());
		logger.LogInfoFormat(LOGSOURCE, "Starting game '%s' using profile '%s'",
			pGame->GetTitle().ToUTF8().GetString(),
			pRunParams.GetGameProfile()->GetName().GetString());
		
		pRunGameThread = new RunGameThread(*this);
		pRunGameThread->Start();
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
	}
}

void dewlLauncher::CleanUp(){
	try{
		if(pRunGameThread){
			pRunGameThread->Stop();
			delete pRunGameThread;
			pRunGameThread = nullptr;
		}
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
	}
}


// Private Functions
//////////////////////

void dewlLauncher::pInitLogger(){
	GetLogger()->RemoveAllLoggers();
	
	AddFileLogger("delauncher-web");
	
	pLoggerJS.TakeOver(new dewlLoggerJS);
	GetLogger()->AddLogger(pLoggerJS);
}

void dewlLauncher::pGameProblems(std::vector<std::string> &problems){
	const delFileFormatList &fileFormatList = pGame->GetFileFormats();
	const int fileFormatCount = fileFormatList.GetCount();
	deLogger &logger = *GetLogger();
	decString problem;
	int i;
	
	logger.LogErrorFormat(LOGSOURCE, "Game '%s'(%s) has the following problems:",
		pGame->GetAliasIdentifier().GetString(),
		pGame->GetIdentifier().ToHexString(false).GetString());
	
	for(i=0; i<fileFormatCount; i++){
		const delFileFormat &fileFormat = *fileFormatList.GetAt(i);
		
		if(!fileFormat.GetSupported()){
			if(deModuleSystem::IsSingleType(fileFormat.GetType())){
				problem.Format("File Format '%s' defines single type %s",
					fileFormat.GetPattern().GetString(),
					deModuleSystem::GetTypeDirectory(fileFormat.GetType()));
				problems.push_back(problem.GetString());
				logger.LogErrorFormat(LOGSOURCE, "- %s", problem.GetString());
				
			}else{
				problem.Format("File Format '%s' is not supported by any loaded modules",
					fileFormat.GetPattern().GetString());
				problems.push_back(problem.GetString());
				logger.LogErrorFormat(LOGSOURCE, "- %s", problem.GetString());
			}
		}
	}
	
	if(!pGame->GetScriptModuleFound()){
		pModuleProblem(problems, pGame->GetScriptModule(), deModuleSystem::emtScript);
	}
}

void dewlLauncher::pProfileProblems(std::vector<std::string> &problems,
const delGameProfile &profile){
	GetLogger()->LogErrorFormat(LOGSOURCE, "Profile '%s' has the following problems:",
		profile.GetName().GetString());
	
	pModuleProblem(problems, profile.GetModuleGraphic(), deModuleSystem::emtGraphic);
	pModuleProblem(problems, profile.GetModuleInput(), deModuleSystem::emtInput);
	pModuleProblem(problems, profile.GetModulePhysics(), deModuleSystem::emtPhysics);
	pModuleProblem(problems, profile.GetModuleAnimator(), deModuleSystem::emtAnimator);
	pModuleProblem(problems, profile.GetModuleAI(), deModuleSystem::emtAI);
	pModuleProblem(problems, profile.GetModuleCrashRecovery(), deModuleSystem::emtCrashRecovery);
	pModuleProblem(problems, profile.GetModuleAudio(), deModuleSystem::emtAudio);
	pModuleProblem(problems, profile.GetModuleSynthesizer(), deModuleSystem::emtSynthesizer);
	pModuleProblem(problems, profile.GetModuleNetwork(), deModuleSystem::emtNetwork);
	pModuleProblem(problems, profile.GetModuleVR(), deModuleSystem::emtVR);
}

void dewlLauncher::pModuleProblem(std::vector<std::string> &problems,
const char *moduleName, deModuleSystem::eModuleTypes moduleType){
	delEngineModule * const module = GetEngine().GetModules().GetNamed(moduleName);
	deLogger &logger = *GetLogger();
	decString problem;
	
	if(!module){
		problem.Format("%s module '%s' does not exist",
			deModuleSystem::GetTypeDirectory(moduleType), moduleName);
		problems.push_back(problem.GetString());
		logger.LogErrorFormat(LOGSOURCE, "- %s", problem.GetString());
		
	}else if(module->GetType() != moduleType){
		problem.Format("Module '%s' is not a %s module", moduleName,
			deModuleSystem::GetTypeDirectory(moduleType));
		problems.push_back(problem.GetString());
		logger.LogErrorFormat(LOGSOURCE, "- %s", problem.GetString());
		
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
		
		problem.Format("%s module '%s' is not working (%s)",
			deModuleSystem::GetTypeDirectory(moduleType), moduleName, reason);
		problems.push_back(problem.GetString());
		logger.LogErrorFormat(LOGSOURCE, "- %s", problem.GetString());
	}
}

void dewlLauncher::pUpdateRunArguments(){
	DEASSERT_NOTNULL(pRunParams.GetGameProfile());
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

void dewlLauncher::pApplyCustomModuleParameters(){
	if(!pModuleParameters){
		return;
	}
	
	// ensure custom profile exists and is initialized with profile used to run the game
	delGameProfile::Ref profile(pGame->GetCustomProfile());
	
	if(!profile){
		profile.TakeOver(CreateGameProfile());
		*profile = *pRunParams.GetGameProfile(); // copy content not pointer
		pGame->SetCustomProfile(profile);
	}
	
	// update custom profile
	profile->GetModules().Update(*pModuleParameters);
	
	// use custom profile
	pRunParams.SetGameProfile(profile);
}

void dewlLauncher::pRunGameThreadFunc(){
	const delEngineInstanceDirect::Factory::Ref factory(
		delEngineInstanceDirect::Factory::Ref::New(
			new delEngineInstanceDirect::Factory(/*pEngineLogger*/ GetLogger())));
	
	// blocks until the game finished
	pGame->StartGame(pRunParams, factory);
}
