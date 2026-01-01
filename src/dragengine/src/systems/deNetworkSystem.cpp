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
#include "deNetworkSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/network/deBaseNetworkModule.h"
#include "modules/network/deBaseNetworkServer.h"
#include "modules/network/deBaseNetworkState.h"
#include "modules/network/deBaseNetworkConnection.h"
#include "modules/network/deBaseNetworkWorld.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/deResourceManager.h"
#include "../resources/network/deServer.h"
#include "../resources/network/deConnection.h"
#include "../resources/network/deNetworkState.h"
#include "../resources/world/deWorld.h"



extern const int *vResourcePeerCreationOrder;



// Class deNetworkSystem
//////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkSystem::deNetworkSystem(deEngine *engine) : deBaseSystem(engine, "Network", deModuleSystem::emtNetwork){
	pActiveModule = NULL;
}

deNetworkSystem::~deNetworkSystem(){
}



// Management
///////////////

void deNetworkSystem::SetActiveModule(deLoadableModule *module){
	deBaseSystem::SetActiveModule(module);
	pActiveModule = (deBaseNetworkModule*)module->GetModule();
}

void deNetworkSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deNetworkSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if(!pActiveModule->Init()){
		DETHROW(deeInvalidAction);
	}
	
	// load network system related stuff
	for(i=0; i<engine.GetResourceManagerCount(); i++){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemNetworkLoad();
	}
}

void deNetworkSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy(pActiveModule);
	
	// unload network system related stuff
	for(i=engine.GetResourceManagerCount()-1; i>=0; i--){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemNetworkUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Module Specific
////////////////////

void deNetworkSystem::ProcessNetwork(){
	if(!GetIsRunning()) return;
	
	pActiveModule->ProcessNetwork();
}

void deNetworkSystem::LoadServer(deServer *server){
	if(!GetIsRunning()) return;
	
	if(server->GetPeerNetwork()) DETHROW(deeInvalidParam);
	server->SetPeerNetwork(pActiveModule->CreateServer(server));
}

void deNetworkSystem::LoadConnection(deConnection *connection){
	if(!GetIsRunning()) return;
	
	if(connection->GetPeerNetwork()) DETHROW(deeInvalidParam);
	connection->SetPeerNetwork(pActiveModule->CreateConnection(connection));
}

void deNetworkSystem::LoadState(deNetworkState *state){
	if(!GetIsRunning()) return;
	
	if(state->GetPeerNetwork()) DETHROW(deeInvalidParam);
	state->SetPeerNetwork (pActiveModule->CreateState(state));
}

void deNetworkSystem::LoadWorld(deWorld *world){
	if(GetIsRunning()){
		if(world->GetPeerNetwork()){
			DETHROW(deeInvalidParam);
		}
		world->SetPeerNetwork(pActiveModule->CreateWorld(world));
	}
}
