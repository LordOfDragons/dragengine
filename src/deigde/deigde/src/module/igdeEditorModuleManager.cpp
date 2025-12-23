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

#include "igdeEditorModuleManager.h"
#include "../configuration/igdeConfiguration.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../gui/igdeWindowMain.h"
#include "../module/igdeEditorModuleDefinition.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeEditorWindow.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"



// Class igdeEditorModuleManager
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditorModuleManager::igdeEditorModuleManager(igdeWindowMain &windowMain) :
pWindowMain(windowMain),
pActiveModule(nullptr)
{
	try{
		decPath pathModules(decPath::CreatePathNative(windowMain.GetConfiguration().GetPathLib()));
		pathModules.AddComponent("modules");
		
		pPathModules = pathModules.GetPathNative();
		
		pScanForModules();
		
	}catch(const deException &e){
		windowMain.GetLogger()->LogException(LOGSOURCE, e);
		pCleanUp();
		throw;
	}
}

igdeEditorModuleManager::~igdeEditorModuleManager(){
	pCleanUp();
}



// Management
///////////////

void igdeEditorModuleManager::SetActiveModule(igdeEditorModuleDefinition *module){
	if(module == pActiveModule){
		return;
	}
	
	if(pActiveModule && pActiveModule->IsModuleRunning()){
		pActiveModule->GetModule()->OnDeactivate();
	}
	
	pActiveModule = module;
	
	pWindowMain.SwitchToModuleWindow();
	
	if(module){
		if(pRecentlyUsed.First() != module){
			ChangeModuleRecentUsedPosition(module, 0);
			pWindowMain.GetConfigurationLocal().SaveConfiguration();
		}
		
		if(module->IsModuleRunning()){
			module->GetModule()->OnActivate();
		}
	}
	
	pWindowMain.RebuildMenu();
	pWindowMain.RebuildToolBars();
}

void igdeEditorModuleManager::ActivateMostRecentModule(){
	igdeEditorModuleDefinition * const module = pRecentlyUsed.FindOrDefault([](const igdeEditorModuleDefinition *m){
		return m->IsModuleRunning();
	});
	if(module){
		SetActiveModule(module);
	}
}

void igdeEditorModuleManager::ActivateProjectManager(){
	igdeEditorModuleDefinition * const module = GetModuleWithID("project");
	if(module){
		SetActiveModule(module);
	}
}



// Modules
////////////

igdeEditorModuleDefinition *igdeEditorModuleManager::GetModuleWithID(const char *id) const{
	for(const auto &module : pModules){
		if(module->GetID() == id){
			return module;
		}
	};
	return nullptr;
}

void igdeEditorModuleManager::RemoveAllModules(){
	SetActiveModule(nullptr);
	
	pRecentlyUsed.RemoveAll();
	
	pModules.VisitReverse([&](igdeEditorModuleDefinition &module){
		if(module.IsModuleRunning()){
			module.UnloadModule();
		}
	});
	pModules.RemoveAll();
}

void igdeEditorModuleManager::StartModules(){
	deLogger &logger = *pWindowMain.GetLogger();
	
	pModules.Visit([&](igdeEditorModuleDefinition &module){
		if(!module.CanLoad() || module.IsModuleRunning()){
			return;
		}
		
		logger.LogInfoFormat(LOGSOURCE, "Starting editor module %s.", module.GetID().GetString());
		if(module.LoadModule(&pWindowMain.GetEnvironment())){
			return;
		}
		
		switch(module.GetErrorCode()){
		case igdeEditorModuleDefinition::eecCreateModuleFailed:
			logger.LogError(LOGSOURCE, "  Creation of module failed.");
			break;
			
		case igdeEditorModuleDefinition::eecLibFileNotFound:
			logger.LogErrorFormat(LOGSOURCE, "  Library %s not found.", module.GetLibraryPath().GetString());
			break;
			
		case igdeEditorModuleDefinition::eecLibFileNotRegularFile:
			logger.LogError(LOGSOURCE, "  Library is not a regular file.");
			break;
			
		case igdeEditorModuleDefinition::eecLibFileSizeMismatch:
			logger.LogError(LOGSOURCE, "  Library file size check failed.");
			break;
			
		case igdeEditorModuleDefinition::eecLibFileCheckSumMismatch:
			logger.LogError(LOGSOURCE, "  Library file checksum failed.");
			break;
			
		case igdeEditorModuleDefinition::eecLibFileOpenFailed:
			logger.LogError(LOGSOURCE, "  Library could not be opened.");
			break;
			
		case igdeEditorModuleDefinition::eecLibFileEntryPointNotFound:
			logger.LogError(LOGSOURCE, "  Library entry point not found.");
			break;
			
		default:
			logger.LogErrorFormat(LOGSOURCE, "  ?? (%d)", module.GetErrorCode());
		}
	});
	
	pActiveModule = nullptr;
	ActivateMostRecentModule();
}

