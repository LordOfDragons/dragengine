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
#include "../systems/deModuleSystem.h"
#include "../systems/modules/deBaseModule.h"
#include "../systems/modules/deLoadableModule.h"
#include "../threading/deMutexGuard.h"



// Definitions
////////////////

#define LOGSOURCE "Parallel Processing"



// Class deParallelProcessing
///////////////////////////////

// Constructor, destructor
////////////////////////////

deParallelProcessing::deParallelProcessing(deEngine &engine) :
pEngine(engine),
pCoreCount(4),
pThreads(NULL),
pThreadCount(0),
pPaused(false),
pOutputDebugMessages(false)
{
	try{
		pDetectCoreCount();
		pCreateThreads(pCoreCount);
		
	}catch(const deException &){
		pCleanUp();
	}
}

deParallelProcessing::~deParallelProcessing(){
	pCleanUp();
}



// Management
///////////////

void deParallelProcessing::Update(){
	deMutexGuard lock(pMutexTasks);
	
	while(! pPaused){
		// we have to remove the finished task from the system before unlocking the mutex
		// otherwise Finished() call can potentially trigger actions on the system causing
		// invariants to be violated. since removing a task from the system drops the strong
		// reference we have to guard it here. this has a small performance penalty due to
		// mutex proteced reference counting but that is necessary
		if(pListFinishedTasks.GetCount() == 0){
			break;
		}
		
		const deParallelTask::Ref task((deParallelTask*)pListFinishedTasks.GetAt(0));
		pListFinishedTasks.RemoveFrom(0);
		pTasks.Remove((deParallelTask*)task);
		
		if(pOutputDebugMessages){
			const decString debugName(task->GetDebugName());
			const decString debugDetails(task->GetDebugDetails());
			pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Task.Finished() [%s] %s",
				debugName.GetString(), debugDetails.GetString());
		}
		
		// NOTE AddTask() and AddTaskAsync() do call Reset() which conflicts with SetFinished().
		//      using pMutexTasks lock prevents them colliding
		// 
		// NOTE it is possible to craft reusable tasks. this requires keeping track of tasks
		//      after Finished() is called. furthermore if the tasks is a dependency of another
		//      task special care has to be taken to not reuse the tasks until the other task
		//      finished working. deParallelProcessing does not take any precaution for reusable
		//      tasks to function properly. this is the responsability of the user
		
		task->RemoveAllDependsOn();
		
		lock.Unlock();
		
		try{
			task->Finished();
			
		}catch(const deException &exception){
			pEngine.GetLogger()->LogException(LOGSOURCE, exception);
		}
		
		lock.Lock();
		
		if(! task->GetMarkFinishedAfterRun()){
			task->SetFinished();
			pSemaphoreNewTasks.Signal();
		}
	}
}

void deParallelProcessing::Pause(){
	deMutexGuard lock(pMutexTasks);
	
	if(pPaused){
		return;
	}
	
	// wait for all threads to sleep
	pPaused = true;
	
	lock.Unlock();
	
	int i = 0;
	while(i < pThreadCount){
		// check if all threads finished
		for(i=0; i<pThreadCount; i++){
			if(! pThreads[i]->IsWaiting()){
				break;
			}
		}
		
		// if threads are still running notify modules to break through problematic situations
		if(i < pThreadCount){
			deModuleSystem &moduleSystem = *pEngine.GetModuleSystem();
			const int count = moduleSystem.GetModuleCount();
			int j;
			
			for(j=0; j<count; j++){
				deLoadableModule &loadableModule = *moduleSystem.GetModuleAt(j);
				if(loadableModule.IsLoaded() && loadableModule.GetModule()){
					loadableModule.GetModule()->PauseParallelProcessingUpdate();
				}
			}
			
		// if no threads are running but there are pending tasks run one pending task
		}else if(pProcessOneTaskDirect(true)){
			i = 0; // we need another round
		}
	}
}

