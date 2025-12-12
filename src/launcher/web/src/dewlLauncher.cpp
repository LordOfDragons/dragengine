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
#include <emscripten/wasmfs.h>

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "dewlLauncher.h"
#include "dewlLauncherBindings.h"

#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/game/fileformat/delFileFormat.h>
#include <delauncher/game/profile/delGPModule.h>
#include <delauncher/game/profile/delGPMParameter.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deLibraryModule.h>
#include <dragengine/threading/deMutexGuard.h>


// Definitions
////////////////

#define LOGSOURCE "WebLauncher"

static const std::string EventInitialized("delauncher:initialized");
static const std::string EventRunGame("delauncher:runGame");

std::vector<dewlLauncher::sEvent> dewlLauncher::pEvents;
pthread_mutex_t dewlLauncher::pMutexEvents = PTHREAD_MUTEX_INITIALIZER;


// Class dewlLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

dewlLauncher::dewlLauncher(const std::string &canvasId, int canvasWidth, int canvasHeight) :
pCanvasId(canvasId.c_str()),
pCanvasSize(canvasWidth, canvasHeight),
pModuleParameters(nullptr),
pHasPatchIdentifier(false),
pPatchesValid(false),
pThreadLauncher(0),
pMutexLauncher(PTHREAD_MUTEX_INITIALIZER),
pConditionLauncher(PTHREAD_COND_INITIALIZER)
{
	dewlLoggerJS::AddLogEntryFormat(dewlLoggerJS::eSeverity::info, LOGSOURCE,
		"Launcher: Init (canvasId='%s' canvasSize=%dx%d)",
		pCanvasId.GetString(), pCanvasSize.x, pCanvasSize.y);
	
	const delEngineInstanceDirect::Factory::Ref factory(
		delEngineInstanceDirect::Factory::Ref::New());
	{
	deOSWebWasm::sConfig config{};
	config.canvasId = pCanvasId;
	config.canvasSize = pCanvasSize;
	factory->SetConfig(config);
	}
	SetEngineInstanceFactory(factory);
	
	DEASSERT_TRUE(pthread_create(&pThreadLauncher, nullptr, pLauncherThreadMainGlue, this) == 0)
}

dewlLauncher::~dewlLauncher(){
	CleanUp();
}


// Management
///////////////

void dewlLauncher::RemoveAllModuleParameters(){
	if(pModuleParameters){
		delete pModuleParameters;
		pModuleParameters = nullptr;
	}
}

void dewlLauncher::AddModuleParameter(const std::string &module,
const std::string &parameter, const std::string &value){
	if(!pModuleParameters){
		pModuleParameters = new delGPModuleList;
	}
	
	delGPModule::Ref gpmodule(pModuleParameters->GetNamed(module.c_str()));
	if(!gpmodule){
		gpmodule.TakeOver(new delGPModule(module.c_str()));
		pModuleParameters->Add(gpmodule);
	}
	
	delGPMParameter::Ref gpparam(gpmodule->GetParameters().GetNamed(parameter.c_str()));
	if(gpparam){
		gpparam->SetValue(value.c_str());
		
	}else{
		gpparam.TakeOver(new delGPMParameter(parameter.c_str(), value.c_str()));
		gpmodule->GetParameters().Add(gpparam);
	}
}


void dewlLauncher::RunGame(){
	pthread_mutex_lock(&pMutexLauncher);
	pCommands.push_back({Commands::RunGame});
	pthread_cond_signal(&pConditionLauncher);
	pthread_mutex_unlock(&pMutexLauncher);
}

std::vector<std::string> dewlLauncher::GetGameProblems(){
	if(pGame && pGame->GetCanRun()){
		return {};
	}
	
	std::vector<std::string> problems;
	pGameProblems(problems);
	if(problems.empty()){
		problems.push_back("Unknown problem, see logs");
	}
	return problems;
}

std::string dewlLauncher::GetLocatedProfileName(){
	return pProfile ? pProfile->GetName().GetString() : "";
}

std::vector<std::string> dewlLauncher::GetProfileProblems(){
	if(pProfile && pProfile->GetValid()){
		return {};
	}
	
	std::vector<std::string> problems;
	pProfileProblems(problems, pProfile);
	if(problems.empty()){
		problems.push_back("Unknown problem, see logs");
	}
	return problems;
}

