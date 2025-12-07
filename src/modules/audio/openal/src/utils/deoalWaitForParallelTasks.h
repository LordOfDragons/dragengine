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

#ifndef _DEOALWAITFORPARALLELTASKS_H_
#define _DEOALWAITFORPARALLELTASKS_H_

#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deSemaphore.h>


/**
 * \brief Asynchronous wait for parallel tasks.
 * 
 * Stores an integer counted protected by a mutex and a semaphore. To use this counter set
 * the counter value to the number of parallel tasks to wait for. Then start the parallel
 * tasks and wait on the semaphore. Whenever a parallel task finishes reduce the counter
 * by one. If the counter reaches 0 the semaphore is signled and the thread woken up.
 */
class deoalWaitForParallelTasks{
private:
	deMutex pMutex;
	deSemaphore pSemaphore;
	bool pWaiting;
	int pTaskCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create waiting for parallel tasks. */
	deoalWaitForParallelTasks(int taskCount);
	
	/** \brief Clean up waiting for parallel tasks. */
	~deoalWaitForParallelTasks();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Wait on semaphore. */
	void WaitForTasks();
	
	/** \brief Parallel task finished. */
	void TaskFinished();
	/*@}*/
};

#endif