void deParallelProcessing::Resume(){
	deMutexGuard lock(pMutexTasks);
	
	if(! pPaused){
		return;
	}
	
	pPaused = false;
	
	if(pListPendingTasks.GetCount() > 0 || pListPendingTasksLowPriority.GetCount() > 0){
		pSemaphoreNewTasks.SignalAll();
	}
}



void deParallelProcessing::WaitForTask(deParallelTask *task){
	if(pPaused){
		DETHROW(deeInvalidAction);
	}
	
	if(! task){
		DETHROW(deeInvalidParam);
	}
	
	if(pOutputDebugMessages){
		const decString debugName(task->GetDebugName());
		const decString debugDetails(task->GetDebugDetails());
		pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Wait for task [%s] %s",
					debugName.GetString(), debugDetails.GetString());
	}
	
	while(! pPaused){
		// check if task is finished
		bool finished = false;
		deMutexGuard lock(pMutexTasks);
		finished = task->GetFinished();
		lock.Unlock();
		
		// if finished do an update pass to process the task and go home
		if(finished){
			if(pOutputDebugMessages){
				const decString debugName(task->GetDebugName());
				const decString debugDetails(task->GetDebugDetails());
				pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Wait for task finished [%s] %s",
					debugName.GetString(), debugDetails.GetString());
			}
			Update(); // drops strong reference to task
			break;
			
		// since we do not take low priority tasks it is possible we get stuck because all
		// threads are waiting on the semaphore and we do not take them either. to solve
		// this the next task semaphore is signled here to allow tasks to continue processing
		// should all be waiting. only required to be done if pProcessOneTaskDirect() below
		// is called with 'false' parameter
		}else{
// 			pSemaphoreNewTasks.Signal();
		}
		
		// otherwise process one task in the main thread so we don't twiddle thumbs wasting
		// time waiting for the task to finish unless only low priority tasks are present
		pProcessOneTaskDirect(false);
	}
}

/*
void deParallelProcessing::WaitForTasks(deParallelTask **tasks, int count){
	if(pPaused){
		DETHROW(deeInvalidAction);
	}
	
	if(count == 0){
		return;
	}
	if(! tasks || count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pOutputDebugMessages){
		int i;
		for(i=0; i<count; i++){
			const decString debugName(tasks[i]->GetDebugName());
			const decString debugDetails(tasks[i]->GetDebugDetails());
			pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Wait for tasks %d [%s] %s", i,
				debugName.GetString(), debugDetails.GetString());
		}
	}
	
	int next = 0;
	
	while(! pPaused){
		// check if tasks are finished
		int lastNext = next;
		
		deMutexGuard lock(pMutexTasks);
		while(next < count && tasks[next]->GetFinished()){
			next++;
		}
		lock.Unlock();
		
		if(pOutputDebugMessages){
			while(lastNext < next){
				const decString debugName(tasks[lastNext]->GetDebugName());
				const decString debugDetails(tasks[lastNext]->GetDebugDetails());
				pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Wait for tasks finished %d [%s] %s",
					lastNext, debugName.GetString(), debugDetails.GetString());
				lastNext++;
			}
		}
		
		// if finished do an update pass to process the task and go home
		if(next == count){
			Update();
			break;
		}
		
		// otherwise process one task in the main thread so we don't twiddle thumbs wasting
		// time waiting for the task to finish unless only low priority tasks are present
		pProcessOneTaskDirect(false);
	}
}
*/



void deParallelProcessing::AddTask(deParallelTask *task){
	// process finished tasks to keep the flow going. this call has to be outside the mutex
	// lock since it locks the mutex itself
	Update();
	
	// all the rest has to be mutex protected
	AddTaskAsync(task);
}

