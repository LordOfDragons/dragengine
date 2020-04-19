/* 
 * Drag[en]gine Basic Network Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debnWorld.h"
#include "deNetworkBasic.h"
#include "states/debnState.h"

#include "dragengine/resources/network/deNetworkState.h"
#include "dragengine/resources/world/deWorld.h"
#include "dragengine/common/exceptions.h"



// Class debnWorld
////////////////////

// Constructor, destructor
////////////////////////////

debnWorld::debnWorld( deNetworkBasic *netBasic, deWorld *world ){
	if( ! netBasic || ! world ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	pWorld = world;
	
	pCanCreateStates = false;
	
	pStateSlots = NULL;
	pStateSlotCount = 0;
	
	pParentNetwork = NULL;
	
	try{
		pStateSlots = new debnState*[ 1000 ];
		if( ! pStateSlots ){
			DETHROW( deeOutOfMemory );
		}
		
		pStateSlotCount = 1000;
		
		for( i=0; i<pStateSlotCount; i++ ){
			pStateSlots[ i ] = NULL;
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debnWorld::~debnWorld(){
	pCleanUp();
}



// Management
///////////////

void debnWorld::SetParentNetwork( debnNetwork *network ){
	pParentNetwork = network;
}

void debnWorld::SetCanCreateStates( bool canCreateStates ){
	pCanCreateStates = canCreateStates;
}

void debnWorld::InitStates(){
	/*int i, count = pScene->GetNetworkStateCount();
	debnState *bnState;
	
	for( i=0; i<count; i++ ){
		bnState = ( debnState* )pScene->GetNetworkStateAt( i )->GetNetworkState();
		
		if( bnState->GetSlot() == -1 ){
			bnState->SetSlot( pNextFreeSlot() );
		}
	}*/
}



// Notifications
//////////////////

void debnWorld::NetworkStateAdded( deNetworkState *state ){
	//debnState *bnState = ( debnState* )state->GetNetworkState();
	
	//bnState->SetParentScene( this );
	//bnState->SetSector( pScene->GetSector() );
	
	//bnState->SetSlot( pNextFreeSlot() );
}

void debnWorld::NetworkStateRemoved( deNetworkState *state ){
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
	
	for( i=0; i<count; i++ ){
		bnState = ( debnState* )pScene->GetNetworkStateAt( i )->GetNetworkState();
		//bnState->SetSlot( -1 );
		bnState->SetParentScene( NULL );
	}
	
	for( i=0; i<pStateSlotCount; i++ ){
		pStateSlots[ i ] = NULL;
	}*/
}



// Private Functions
//////////////////////

void debnWorld::pCleanUp(){
	AllNetworkStatesRemoved();
	
	if( pStateSlots ) delete [] pStateSlots;
}

int debnWorld::pNextFreeSlot(){
	int i;
	
	for( i=0; i<pStateSlotCount; i++ ){
		if( ! pStateSlots[ i ] ){
			return i;
		}
	}
	
	return -1;
}
