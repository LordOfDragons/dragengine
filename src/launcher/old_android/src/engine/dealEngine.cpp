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
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <dlfcn.h>

#include "alconfig.h"
#include "dealEngine.h"
#include "dealEngineConfigXML.h"
#include "modules/dealEngineModule.h"
#include "modules/dealEngineModuleXML.h"
#include "modules/parameter/dealEMParameter.h"
#include "../dealLauncher.h"
#include "../config/dealConfiguration.h"
#include "../../srcengine/dealIEngineInstance.h"

#include "../logger/deLogger.h"
#include "../logger/deLoggerConsole.h"
#include "../filesystem/dePathList.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../filesystem/deVFSContainer.h"
#include "../filesystem/deVFSDiskDirectory.h"
#include "../filesystem/deCollectDirectorySearchVisitor.h"
#include "../common/file/decPath.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/file/decDiskFileWriter.h"
#include "../common/string/decString.h"
#include "../common/string/decStringList.h"
#include "../common/exceptions.h"



// Definitions
////////////////

#define LOGSOURCE "DEAndroidLauncher"



// Class dealEngine
/////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEngine::dealEngine(dealLauncher &launcher) :
pLauncher(launcher),
pLogFile("delauncherandroid-engine.log"),

pHandleLibDragengine(NULL),
pHandleLibEngine(NULL),
pEngine(NULL){
}

dealEngine::~dealEngine(){
	Stop();
}



// Management
///////////////

void dealEngine::SetPathConfig(const char *path){
	pPathConfig = path;
}

void dealEngine::SetPathShare(const char *path){
	pPathShare = path;
}

void dealEngine::SetPathLib(const char *path){
	pPathLib = path;
}

void dealEngine::SetPathCache(const char *path){
	pPathCache = path;
}

void dealEngine::SetLogFile(const char *path){
	pLogFile = path;
}



void dealEngine::Start(const char *cacheAppID){
	if(pEngine || ! cacheAppID){
		return;
	}
	
	// load libdragengine.so if not loaded yet
	decString pathLib;
	pathLib.Format("%s/%s/lib", pLauncher.GetInternalPath(), ANDROID_JNIDIR);
	
	if(! pHandleLibDragengine){
		decString pathLibDragengine(pathLib + "/libdragengine.so");
		pHandleLibDragengine = dlopen(pathLibDragengine, RTLD_NOW);
		
		if(! pHandleLibDragengine){
			pLauncher.GetLogger().LogErrorFormat(LOGSOURCE, "libdragengine.so dlerror: %s.", dlerror());
			DETHROW(deeInvalidAction);
		}
		
		pLauncher.GetLogger().LogInfo(LOGSOURCE, "Loaded libdragengine.so");
	}
	
	// load libdelauncher_engine.so if not loaded
	if(! pHandleLibEngine){
		decString pathLibEngine(pathLib + "/libdelauncher_engine.so");
		pHandleLibEngine = dlopen(pathLibEngine, RTLD_NOW);
		
		if(! pHandleLibEngine){
			pLauncher.GetLogger().LogErrorFormat(LOGSOURCE, "libdelauncher_engine.so dlerror: %s.", dlerror());
			DETHROW(deeInvalidAction);
		}
		
		pLauncher.GetLogger().LogInfo(LOGSOURCE, "Loaded libdelauncher_engine.so");
	}
	
	// create engine
	typedef dealIEngineInstance* (*FUNC_CREATEENGINE)(android_app*);
	FUNC_CREATEENGINE funcCreateModule = (FUNC_CREATEENGINE)dlsym(pHandleLibEngine, "CreateEngine");
	
	if(! funcCreateModule){
		pLauncher.GetLogger().LogError(LOGSOURCE, "Library function CreateEngine not found");
		DETHROW(deeInvalidAction);
	}
	
	pEngine = funcCreateModule(&pLauncher.GetAndroidApp());
	if(! pEngine){
		DETHROW(deeInvalidAction);
	}
	
	pLauncher.GetLogger().LogInfo(LOGSOURCE, "Engine created");
	
	// start engine
	decPath logfile;
	logfile.SetFromNative(pLauncher.GetConfiguration().GetPathLogs());
	logfile.AddUnixPath(pLogFile);
	
	if(! pEngine->Start(logfile.GetPathNative(), cacheAppID)){
		DETHROW(deeInvalidAction);
	}
	pLauncher.GetLogger().LogInfo(LOGSOURCE, "Engine started");
}

