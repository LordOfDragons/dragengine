/* 
 * Drag[en]gine Game Engine
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

deNetworkSystem::deNetworkSystem( deEngine *engine ) : deBaseSystem( engine, "Network", deModuleSystem::emtNetwork ){
	pActiveModule = NULL;
}

deNetworkSystem::~deNetworkSystem(){
}



// Management
///////////////

void deNetworkSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseNetworkModule* )module->GetModule();
}

void deNetworkSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deNetworkSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if( ! pActiveModule->Init() ){
		DETHROW( deeInvalidAction );
	}
	
	// load network system related stuff
	for( i=0; i<engine.GetResourceManagerCount(); i++ ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemNetworkLoad();
	}
}

void deNetworkSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	// unload network system related stuff
	for( i=engine.GetResourceManagerCount()-1; i>=0; i-- ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemNetworkUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Module Specific
////////////////////

void deNetworkSystem::ProcessNetwork(){
	if( ! GetIsRunning() ) return;
	
	pActiveModule->ProcessNetwork();
}

void deNetworkSystem::LoadServer( deServer *server ){
	if( ! GetIsRunning() ) return;
	
	if( server->GetPeerNetwork() ) DETHROW( deeInvalidParam );
	server->SetPeerNetwork( pActiveModule->CreateServer( server ) );
	if( ! server->GetPeerNetwork() ) DETHROW( deeOutOfMemory );
}

void deNetworkSystem::LoadConnection( deConnection *connection ){
	if( ! GetIsRunning() ) return;
	
	if( connection->GetPeerNetwork() ) DETHROW( deeInvalidParam );
	connection->SetPeerNetwork( pActiveModule->CreateConnection( connection ) );
	if( ! connection->GetPeerNetwork() ) DETHROW( deeOutOfMemory );
}

void deNetworkSystem::LoadState( deNetworkState *state ){
	if( ! GetIsRunning() ) return;
	
	if( state->GetPeerNetwork() ) DETHROW( deeInvalidParam );
	state->SetPeerNetwork ( pActiveModule->CreateState( state ) );
	if( ! state->GetPeerNetwork() ) DETHROW( deeOutOfMemory );
}

void deNetworkSystem::LoadWorld( deWorld *world ){
	if( GetIsRunning() ){
		if( world->GetPeerNetwork() ){
			DETHROW( deeInvalidParam );
		}
		world->SetPeerNetwork( pActiveModule->CreateWorld( world ) );
		if( ! world->GetPeerNetwork() ){
			DETHROW( deeOutOfMemory );
		}
	}
}
