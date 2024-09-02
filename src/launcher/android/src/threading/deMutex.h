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

// include only once
#ifndef _DEMUTEX_H_
#define _DEMUTEX_H_

#include "../dragengine_configuration.h"

#if defined OS_UNIX || defined OS_BEOS
#	include <pthread.h>
#endif

#ifdef OS_W32
#	include "../app/include_windows.h"
#endif


/**
 * @brief Mutex.
 *
 * Provides mutual exclusive access support using a thin wrapper around
 * the operating system specific threading mutex.
 */
class deMutex{
private:
#if defined OS_UNIX || defined OS_BEOS
	pthread_mutex_t pMutex;
#endif
#ifdef OS_W32
	CRITICAL_SECTION pCritSec;
#endif
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a mutex. */
	deMutex();
	/** Cleans the mutex. */
	~deMutex();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Locks the mutex. If the mutex is already locked this call blocks
	 * until the original called released it.
	 */
	void Lock();
	/**
	 * Locks the mutex. If the mutex is already locked this call returns
	 * false and no lock is taken. If the mutex is not locked this call
	 * acquires the lock and return true.
	 */
	bool TryLock();
	/**
	 * Unlocks the mutex. An exception is thrown if this thread is not
	 * holding the mutex.
	 */
	void Unlock();
	/*@}*/
};

// end of include only once
#endif
