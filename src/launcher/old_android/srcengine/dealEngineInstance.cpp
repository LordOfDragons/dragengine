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
#include <unistd.h>

#include "dealEngineInstance.h"
#include "android_native_app_glue.h"
#include "dealFDFileReader.h"
#include "dealVFSZipArchive.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSAndroid.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSRedirect.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/logger/deLoggerConsole.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/resources/loader/deResourceLoader.h>

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>



#ifdef __cplusplus
extern "C" {
#endif
dealIEngineInstance *CreateEngine(android_app* androidApp);
#ifdef  __cplusplus
}
#endif


// Entry Point
////////////////

dealIEngineInstance *CreateEngine(android_app *androidApp){
	if(!androidApp){
		return NULL;
	}
	
	dealEngineInstance *engine = NULL;
	
	try{
		engine = new dealEngineInstance(*androidApp);
		
	}catch(const deException &){
		return NULL;
	}
	
	return engine;
}



// Definitions
////////////////

#define LOGSOURCE "DEAndroidLauncher"



// Class dealEngineInstance
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEngineInstance::dealEngineInstance(android_app &androidApp) :
pAndroidApp(androidApp),

pLogger(NULL),

pEngineAsset(NULL),
pEngineAssetFileDescriptor(-1),
pOSFileSystem(NULL),

pOSAndroid(NULL),
pEngine(NULL),

pDelga(NULL){
}

dealEngineInstance::~dealEngineInstance(){
	pCleanUp();
}



// Management
///////////////

bool dealEngineInstance::IsRunning() const{
	return pEngine != nullptr;
}

bool dealEngineInstance::Start(const char *logfile, const char *cacheAppID){
	if(pEngine){
		pLogger->LogError(LOGSOURCE, "Engine is already running");
		return false;
	}
	
	try{
		// create logger for engine
		pCreateLogger(logfile);
		
		// create file system to access engine data out of the asset file
		//pCreateOSFileSystem();
		
		// create engine
		pOSAndroid = new deOSAndroid(*pAndroidApp.activity, *pAndroidApp.config,
			*pAndroidApp.looper, *pAndroidApp.inputQueue, *pAndroidApp.window );
		pEngine = new deEngine(pOSAndroid, pOSFileSystem);
		
		pEngine->SetLogger(pLogger);
		pEngine->SetCacheAppID(cacheAppID);
		
		pEngine->LoadModules();
		
		// android specific debug stuff
		pEngine->GetResourceLoader()->SetOutputDebugMessages(true);
		
	}catch(const deException &e){
		if(pLogger){
			pLogger->LogException(LOGSOURCE, e);
		}
		Stop();
		return false;
	}
	
	return true;
}

bool dealEngineInstance::Stop(){
	if(pEngine){
		try{
			delete pEngine;
			pEngine = NULL;
			
		}catch(const deException &e){
			if(pLogger){
				pLogger->LogException(LOGSOURCE, e);
			}
			return false;
		}
	}
	
	pCloseOSFileSystem();
	return true;
}

void dealEngineInstance::ProcessInputEvent(const AInputEvent &event){
	if(!pEngine){
		return;
	}
	
	deBaseInputModule * const module = pEngine->GetInputSystem()->GetActiveModule();
	if(!module){
		return;
	}
	
	module->EventLoop(event);
}

bool dealEngineInstance::FrameUpdate(bool &keepRunning){
	if(!pEngine){
		return false;
	}
	
	try{
		// process event loop and check for quit
//  		pLogger->LogInfo( LOGSOURCE, "FrameUpdate: process event loop" );
		pEngine->GetOS()->ProcessEventLoop(true);
		if(pEngine->GetQuitRequest()){
			keepRunning = false;
			return true;
		}
		
		// process input module events into engine events
// 		pLogger->LogInfo( LOGSOURCE, "FrameUpdate: process events" );
		pEngine->GetInputSystem()->GetActiveModule()->ProcessEvents();
		
		// render frame
		pEngine->UpdateElapsedTime();
// 		pLogger->LogInfoFormat( LOGSOURCE, "Tick, tock: %f", pEngine->GetElapsedTime() );
		
// 		pLogger->LogInfo( LOGSOURCE, "FrameUpdate: run single frame" );
		pEngine->RunSingleFrame();
		
		// check for errors
		if(pEngine->GetScriptFailed() || pEngine->GetSystemFailed()){
			DETHROW(deeInvalidAction);
		}
// 		pLogger->LogInfo( LOGSOURCE, "FrameUpdate: finished" );
		
	}catch(const deException &e){
		pLogger->LogError(LOGSOURCE, "FrameUpdate: Exception:");
		pLogger->LogException(LOGSOURCE, e);
		StopGame();
		return false;
	}
	
	return true;
}



