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

#ifndef _DEPARALLELPROCESSING_H_
#define _DEPARALLELPROCESSING_H_

#include "deParallelTask.h"
#include "../common/string/decStringList.h"
#include "../common/collection/decPointerList.h"
#include "../threading/deMutex.h"
#include "../threading/deSemaphore.h"

class deParallelThread;
class deEngine;
class deBaseModule;


/**
 * \brief Parallel task processing.
 */
class DE_DLL_EXPORT deParallelProcessing{
private:
	deEngine &pEngine;
	int pCoreCount;
	
	deParallelThread **pThreads;
	int pThreadCount;
	bool pPaused;
	
	deParallelTask::TaskList pTasks;
	deParallelTask::TaskPointerList pListPendingTasks;
	deParallelTask::TaskPointerList pListPendingTasksLowPriority;
	deParallelTask::TaskPointerList pListFinishedTasks;
	deMutex pMutexTasks;
	deSemaphore pSemaphoreNewTasks;
	
	bool pOutputDebugMessages;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parallel task processor. */
	deParallelProcessing(deEngine &engine);
	
	/** \brief Clean up parallel task processor. */
	~deParallelProcessing();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game engine. */
	inline deEngine &GetEngine() const{ return pEngine; }
	
	/** \brief Count of detected CPU cores. */
	inline int GetCoreCount() const{ return pCoreCount; }
	
	/** \brief Count of threads. */
	inline int GetThreadCount() const{ return pThreadCount; }
	
	/**
	 * \brief Update task processing.
	 * 
	 * Call once a frame to update tasks. Assigns tasks to waiting threads and promotes
	 * finished tasks to the finished queue. Called by the game engine before the
	 * scripting module does frame updates.
	 */
	void Update();
	
	/**
	 * \brief Parallel processing is paused.
	 * 
	 * All threads are waiting. It is safe to modify the game engine.
	 */
	inline bool GetPaused() const{ return pPaused; }
	
	/**
	 * \brief Pause parallel processing.
	 * 
	 * Finishes running all threads and does not start new tasks. After this call returns
	 * All threads are waiting and it is safe to modify the game engine.
	 */
	void Pause();
	
	/**
	 * \brief Resume parallel processing.
	 * 
	 * Restarts all threads commencing processing tasks if any are pending.
	 */
	void Resume();
	
	
	
	/**
	 * \brief Wait for a task to finish.
	 * 
	 * Blocks until the task is finished.
	 * \throws deeInvalidAction Parallel processing is paused.
	 * \throws deeInvalidParam \em task is NULL.
	 */
	void WaitForTask(deParallelTask *task);
	
	/**
	 * \brief Wait for multiple tasks to finish.
	 * 
	 * Blocks until the tasks are finished.
	 * 
	 * \warning Call only from the <em>main thread</em>! Never call from other threads!
	 * \throws deeInvalidAction Parallel processing is paused.
	 * \throws deeInvalidParam \em task is NULL.
	 */
// 	void WaitForTasks( deParallelTask **tasks, int count );
	
	
	
	/**
	 * \brief Add task to run the next time possible.
	 * 
	 * \warning Adding a task starts processing it immediately if a thread is ready.
	 * Add all dependencies before adding the task otherwise it can be run
	 * although the dependencies added later on are not finished.
	 * 
	 * \warning Call only from the <em>main thread</em>! Never call from other threads!
	 * \throws deeInvalidParam \em task is NULL.
	 */
	void AddTask(deParallelTask *task);
	
	/**
	 * \brief Add task to run the next time possible.
	 * 
	 * \warning Adding a task starts processing it immediately if a thread is ready.
	 * Add all dependencies before adding the task otherwise it can be run
	 * although the dependencies added later on are not finished.
	 * 
	 * \note Safe to be called from all kinds of threads.
	 * 
	 * \note If parallel processing is paused the task is run immediately without
	 *       being queued. This avoids synchronization issues.
	 * \throws deeInvalidParam \em task is NULL.
	 */
	void AddTaskAsync(deParallelTask *task);
	
	/**
	 * \brief Finish threads owned by module removing them from parallel processing.
	 * 
	 * To be called only by module systems during shut-down phase.
	 * 
	 * If a task is still pending it is first cancelled and waited for completion.
	 * This method has to be called to prevent accessing invalid memory if modules
	 * are unloaded for which tasks still exist. Cancelling the task is not enough
	 * since it stays in the list of finished tasks until finished.
	 */
	void FinishAndRemoveTasksOwnedBy(deBaseModule *module);
	
	/**
	 * \brief Finish all threads removing them from parallel processing.
	 * 
	 * To be called only by game engine during shut-down phase.
	 * 
	 * If a task is still pending it is first cancelled and waited for completion.
	 * This method has to be called to prevent accessing invalid memory if modules
	 * are unloaded for which tasks still exist. Cancelling the task is not enough
	 * since it stays in the list of finished tasks until finished.
	 */
	void FinishAndRemoveAllTasks();
	/*@}*/
	
	
	
	/** \name deParallelThread Only */
	/*@{*/
	/**
	 * \brief Next pending task or NULL if there is none.
	 * \warning For use by deParallelTask only.
	 */
	deParallelTask *NextPendingTask(bool takeLowPriorityTasks);
	
	/**
	 * \brief Wait on the new tasks semaphore.
	 * \warning For use by deParallelTask only.
	 */
	void WaitOnNewTasksSemaphore();
	
	/**
	 * \brief Add task to the list of finished tasks.
	 * \warning For use by deParallelTask only.
	 */
	void AddFinishedTask(deParallelTask *task);
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Debug messages are output to the engine logger. */
	inline bool GetOutputDebugMessages() const{ return pOutputDebugMessages; }
	
	/** \brief Log thread and task overview. */
	void LogThreadAndTasks();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pDetectCoreCount();
	void pCreateThreads(int count);
	void pDestroyThreads();
	void pStopAllThreads();
	
	bool pProcessOneTaskDirect(bool takeLowPriorityTasks);
	void pEnsureRunTaskNow(deParallelTask *task);
	
	void pLogTask(const char *prefix, const char *contPrefix, const deParallelTask &task);
};

#endif
