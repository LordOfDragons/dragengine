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

#include "deAnimatorSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/animator/deBaseAnimatorModule.h"
#include "deModuleSystem.h"
#include "../deEngine.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/animator/deAnimator.h"
#include "../resources/animator/deAnimatorManager.h"
#include "../resources/animator/deAnimatorInstance.h"
#include "../resources/animation/deAnimation.h"
#include "../resources/component/deComponent.h"
#include "../common/exceptions.h"



extern const int *vResourcePeerCreationOrder;



// Class deAnimatorSystem
///////////////////////////

// Constructor, Destructor
////////////////////////////

deAnimatorSystem::deAnimatorSystem( deEngine *engine ) :
deBaseSystem( engine, "Animator", deModuleSystem::emtAnimator ),
pActiveModule( NULL ){
}

deAnimatorSystem::~deAnimatorSystem(){
}



// Management
///////////////

void deAnimatorSystem::LoadAnimator( deAnimator *animator ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( animator->GetPeerAnimator() ){
		DETHROW( deeInvalidParam );
	}
	animator->SetPeerAnimator( pActiveModule->CreateAnimator( animator ) );
	if( ! animator->GetPeerAnimator() ){
		DETHROW( deeOutOfMemory );
	}
}

void deAnimatorSystem::LoadAnimatorInstance( deAnimatorInstance *instance ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( instance->GetPeerAnimator() ){
		DETHROW( deeInvalidParam );
	}
	instance->SetPeerAnimator( pActiveModule->CreateAnimatorInstance( instance ) );
	if( ! instance->GetPeerAnimator() ){
		DETHROW( deeOutOfMemory );
	}
}

void deAnimatorSystem::LoadAnimation( deAnimation *animation ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( animation->GetPeerAnimator() ){
		DETHROW( deeInvalidParam );
	}
	animation->SetPeerAnimator( pActiveModule->CreateAnimation( animation ) );
	if( ! animation->GetPeerAnimator() ){
		DETHROW( deeOutOfMemory );
	}
}

void deAnimatorSystem::LoadComponent( deComponent *component ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( component->GetPeerAnimator() ){
		DETHROW( deeInvalidParam );
	}
	component->SetPeerAnimator( pActiveModule->CreateComponent( component ) );
	if( ! component->GetPeerAnimator() ){
		DETHROW( deeOutOfMemory );
	}
}



// Overloadables
//////////////////

void deAnimatorSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseAnimatorModule* )module->GetModule();
}

void deAnimatorSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deAnimatorSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if( ! pActiveModule->Init() ){
		DETHROW( deeInvalidAction );
	}
	
	// load animator system related stuff
	for( i=0; i<engine.GetResourceManagerCount(); i++ ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemAnimatorLoad();
	}
}

void deAnimatorSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	// unload animator system related stuff
	for( i=engine.GetResourceManagerCount()-1; i>=0; i-- ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemAnimatorUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Private functions
//////////////////////

void deAnimatorSystem::pCleanUp(){
}