std::vector<std::string> dewlLauncher::GetPatchProblems(){
	if(pPatchesValid){
		return {};
	}
	
	std::vector<std::string> problems;
	problems.push_back(pPatchProblems.GetString());
	return problems;
}

void dewlLauncher::CleanUp(){
	/*
	try{
		if(pRunGameThread){
			pRunGameThread->Stop();
			delete pRunGameThread;
			pRunGameThread = nullptr;
		}
		
	}catch(const deException &e){
		GetLogger()->LogException("Launcher", e);
	}
	*/
	
	pthread_cond_destroy(&pConditionLauncher);
	pthread_mutex_destroy(&pMutexLauncher);
}

void dewlLauncher::MainThreadUpdate(){
	static std::vector<sEvent> events;
	
	if(pthread_mutex_trylock(&pMutexEvents) == 0){
		events = pEvents;
		pEvents.clear();
		pthread_mutex_unlock(&pMutexEvents);
		
	}else{
		events.clear();
		return;
	}
	
	for(auto event : events){
		const char * const ev = event.event.c_str();
		const int resultCode = (int)event.resultCode;
		const char * const error = event.error.c_str();
		
		EM_ASM({
			this.dispatchEvent(new CustomEvent(UTF8ToString($0), {
				detail: {
					result: $1,
					error: UTF8ToString($2)
				}
			}))
		}, ev, resultCode, error);
	}
}


// Private Functions
//////////////////////

