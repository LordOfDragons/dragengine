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

deParallelThread::deParallelThread(deParallelProcessing &parallelProcessing,
	int number, bool takeLowPriorityTasks) :
pParallelProcessing(parallelProcessing),
pNumber(number),
pTakeLowPriorityTasks(takeLowPriorityTasks),
pExitThread(false),
pWaiting(false),
pTask(NULL){
}

deParallelThread::~deParallelThread(){
}



// Management
///////////////

bool deParallelThread::IsWaiting(){
	const deMutexGuard lock(pMutexWaiting);
	return pWaiting;
}

void deParallelThread::RequestExit(){
	if(! IsRunning()){
		return;
	}
	
	const deMutexGuard lock(pMutexWaiting);
	pExitThread = true;
}

deParallelTask *deParallelThread::GetTask(){
	const deMutexGuard lock(pMutexTask);
	return pTask;
}

void deParallelThread::Run(){
	while(true){
		// get the next task to process if there is any
		// 
		// NOTE potential dead-lock:
		//      - Run() locks task:pMutexTask
		//      - NextPendingTask locks pp:pMutexTasks
		//      - if NextPendingTask somehow manages to call GetTask() a dead lock happens
		//      - (same for AddFinishedTask)
		//      
		//      this situation should not be possible to happen. to prevent any possibility
		//      of a dead-lock the task is first acquired unlocked then written locked.
		{
		deParallelTask * const task = pParallelProcessing.NextPendingTask(pTakeLowPriorityTasks);
		
		const deMutexGuard lock(pMutexTask);
		pTask = task;
		
		if(pParallelProcessing.GetOutputDebugMessages()){
			if(pTask){
				const decString debugName(pTask->GetDebugName());
				const decString debugDetails(pTask->GetDebugDetails());
				pParallelProcessing.GetEngine().GetLogger()->LogInfoFormat(LOGSOURCE,
					"Thread %i: Assign [%s] %s", pNumber,
					debugName.GetString(), debugDetails.GetString());
				
			}else{
				pParallelProcessing.GetEngine().GetLogger()->LogInfoFormat(LOGSOURCE,
					"Thread %i: No task, going to sleep", pNumber);
			}
		}
		}
		
		// if there is a task process it
		if(pTask){
			try{
				pTask->Run();
				
			}catch(const deException &exception){
				const decString debugName(pTask->GetDebugName());
				const decString debugDetails(pTask->GetDebugDetails());
				pParallelProcessing.GetEngine().GetLogger()->LogErrorFormat(LOGSOURCE,
					"Thread %i: Task failed [%s] %s.\n", pNumber,
					debugName.GetString(), debugDetails.GetString());
				pParallelProcessing.GetEngine().GetLogger()->LogException(LOGSOURCE, exception);
				pTask->Cancel();  // tell task it failed
			}
			
			// send the finished task back
			if(pParallelProcessing.GetOutputDebugMessages()){
				const decString debugName(pTask->GetDebugName());
				const decString debugDetails(pTask->GetDebugDetails());
				pParallelProcessing.GetEngine().GetLogger()->LogInfoFormat(LOGSOURCE,
					"Thread %i: Finished Task [%s] %s.", pNumber,
					debugName.GetString(), debugDetails.GetString());
			}
			
			{
			deParallelTask *task;
			{
			const deMutexGuard lock(pMutexTask);
			task = pTask;
			pTask = nullptr;
			}
			pParallelProcessing.AddFinishedTask(task);
			}
			
		// otherwise go to sleep until new tasks become available
		}else{
			{
			const deMutexGuard lock(pMutexWaiting);
			pWaiting = true;
			if(pExitThread){
				break;
			}
			}
			
			pParallelProcessing.WaitOnNewTasksSemaphore();
			
			{
			const deMutexGuard lock(pMutexWaiting);
			pWaiting = false;
			if(pExitThread){
				break;
			}
			}
		}
	}
}