void deParallelProcessing::AddTaskAsync(deParallelTask *task){
	if(! task){
		DETHROW(deeInvalidParam);
	}
	
	const deMutexGuard lock(pMutexTasks);
	
	pTasks.Add(task); // strong reference held until task leaves parallel task system
	
	task->Reset(); // mark not cancelled and not finished. collides with SetFinished()
	
	if(task->GetLowPriority()){
		pListPendingTasksLowPriority.Add(task);
		
	}else{
		pListPendingTasks.Add(task);
	}
	
	if(pOutputDebugMessages){
		pLogTask("AddTask ", "  ", *task);
	}
	
	if(! pPaused){
		pSemaphoreNewTasks.Signal();
		
	}else{
		// parallel processing is paused. we have to process the task now otherwise the caller
		// potentially dead-locks if resuming depends on this task finishing
		pEnsureRunTaskNow(task);
	}
}

void deParallelProcessing::FinishAndRemoveTasksOwnedBy(deBaseModule *module){
	if(! module){
		DETHROW(deeInvalidParam);
	}
	
	// cancel pending tasks owned by module
	deMutexGuard lock(pMutexTasks);
	
	int i, count = pListPendingTasks.GetCount();
	for(i=0; i<count; i++){
		deParallelTask * const task = (deParallelTask*)pListPendingTasks.GetAt(i);
		if(task->GetOwner() == module){
			task->Cancel();
		}
	}
	
	count = pListPendingTasksLowPriority.GetCount();
	for(i=0; i<count; i++){
		deParallelTask * const task = (deParallelTask*)pListPendingTasksLowPriority.GetAt(i);
		if(task->GetOwner() == module){
			task->Cancel();
		}
	}
	
	// make sure the tasks are finished otherwise
	// strange problems can happen with certain tasks
	if(pPaused){
		int nextIndex = 0;
		while(pListPendingTasks.GetCount() > nextIndex){
			deParallelTask * const task = (deParallelTask*)pListPendingTasks.GetAt(nextIndex);
			
			if(task->IsCancelled()){
				pListPendingTasks.RemoveFrom(nextIndex);
				
				if(task->GetMarkFinishedAfterRun()){
					task->SetFinished();
				}
				
				pListFinishedTasks.Add(task);
				
			}else{
				nextIndex++;
			}
		}
		
		nextIndex = 0;
		while(pListPendingTasksLowPriority.GetCount() > nextIndex){
			deParallelTask * const task = (deParallelTask*)pListPendingTasksLowPriority.GetAt(nextIndex);
			
			if(task->IsCancelled()){
				pListPendingTasksLowPriority.RemoveFrom(nextIndex);
				
				if(task->GetMarkFinishedAfterRun()){
					task->SetFinished();
				}
				
				pListFinishedTasks.Add(task);
				
			}else{
				nextIndex++;
			}
		}
		
		while(pListFinishedTasks.GetCount() > 0){
			// we have to remove the finished task from the system before unlocking the mutex
			// otherwise Finished() call can potentially trigger actions on the system causing
			// invariants to be violated. since removing a task from the system drops the strong
			// reference we have to guard it here. this has a small performance penalty due to
			// mutex proteced reference counting but that is necessary
			const deParallelTask::Ref task((deParallelTask*)pListFinishedTasks.GetAt(0));
			pListFinishedTasks.RemoveFrom(0);
			pTasks.Remove((deParallelTask*)task);
			
			task->RemoveAllDependsOn();
			
			lock.Unlock();
			
			try{
				task->Finished();
				
			}catch(const deException &exception){
				pEngine.GetLogger()->LogException(LOGSOURCE, exception);
			}
			
			lock.Lock();
			
			if(! task->GetMarkFinishedAfterRun()){
				task->SetFinished();
			}
		}
		
	// cancel and wait for tasks in progress owned by module if not paused
	}else{
		for(i=0; i<pThreadCount; i++){
			deParallelTask * const task = pThreads[i]->GetTask();
			if(task && task->GetOwner() == module){
				task->Cancel();
				
				lock.Unlock();
				
				try{
					WaitForTask(task); // removes strong reference
					
				}catch(const deException &e){
					// do not throw since this is usually called during module shutdown
					pEngine.GetLogger()->LogException(LOGSOURCE, e);
				}
				
				lock.Lock();
			}
		}
	}
	
	// here all strong references to matching tasks have been dropped. no further cleaning
	// is required. we could though add here a check later on to make double sure of this
	
	/*
	for(i=0; i<pListPendingTasks.GetCount(); i++){
		deParallelTask * const task = (deParallelTask*)pListPendingTasks.GetAt(i);
		if(task->GetOwner() != module){
			continue;
		}
		
		task->RemoveFromAllDependedOnTasks();
		pListPendingTasks.RemoveFrom(i);
		i--;
	}
	
	for(i=0; i<pListPendingTasksLowPriority.GetCount(); i++){
		deParallelTask * const task = (deParallelTask*)pListPendingTasksLowPriority.GetAt(i);
		if(task->GetOwner() != module){
			continue;
		}
		
		task->RemoveFromAllDependedOnTasks();
		pListPendingTasksLowPriority.RemoveFrom(i);
		i--;
	}
	
	for(i=0; i<pListFinishedTasks.GetCount(); i++){
		deParallelTask * const task = (deParallelTask*)pListFinishedTasks.GetAt(i);
		if(task->GetOwner() == module){
			continue;
		}
		
		task->RemoveFromAllDependedOnTasks();
		pListFinishedTasks.RemoveFrom(i);
		i--;
	}
	*/
}

