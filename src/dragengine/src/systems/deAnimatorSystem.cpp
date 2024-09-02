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
