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
class deThread{
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
	void SetName( const char *name );
	#endif
	/*@}*/
	
	
	
private:
	#if defined OS_UNIX || defined OS_BEOS
	static void *pThreadRunner( void *parameter );
	#endif
	
	#ifdef OS_W32
	static DWORD WINAPI pThreadRunner( LPVOID parameter );
	#endif
	
	#ifdef OS_BEOS
	decString pName;
	thread_id pBeOSThreadID;
	#endif
};

#endif