void deParallelProcessing::FinishAndRemoveAllTasks(){
	// cancel pending tasks
	deMutexGuard lock(pMutexTasks);
	
	int i, count = pListPendingTasks.GetCount();
	for(i=0; i<count; i++){
		((deParallelTask*)pListPendingTasks.GetAt(i))->Cancel();
	}
	
	count = pListPendingTasksLowPriority.GetCount();
	for(i=0; i<count; i++){
		((deParallelTask*)pListPendingTasksLowPriority.GetAt(i))->Cancel();
	}
	
	// make sure the tasks are finished otherwise strange problems can happen with certain tasks
	if(pPaused){
		int nextIndex = 0;
		while(pListPendingTasks.GetCount() > nextIndex){
			deParallelTask * const task = (deParallelTask*)pListPendingTasks.GetAt(nextIndex);
			
			if(task->IsCancelled()){
				pListPendingTasks.RemoveFrom(nextIndex);
				
				if(task->GetMarkFinishedAfterRun()){
					task->SetFinished();
				}
				
				pListFinishedTasks.Add(task);
				
			}else{
				nextIndex++;
			}
		}
		
		nextIndex = 0;
		while(pListPendingTasksLowPriority.GetCount() > nextIndex){
			deParallelTask * const task = (deParallelTask*)pListPendingTasksLowPriority.GetAt(nextIndex);
			
			if(task->IsCancelled()){
				pListPendingTasksLowPriority.RemoveFrom(nextIndex);
				
				if(task->GetMarkFinishedAfterRun()){
					task->SetFinished();
				}
				
				pListFinishedTasks.Add(task);
				
			}else{
				nextIndex++;
			}
		}
		
		while(pListFinishedTasks.GetCount() > 0){
			// we have to remove the finished task from the system before unlocking the mutex
			// otherwise Finished() call can potentially trigger actions on the system causing
			// invariants to be violated. since removing a task from the system drops the strong
			// reference we have to guard it here. this has a small performance penalty due to
			// mutex proteced reference counting but that is necessary
			const deParallelTask::Ref task((deParallelTask*)pListFinishedTasks.GetAt(0));
			pListFinishedTasks.RemoveFrom(0);
			pTasks.Remove((deParallelTask*)task);
			
			task->RemoveAllDependsOn();
			
			lock.Unlock();
			
			try{
				task->Finished();
				
			}catch(const deException &exception){
				pEngine.GetLogger()->LogException(LOGSOURCE, exception);
			}
			
			lock.Lock();
			
			if(! task->GetMarkFinishedAfterRun()){
				task->SetFinished();
			}
		}
		
	// cancel and wait for all tasks in progress if not paused
	}else{
		for(i=0; i<pThreadCount; i++){
			deParallelTask * const task = pThreads[i]->GetTask();
			if(! task){
				continue;
			}
			
			task->Cancel();
			
			lock.Unlock();
			
			try{
				WaitForTask(task); // removes strong reference
				
			}catch(const deException &e){
				// do not throw since this is usually called during module shutdown
				pEngine.GetLogger()->LogException(LOGSOURCE, e);
			}
			
			lock.Lock();
		}
	}
}



