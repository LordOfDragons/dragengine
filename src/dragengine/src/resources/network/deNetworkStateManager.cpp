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

#include "deNetworkState.h"
#include "deNetworkStateManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deNetworkSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/network/deBaseNetworkState.h"
#include "../../systems/modules/scripting/deBaseScriptingNetworkState.h"
#include "../../common/exceptions.h"



// Class deNetworkStateManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkStateManager::deNetworkStateManager(deEngine *engine) :
deResourceManager(engine, ertNetworkState)
{
	SetLoggingName("network state");
}

deNetworkStateManager::~deNetworkStateManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNetworkStateManager::GetNetworkStateCount() const{
	return pStates.GetCount();
}

deNetworkState *deNetworkStateManager::GetRootNetworkState() const{
	return (deNetworkState*)pStates.GetRoot();
}

deNetworkState *deNetworkStateManager::CreateState(bool readOnly){
	deNetworkState *state = NULL;
	
	try{
		state = new deNetworkState(this, readOnly);
		
		GetNetworkSystem()->LoadState(state);
		GetScriptingSystem()->LoadNetworkState(state);
		
		pStates.Add(state);
		
	}catch(const deException &){
		if(state){
			state->FreeReference();
		}
		throw;
	}
	
	return state;
}



void deNetworkStateManager::ReleaseLeakingResources(){
	const int count = GetNetworkStateCount();
	if(count > 0){
		LogWarnFormat("%i leaking network states", count);
		pStates.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNetworkStateManager::SystemNetworkLoad(){
	deNetworkState *state = (deNetworkState*)pStates.GetRoot();
	deNetworkSystem &netSys = *GetNetworkSystem();
	
	while(state){
		if(! state->GetPeerNetwork()){
			netSys.LoadState(state);
		}
		
		state = (deNetworkState*)state->GetLLManagerNext();
	}
}

void deNetworkStateManager::SystemNetworkUnload(){
	deNetworkState *state = (deNetworkState*)pStates.GetRoot();
	
	while(state){
		state->SetPeerNetwork (NULL);
		state = (deNetworkState*)state->GetLLManagerNext();
	}
}

void deNetworkStateManager::SystemScriptingLoad(){
	deNetworkState *state = (deNetworkState*)pStates.GetRoot();
	deScriptingSystem &scrSys = *GetScriptingSystem();
	
	while(state){
		if(! state->GetPeerScripting()){
			scrSys.LoadNetworkState(state);
		}
		
		state = (deNetworkState*)state->GetLLManagerNext();
	}
}

void deNetworkStateManager::SystemScriptingUnload(){
	deNetworkState *state = (deNetworkState*)pStates.GetRoot();
	
	while(state){
		state->SetPeerScripting(NULL);
		state = (deNetworkState*)state->GetLLManagerNext();
	}
}

void deNetworkStateManager::RemoveResource(deResource *resource){
	pStates.RemoveIfPresent(resource);
}
