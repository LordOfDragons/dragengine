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

#ifndef _DETHREAD_H_
#define _DETHREAD_H_

#include "../dragengine_configuration.h"

#include "deMutex.h"

#ifdef OS_BEOS
#include <os/kernel/OS.h>
#include "../common/string/decString.h"
#endif


/**
 * \brief Thread.
 *
 * Runs a thread. Thin layer over the operating system thread support.
 * Subclass to add your own run function.
 */
class DE_DLL_EXPORT deThread{
private:
#if defined OS_UNIX || defined OS_BEOS
	pthread_t pThread;
#endif
#ifdef OS_W32
	HANDLE pThread;
#endif
	bool pCreated;
	bool pRunning;
	deMutex pMutexState;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create thread. */
	deThread();
	
	/** \brief Cleans the thread. */
	virtual ~deThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Thread is running. */
	bool IsRunning();
	
	/** \brief Start thread if not running already. */
	void Start();
	
#ifdef OS_WEBWASM
	/** \brief Start thread if not running already with thread attributes. */
	void Start(pthread_attr_t &attributes);
#endif
	
	/**
	 * \brief Stop thread.
	 * 
	 * Returns as soon as the thread stopped.
	 */
	void Stop();
	
	/**
	 * \brief Wait for thread to end and then returns.
	 * 
	 * This is the safe way to end a thread especially if mutices or semaphores are used.
	 * Make sure the thread ends soon or else this call deadlocks your application.
	 */
	void WaitForExit();
	
	/**
	 * \brief Run function of the thread.
	 * 
	 * Overwrite to implement what the threa is supposed to be doing. Once this function
	 * ends the thread is stopped.
	 */
	virtual void Run() = 0;
	
	#ifdef OS_BEOS
	/** \brief Set name of thread used for debugging. */
	void SetName(const char *name);
	#endif
	/*@}*/
	
	
	
protected:
	/**
	 * \brief State mutex.
	 * 
	 * Locked while changing thread state.
	 */
	inline deMutex &GetMutexState(){return pMutexState;}
	
	
private:
	#if defined OS_UNIX || defined OS_BEOS
	static void *pThreadRunner(void *parameter);
	#endif
	
	#ifdef OS_W32
	static DWORD WINAPI pThreadRunner(LPVOID parameter);
	#endif
	
	#ifdef OS_BEOS
	decString pName;
	thread_id pBeOSThreadID;
	#endif
};

#endif