// deParallelThread Only
//////////////////////////

deParallelTask *deParallelProcessing::NextPendingTask(bool takeLowPriorityTasks){
	// NOTE
	// this method is called from worker threads and potentially the main thread.
	// this especially means it is not allowed to modify depends-on of tasks here
	// not adding or releasing task references
	
	if(pPaused){
		return NULL;
	}
	
	int nextIndex = 0;
	
	deMutexGuard lock(pMutexTasks);
	
	while(pListPendingTasks.GetCount() > nextIndex){
		deParallelTask * const task = (deParallelTask*)pListPendingTasks.GetAt(nextIndex);
		
		if(task->IsCancelled()){
			pListPendingTasks.RemoveFrom(nextIndex);
			
			if(task->GetMarkFinishedAfterRun()){
				task->SetFinished();
				pSemaphoreNewTasks.Signal();
			}
			
			pListFinishedTasks.Add(task);
			
		}else if(task->CanRun()){
			pListPendingTasks.RemoveFrom(nextIndex);
			
			if(task->GetEmptyRun()){
				// task has been marked has having no run implementation. we can optimize
				// this case by not sending the task to the thread but instead moving it
				// straight to the finished list
				if(task->GetMarkFinishedAfterRun()){
					task->SetFinished();
					pSemaphoreNewTasks.Signal();
				}
				
				pListFinishedTasks.Add(task);
				
			}else{
				//pSemaphoreNewTasks.TryWait();
				return task;
			}
			
		}else{
			nextIndex++; // this task is blocked, try the next one
		}
	}
	
	// low priority task only if the tasks accepts
	if(! takeLowPriorityTasks){
		// NOTE if only one thread is called and this thread happens to not take low priority
		//      tasks but there are some then we can end up dead-locking since this thread
		//      goes to sleep and the others able to take it are not woken up. to avoid this
		//      problem we wake up another thread.
		//      
		//      right now only one thread does not take low priority tasks to keep important
		//      threads running. in this case waking up one thread will wake up one which can
		//      take the low priority task. if more than one thread are not taking low priority
		//      tasks then there is the potential risk of ping-pong between two threads not
		//      taking low priority tasks. i doubt though this can cause a problem on regular
		//      hardware. should this though be a problem using SignalAll() instead of Signal()
		//      can help. using SignalAll() too often can though cause the counter to sky-rocket.
		if(pListPendingTasksLowPriority.GetCount() > 0){
			pSemaphoreNewTasks.Signal();
		}
		return NULL;
	}
	
	nextIndex = 0;
	
	while(pListPendingTasksLowPriority.GetCount() > nextIndex){
		deParallelTask * const task = (deParallelTask*)pListPendingTasksLowPriority.GetAt(nextIndex);
		
		if(task->IsCancelled()){
			pListPendingTasksLowPriority.RemoveFrom(nextIndex);
			
			if(task->GetMarkFinishedAfterRun()){
				task->SetFinished();
				pSemaphoreNewTasks.Signal();
			}
			
			pListFinishedTasks.Add(task);
			
		}else if(task->CanRun()){
			pListPendingTasksLowPriority.RemoveFrom(nextIndex);
			
			if(task->GetEmptyRun()){
				// task has been marked has having no run implementation. we can optimize
				// this case by not sending the task to the thread but instead moving it
				// straight to the finished list
				if(task->GetMarkFinishedAfterRun()){
					task->SetFinished();
					pSemaphoreNewTasks.Signal();
				}
				
				pListFinishedTasks.Add(task);
				
			}else{
				//pSemaphoreNewTasks.TryWait();  // not a good idea
				return task;
			}
			
		}else{
			nextIndex++; // this task is blocked, try the next one
		}
	}
	
	return NULL;
}

