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

#include "deSynthesizerSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/synthesizer/deBaseSynthesizerModule.h"
#include "deModuleSystem.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/synthesizer/deSynthesizer.h"
#include "../resources/synthesizer/deSynthesizerManager.h"
#include "../resources/synthesizer/deSynthesizerInstance.h"
#include "../resources/sound/deSound.h"



extern const int *vResourcePeerCreationOrder;



// Class deSynthesizerSystem
///////////////////////////////

// Constructor, Destructor
////////////////////////////

deSynthesizerSystem::deSynthesizerSystem(deEngine *engine) :
deBaseSystem(engine, "Synthesizer", deModuleSystem::emtSynthesizer),
pActiveModule(NULL){
}

deSynthesizerSystem::~deSynthesizerSystem(){
}



// Management
///////////////

void deSynthesizerSystem::LoadSound(deSound *sound){
	if(!GetIsRunning()){
		return;
	}
	
	if(sound->GetPeerSynthesizer()){
		DETHROW(deeInvalidParam);
	}
	sound->SetPeerSynthesizer(pActiveModule->CreateSound(sound));
}

void deSynthesizerSystem::LoadSynthesizer(deSynthesizer *synthesizer){
	if(!GetIsRunning()){
		return;
	}
	
	if(synthesizer->GetPeerSynthesizer()){
		DETHROW(deeInvalidParam);
	}
	synthesizer->SetPeerSynthesizer(pActiveModule->CreateSynthesizer(synthesizer));
}

void deSynthesizerSystem::LoadSynthesizerInstance(deSynthesizerInstance *instance){
	if(!GetIsRunning()){
		return;
	}
	
	if(instance->GetPeerSynthesizer()){
		DETHROW(deeInvalidParam);
	}
	instance->SetPeerSynthesizer(pActiveModule->CreateSynthesizerInstance(instance));
}



// Overloadables
//////////////////

void deSynthesizerSystem::SetActiveModule(deLoadableModule *module){
	deBaseSystem::SetActiveModule(module);
	pActiveModule = (deBaseSynthesizerModule*)module->GetModule();
}

void deSynthesizerSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deSynthesizerSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if(!pActiveModule->Init()){
		DETHROW(deeInvalidAction);
	}
	
	// load synthesizer system related stuff
	for(i=0; i<engine.GetResourceManagerCount(); i++){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemSynthesizerLoad();
	}
}

void deSynthesizerSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy(pActiveModule);
	
	// unload synthesizer system related stuff
	for(i=engine.GetResourceManagerCount()-1; i>=0; i--){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemSynthesizerUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Private functions
//////////////////////

void deSynthesizerSystem::pCleanUp(){
}
