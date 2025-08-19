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
	if(GetIsRunning()){
		pActiveModule->ProcessEvents();
	}
}

bool deVRSystem::RuntimeUsable(){
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	return pActiveModule->RuntimeUsable();
}

void deVRSystem::RequestFeatureEyeGazeTracking( deBaseVRModule::eFeatureSupportLevel level ){
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	pRequestFeatureEyeGazeTracking = level;
}

void deVRSystem::RequestFeatureFacialTracking( deBaseVRModule::eFeatureSupportLevel level ){
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	pRequestFeatureFacialTracking = level;
}

void deVRSystem::StartRuntime(){
	if(!GetIsRunning()){
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
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	pActiveModule->StopRuntime();
}

bool deVRSystem::IsRuntimeRunning(){
	if(!GetIsRunning()){
		DETHROW_INFO(deeInvalidAction, "Module not running");
	}
	
	return pActiveModule->IsRuntimeRunning();
}

void deVRSystem::SetCamera( deCamera *camera ){
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	if( camera == pCamera ){
		return;
	}
	
	pCamera = camera;
	
	pActiveModule->SetCamera( camera );
}

bool deVRSystem::SupportsPassthrough(){
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	return pActiveModule->SupportsPassthrough();
}

void deVRSystem::SetEnablePassthrough( bool enable ){
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	if( enable == pEnablePassthrough ){
		return;
	}
	
	pEnablePassthrough = enable;
	
	pActiveModule->SetEnablePassthrough( enable );
}

void deVRSystem::SetPassthroughTransparency( float transparency ){
	if(!GetIsRunning()){
		DETHROW_INFO( deeInvalidAction, "Module not running" );
	}
	
	if( fabsf( transparency - pPassthroughTransparency ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPassthroughTransparency = transparency;
	
	pActiveModule->SetPassthroughTransparency( transparency );
}

void deVRSystem::CenterPlayspace(){
	if(!GetIsRunning()){
		DETHROW_INFO(deeInvalidAction, "Module not running");
	}
	
	pActiveModule->CenterPlayspace();
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