bool dealEngineInstance::GetModuleStatus(const char *moduleName, const char *moduleVersion, int &status){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleStatus: Engine not running");
		return false;
	}
	
	const deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed(moduleName, moduleVersion);
	if(!module){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleStatus: module '%s':%s not found", moduleName, moduleVersion);
		return false;
	}
	
	status = module->GetErrorCode();
	return true;
}

bool dealEngineInstance::GetModuleParameterCount(const char *moduleName,
const char *moduleVersion, int &parameterCount){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleParameterCount: Engine not running");
		return false;
	}
	
	const deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed(moduleName, moduleVersion);
	if(!module){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleParameterCount: module '%s':%s not found", moduleName, moduleVersion);
		return false;
	}
	
	const deBaseModule * const baseModule = module->GetModule();
	if(!baseModule){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleParameterCount: module '%s':%s not created", moduleName, moduleVersion);
		return false;
	}
	
	parameterCount = baseModule->GetParameterCount();
	return true;
}

bool dealEngineInstance::GetModuleParameterData(const char *moduleName, const char *moduleVersion,
int parameter, const char *&parameterName, const char *&parameterDescription,
const char *&parameterValue){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleParameterData: Engine not running");
		return false;
	}
	
	const deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed(moduleName, moduleVersion);
	if(!module){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleParameterData: module '%s':%s not found", moduleName, moduleVersion);
		return false;
	}
	
	const deBaseModule * const baseModule = module->GetModule();
	if(!baseModule){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleParameterData: module '%s':%s not created", moduleName, moduleVersion);
		return false;
	}
	
	const int parameterCount = baseModule->GetParameterCount();
	if(parameter < 0 || parameter >= parameterCount){
		pLogger->LogErrorFormat(LOGSOURCE, "GetModuleParameterData: module '%s':%s parameter index %i out of boundary",
			moduleName, moduleVersion, parameter);
		return false;
	}
	
	baseModule->GetParameterInfo(parameter, pModuleParameter);
	
	parameterName = pModuleParameter.GetName();
	parameterDescription = pModuleParameter.GetDescription();
	parameterValue = baseModule->GetParameterValue(pModuleParameter.GetName());
	return true;
}

bool dealEngineInstance::SetModuleParameterValue(const char *moduleName,
const char *moduleVersion, const char *parameter, const char *value){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "SetModuleParameterValue: Engine not running");
		return false;
	}
	
	const deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed(moduleName, moduleVersion);
	if(!module){
		pLogger->LogErrorFormat(LOGSOURCE, "SetModuleParameterValue: module '%s':%s not found", moduleName, moduleVersion);
		return false;
	}
	
	deBaseModule * const baseModule = module->GetModule();
	if(!baseModule){
		pLogger->LogErrorFormat(LOGSOURCE, "SetModuleParameterValue: module '%s':%s not created", moduleName, moduleVersion);
		return false;
	}
	
	if(baseModule->IndexOfParameterNamed(parameter) == -1){
		pLogger->LogErrorFormat(LOGSOURCE,
			"SetModuleParameterValue: module '%s':%s parameter %s not found",
			moduleName, moduleVersion, parameter);
		return false;
	}
	
	baseModule->SetParameterValue(parameter, value);
	return true;
}

