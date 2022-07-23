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

#ifndef _DEPARALLELTHREAD_H_
#define _DEPARALLELTHREAD_H_

#include "../threading/deMutex.h"
#include "../threading/deThread.h"

class deParallelProcessing;
class deParallelTask;


/**
 * \brief Parallel processing thread.
 * 
 * Thread stores only a weak reference to the task in progress. Only deParallelProcessing is
 * storing strong references to tasks.
 */
class DE_DLL_EXPORT deParallelThread : public deThread{
private:
	deParallelProcessing &pParallelProcessing;
	
	int pNumber;
	bool pTakeLowPriorityTasks;
	
	deMutex pMutexTask;
	bool pExitThread;
	
	deMutex pMutexWaiting;
	bool pWaiting;
	
	deParallelTask *pTask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create thread. */
	deParallelThread( deParallelProcessing &parallelProcessing, int number, bool takeLowPriorityTasks );
	
	/** \brief Clean up thread. */
	virtual ~deParallelThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Thread number. */
	inline int GetNumber() const{ return pNumber; }
	
	/** \brief Thread takes low priority tasks. */
	inline bool GetTakeLowPriorityTasks() const{ return pTakeLowPriorityTasks; }
	
	
	
	/** \brief Thread is waiting. */
	bool IsWaiting();
	
	/** \brief Request the thread to exit. */
	void RequestExit();
	
	
	
	/** \brief Assigned task. */
	deParallelTask *GetTask();
	inline deParallelTask *GetTaskDebug() const{ return pTask; }
	
	
	
	/** \brief Run task. */
	virtual void Run();
	/*@}*/
};

#endif
