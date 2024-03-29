/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deVRSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../input/deInputEvent.h"
#include "../input/deInputEventQueue.h"
#include "../parallel/deParallelProcessing.h"



// Class deVRSystem
/////////////////////

// Constructor, Destructor
////////////////////////////

deVRSystem::deVRSystem( deEngine *engine ) :
deBaseSystem( engine, "VR", deModuleSystem::emtVR ),
pActiveModule( nullptr ),
pEventQueue( 100 ),
pRequestFeatureEyeGazeTracking( deBaseVRModule::efslDisabled ),
pRequestFeatureFacialTracking( deBaseVRModule::efslDisabled ),
pEnablePassthrough( false ),
pPassthroughTransparency( 1.0f ){
}

deVRSystem::~deVRSystem(){
}



// Management
///////////////

void deVRSystem::ProcessEvents(){
	if( GetIsRunning() ){
		pActiveModule->ProcessEvents();
	}
}

bool deVRSystem::RuntimeUsable(){
	if( ! GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	return pActiveModule->RuntimeUsable();
}

void deVRSystem::RequestFeatureEyeGazeTracking( deBaseVRModule::eFeatureSupportLevel level ){
	if( ! GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	pRequestFeatureEyeGazeTracking = level;
}

void deVRSystem::RequestFeatureFacialTracking( deBaseVRModule::eFeatureSupportLevel level ){
	if( ! GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	pRequestFeatureFacialTracking = level;
}

void deVRSystem::StartRuntime(){
	if( ! GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	pActiveModule->RequestFeatureEyeGazeTracking( pRequestFeatureEyeGazeTracking );
	pActiveModule->RequestFeatureFacialTracking( pRequestFeatureFacialTracking );
	
	pActiveModule->StartRuntime();
	
	pActiveModule->SetCamera( pCamera );
	pActiveModule->SetEnablePassthrough( pEnablePassthrough );
	pActiveModule->SetPassthroughTransparency( pPassthroughTransparency );
}

void deVRSystem::StopRuntime(){
	if( ! GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	pActiveModule->StopRuntime();
}

void deVRSystem::SetCamera( deCamera *camera ){
	if( ! GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	if( camera == pCamera ){
		return;
	}
	
	pCamera = camera;
	
	pActiveModule->SetCamera( camera );
}

bool deVRSystem::SupportsPassthrough(){
	if( ! GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	return pActiveModule->SupportsPassthrough();
}

void deVRSystem::SetEnablePassthrough( bool enable ){
	if( GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	if( enable == pEnablePassthrough ){
		return;
	}
	
	pEnablePassthrough = enable;
	
	pActiveModule->SetEnablePassthrough( enable );
}

void deVRSystem::SetPassthroughTransparency( float transparency ){
	if( GetIsRunning() ){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	if( fabsf( transparency - pPassthroughTransparency ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPassthroughTransparency = transparency;
	
	pActiveModule->SetPassthroughTransparency( transparency );
}



void deVRSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseVRModule* )module->GetModule();
}

void deVRSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deVRSystem::PostStart(){
	if( ! pActiveModule || ! pActiveModule->Init() ){
		DETHROW( deeInvalidAction );
	}
}

void deVRSystem::PreStop(){
	if( ! pActiveModule ){
		return;
	}
	
	// remove all parallel tasks if present
	GetEngine()->GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	pActiveModule->CleanUp();
}