bool dealEngineInstance::GetProperty(int property, const char *&value){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "GetProperty: Engine not running");
		return false;
	}
	
	switch(property){
	case epPathEngineConfig:
		value = pEngine->GetOS()->GetPathUserConfig();
		return true;
		
	case epPathEngineShare:
		value = pEngine->GetOS()->GetPathShare();
		return true;
		
	case epPathEngineLib:
		value = pEngine->GetOS()->GetPathEngine();
		return true;
		
	case epPathEngineCache:
		value = pEngine->GetOS()->GetPathUserCache();
		return true;
		
	default:
		pLogger->LogErrorFormat(LOGSOURCE, "GetProperty: Unknown property %i", property);
		return false;
	}
}

bool dealEngineInstance::ActivateModule(const char *moduleName, const char *moduleVersion){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "ActivateModule: Engine not running");
		return false;
	}
	
	deLoadableModule *module = NULL;
	
	try{
		if(strlen(moduleVersion) == 0){
			module = pEngine->GetModuleSystem()->GetModuleNamed(moduleName);
			
		}else{
			module = pEngine->GetModuleSystem()->GetModuleNamed(moduleName, moduleVersion);
		}
		
		if(!module){
			pLogger->LogErrorFormat(LOGSOURCE, "ActivateModule: module '%s':%s not found", moduleName, moduleVersion);
			return false;
		}
		
		const int type = module->GetType();
		
		switch(type){
		case deModuleSystem::emtGraphic:
			pEngine->GetGraphicSystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtAudio:
			pEngine->GetAudioSystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtInput:
			pEngine->GetInputSystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtNetwork:
			pEngine->GetNetworkSystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtPhysics:
			pEngine->GetPhysicsSystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtAnimator:
			pEngine->GetAnimatorSystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtSynthesizer:
			pEngine->GetSynthesizerSystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtAI:
			pEngine->GetAISystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtCrashRecovery:
			pEngine->GetCrashRecoverySystem()->SetActiveModule(module);
			break;
			
		case deModuleSystem::emtScript:
			pEngine->GetScriptingSystem()->SetActiveModule(module);
			break;
			
		default:
			return false;
		}
		
	}catch(const deException &e){
		pLogger->LogErrorFormat(LOGSOURCE, "ActivateModule: failed with exception (module='%s':%s):",
			moduleName, moduleVersion);
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::EnableModule(const char *moduleName, const char *moduleVersion, bool enable){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "EnableModule: Engine not running");
		return false;
	}
	
	try{
		deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed(moduleName, moduleVersion);
		
		if(!module){
			pLogger->LogErrorFormat(LOGSOURCE, "EnableModule: module '%s':%s not found", moduleName, moduleVersion);
			return false;
		}
		
		module->SetEnabled(enable);
		
	}catch(const deException &e){
		pLogger->LogErrorFormat(LOGSOURCE, "EnableModule failed with exception (module='%s':%s):",
			moduleName, moduleVersion);
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::SetCmdLineArgs(const char *arguments) const{
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "SetCmdLineArgs: Engine not running");
		return false;
	}
	
	pEngine->GetArguments()->AddArgsSplit(arguments);
	return true;
}

bool dealEngineInstance::SetDataDirectory(const char *directory){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "SetDataDirectory: Engine not running");
		return false;
	}
	
	pEngine->SetDataDir(directory);
	return true;
}



