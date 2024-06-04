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
