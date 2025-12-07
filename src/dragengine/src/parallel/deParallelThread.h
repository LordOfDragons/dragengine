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
	deParallelThread(deParallelProcessing &parallelProcessing, int number, bool takeLowPriorityTasks);
	
	/** \brief Clean up thread. */
	virtual ~deParallelThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Thread number. */
	inline int GetNumber() const{return pNumber;}
	
	/** \brief Thread takes low priority tasks. */
	inline bool GetTakeLowPriorityTasks() const{return pTakeLowPriorityTasks;}
	
	
	
	/** \brief Thread is waiting. */
	bool IsWaiting();
	
	/** \brief Request the thread to exit. */
	void RequestExit();
	
	
	
	/** \brief Assigned task. */
	deParallelTask *GetTask();
	inline deParallelTask *GetTaskDebug() const{return pTask;}
	
	
	
	/** \brief Run task. */
	virtual void Run();
	/*@}*/
};

#endif