bool dealEngineInstance::OpenDelga(int fileDescriptor, long fileOffset, long fileLength){
	if(!pEngine || fileDescriptor == 0 || fileOffset < 0 || fileLength < 0 || pDelga){
		return false;
	}
	
	pLogger->LogInfoFormat(LOGSOURCE, "Open DELGA: fd=%d offset=%ld len=%ld",
		fileDescriptor, fileOffset, fileLength);
	
	dealFDFileReader *fileReader = NULL;
	decPath pathRoot;
	pathRoot.SetFromUnix("/");
	
	try{
		fileReader = new dealFDFileReader("DELGA", fileDescriptor, fileOffset, fileLength);
		
		pDelga.TakeOverWith(*this, fileReader, pathRoot);
		fileReader->FreeReference();
		fileReader = NULL;
		
	}catch(const deException &e){
		if(fileReader){
			fileReader->FreeReference();
		}
		pLogger->LogErrorFormat(LOGSOURCE, "OpenDelga: fd=%d offset=%ld len=%ld",
			fileDescriptor, fileOffset, fileLength);
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}



bool dealEngineInstance::VFSAddDiskDir(const char *vfsRoot, const char *nativeDirectory, bool readOnly){
	if(!pEngine){
		return false;
	}
	
	pLogger->LogInfoFormat(LOGSOURCE, "Add VFS Disk Directory: root=%s disk=%s ro=%c",
		vfsRoot, nativeDirectory, readOnly ? 't' : 'n');
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	deVFSDiskDirectory *container = NULL;
	decPath pathRoot, pathDisk;
	
	try{
		pathRoot.SetFromUnix(vfsRoot);
		pathDisk.SetFromNative(nativeDirectory);
		
		container = new deVFSDiskDirectory(pathRoot, pathDisk);
		container->SetReadOnly(readOnly);
		vfs.AddContainer(container);
		container->FreeReference();
		
	}catch(const deException &e){
		if(container){
			container->FreeReference();
		}
		pLogger->LogErrorFormat(LOGSOURCE, "VFSAddDiskDir(root=%s,disk=%s,ro=%c):",
			vfsRoot, nativeDirectory, readOnly ? 't' : 'n');
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::VFSAddScriptSharedDataDir(){
	if(!pEngine){
		return false;
	}
	
	pLogger->LogInfo(LOGSOURCE, "Add VFS Script Shared Directory");
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	
	try{
		pEngine->GetScriptingSystem()->AddVFSSharedDataDir(vfs);
		
	}catch(const deException &e){
		pLogger->LogError(LOGSOURCE, "VFSAddScriptSharedDataDir:");
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::VFSAddDelga(const char *vfsRoot, const char *vfsBase){
	if(!pEngine || !pDelga || !vfsRoot || !vfsBase){
		return false;
	}
	
	pLogger->LogInfoFormat(LOGSOURCE, "Add VFS DELGA: root=%s base=%s", vfsRoot, vfsBase);
	
	deVFSRedirect *container = NULL;
	decPath pathRoot, pathBase;
	
	try{
		pathRoot.SetFromUnix(vfsRoot);
		pathBase.SetFromUnix(vfsBase);
		
		container = new deVFSRedirect(pathRoot, pathBase, pDelga);
		pEngine->GetVirtualFileSystem()->AddContainer(container);
		container->FreeReference();
		
	}catch(const deException &e){
		if(container){
			container->FreeReference();
		}
		pLogger->LogErrorFormat(LOGSOURCE, "VFSAddDelga: root=%s base=%s", vfsRoot, vfsBase);
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::VFSAddRedirect(const char *root, const char *redirect){
	if(!pEngine){
		return false;
	}
	
	pLogger->LogInfoFormat(LOGSOURCE, "Add VFS Redirect: root=%s redirect=%s", root, redirect);
	
	deVirtualFileSystem * const vfs = pEngine->GetVirtualFileSystem();
	deVFSRedirect *container = NULL;
	decPath pathRoot, pathRedirect;
	
	try{
		pathRoot.SetFromUnix(root);
		pathRedirect.SetFromNative(redirect);
		
		container = new deVFSRedirect(pathRoot, pathRedirect, vfs, false);
		vfs->AddContainer(container);
		container->FreeReference();
		
	}catch(const deException &e){
		if(container){
			container->FreeReference();
		}
		pLogger->LogErrorFormat(LOGSOURCE, "VFSAddRedirect(root=%s,redirect=%s):", root, redirect);
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::ModulesAddVFSContainers(const char *stage){
	if(!pEngine){
		return false;
	}
	
	pLogger->LogInfoFormat(LOGSOURCE, "ModulesAddVFSContainers: stage=%s", stage);
	
	try{
		deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
		pEngine->GetModuleSystem()->ServicesAddVFSContainers(vfs, stage);
		pEngine->GetScriptingSystem()->AddVFSContainers(vfs, stage);
		
	}catch(const deException &e){
		pLogger->LogErrorFormat(LOGSOURCE, "ModulesAddVFSContainers(stage=%s):", stage);
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}



bool dealEngineInstance::CreateRenderWindow(int width, int height, bool fullScreen, const char *windowTitle){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "CreateRenderWindow: Engine not running");
		return false;
	}
	
	// TODO game can request a window size with is different from our window size. in this
	//      case we need to render to the requested window size and for example up-scale
	//      to the screen resolution.
	//      
	//      for the time being though we simply ignore the values given to us and always
	//      use the full screen window size
	const decPoint screenSize(pEngine->GetOS()->GetDisplayCurrentResolution(0));
	width = screenSize.x;
	height = screenSize.y;
	fullScreen = true;
	
	try{
		pEngine->GetGraphicSystem()->CreateAndSetRenderWindow(width, height, fullScreen, windowTitle, NULL);
		
	}catch(const deException &e){
		pLogger->LogErrorFormat(LOGSOURCE, "CreateRenderWindow: Exception (width=%i,height=%i,title=%s,fullScreen=%c):",
			width, height, windowTitle, fullScreen);
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::StartGame(const char *scriptDirectory, const char *gameObject){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "StartGame: Engine not running");
		return false;
	}
	
	try{
		pLogger->LogInfo(LOGSOURCE, "StartGame: Launching game");
		const decPoint screenSize(pEngine->GetOS()->GetDisplayCurrentResolution(0));
		pLogger->LogInfoFormat(LOGSOURCE, "StartGame: Screen %ix%i.", screenSize.x, screenSize.y);
		
		pEngine->GetScriptingSystem()->SetScriptDirectory(scriptDirectory);
		pEngine->GetScriptingSystem()->SetGameObject(gameObject);
		
		// start systems
		if(!pEngine->GetCrashRecoverySystem()->CanStart()
		||  !pEngine->GetGraphicSystem()->CanStart()
		||  !pEngine->GetAudioSystem()->CanStart()
		||  !pEngine->GetPhysicsSystem()->CanStart()
		||  !pEngine->GetNetworkSystem()->CanStart()
		||  !pEngine->GetInputSystem()->CanStart()
		||  !pEngine->GetAnimatorSystem()->CanStart()
		||  !pEngine->GetSynthesizerSystem()->CanStart()
		||  !pEngine->GetAISystem()->CanStart()
		||  !pEngine->GetSynthesizerSystem()->CanStart()
		||  !pEngine->GetScriptingSystem()->CanStart()){
			pLogger->LogErrorFormat(LOGSOURCE, "StartGame: Not all systems are ready to start");
			return false;
		}
		
		pEngine->ResetFailureFlags();
		
		// start all systems
		if(!pEngine->GetCrashRecoverySystem()->GetIsRunning()){
			pEngine->GetCrashRecoverySystem()->Start();
		}
		if(!pEngine->GetGraphicSystem()->GetIsRunning()){
			pEngine->GetGraphicSystem()->Start();
		}
		if(!pEngine->GetAudioSystem()->GetIsRunning()){
			pEngine->GetAudioSystem()->Start();
		}
		if(!pEngine->GetPhysicsSystem()->GetIsRunning()){
			pEngine->GetPhysicsSystem()->Start();
		}
		if(!pEngine->GetNetworkSystem()->GetIsRunning()){
			pEngine->GetNetworkSystem()->Start();
		}
		if(!pEngine->GetInputSystem()->GetIsRunning()){
			pEngine->GetInputSystem()->Start();
		}
		if(!pEngine->GetAnimatorSystem()->GetIsRunning()){
			pEngine->GetAnimatorSystem()->Start();
		}
		if(!pEngine->GetSynthesizerSystem()->GetIsRunning()){
			pEngine->GetSynthesizerSystem()->Start();
		}
		if(!pEngine->GetAISystem()->GetIsRunning()){
			pEngine->GetAISystem()->Start();
		}
		if(!pEngine->GetSynthesizerSystem()->GetIsRunning()){
			pEngine->GetSynthesizerSystem()->Start();
		}
		if(!pEngine->GetScriptingSystem()->GetIsRunning()){
			pEngine->GetScriptingSystem()->Start();
		}
		
		// reset time
		pEngine->ResetTimers();
		
		// init the game if not done already
		pEngine->GetScriptingSystem()->InitGame();
		
		// clear all event queues
		pEngine->GetInputSystem()->ClearEventQueues();
		
	}catch(const deException &e){
		pLogger->LogErrorFormat(LOGSOURCE, "StartGame: Exception ("
			"scriptDir=%s gameObject=%s):", scriptDirectory, gameObject);
		pLogger->LogException(LOGSOURCE, e);
		StopGame();
		return false;
	}
	
	return true;
}

bool dealEngineInstance::StopGame(){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "StopGame: Engine not running");
		return false;
	}
	
	try{
		// exit the game if it is running
		pEngine->GetScriptingSystem()->ExitGame();
		
	}catch(const deException &e){
		pLogger->LogError(LOGSOURCE, "StopGame: Exception:");
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

int dealEngineInstance::IsGameRunning(){
	if(!pEngine){
		pLogger->LogErrorFormat(LOGSOURCE, "IsGameRunning: Engine not running");
		return 0;
	}
	
	return 0;
}



bool dealEngineInstance::FocusGained(){
	if(!pEngine){
		return true;
	}
	
	pLogger->LogInfo(LOGSOURCE, "FocusGained");
	pEngine->GetOS()->SetAppActive(true);
	
	return true;
}

bool dealEngineInstance::FocusLost(){
	if(!pEngine){
		return true;
	}
	
	pLogger->LogInfo(LOGSOURCE, "FocusLost");
	pEngine->GetOS()->SetAppActive(false);
	
	return true;
}

bool dealEngineInstance::Freeze(){
	if(!pEngine){
		return true;
	}
	
	deOSAndroid &os = *pEngine->GetOS()->CastToOSAndroid();
	if(os.GetAppFrozen()){
		return true;
	}
	
	pLogger->LogInfo(LOGSOURCE, "Freeze");
	os.SetAppFrozen(true);
	
	//pEngine->GetParallelProcessing().
	
	return true;
}

bool dealEngineInstance::Thaw(){
	if(!pEngine){
		return true;
	}
	
	deOSAndroid &os = *pEngine->GetOS()->CastToOSAndroid();
	if(os.GetAppFrozen()){
		return true;
	}
	
	pLogger->LogInfo(LOGSOURCE, "Thaw");
	os.SetAppFrozen(false);
	
	//pEngine->GetParallelProcessing().
	
	return true;
}

bool dealEngineInstance::InitAppWindow(){
	if(!pEngine){
		return true;
	}
	
	pLogger->LogInfo(LOGSOURCE, "InitAppWindow");
	
	try{
		pEngine->GetGraphicSystem()->InitAppWindow();
		
	}catch(const deException &e){
		pLogger->LogError(LOGSOURCE, "InitAppWindow: Exception:");
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}

bool dealEngineInstance::TerminateAppWindow(){
	if(!pEngine){
		return true;
	}
	
	pLogger->LogInfo(LOGSOURCE, "TerminateAppWindow");
	
	try{
		pEngine->GetGraphicSystem()->TerminateAppWindow();
		
	}catch(const deException &e){
		pLogger->LogError(LOGSOURCE, "TerminateAppWindow: Exception:");
		pLogger->LogException(LOGSOURCE, e);
		return false;
	}
	
	return true;
}



// Private Functions
//////////////////////

void dealEngineInstance::pCleanUp(){
	Stop();
	
	
}



void dealEngineInstance::pCreateLogger(const char *logfile){
	if(strlen(logfile) == 0){
		pLogger = new deLoggerConsole;
		
	}else{
		decBaseFileWriter *fileWriter = NULL;
		decPath diskPath;
		decPath filePath;
		
		diskPath.SetFromNative(logfile);
		filePath.SetFromUnix(diskPath.GetLastComponent());
		diskPath.RemoveLastComponent();
		
		deVFSDiskDirectory *diskDir = NULL;
		
		try{
			diskDir = new deVFSDiskDirectory(diskPath);
			
			if(diskDir->ExistsFile(filePath)){
				fileWriter = new decDiskFileWriter(logfile, false);
				
			}else{
				fileWriter = diskDir->OpenFileForWriting(filePath);
			}
			
			pLogger.TakeOver(new deLoggerFile(fileWriter));
			fileWriter->FreeReference();
			
			diskDir->FreeReference();
			
		}catch(const deException &){
			if(fileWriter){
				fileWriter->FreeReference();
			}
			if(diskDir){
				diskDir->FreeReference();
			}
			throw;
		}
	}
}

void dealEngineInstance::pCreateOSFileSystem(){
	if(pOSFileSystem || pEngineAssetFileDescriptor != -1 || pEngineAsset){
		DETHROW(deeInvalidAction);
	}
	
	decString filename;
	off_t fileOffset, fileLength;
	filename.Format("install_%s.zip", ANDROID_JNIDIR);
	deVFSRedirect *containerRedirect = NULL;
	dealVFSZipArchive *zipArchive = NULL;
	dealFDFileReader *fileReader = NULL;
	decPath pathRoot, pathRedirect;
	
	try{
		// open asset file. use random access to read the zip file. the asset is kept open for
		// the entire lifetime of the virtual file system. for reading a file descriptor is used
		pEngineAsset = AAssetManager_open(pAndroidApp.activity->assetManager, filename, AASSET_MODE_RANDOM);
		if(!pEngineAsset){
			DETHROW(deeOpenFile);
		}
		
		pEngineAssetFileDescriptor = AAsset_openFileDescriptor(pEngineAsset, &fileOffset, &fileLength);
		if(pEngineAssetFileDescriptor == -1){
			DETHROW(deeOpenFile);
		}
		
		// create zip archive container using the asset file descriptor
		pathRoot.SetFromUnix("/");
		fileReader = new dealFDFileReader("Dragengine", pEngineAssetFileDescriptor, fileOffset, fileLength);
		zipArchive = new dealVFSZipArchive(*this, fileReader, pathRoot);
		fileReader->FreeReference();
		fileReader = NULL;
		
		// create virtual file system and add containers redirecting to zip archive.
		pOSFileSystem = new deVirtualFileSystem;
		
		// /engine : read-only container with engine module libraries and definitions
		pathRoot.SetFromUnix("/engine");
		pathRedirect.SetFromUnix("/");
		pathRedirect.AddComponent(ANDROID_JNIDIR);
		pathRedirect.AddComponent("lib");
		pathRedirect.AddComponent("dragengine");
		
		containerRedirect = new deVFSRedirect(pathRoot, pathRedirect, zipArchive);
		pOSFileSystem->AddContainer(containerRedirect);
		containerRedirect->FreeReference();
		containerRedirect = NULL;
		
		// /share : read-only container with shared engine and module files
		pathRoot.SetFromUnix("/share");
		pathRedirect.SetFromUnix("/");
		pathRedirect.AddComponent(ANDROID_JNIDIR);
		pathRedirect.AddComponent("share");
		pathRedirect.AddComponent("dragengine");
		
		containerRedirect = new deVFSRedirect(pathRoot, pathRedirect, zipArchive);
		pOSFileSystem->AddContainer(containerRedirect);
		containerRedirect->FreeReference();
		containerRedirect = NULL;
		
		// /config : read-write container with engine and module config files.
		// TODO add container for writing support. right now no module uses writing so this is ignored
		pathRoot.SetFromUnix("/config");
		pathRedirect.SetFromUnix("/");
		pathRedirect.AddComponent(ANDROID_JNIDIR);
		pathRedirect.AddComponent("config");
		pathRedirect.AddComponent("dragengine");
		
		containerRedirect = new deVFSRedirect(pathRoot, pathRedirect, zipArchive);
		pOSFileSystem->AddContainer(containerRedirect);
		containerRedirect->FreeReference();
		containerRedirect = NULL;
		
	}catch(const deException &){
		if(containerRedirect){
			containerRedirect->FreeReference();
		}
		if(zipArchive){
			zipArchive->FreeReference();
		}
		if(fileReader){
			fileReader->FreeReference();
		}
		pCloseOSFileSystem();
		throw;
	}
}

void dealEngineInstance::pCloseOSFileSystem(){
		pOSFileSystem = NULL;
	}
	
	if(pEngineAssetFileDescriptor != -1){
		close(pEngineAssetFileDescriptor);
		pEngineAssetFileDescriptor = -1;
	}
	
	if(pEngineAsset){
		AAsset_close(pEngineAsset);
		pEngineAsset = NULL;
	}
}
