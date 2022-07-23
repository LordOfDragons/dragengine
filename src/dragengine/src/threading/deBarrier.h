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
	HANDLE pEvent;
	CRITICAL_SECTION pCSWaitCounter;
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