void igdeEditorModuleManager::StopModules(){
	deLogger &logger = *pWindowMain.GetLogger();
	
	SetActiveModule(nullptr);
	
	pModules.Visit([&](igdeEditorModuleDefinition &module){
		if(!module.IsModuleRunning()){
			return;
		}
		
		logger.LogInfoFormat(LOGSOURCE, "Stopping editor module %s.", module.GetID().GetString());
		try{
			module.UnloadModule();
			
		}catch(const deException &e){
			logger.LogException(LOGSOURCE, e);
		}
	});
}

void igdeEditorModuleManager::ChangeModuleRecentUsedPosition(igdeEditorModuleDefinition *module, int position){
	try{
		pRecentlyUsed.Move(module, position);
	}catch(...){}
	
#ifdef OS_W32
	int i;
	const int maxCountBeforeSaveResHandles = 6;
	for(i=maxCountBeforeSaveResHandles; i<pRecentlyUsed.GetCount(); i++){
		module = (igdeEditorModuleDefinition*)pRecentlyUsed.GetAt(i);
		if(module->IsModuleRunning()){
			module->GetModule()->GetEditorWindow()->SaveResourceHandlers();
		}
	}
#endif
}

void igdeEditorModuleManager::ResetRecentUsedPosition(){
	const int count = pModules.GetCount();
	int i;
	
	pRecentlyUsed.RemoveAll();
	
	for(i=0; i<count; i++){
		pRecentlyUsed.Add((igdeEditorModuleDefinition*)pModules.GetAt(i));
	}
}



// Private Functions
//////////////////////

void igdeEditorModuleManager::pCleanUp(){
	RemoveAllModules();
}

void igdeEditorModuleManager::pScanForModules(){
	deLogger &logger = *pWindowMain.GetLogger();
	int i;
	
	try{
		deVirtualFileSystem::Ref vfs(deVirtualFileSystem::Ref::New());
		
		const decPath searchPath(decPath::CreatePathNative(pPathModules));
		vfs->AddContainer(deVFSDiskDirectory::Ref::New(searchPath));
		
		deCollectDirectorySearchVisitor collectDirectories;
		vfs->SearchFiles(decPath::CreatePathUnix("/"), collectDirectories);
		
		const dePathList &pathList = collectDirectories.GetDirectories();
		const int count = pathList.GetCount();
		for(i=0; i<count; i++){
			// create path for module definition file
			decPath modulePath(pathList.GetAt(i));
			modulePath.AddUnixPath("module.xml");
			
			// test if module file exists and is a regular file
			if(!vfs->ExistsFile(modulePath)
			|| vfs->GetFileType(modulePath) != deVFSContainer::eftRegularFile){
				continue;
			}
			logger.LogInfoFormat(LOGSOURCE, "Loading module %s",
				pathList.GetAt(i).GetLastComponent().GetString());
			
			// create native path for module definition file
			modulePath.SetFrom(searchPath);
			modulePath.Add(pathList.GetAt(i));
			modulePath.AddUnixPath("module.xml");
			
			// try loading module
			const igdeEditorModuleDefinition::Ref module(
				igdeEditorModuleDefinition::Ref::New(*this, modulePath.GetPathNative()));
			
			if(module->GetErrorCode() != igdeEditorModuleDefinition::eecSuccess){
				switch(module->GetErrorCode()){
				case igdeEditorModuleDefinition::eecCreateModuleFailed:
					logger.LogError(LOGSOURCE, "  Creation of module failed.");
					break;
					
				case igdeEditorModuleDefinition::eecLibFileNotFound:
					logger.LogErrorFormat(LOGSOURCE, "  Library %s not found.",
						module->GetLibraryPath().GetString());
					break;
					
				case igdeEditorModuleDefinition::eecLibFileNotRegularFile:
					logger.LogError(LOGSOURCE, "  Library is not a regular file.");
					break;
					
				case igdeEditorModuleDefinition::eecLibFileSizeMismatch:
					logger.LogError(LOGSOURCE, "  Library file size check failed.");
					break;
					
				case igdeEditorModuleDefinition::eecLibFileCheckSumMismatch:
					logger.LogError(LOGSOURCE, "  Library file checksum failed.");
					break;
					
				case igdeEditorModuleDefinition::eecLibFileOpenFailed:
					logger.LogError(LOGSOURCE, "  Library could not be opened.");
					break;
					
				case igdeEditorModuleDefinition::eecLibFileEntryPointNotFound:
					logger.LogError(LOGSOURCE, "  Library entry point not found.");
					break;
					
				default:
					logger.LogErrorFormat(LOGSOURCE, "  ?? (%d).", module->GetErrorCode());
				}
			}
			
			// add module
			pModules.Add(module);
			pRecentlyUsed.Add(module);
		}
		
	}catch(const deException &e){
		logger.LogException(LOGSOURCE, e);
	}
}
