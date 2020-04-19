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

#ifndef _DESEMAPHORE_H_
#define _DESEMAPHORE_H_

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
class deSemaphore{
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
	deSemaphore( int count );
	
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