void deParallelProcessing::WaitOnNewTasksSemaphore(){
	pSemaphoreNewTasks.Wait();
}

void deParallelProcessing::AddFinishedTask(deParallelTask *task){
	if(! task){
		DETHROW(deeInvalidParam);
	}
	
	deMutexGuard lock(pMutexTasks);
	
	if(task->GetMarkFinishedAfterRun()){
		task->SetFinished();
		pSemaphoreNewTasks.Signal();
		// NOTE usually the calling thread is going to call NextPendingTask() after exiting this
		//      call. if AddFinishedTask() is called by a WaitForTask*() call and the waiting
		//      condition is fulfille then the WaitForTask*() call exits without calling
		//      NextPendingTask(). in this situation it can happen tasks are still pending but
		//      because all threads are sleeping already the remaining tasks are not processed
		//      anymore. in certain situations this can lead to dead-locks. for this reason
		//      the semaphore is signaled here always to avoid this situation. the worst that
		//      can happen is a thread waking up just to find no work to do and go sleeping.
		//      important is that processing of pending tasks never stops if there are tasks
		//      present that could be run
	}
	
	pListFinishedTasks.Add(task);
}



// Debugging
//////////////

void deParallelProcessing::LogThreadAndTasks(){
	deMutexGuard lock(pMutexTasks);
	
	const char * const paused = pPaused ? " (paused)" : "";
	deLogger &logger = *pEngine.GetLogger();
	decString taskName, taskDetails;
	int i, count;
	
	logger.LogInfoFormat(LOGSOURCE, "Parallel Processing%s - Threads:", paused);
	for(i=0; i<pThreadCount; i++){
		const deParallelTask * const task = pThreads[i]->GetTask();
		if(task){
			pLogTask("- Running ", "  ", *task);
			
		}else{
			logger.LogInfo(LOGSOURCE, "- Waiting");
		}
	}
	
	logger.LogInfoFormat(LOGSOURCE, "Parallel Processing%s - Finished Tasks:", paused);
	count = pListFinishedTasks.GetCount();
	for(i=0; i<count; i++){
		pLogTask("- ", "  ", *((const deParallelTask *)pListFinishedTasks.GetAt(i)));
	}
	
	logger.LogInfoFormat(LOGSOURCE, "Parallel Processing%s - Pending Tasks:", paused);
	count = pListPendingTasks.GetCount();
	for(i=0; i<count; i++){
		pLogTask("- ", "  ", *((const deParallelTask *)pListPendingTasks.GetAt(i)));
	}
	
	logger.LogInfoFormat(LOGSOURCE, "Parallel Processing%s - Pending Low Priority Tasks:", paused);
	count = pListPendingTasksLowPriority.GetCount();
	for(i=0; i<count; i++){
		pLogTask("- ", "  ", *((const deParallelTask *)pListPendingTasksLowPriority.GetAt(i)));
	}
}



// Private Functions
//////////////////////

void deParallelProcessing::pCleanUp(){
	pStopAllThreads();
	pDestroyThreads();
	
	pListPendingTasks.RemoveAll();
	pListPendingTasksLowPriority.RemoveAll();
	pListFinishedTasks.RemoveAll();
	
	pTasks.RemoveAll(); // drops all strong references
}

#ifdef OS_UNIX
#include <unistd.h>
#endif