void dewlLauncher::pInitLogger(){
	GetLogger()->RemoveAllLoggers();
	
	AddFileLogger("delauncher-web");
	
	pLoggerJS = dewlLoggerJS::Ref::New();
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
	dewlLoggerJS::AddLogEntryFormat(dewlLoggerJS::eSeverity::error, LOGSOURCE,
		"Profile '%s' has the following problems:", profile.GetName().GetString());
	
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

void dewlLauncher::pFSMkdirAbsent(const char *path){
	struct stat s{};
	if(stat(path, &s) == 0){
		return;
	}
	
	if(mkdir(path, 0777) != 0){
		const int e = errno;
		decString message;
		message.Format("Create directory failed: %s (%d)", path, e);
		DETHROW_INFO(deeWriteFile, message);
	}
}

void dewlLauncher::pFSSymlink(const char *pathTarget, const char *pathLink){
	if(symlink(pathTarget, pathLink) != 0){
		const int e = errno;
		decString message;
		message.Format("Create symlink failed: %s -> %s (%d)", pathTarget, pathLink, e);
		DETHROW_INFO(deeWriteFile, message);
	}
}

void *dewlLauncher::pLauncherThreadMainGlue(void *parameter){
	((dewlLauncher*)parameter)->pLauncherThreadMain();
	return nullptr;
}

void dewlLauncher::pLauncherThreadMain(){
	dewlLoggerJS::AddLogEntry(dewlLoggerJS::eSeverity::info, LOGSOURCE, "LauncherThread: Enter");
	
	try{
		// initialize file system
		dewlLoggerJS::AddLogEntry(dewlLoggerJS::eSeverity::info, LOGSOURCE,
			"LauncherThread: Initialize file system");
		backend_t opfs = wasmfs_create_opfs_backend();
		DEASSERT_TRUE(wasmfs_create_directory("/opfs", 0777, opfs) == 0)
		
		pFSMkdirAbsent("/opfs/config");
		pFSMkdirAbsent("/opfs/cache");
		pFSMkdirAbsent("/opfs/capture");
		
		pFSSymlink("/opfs/config", "/dragengine/userConfig");
		pFSSymlink("/opfs/cache", "/dragengine/userCache");
		pFSSymlink("/opfs/capture", "/dragengine/userCapture");
		
		pFSMkdirAbsent("/dragengine/userCache/delgas");
		
		pFSMkdirAbsent("/localDelgas");
		pFSSymlink("/localDelgas", "/dragengine/localDelgas");
		
		// initialize loggers
		dewlLoggerJS::AddLogEntry(dewlLoggerJS::eSeverity::info, LOGSOURCE,
			"LauncherThread: Initialize loggers");
		pInitLogger();
		
		// finished
		dewlLoggerJS::AddLogEntry(dewlLoggerJS::eSeverity::info, LOGSOURCE,
			"LauncherThread: Initialized");
		pDispatchEvent(EventInitialized);
		
	}catch(const deException &e){
		dewlLoggerJS::AddLogEntry(dewlLoggerJS::eSeverity::error,
			LOGSOURCE, e.FormatOutput().Join("/"));
		pDispatchEvent(EventInitialized, e);
		return;
	}
	
	// process commands
	sCommand command;
	
	while(true){
		// get next command
		if(pthread_mutex_lock(&pMutexLauncher) != 0){
			return;
		}
		
		if(pCommands.empty()){
			if(pthread_cond_wait(&pConditionLauncher, &pMutexLauncher) != 0){
				pthread_mutex_unlock(&pMutexLauncher);
				return;
			}
		}
		
		bool hasCommand = !pCommands.empty();
		if(hasCommand){
			command = pCommands.front();
			pCommands.erase(pCommands.begin());
		}
		
		pthread_mutex_unlock(&pMutexLauncher);
		
		emscripten_log(EM_LOG_INFO, "LauncherThread: %d %d", hasCommand, (int)command.command);
		GetLogger()->LogInfoFormat(LOGSOURCE, "LauncherThread: %d %d", hasCommand, (int)command.command);
		
		// evaluate command
		if(!hasCommand){
			continue;
		}
		
		switch(command.command){
		case Commands::RunGame:
			pCommandRunGame();
			break;
		}
	}
	
	GetLogger()->LogInfo(LOGSOURCE, "LauncherThread: Exit");
}

void dewlLauncher::pCommandRunGame(){
	try{
		pPrepare();
		pLocateGame();
		if(!pGame || !pGame->GetCanRun()){
			pDispatchEvent(EventRunGame, eResultCodes::GameProblems, "Game problems");
			return;
		}
		
		pLocateProfile();
		if(!pProfile || !pProfile->GetValid()){
			pDispatchEvent(EventRunGame, eResultCodes::ProfileProblems, "Profile problems");
			return;
		}
		
		pLocatePatches();
		if(!pPatchesValid){
			pDispatchEvent(EventRunGame, eResultCodes::PatchProblems, pPatchProblems.GetString());
			return;
		}
		
		pPrepareRunParameters();
		pStartGame();
		
	}catch(const deException &e){
		pDispatchEvent(EventRunGame, e);
	}
}

void dewlLauncher::pPrepare(){
	delLauncher::Prepare();
}

void dewlLauncher::pLocateGame(){
	delGameManager &gameManager = GetGameManager();
	deLogger &logger = *GetLogger();
	
	delGameList list;
	
	{
	const delEngineInstance::Ref instance(GetEngineInstanceFactory().
		CreateEngineInstance(*this, GetEngine().GetLogFile()));
	
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
}

void dewlLauncher::pLocateProfile(){
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
}

void dewlLauncher::pLocatePatches(){
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
	
	pPatchProblems.Empty();
	pPatchesValid = pRunParams.FindPatches(pGame,
		pGame->GetUseLatestPatch(), pPatchIdentifier, pPatchProblems);
	if(!pPatchesValid){
		dewlLoggerJS::AddLogEntry(dewlLoggerJS::eSeverity::error,
			LOGSOURCE, pPatchProblems.GetString());
	}
}

void dewlLauncher::pPrepareRunParameters(){
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
}

void dewlLauncher::pStartGame(){
	deLogger &logger = *GetLogger();
	logger.LogInfoFormat(LOGSOURCE, "Cache application ID = '%s'",
		pGame->GetIdentifier().ToHexString(false).GetString());
	logger.LogInfoFormat(LOGSOURCE, "Starting game '%s' using profile '%s'",
		pGame->GetTitle().ToUTF8().GetString(),
		pRunParams.GetGameProfile()->GetName().GetString());
	
	pGame->StartGame(pRunParams);
}

void dewlLauncher::pDispatchEvent(const std::string &event, eResultCodes resultCode, const char *error){
	if(pthread_mutex_lock(&pMutexEvents) == 0){
		pEvents.push_back({event, resultCode, error});
		pthread_mutex_unlock(&pMutexEvents);
	}
}

void dewlLauncher::pDispatchEvent(const std::string &event, const deException &exception){
	pDispatchEvent(event, eResultCodes::Exception, exception.FormatOutput().Join("\n"));
}
