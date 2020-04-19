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

#include <stdio.h>
#include <stdlib.h>

#include "deThread.h"
#include "../common/exceptions.h"



// Class deThread
///////////////////

// Constructor, destructor
////////////////////////////

deThread::deThread(){
	pCreated = false;
	pRunning = false;
	
	#ifdef OS_W32
	pThread = NULL;
	#endif
	
	#ifdef OS_BEOS
	pBeOSThreadID = -1;
	#endif
}

deThread::~deThread(){
	Stop();
}



// Management
///////////////

bool deThread::IsRunning(){
	bool running;
	
	pMutexState.Lock();
	running = pRunning;
	pMutexState.Unlock();
	
	return running;
}

void deThread::Start(){
	bool running = IsRunning();
	
	if( pCreated && ! running ) Stop();
	
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if( ! pCreated ){
		pCreated = true;
		pRunning = true;
		if( pthread_create( &pThread, NULL, deThread::pThreadRunner, this ) != 0 ){
			pCreated = false;
			pRunning = false;
			DETHROW( deeOutOfMemory );
		}
	}
	#endif
	
	// window
	#ifdef OS_W32
	if( ! pCreated ){
		pCreated = true;
		pRunning = true;
		pThread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )deThread::pThreadRunner, this, 0, NULL );
		if( ! pThread ){
			pCreated = false;
			pRunning = false;
			DETHROW( deeOutOfMemory );
		}
	}
	#endif
}

void deThread::Stop(){
	bool running = IsRunning();
	
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if( running ){
		#ifdef HAS_FUNC_PTRHEAD_CANCEL
		if( pthread_cancel( pThread ) != 0 ){
			 DETHROW( deeInvalidAction );
		}
		#else
		if( pthread_kill( pThread, SIGTERM ) != 0 ){
			 DETHROW( deeInvalidAction );
		}
		#endif
	}
	if( pCreated ){
		if( pthread_join( pThread, NULL ) != 0 ) DETHROW( deeInvalidAction );
		pCreated = false;
		pRunning = false; // just to make sure
	}
	#endif
	
	// windows
	#ifdef OS_W32
	if( running ){
		if( TerminateThread( pThread, 0 ) == 0 ) DETHROW( deeInvalidAction );
	}
	if( pCreated ){
		DWORD exitCode = STILL_ACTIVE;
		while( exitCode == STILL_ACTIVE ){
			if( GetExitCodeThread( pThread, &exitCode ) == 0 ) DETHROW( deeInvalidAction );
		}
		CloseHandle( pThread );
		pThread = NULL;
		pCreated = false;
		pRunning = false; // just to make sure
	}
	#endif
}

void deThread::WaitForExit(){
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if( IsRunning() && pCreated ){
		if( pthread_join( pThread, NULL ) != 0 ) DETHROW( deeInvalidAction );
		pCreated = false;
		pRunning = false; // just to make sure
	}
	#endif
	
	// windows
	#ifdef OS_W32
	if( IsRunning() && pCreated ){
		DWORD exitCode = STILL_ACTIVE;
		while( exitCode == STILL_ACTIVE ){
			if( GetExitCodeThread( pThread, &exitCode ) == 0 ) DETHROW( deeInvalidAction );
		}
		CloseHandle( pThread );
		pThread = NULL;
		pCreated = false;
		pRunning = false; // just to make sure
	}
	#endif
}



#ifdef OS_BEOS
void deThread::SetName( const char *name ){
	pName = name;
	if( pBeOSThreadID != -1 ){
		if( rename_thread( pBeOSThreadID, name ) != B_OK ){
			DETHROW( deeInvalidParam );
		}
	}
}
#endif


// Private Functions
//////////////////////

#if defined OS_UNIX || defined OS_BEOS
void *deThread::pThreadRunner( void *parameter )
#endif
#ifdef OS_W32
DWORD WINAPI deThread::pThreadRunner( LPVOID parameter )
#endif
{
	deThread *thread = ( deThread* )parameter;
	
	#ifdef OS_BEOS
	thread->pBeOSThreadID = find_thread( NULL );
	rename_thread( thread->pBeOSThreadID, thread->pName );
	#endif
	
	thread->Run();
	
	thread->pMutexState.Lock();
	thread->pRunning = false;
	#ifdef OS_BEOS
	thread->pBeOSThreadID = -1;
	#endif
	thread->pMutexState.Unlock();
	
#if defined OS_UNIX || defined OS_BEOS
	return NULL;
#endif
#ifdef OS_W32
	return 0;
#endif
}
