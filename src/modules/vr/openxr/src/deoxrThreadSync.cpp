/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deoxrThreadSync.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"
#include "deoxrGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoxrThreadSync
//////////////////////////

deoxrThreadSync::deoxrThreadSync( deVROpenXR &oxr ) :
pOxr( oxr ),
pNextCommand( ecNone ),
pWaitFrameRunning( false ),
pWaitFrameFailed( false ){
}

deoxrThreadSync::~deoxrThreadSync(){
}



// Management
///////////////

void deoxrThreadSync::StartWaitFrame(){
	const deMutexGuard guard( pMutex );
	if( pNextCommand == ecExit ){
		return;
	}
	DEASSERT_TRUE( pNextCommand == ecNone )
	
	pNextCommand = ecWaitFrame;
	pWaitFrameFailed = false;
	pWaitFrameRunning = true;
	pSemaphoreNextCommand.Signal();
}

void deoxrThreadSync::WaitWaitFrameFinished(){
	{
	const deMutexGuard guard( pMutex );
	if( pNextCommand != ecWaitFrame ){
		return;
	}
	if( ! pWaitFrameRunning ){
		DEASSERT_FALSE( pWaitFrameFailed )
	}
	}
	
	pSemaphoreWaitFrameFinished.Wait();
	
	const deMutexGuard guard( pMutex );
	if( pNextCommand == ecExit ){
		return;
	}
	DEASSERT_FALSE( pWaitFrameFailed )
}



void deoxrThreadSync::ExitThread(){
	const deMutexGuard guard( pMutex );
	pNextCommand = ecExit;
	pSemaphoreNextCommand.Signal();
}



void deoxrThreadSync::Run(){
	while( true ){
		pSemaphoreNextCommand.Wait();
		
		eCommand nextCommand;
		{
		const deMutexGuard guard( pMutex );
		nextCommand = pNextCommand;
		}
		
		switch( nextCommand ){
		case ecNone:
			break;
			
		case ecExit:
			return;
			
		case ecWaitFrame:
			pWaitFrame();
			break;
			
		default:
			break;
		}
	}
}



// Private Functions
//////////////////////

void deoxrThreadSync::pWaitFrame(){
	try{
		const deMutexGuard guard( pOxr.GetMutexOpenXR() );
		if( pOxr.GetSession() ){
			pOxr.GetSession()->WaitFrame();
		}
		pWaitFrameFailed = false;
		
	}catch( const deException &e ){
		pOxr.LogException( e );
		pWaitFrameFailed = true;
	}
	
	const deMutexGuard guard( pMutex );
	
	pWaitFrameRunning = false;
	
	if( pNextCommand != ecExit ){
		pNextCommand = ecNone;
	}
	
	pSemaphoreWaitFrameFinished.Signal();
}
