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

#include "deParallelProcessing.h"
#include "deParallelTask.h"
#include "deParallelThread.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"
#include "../threading/deMutexGuard.h"



// Definitions
////////////////

#define LOGSOURCE "Parallel Processing"



// Class deParallelThread
///////////////////////////

// Constructor, destructor
////////////////////////////

deParallelThread::deParallelThread( deParallelProcessing &parallelProcessing,
	int number, bool takeLowPriorityTasks ) :
pParallelProcessing( parallelProcessing ),
pNumber( number ),
pTakeLowPriorityTasks( takeLowPriorityTasks ),
pExitThread( false ),
pWaiting( false ),
pTask( NULL ){
}

deParallelThread::~deParallelThread(){
}



// Management
///////////////

bool deParallelThread::IsWaiting(){
	deMutexGuard lock( pMutexWaiting );
	return pWaiting;
}

void deParallelThread::RequestExit(){
	if( ! IsRunning() ){
		return;
	}
	
	deMutexGuard lock( pMutexWaiting );
	pExitThread = true;
}

deParallelTask *deParallelThread::GetTask(){
	deMutexGuard lock( pMutexTask );
	return pTask;
}

void deParallelThread::Run(){
	while( true ){
		// get the next task to process if there is any
		deMutexGuard lock( pMutexTask );
		
		pTask = pParallelProcessing.NextPendingTask( pTakeLowPriorityTasks );
		
		if( pParallelProcessing.GetOutputDebugMessages() ){
			if( pTask ){
				const decString debugName( pTask->GetDebugName() );
				const decString debugDetails( pTask->GetDebugDetails() );
				pParallelProcessing.GetEngine().GetLogger()->LogInfoFormat( LOGSOURCE,
					"Thread %i: Assign [%s] %s", pNumber,
					debugName.GetString(), debugDetails.GetString() );
				
			}else{
				pParallelProcessing.GetEngine().GetLogger()->LogInfoFormat( LOGSOURCE,
					"Thread %i: No task, going to sleep", pNumber );
			}
		}
		
		lock.Unlock();
		
		// if there is a task process it
		if( pTask ){
			try{
				pTask->Run();
				
			}catch( const deException &exception ){
				const decString debugName( pTask->GetDebugName() );
				const decString debugDetails( pTask->GetDebugDetails() );
				pParallelProcessing.GetEngine().GetLogger()->LogErrorFormat( LOGSOURCE,
					"Thread %i: Task failed [%s] %s.\n", pNumber,
					debugName.GetString(), debugDetails.GetString() );
				pParallelProcessing.GetEngine().GetLogger()->LogException( LOGSOURCE, exception );
				pTask->Cancel();  // tell task it failed
			}
			
			// send the finished task back
			if( pParallelProcessing.GetOutputDebugMessages() ){
				const decString debugName( pTask->GetDebugName() );
				const decString debugDetails( pTask->GetDebugDetails() );
				pParallelProcessing.GetEngine().GetLogger()->LogInfoFormat( LOGSOURCE,
					"Thread %i: Finished Task [%s] %s.", pNumber,
					debugName.GetString(), debugDetails.GetString() );
			}
			
			lock.Lock();
			
			pParallelProcessing.AddFinishedTask( pTask );
			pTask = NULL;
			
			lock.Unlock();
			
		// otherwise go to sleep until new tasks become available
		}else{
			deMutexGuard lockWait( pMutexWaiting );
			pWaiting = true;
			if( pExitThread ){
				break;
			}
			lockWait.Unlock();
			
			pParallelProcessing.WaitOnNewTasksSemaphore();
			
			lockWait.Lock();
			pWaiting = false;
			if( pExitThread ){
				break;
			}
		}
	}
}
