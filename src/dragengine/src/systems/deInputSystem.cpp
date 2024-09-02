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

#include "deInputSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/input/deBaseInputModule.h"
#include "../deEngine.h"
#include "../app/deOS.h"
#include "../common/exceptions.h"
#include "../input/deInputEvent.h"
#include "../input/deInputEventQueue.h"
#include "../parallel/deParallelProcessing.h"



// Class deInputSystem
////////////////////////

// Constructor, Destructor
////////////////////////////

deInputSystem::deInputSystem( deEngine *engine ) :
deBaseSystem( engine, "Input", deModuleSystem::emtInput ),
pActiveModule( NULL ),
pEventQueue( NULL ),
pCaptureInputDevices( false ),
pDropInputCount( 0 )
{
	pEventQueue = new deInputEventQueue( 100 );
}

deInputSystem::~deInputSystem(){
	if( pEventQueue ){
		delete pEventQueue;
	}
}



// Management
///////////////

void deInputSystem::ClearEventQueues(){
	GetEngine()->GetOS()->ProcessEventLoop( false );
	if( GetIsRunning() ){
		pActiveModule->ClearEvents();
	}
	pEventQueue->RemoveAllEvents();
}

void deInputSystem::ScreenSizeChanged(){
	if( GetIsRunning() ){
		pActiveModule->ScreenSizeChanged();
	}
}

void deInputSystem::AppActivationChanged(){
	if( GetIsRunning() ){
		pActiveModule->AppActivationChanged();
	}
}

void deInputSystem::SetCaptureInputDevices( bool captureInputDevices ){
	if( captureInputDevices == pCaptureInputDevices ){
		return;
	}
	
	pCaptureInputDevices = captureInputDevices;
	
	if( GetIsRunning() ){
		pActiveModule->CaptureInputDevicesChanged();
	}
}

void deInputSystem::StartDropInput(){
	pDropInputCount++;
}

void deInputSystem::StopDropInput(){
	DEASSERT_TRUE( pDropInputCount > 0 )
	pDropInputCount--;
}

bool deInputSystem::GetDropModeEnabled() const{
	return pDropInputCount > 0;
}

bool deInputSystem::DropEvent(const deInputEvent& event) const{
	if( pDropInputCount == 0 ){
		return false;
	}
	
	switch( event.GetType() ){
	case deInputEvent::eeKeyPress:
	case deInputEvent::eeKeyRelease:
	case deInputEvent::eeMousePress:
	case deInputEvent::eeMouseRelease:
	case deInputEvent::eeMouseMove:
	case deInputEvent::eeMouseWheel:
	case deInputEvent::eeAxisMove:
	case deInputEvent::eeButtonPress:
	case deInputEvent::eeButtonRelease:
	case deInputEvent::eeButtonTouch:
	case deInputEvent::eeButtonUntouch:
		return true;
		
	case deInputEvent::eeDeviceAttached:
	case deInputEvent::eeDeviceDetached:
	case deInputEvent::eeDeviceParamsChanged:
	case deInputEvent::eeDevicesAttachedDetached:
		return false;
		
	default:
		return false;
	}
}



void deInputSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseInputModule* )module->GetModule();
}

void deInputSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deInputSystem::PostStart(){
	if( ! pActiveModule->Init() ){
		DETHROW( deeInvalidAction );
	}
}

void deInputSystem::PreStop(){
	// remove all parallel tasks if present
	GetEngine()->GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	pActiveModule->CleanUp();
}
