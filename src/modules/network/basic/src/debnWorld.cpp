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

#include "debnWorld.h"
#include "deNetworkBasic.h"
#include "states/debnState.h"

#include <dragengine/resources/network/deNetworkState.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>



// Class debnWorld
////////////////////

// Constructor, destructor
////////////////////////////

debnWorld::debnWorld(deNetworkBasic *netBasic, deWorld *world){
	if(!netBasic || !world){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	pWorld = world;
	
	pCanCreateStates = false;
	
	pStateSlots = NULL;
	pStateSlotCount = 0;
	
	pParentNetwork = NULL;
	
	try{
		pStateSlots = new debnState*[1000];
		
		pStateSlotCount = 1000;
		
		for(i=0; i<pStateSlotCount; i++){
			pStateSlots[i] = NULL;
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

debnWorld::~debnWorld(){
	pCleanUp();
}



// Management
///////////////

void debnWorld::SetParentNetwork(debnNetwork *network){
	pParentNetwork = network;
}

void debnWorld::SetCanCreateStates(bool canCreateStates){
	pCanCreateStates = canCreateStates;
}

void debnWorld::InitStates(){
	/*int i, count = pScene->GetNetworkStateCount();
	debnState *bnState;
	
	for(i=0; i<count; i++){
		bnState = (debnState*)pScene->GetNetworkStateAt(i)->GetNetworkState();
		
		if(bnState->GetSlot() == -1){
			bnState->SetSlot(pNextFreeSlot());
		}
	}*/
}



// Notifications
//////////////////

void debnWorld::NetworkStateAdded(deNetworkState *state){
	//debnState *bnState = ( debnState* )state->GetNetworkState();
	
	//bnState->SetParentScene( this );
	//bnState->SetSector( pScene->GetSector() );
	
	//bnState->SetSlot( pNextFreeSlot() );
}

void debnWorld::NetworkStateRemoved(deNetworkState *state){
	//debnState *bnState = ( debnState* )state->GetNetworkState();
	//int slot = bnState->GetSlot();
	
	//if( slot != -1 ){
	//	pStateSlots[ slot ] = NULL;
		//bnState->SetSlot( -1 );
	//}
	
	//bnState->SetParentScene( NULL );
}

void debnWorld::AllNetworkStatesRemoved(){
	/*int i, count = pScene->GetNetworkStateCount();
	debnState *bnState;
	
	for(i=0; i<count; i++){
		bnState = (debnState*)pScene->GetNetworkStateAt(i)->GetNetworkState();
		//bnState->SetSlot( -1 );
		bnState->SetParentScene(NULL);
	}
	
	for(i=0; i<pStateSlotCount; i++){
		pStateSlots[i] = NULL;
	}*/
}



// Private Functions
//////////////////////

void debnWorld::pCleanUp(){
	AllNetworkStatesRemoved();
	
	if(pStateSlots) delete [] pStateSlots;
}

int debnWorld::pNextFreeSlot(){
	int i;
	
	for(i=0; i<pStateSlotCount; i++){
		if(!pStateSlots[i]){
			return i;
		}
	}
	
	return -1;
}
