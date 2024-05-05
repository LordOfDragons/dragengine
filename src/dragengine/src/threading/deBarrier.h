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

#ifndef _DEBARRIER_H_
#define _DEBARRIER_H_

#include "../dragengine_export.h"
#include "../dragengine_configuration.h"

#if defined OS_UNIX || defined OS_BEOS
#include <pthread.h>
#endif

#ifdef OS_W32
#include "../app/include_windows.h"
#endif


/**
 * \brief Synchronization barrier.
 * 
 * Implements barrier type synchronization for multiple threads. In the initial state
 * the barrier is closed with an open threshold larger than 0 blocking all threads
 * trying to get through. Each thread calling Wait() increments the barrier counter
 * and is blocked. Once the counter reaches the threshold the barrier is opened up
 * and all waiting threads woken up to pass through. Once all threads have passed
 * through the barrier automatically closes again to be ready for another round. In
 * addition the barrier can be forcefully opened with Open() even though the threshold
 * has not been reached yet. This behavior has a few implications to keep in mind. If
 * a thread arrives while the barrier is still open and tries to wait it slips through
 * the open barrier without waiting. Reusing a single barrier alone can then lead to
 * unexpected behavior. If a thread passes through and immediately waits again on the
 * barrier for example because it finished so fast the barrier is still open and the
 * thread slips through again getting another turn. To prevent this problem use two
 * barriers alternating between them. Unless forcefully opened at most one of the
 * barriers can be open at each time. This ensures all threads run through a code
 * path exactly once before all are allowed to enter the section again. 
 */
class DE_DLL_EXPORT deBarrier{
private:
	int pThreshold;
	int pCounter;
	bool pOpen;
	
	#if defined OS_UNIX || defined OS_BEOS
	pthread_mutex_t pMutex;
	pthread_cond_t pCondition;
	#endif
	
	#ifdef OS_W32
	CONDITION_VARIABLE pConditionVariable;
	CRITICAL_SECTION pCriticalSection;
	#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create barrier with initial threshold.
	 * \throws deeInvalidParam \em threshold is less than 1.
	 */
	deBarrier( int threshold );
	
	/** \brief Clean up barrier. */
	~deBarrier();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Increment counter and block thread on barrier until threshold is hit. */
	void Wait();
	
	/**
	 * \brief Increment counter and block thread on barrier until threshold is hit.
	 * \version 1.16
	 * 
	 * If timeout elapses before barrier is opened the counter is decremented returns false.
	 * If barrier opens before timeout elapses true is returned.
	 * 
	 * \param[in] timeout Timeout in milli-seconds.
	 */
	bool TryWait( int timeout );
	
	/** \brief Forcefully open barrier if counter is not 0. */
	void Open();
	
	/**
	 * \brief Set threshold.
	 * 
	 * If the threshold is equal to the counter or less opens the barrier.
	 * 
	 * \throws deeInvalidParam \em threshold is less than 1.
	 */
	void SetThreshold( int threshold );
	/*@}*/
};

#endif