void deParallelProcessing::pDetectCoreCount(){
	#ifdef OS_W32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	pCoreCount = sysinfo.dwNumberOfProcessors;
	#endif
	
	#ifdef OS_UNIX
	pCoreCount = sysconf(_SC_NPROCESSORS_ONLN);
	#endif
	
	if(pCoreCount < 1){
		pCoreCount = 1;
	}
}

void deParallelProcessing::pCreateThreads(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	if(count == 0){
		return;
	}
	
	pThreads = new deParallelThread*[count];
	
	while(pThreadCount < count){
		pThreads[pThreadCount] = new deParallelThread(*this, pThreadCount, pThreadCount > 0);
		pThreadCount++;
		
		#ifdef OS_BEOS
		decString name;
		name.Format("ParallelProcessing#%d", pThreadCount - 1);
		pThreads[pThreadCount - 1]->SetName(name);
		#endif
		
		pThreads[pThreadCount - 1]->Start();
	}
	
	if(pOutputDebugMessages){
		pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Created %i worker threads", pThreadCount);
	}
}

void deParallelProcessing::pDestroyThreads(){
	if(! pThreads){
		return;
	}
	
	while(pThreadCount > 0){
		pThreadCount--;
		delete pThreads[pThreadCount];
	}
	delete [] pThreads;
	
	pThreads = NULL;
	pThreadCount = 0;
	
	if(pOutputDebugMessages){
		pEngine.GetLogger()->LogInfo(LOGSOURCE, "Destroyed worker threads");
	}
}

void deParallelProcessing::pStopAllThreads(){
	int i;
	
	if(pOutputDebugMessages){
		pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Stopping %i thread", pThreadCount);
	}
	
	// tell all threads to exit
	for(i=0; i<pThreadCount; i++){
		pThreads[i]->RequestExit();
	}
	
	// wake them all up so they all see the exit request and can properly exit
	pSemaphoreNewTasks.SignalAll();
	
	// wait for all of them to stop
	for(i=0; i<pThreadCount; i++){
		pThreads[i]->WaitForExit();
	}
}



bool deParallelProcessing::pProcessOneTaskDirect(bool takeLowPriorityTasks){
	deParallelTask * const task = NextPendingTask(takeLowPriorityTasks);
	if(! task){
		return false;
	}
	
	if(pOutputDebugMessages){
		const decString debugName(task->GetDebugName());
		const decString debugDetails(task->GetDebugDetails());
		pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "ProcessOneTask [%s] %s",
				debugName.GetString(), debugDetails.GetString());
	}
	
	try{
		task->Run();
		
	}catch(const deException &exception){
		const decString debugName(task->GetDebugName());
		const decString debugDetails(task->GetDebugDetails());
		pEngine.GetLogger()->LogErrorFormat(LOGSOURCE, "ProcessOneTask Failed [%s] %s",
			debugName.GetString(), debugDetails.GetString());
		pEngine.GetLogger()->LogException(LOGSOURCE, exception);
		task->Cancel();  // tell task it failed
	}
	
	// send the finished task back
	if(pOutputDebugMessages){
		const decString debugName(task->GetDebugName());
		const decString debugDetails(task->GetDebugDetails());
		pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "ProcessOneTask Finished [%s] %s",
			debugName.GetString(), debugDetails.GetString());
	}
	
	AddFinishedTask(task);
	return true;
}

