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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deCrashRecoverySystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/crashrecovery/deBaseCrashRecoveryModule.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"



// class deCrashRecoverySystem
////////////////////////////////

// constructor, destructor
deCrashRecoverySystem::deCrashRecoverySystem(deEngine *engine) : deBaseSystem(engine, "Crash-Recovery", deModuleSystem::emtCrashRecovery){
	pActiveModule = NULL;
}
deCrashRecoverySystem::~deCrashRecoverySystem(){
}

// extend those
void deCrashRecoverySystem::SetActiveModule(deLoadableModule *module){
	deBaseSystem::SetActiveModule(module);
	pActiveModule = (deBaseCrashRecoveryModule*)module->GetModule();
}
void deCrashRecoverySystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

// overload those
void deCrashRecoverySystem::PostStart(){
	if(!pActiveModule->Init()) DETHROW(deeInvalidAction);
}
void deCrashRecoverySystem::PreStop(){
	// remove all parallel tasks if present
	GetEngine()->GetParallelProcessing().FinishAndRemoveTasksOwnedBy(pActiveModule);
	
	pActiveModule->CleanUp();
}

// management
bool deCrashRecoverySystem::RecoverFromError(){
	if(!GetIsRunning()) return false;
	return pActiveModule->RecoverFromError();
}