void dealEngine::Stop(){
	if(pEngine){
		delete pEngine;
		pEngine = NULL;
	}
	
	if(pHandleLibEngine){
		dlclose(pHandleLibEngine);
		pHandleLibEngine = NULL;
	}
	
	if(pHandleLibDragengine){
		dlclose(pHandleLibDragengine);
		pHandleLibDragengine = NULL;
	}
}

void dealEngine::ProcessInputEvent(const AInputEvent &event){
	if(! pEngine){
		return;
	}
	pEngine->ProcessInputEvent(event);
}



void dealEngine::LoadModuleList(){
	pModuleList.RemoveAllModules();
	
	pLauncher.GetLogger().LogInfo(LOGSOURCE, "loading module list");
	AddModulesFrom("/engine/lib/modules/crashrecovery", dealEngineModule::emtCrashRecovery);
	AddModulesFrom("/engine/lib/modules/graphic", dealEngineModule::emtGraphic);
	AddModulesFrom("/engine/lib/modules/input", dealEngineModule::emtInput);
	AddModulesFrom("/engine/lib/modules/physics", dealEngineModule::emtPhysics);
	AddModulesFrom("/engine/lib/modules/audio", dealEngineModule::emtAudio);
	AddModulesFrom("/engine/lib/modules/network", dealEngineModule::emtNetwork);
	AddModulesFrom("/engine/lib/modules/scripting", dealEngineModule::emtScript);
	AddModulesFrom("/engine/lib/modules/animator", dealEngineModule::emtAnimator);
	AddModulesFrom("/engine/lib/modules/synthesizer", dealEngineModule::emtSynthesizer);
	AddModulesFrom("/engine/lib/modules/ai", dealEngineModule::emtAI);
	
	AddModulesFrom("/engine/lib/modules/animation", dealEngineModule::emtAnimation);
	AddModulesFrom("/engine/lib/modules/font", dealEngineModule::emtFont);
	AddModulesFrom("/engine/lib/modules/image", dealEngineModule::emtImage);
	AddModulesFrom("/engine/lib/modules/model", dealEngineModule::emtModel);
	AddModulesFrom("/engine/lib/modules/rig", dealEngineModule::emtRig);
	AddModulesFrom("/engine/lib/modules/skin", dealEngineModule::emtSkin);
	AddModulesFrom("/engine/lib/modules/langpack", dealEngineModule::emtLanguagePack);
	AddModulesFrom("/engine/lib/modules/sound", dealEngineModule::emtSound);
	AddModulesFrom("/engine/lib/modules/video", dealEngineModule::emtVideo);
	AddModulesFrom("/engine/lib/modules/occlusionmesh", dealEngineModule::emtOcclusionMesh);
	
	CheckModules();
}

