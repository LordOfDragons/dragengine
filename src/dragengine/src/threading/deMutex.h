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

#ifndef _DEMUTEX_H_
#define _DEMUTEX_H_

#include "../dragengine_export.h"
#include "../dragengine_configuration.h"

#if defined OS_UNIX || defined OS_BEOS
#	include <pthread.h>
#endif

#ifdef OS_W32
#	include "../app/include_windows.h"
#endif


/**
 * \brief Mutex.
 *
 * Provides mutual exclusive access support using a thin wrapper around
 * the operating system specific threading mutex.
 */
class DE_DLL_EXPORT deMutex{
private:
#if defined OS_UNIX || defined OS_BEOS
	pthread_mutex_t pMutex;
#endif
#ifdef OS_W32
	HANDLE pMutex;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create mutex. */
	deMutex();
	
	/** \brief Cleans the mutex. */
	~deMutex();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Lock mutex.
	 * 
	 * If the mutex is already locked this call blocks until the original called released it.
	 */
	void Lock();
	
	/**
	 * \brief Try locking mutex.
	 * 
	 * If the mutex is already locked this call returns false and no lock is taken.
	 * If the mutex is not locked this call acquires the lock and return true.
	 */
	bool TryLock();
	
	/**
	 * \brief Try locking mutex with timeout.
	 * \version 1.16
	 * 
	 * If the mutex is already locked this call returns false and no lock is taken.
	 * If the mutex is not locked this call acquires the lock and return true.
	 * If the timeout elapses before acquiring the lock false is returned.
	 * 
	 * \param[in] timeout Timeout in milli-seconds.
	 */
	bool TryLock( int timeout );
	
	/**
	 * \brief Unlock the mutex.
	 * 
	 * An exception is thrown if this thread is not holding the mutex.
	 */
	void Unlock();
	/*@}*/
};

#endif
