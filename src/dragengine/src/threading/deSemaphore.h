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

#ifndef _DESEMAPHORE_H_
#define _DESEMAPHORE_H_

#include "../dragengine_export.h"
#include "../dragengine_configuration.h"

#if defined OS_UNIX || defined OS_BEOS
#include <pthread.h>
#endif

#ifdef OS_W32
#include "../app/include_windows.h"
#endif


/**
 * \brief Integer based semaphore.
 * 
 * This class also counts the number of signals called to ensure the same number (or more)
 * wait calls are returned no matter in what order these calls are issued.
 */
class DE_DLL_EXPORT deSemaphore{
private:
	int pCounter;
	int pSignalCounter;
	
	#if defined OS_UNIX || defined OS_BEOS
	pthread_mutex_t pMutex;
	pthread_cond_t pCondition;
	#endif
	
	#ifdef OS_W32
	HANDLE pEvent;
	CRITICAL_SECTION pCSWaitCounter;
	#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create semaphore with initial count 0. */
	deSemaphore();
	
	/**
	 * \brief Create semaphore with initial count.
	 * \throws deeInvalidParam \em value is less than 0.
	 */
	deSemaphore(int count);
	
	/** \brief Clean up semaphore. */
	~deSemaphore();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Decrement counter by one and blocks thread if counter is now negative. */
	void Wait();
	
	/**
	 * \brief If counter is larger than 0 decrement counter by one.
	 * 
	 * This call is useful to decrement if possible ensuring the thread never blocks.
	 * 
	 * \returns true if counter has been decremented or false if the thread would have been blocked.
	 */
	bool TryWait();
	
	/** \brief Increment counter unblocking one thread if counter has been negative. */
	void Signal();
	
	/**
	 * \brief If counter is negative set counter to 0 and wake up all waiting threads.
	 * 
	 * Same as calling Signal() -counter times.
	 */
	void SignalAll();
	/*@}*/
};

#endif