void dealEngine::CheckModules(){
	int i, count = pModuleList.GetModuleCount();
	dealEMParameter *parameter = NULL;
	int parameterCount;
	decPath logfile;
	int status;
	int j;
	
	for(i=0; i<count; i++){
		dealEngineModule &module = *pModuleList.GetModuleAt(i);
		dealEMParameterList &parameterList = module.GetParameterList();
		
		try{
			// get module status
			if(! pEngine->GetModuleStatus(module.GetName(), module.GetVersion(), status)){
				DETHROW(deeInvalidParam);
			}
			
			module.SetErrorCode(status);
			
			if(module.GetErrorCode() == dealEngineModule::eecSuccess){
				module.SetStatus(dealEngineModule::emsReady);
				
			}else{
				module.SetStatus(dealEngineModule::emsBroken);
				continue;
			}
			
			// get module parameters
			if(! pEngine->GetModuleParameterCount(module.GetName(), module.GetVersion(), parameterCount)){
				DETHROW(deeInvalidParam);
			}
			
			for(j=0; j<parameterCount; j++){
				const char *name;
				const char *desc;
				const char *value;
				
				if(! pEngine->GetModuleParameterData(module.GetName(),
						module.GetVersion(), j, name, desc, value)){
					DETHROW(deeInvalidParam);
				}
				
				parameter = new dealEMParameter;
				parameter->SetIndex(j);
				parameter->SetName(name);
				parameter->SetDescription(desc);
				parameter->SetValue(value);
				
				parameterList.AddParameter(parameter);
				parameter->FreeReference();
			}
			
		}catch(const deException &e){
			if(parameter){
				parameter->FreeReference();
				parameter = NULL;
			}
			
			pLauncher.GetLogger().LogErrorFormat(LOGSOURCE,
				"Engine.CheckModules failed with exception (module=%s)",
				module.GetName().GetString());
			pLauncher.GetLogger().LogException(LOGSOURCE, e);
			module.SetStatus(dealEngineModule::emsBroken);
		}
	}
}

void dealEngine::AddModulesFrom(const char *directory, int type){
	dealEngineModuleXML moduleXML(&pLauncher.GetLogger(), LOGSOURCE);
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
	decBaseFileReader *reader;
	dealEngineModule *module;
	decPath pattern;
	int i, j;
	
	deCollectDirectorySearchVisitor collect;
	vfs.SearchFiles(decPath::CreatePathUnix(directory), collect);
	
	const dePathList &moduleDirs = collect.GetDirectories();
	const int count = moduleDirs.GetCount();
	for(i=0; i<count; i++){
		deCollectDirectorySearchVisitor collect2;
		vfs.SearchFiles(moduleDirs.GetAt(i), collect2);
		
		const dePathList &versionDirs = collect2.GetDirectories();
		const int count2 = versionDirs.GetCount();
		for(j=0; j<count2; j++){
			const decPath &versionDir = versionDirs.GetAt(j);
			
			pattern.SetFrom(versionDir);
			pattern.AddComponent("module.xml");
			
			if(! vfs.ExistsFile(pattern) || vfs.GetFileType(pattern) != deVFSContainer::eftRegularFile){
				continue;
			}
			
			logger.LogInfoFormat(LOGSOURCE, "Reading module definition from '%s'", pattern.GetPathUnix().GetString());
			reader = NULL;
			module = NULL;
			
			try{
				reader = vfs.OpenFileForReading(pattern);
				
				module = new dealEngineModule;
				
				moduleXML.ReadFromFile(*reader, *module);
				reader->FreeReference();
				reader = NULL;
				
				pModuleList.AddModule(module);
				module->FreeReference();
				
			}catch(const deException &e){
				logger.LogErrorFormat(LOGSOURCE, "Engine.AddModulesFrom failed reading module file with exception (dir=%s,type=%i)", directory, type);
				logger.LogException(LOGSOURCE, e);
				if(module){
					module->FreeReference();
				}
				if(reader){
					reader->FreeReference();
				}
			}
		}
	}
}

dealEngineModule *dealEngine::GetBestModuleForType(int moduleType){
	int i, count = pModuleList.GetModuleCount();
	dealEngineModule *bestModule = NULL;
	dealEngineModule *module;
	
	// for the time being we simply pick the first module which matches the type and is ready
	// to be used. later on this has to be improved to use a matching metrics which tells
	// how well a module matches a given set of feature requirements.
	for(i=0; i<count; i++){
		module = pModuleList.GetModuleAt(i);
		
		if(module->GetType() == moduleType && module->GetStatus() == dealEngineModule::emsReady){
			// non-fallback > fallback > none
			if(module->GetIsFallback()){
				if(! bestModule){
					bestModule = module;
				}
				
			}else{
				if(! bestModule || bestModule->GetIsFallback()){
					bestModule = module;
				}
			}
		}
	}
	
	return bestModule;
}



