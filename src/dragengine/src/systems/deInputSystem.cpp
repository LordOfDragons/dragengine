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
pCaptureInputDevices( false )
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
