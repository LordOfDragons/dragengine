/* 
 * Drag[en]gine OpenAL Audio Module
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
	deoalWaitForParallelTasks( int taskCount );
	
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