void dealEngine::PutEngineIntoVFS(){
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
	deVFSDiskDirectory *diskDir = NULL;
	decPath pathRootDir, pathDiskDir;
	const char *value;
	decPath logfile;
	
	try{
		// get the properties from the engine
		if(! pEngine->GetProperty(dealIEngineInstance::epPathEngineConfig, value)){
			DETHROW(deeInvalidParam);
		}
		pPathConfig = value;
		
		if(! pEngine->GetProperty(dealIEngineInstance::epPathEngineShare, value)){
			DETHROW(deeInvalidParam);
		}
		pPathShare = value;
		
		if(! pEngine->GetProperty(dealIEngineInstance::epPathEngineLib, value)){
			DETHROW(deeInvalidParam);
		}
		pPathLib = value;
		
		if(! pEngine->GetProperty(dealIEngineInstance::epPathEngineCache, value)){
			DETHROW(deeInvalidParam);
		}
		pPathCache = value;
		
		// log information
		logger.LogInfoFormat(LOGSOURCE, "Engine config path = '%s'", pPathConfig.GetString());
		logger.LogInfoFormat(LOGSOURCE, "Engine share path = '%s'", pPathShare.GetString());
		logger.LogInfoFormat(LOGSOURCE, "Engine lib path = '%s'", pPathLib.GetString());
		logger.LogInfoFormat(LOGSOURCE, "Engine cache path = '%s'", pPathCache.GetString());
		
		// add the directories so they can be easily used later on.
		if(! pPathConfig.IsEmpty()){
			pathRootDir.SetFromUnix("/engine/config");
			pathDiskDir.SetFromNative(pPathConfig.GetString());
			diskDir = new deVFSDiskDirectory(pathRootDir, pathDiskDir);
			diskDir->SetReadOnly(false);
			vfs.AddContainer(diskDir);
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if(! pPathShare.IsEmpty()){
			pathRootDir.SetFromUnix("/engine/share");
			pathDiskDir.SetFromNative(pPathShare.GetString());
			diskDir = new deVFSDiskDirectory(pathRootDir, pathDiskDir);
			diskDir->SetReadOnly(false);
			vfs.AddContainer(diskDir);
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if(! pPathLib.IsEmpty()){
			pathRootDir.SetFromUnix("/engine/lib");
			pathDiskDir.SetFromNative(pPathLib.GetString());
			diskDir = new deVFSDiskDirectory(pathRootDir, pathDiskDir);
			diskDir->SetReadOnly(false);
			vfs.AddContainer(diskDir);
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if(! pPathCache.IsEmpty()){
			pathRootDir.SetFromUnix("/engine/cache");
			pathDiskDir.SetFromNative(pPathCache);
			diskDir = new deVFSDiskDirectory(pathRootDir, pathDiskDir);
			diskDir->SetReadOnly(false);
			vfs.AddContainer(diskDir);
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
	}catch(const deException &e){
		logger.LogError(LOGSOURCE, "Engine.PutEngineIntoVFS failed with exception:");
		logger.LogException(LOGSOURCE, e);
		if(diskDir){
			diskDir->FreeReference();
		}
		throw;
	}
}



const char *dealEngine::GetModuleTypeText(int moduleType) const{
	switch(moduleType){
	case dealEngineModule::emtAI:
		return "AI";
		
	case dealEngineModule::emtAnimation:
		return "Animation";
		
	case dealEngineModule::emtAnimator:
		return "Animator";
		
	case dealEngineModule::emtAudio:
		return "Audio";
		
	case dealEngineModule::emtCrashRecovery:
		return "Crash Recovery";
		
	case dealEngineModule::emtFont:
		return "Font";
		
	case dealEngineModule::emtGraphic:
		return "Graphic";
		
	case dealEngineModule::emtImage:
		return "Image";
		
	case dealEngineModule::emtInput:
		return "Input";
		
	case dealEngineModule::emtLanguagePack:
		return "Language Pack";
		
	case dealEngineModule::emtModel:
		return "Model";
		
	case dealEngineModule::emtNetwork:
		return "Network";
		
	case dealEngineModule::emtPhysics:
		return "Physics";
		
	case dealEngineModule::emtRig:
		return "Rig";
		
	case dealEngineModule::emtScript:
		return "Script";
		
	case dealEngineModule::emtSkin:
		return "Skin";
		
	case dealEngineModule::emtSound:
		return "Sound";
		
	case dealEngineModule::emtVideo:
		return "Video";
		
	case dealEngineModule::emtOcclusionMesh:
		return "Occlusion Mesh";
		
	case dealEngineModule::emtSynthesizer:
		return "Synthesizer";
		
	default:
		return "Unknown";
	}
}



void dealEngine::LoadConfig(){
	dealEngineConfigXML configXML(&pLauncher.GetLogger(), LOGSOURCE);
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
	decBaseFileReader *reader = NULL;
	decString filename;
	decPath pathFile;
	
	pathFile.SetFromUnix("/config/user/engine.xml");
	
	if(vfs.ExistsFile(pathFile)){
		if(vfs.GetFileType(pathFile) == deVFSContainer::eftRegularFile){
			logger.LogInfo(LOGSOURCE, "Reading engine configuration file");
			reader = NULL;
			
			try{
				reader = vfs.OpenFileForReading(pathFile);
				configXML.ReadFromFile(*reader, pLauncher);
				reader->FreeReference();
				
			}catch(const deException &){
				if(reader){
					reader->FreeReference();
				}
				throw;
			}
			
		}else{
			logger.LogErrorFormat(LOGSOURCE, "Engine configuration file is not a regular file");
			DETHROW(deeInvalidParam);
		}
		
	}else{
		logger.LogInfo(LOGSOURCE, "Engine configuration file not found, will be created upon exiting");
	}
}

void dealEngine::SaveConfig(){
	dealEngineConfigXML configXML(&pLauncher.GetLogger(), LOGSOURCE);
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
	decBaseFileWriter *writer = NULL;
	decString filename;
	decPath pathFile;
	
	pathFile.SetFromUnix("/config/user/engine.xml");
	
	if(vfs.CanWriteFile(pathFile)){
		logger.LogInfo(LOGSOURCE, "Writing engine configuration file");
		
		try{
			writer = vfs.OpenFileForWriting(pathFile);
			configXML.WriteToFile(*writer, pLauncher);
			writer->FreeReference();
			
		}catch(const deException &e){
			if(writer){
				writer->FreeReference();
			}
			logger.LogErrorFormat(LOGSOURCE, "Failed to write engine configuration file (file permission problem)");
			logger.LogException(LOGSOURCE, e);
			// DIALOG BOX
			// "Engine configuration can not be written!\n"
			// "Make sure you have write permission for the file and parent directory.\n"
			// "Engine configuration has not been saved!\n"
		}
		
	}else{
		logger.LogErrorFormat(LOGSOURCE, "Failed to write engine configuration file (writing file problem)");
		// DIALOG BOX
		// "Engine configuration can not be written!\n"
		// "Make sure you have write permission for the file and parent directory.\n"
		// "Engine configuration has not been saved!\n"
	}
}



void dealEngine::FocusGained(){
	if(! pEngine){
		return;
	}
	if(! pEngine->FocusGained()){
		DETHROW(deeInvalidAction);
	}
}

void dealEngine::FocusLost(){
	if(! pEngine){
		return;
	}
	if(! pEngine->FocusLost()){
		DETHROW(deeInvalidAction);
	}
}

void dealEngine::Freeze(){
	if(! pEngine){
		return;
	}
	if(! pEngine->Freeze()){
		DETHROW(deeInvalidAction);
	}
}

void dealEngine::Thaw(){
	if(! pEngine){
		return;
	}
	if(! pEngine->Thaw()){
		DETHROW(deeInvalidAction);
	}
}

void dealEngine::InitAppWindow(){
	if(! pEngine){
		return;
	}
	if(! pEngine->InitAppWindow()){
		DETHROW(deeInvalidAction);
	}
}

void dealEngine::TerminateAppWindow(){
	if(! pEngine){
		return;
	}
	if(! pEngine->TerminateAppWindow()){
		DETHROW(deeInvalidAction);
	}
}
