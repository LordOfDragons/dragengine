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

deAISystem::deAISystem( deEngine *engine ) :
deBaseSystem( engine, "AI", deModuleSystem::emtAI ),
pActiveModule( NULL ){
}

deAISystem::~deAISystem(){
}



// Management
///////////////

void deAISystem::LoadWorld( deWorld *world ){
	if( GetIsRunning() ){
		if( world->GetPeerAI() ){
			DETHROW( deeInvalidParam );
		}
		world->SetPeerAI( pActiveModule->CreateWorld( world ) );
		if( ! world->GetPeerAI() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deAISystem::LoadNavigationSpace( deNavigationSpace *navspace ){
	if( GetIsRunning() ){
		if( navspace->GetPeerAI() ){
			DETHROW( deeInvalidParam );
		}
		navspace->SetPeerAI( pActiveModule->CreateNavigationSpace( navspace ) );
		if( ! navspace->GetPeerAI() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deAISystem::LoadNavigationBlocker( deNavigationBlocker *blocker ){
	if( GetIsRunning() ){
		if( blocker->GetPeerAI() ){
			DETHROW( deeInvalidParam );
		}
		
		blocker->SetPeerAI( pActiveModule->CreateNavigationBlocker( blocker ) );
		if( ! blocker->GetPeerAI() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deAISystem::LoadNavigator( deNavigator *navigator ){
	if( GetIsRunning() ){
		if( navigator->GetPeerAI() ){
			DETHROW( deeInvalidParam );
		}
		navigator->SetPeerAI( pActiveModule->CreateNavigator( navigator ) );
		if( ! navigator->GetPeerAI() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deAISystem::LoadHeightTerrain( deHeightTerrain &heightTerrain ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( heightTerrain.GetPeerAI() ){
		DETHROW( deeInvalidParam );
	}
	heightTerrain.SetPeerAI( pActiveModule->CreateHeightTerrain( heightTerrain ) );
	if( ! heightTerrain.GetPeerAI() ){
		DETHROW( deeOutOfMemory );
	}
}



// Overloadables
//////////////////

void deAISystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseAIModule* )module->GetModule();
}

void deAISystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deAISystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if( ! pActiveModule->Init() ){
		DETHROW( deeInvalidAction );
	}
	
	// load animator system related stuff
	for( i=0; i<engine.GetResourceManagerCount(); i++ ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemAILoad();
	}
}

void deAISystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	// unload animator system related stuff
	for( i=engine.GetResourceManagerCount()-1; i>=0; i-- ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemAIUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Private functions
//////////////////////

void deAISystem::pCleanUp(){
}
