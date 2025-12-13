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

#include "deAISystem.h"
#include "modules/deLoadableModule.h"
#include "modules/ai/deBaseAIModule.h"
#include "deModuleSystem.h"
#include "../deEngine.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/navigation/navigator/deNavigator.h"
#include "../resources/navigation/navigator/deNavigatorManager.h"
#include "../resources/navigation/space/deNavigationSpace.h"
#include "../resources/navigation/space/deNavigationSpaceManager.h"
#include "../resources/navigation/blocker/deNavigationBlocker.h"
#include "../resources/world/deWorld.h"
#include "../resources/terrain/heightmap/deHeightTerrain.h"
#include "../common/exceptions.h"



extern const int *vResourcePeerCreationOrder;



// Class deAISystem
/////////////////////

// Constructor, destructor
////////////////////////////

deAISystem::deAISystem(deEngine *engine) :
deBaseSystem(engine, "AI", deModuleSystem::emtAI),
pActiveModule(NULL){
}

deAISystem::~deAISystem(){
}



// Management
///////////////

void deAISystem::LoadWorld(deWorld *world){
	if(GetIsRunning()){
		if(world->GetPeerAI()){
			DETHROW(deeInvalidParam);
		}
		world->SetPeerAI(pActiveModule->CreateWorld(world));
	}
}

void deAISystem::LoadNavigationSpace(deNavigationSpace *navspace){
	if(GetIsRunning()){
		if(navspace->GetPeerAI()){
			DETHROW(deeInvalidParam);
		}
		navspace->SetPeerAI(pActiveModule->CreateNavigationSpace(navspace));
	}
}

void deAISystem::LoadNavigationBlocker(deNavigationBlocker *blocker){
	if(GetIsRunning()){
		if(blocker->GetPeerAI()){
			DETHROW(deeInvalidParam);
		}
		
		blocker->SetPeerAI(pActiveModule->CreateNavigationBlocker(blocker));
	}
}

void deAISystem::LoadNavigator(deNavigator *navigator){
	if(GetIsRunning()){
		if(navigator->GetPeerAI()){
			DETHROW(deeInvalidParam);
		}
		navigator->SetPeerAI(pActiveModule->CreateNavigator(navigator));
	}
}

void deAISystem::LoadHeightTerrain(deHeightTerrain &heightTerrain){
	if(!GetIsRunning()){
		return;
	}
	
	if(heightTerrain.GetPeerAI()){
		DETHROW(deeInvalidParam);
	}
	heightTerrain.SetPeerAI(pActiveModule->CreateHeightTerrain(heightTerrain));
}



// Overloadables
//////////////////

void deAISystem::SetActiveModule(deLoadableModule *module){
	deBaseSystem::SetActiveModule(module);
	pActiveModule = (deBaseAIModule*)module->GetModule();
}

void deAISystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deAISystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if(!pActiveModule->Init()){
		DETHROW(deeInvalidAction);
	}
	
	// load animator system related stuff
	for(i=0; i<engine.GetResourceManagerCount(); i++){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemAILoad();
	}
}

void deAISystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy(pActiveModule);
	
	// unload animator system related stuff
	for(i=engine.GetResourceManagerCount()-1; i>=0; i--){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemAIUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Private functions
//////////////////////

void deAISystem::pCleanUp(){
}