void deParallelProcessing::pEnsureRunTaskNow(deParallelTask *task){
	bool deadLoopCheck = false;
	
	while(! task->GetFinished()){
		if(task->IsCancelled()){
			if(task->GetLowPriority()){
				pListPendingTasksLowPriority.RemoveFrom(pListPendingTasksLowPriority.IndexOf(task));
				
			}else{
				pListPendingTasks.RemoveFrom(pListPendingTasks.IndexOf(task));
			}
			
			if(task->GetMarkFinishedAfterRun()){
				task->SetFinished();
			}
			
			pListFinishedTasks.Add(task);
			return;
		}
		
		if(task->CanRun()){
			if(pOutputDebugMessages){
				const decString debugName(task->GetDebugName());
				const decString debugDetails(task->GetDebugDetails());
				pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "pEnsureRunTaskNow [%s] %s",
						debugName.GetString(), debugDetails.GetString());
			}
			
			try{
				task->Run();
				
			}catch(const deException &exception){
				const decString debugName(task->GetDebugName());
				const decString debugDetails(task->GetDebugDetails());
				pEngine.GetLogger()->LogErrorFormat(LOGSOURCE, "pEnsureRunTaskNow Failed [%s] %s",
					debugName.GetString(), debugDetails.GetString());
				pEngine.GetLogger()->LogException(LOGSOURCE, exception);
				task->Cancel();  // tell task it failed
			}
			
			// send the finished task back
			if(pOutputDebugMessages){
				const decString debugName(task->GetDebugName());
				const decString debugDetails(task->GetDebugDetails());
				pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "pEnsureRunTaskNow Finished [%s] %s",
						debugName.GetString(), debugDetails.GetString());
			}
			
			if(task->GetLowPriority()){
				pListPendingTasksLowPriority.RemoveFrom(pListPendingTasksLowPriority.IndexOf(task));
				
			}else{
				pListPendingTasks.RemoveFrom(pListPendingTasks.IndexOf(task));
			}
			
			if(task->GetMarkFinishedAfterRun()){
				task->SetFinished();
			}
			pListFinishedTasks.Add(task);
			return;
		}
		
		const int count = task->GetDependsOnCount();
		deParallelTask *deptask = NULL;
		int i;
		
		for(i=0; i<count; i++){
			deptask = task->GetDependsOnAt(i);
			if(! deptask->GetFinished() && ! deptask->IsCancelled()){
				break;
			}
			deptask = NULL;
		}
		
		if(deptask){
			deadLoopCheck = false;
			pEnsureRunTaskNow(deptask);
			
		}else if(deadLoopCheck){
			task->Cancel();
			
		}else{
			deadLoopCheck = true;
		}
	}
}



void deParallelProcessing::pLogTask(const char *prefix, const char *contPrefix,
const deParallelTask &task){
	deLogger &logger = *pEngine.GetLogger();
	const decString taskName(task.GetDebugName());
	const decString taskDetails(task.GetDebugDetails());
	
	logger.LogInfoFormat(LOGSOURCE, "%s%s[%c%c%c]: %s", prefix, taskName.GetString(),
		task.GetFinished() ? 'F' : ' ', task.IsCancelled() ? 'C' : ' ',
		task.GetLowPriority() ? 'L' : ' ', taskDetails.GetString());
	
	const int count = task.GetDependsOnCount();
	const decString scontPrefix(contPrefix);
	decString dependPrefix, dependContPrefix;
	int i;
	
	for(i=0; i<count; i++){
		if(i < count - 1){
			dependPrefix = scontPrefix + "+ ";
			dependContPrefix = scontPrefix + "| ";
			
		}else{
			dependPrefix = scontPrefix + "+ ";
			dependContPrefix = scontPrefix + "  ";
		}
		
		pLogTask(dependPrefix, dependContPrefix, *task.GetDependsOnAt(i));
	}
}



#if 0
void deParallelProcessing::WaitForAllTasks(){
	if(pPaused){
		DETHROW(deeInvalidAction);
	}
	
	while(! pPaused){
		// check if all threads are sleeping
		bool finished = true;
		int i;
		for(i=0; i<pThreadCount; i++){
			if(! pThreads[i]->IsWaiting()){
				finished = false;
				break;
			}
		}
		
		// if finished do an update pass to process the task and go home
		if(finished){
			Update();
			break;
		}
		
		// otherwise process one task in the main thread so we don't
		// twiddle thumbs wasting time waiting for the task to finish
		pProcessOneTaskDirect();
	}
}
#endif
