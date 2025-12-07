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

#include "deBaseSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"
#include "../parallel/deParallelProcessing.h"



// Definitions
////////////////

#define LOGSOURCE "Dragengine"



// Class deBaseSystem
///////////////////////

// Constructor, destructor
////////////////////////////

deBaseSystem::deBaseSystem(deEngine *engine, const char *systemName, int requiredModuleType) :
pSystemName(systemName),
pEngine(engine),
pActiveLoadableModule(nullptr),
pRequiredModuleType(requiredModuleType),
pRunning(false),
pFailed(false)
{
	if(! engine){
		DETHROW(deeInvalidParam);
	}
}

deBaseSystem::~deBaseSystem(){
	if(pRunning){
		Stop();
	}
	
	ClearPermanents();
	
	if(pActiveLoadableModule){
		pActiveLoadableModule->FreeReference();
		pActiveLoadableModule = NULL;
	}
}



// Management
///////////////

void deBaseSystem::SetHasFailed(bool hasFailed){
	pFailed = hasFailed;
}

bool deBaseSystem::CanStart(){
	return pActiveLoadableModule && pActiveLoadableModule->IsLoaded();
}

void deBaseSystem::Start(){
	if(pRunning || ! CanStart()){
		DETHROW(deeInvalidAction);
	}
	
	LogInfoFormat("Starting %s module %s", GetSystemName().GetString(), pActiveLoadableModule->GetName().GetString());
	
	deParallelProcessing &parallelProcessing = pEngine->GetParallelProcessing();
	const bool resumeParallelProcessing = ! parallelProcessing.GetPaused();
	
	if(resumeParallelProcessing){
		parallelProcessing.Pause();
	}
	
	try{
		// switch to running mode and lock module
		pActiveLoadableModule->Lock();
		pRunning = true;
		
		// post start
		PostStart();
		
	}catch(const deException &){
		Stop();
		if(resumeParallelProcessing){
			parallelProcessing.Resume();
		}
		throw;
	}
	
	if(resumeParallelProcessing){
		parallelProcessing.Resume();
	}
}

void deBaseSystem::Stop(){
	if(! pRunning || ! pActiveLoadableModule){
		return;
	}
	
	LogInfoFormat("Stopping %s module %s", GetSystemName().GetString(), pActiveLoadableModule->GetName().GetString());
	
	deParallelProcessing &parallelProcessing = pEngine->GetParallelProcessing();
	const bool resumeParallelProcessing = ! parallelProcessing.GetPaused();
	
	if(resumeParallelProcessing){
		parallelProcessing.Pause();
	}
	
	try{
		PreStop();
		
		// switch to non-running mode and unlock module
		pRunning = false;
		pActiveLoadableModule->Unlock();
		
	}catch(const deException &){
		if(resumeParallelProcessing){
			parallelProcessing.Resume();
		}
		throw;
	}
	
	if(resumeParallelProcessing){
		parallelProcessing.Resume();
	}
}



// Helper Functions
/////////////////////

void deBaseSystem::CheckAndActivateFirst(deModuleSystem::eModuleTypes type){
	deModuleSystem *moduleSystem = pEngine->GetModuleSystem();
	
	// check for modules
	if(moduleSystem->GetLoadedModuleCountFor(type) == 0){
		LogErrorFormat("No loaded %s modules found", GetSystemName().GetString());
		DETHROW(deeNoModuleFound);
	}
	
	// select first module as default one
	SetActiveModule(moduleSystem->GetFirstLoadedModuleFor(type));
}



// Extend those
/////////////////

void deBaseSystem::ClearPermanents(){
}

void deBaseSystem::SetActiveModule(deLoadableModule *module){
	if(! module || ! module->IsLoaded() || module->GetType() != pRequiredModuleType){
		DETHROW(deeInvalidParam);
	}
	if(pRunning){
		DETHROW(deeInvalidAction);
	}
	
	LogInfoFormat("Activating %s module %s %s", GetSystemName().GetString(),
		module->GetName().GetString(), module->GetVersion().GetString());
	
	// set new module
	if(pActiveLoadableModule){
		pActiveLoadableModule->FreeReference();
	}
	
	pActiveLoadableModule = module;
	
	if(pActiveLoadableModule){
		pActiveLoadableModule->AddReference();
	}
}



// Overload those
///////////////////

void deBaseSystem::PostStart(){
}

void deBaseSystem::PreStop(){
}



// Debugging
//////////////

void deBaseSystem::LogInfo(const char *message){
	pEngine->GetLogger()->LogInfo(LOGSOURCE, message);
}

void deBaseSystem::LogInfoFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	pEngine->GetLogger()->LogInfoFormatUsing(LOGSOURCE, message, list);
	va_end(list);
}

void deBaseSystem::LogInfoFormatUsing(const char *message, va_list args){
	pEngine->GetLogger()->LogInfoFormatUsing(LOGSOURCE, message, args);
}

void deBaseSystem::LogWarn(const char *message){
	pEngine->GetLogger()->LogWarn(LOGSOURCE, message);
}

void deBaseSystem::LogWarnFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	pEngine->GetLogger()->LogWarnFormatUsing(LOGSOURCE, message, list);
	va_end(list);
}

void deBaseSystem::LogWarnFormatUsing(const char *message, va_list args){
	pEngine->GetLogger()->LogWarnFormatUsing(LOGSOURCE, message, args);
}

void deBaseSystem::LogError(const char *message){
	pEngine->GetLogger()->LogError(LOGSOURCE, message);
}

void deBaseSystem::LogErrorFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	pEngine->GetLogger()->LogErrorFormatUsing(LOGSOURCE, message, list);
	va_end(list);
}

void deBaseSystem::LogErrorFormatUsing(const char *message, va_list args){
	pEngine->GetLogger()->LogErrorFormatUsing(LOGSOURCE, message, args);
}

void deBaseSystem::LogException(const deException &exception){
	pEngine->GetLogger()->LogException(LOGSOURCE, exception);
}
