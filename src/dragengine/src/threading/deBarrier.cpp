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

#ifdef OS_UNIX
#include <errno.h>
#endif

#include "deBarrier.h"
#include "../common/exceptions.h"



// #define DEBUG_BARRIER 1

#ifdef DEBUG_BARRIER
	#define DBGBARRIER(f) printf( "Debug-Barrier %p %s: counter=%i threshold=%i open=%d\n", this, f, pCounter, pThreshold, pOpen )
#else
	#define DBGBARRIER(f)
#endif



// Class deSemaphore
//////////////////////

// Constructor, destructor
////////////////////////////

deBarrier::deBarrier( int threshold ) :
pThreshold( threshold ),
pCounter( 0 ),
pOpen( false )

#ifdef OS_W32
,pEvent( NULL )
#endif
{
	if( threshold < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	// unix, beos
#if defined OS_UNIX || defined OS_BEOS
	if( pthread_mutex_init( &pMutex, NULL ) != 0 ){
		DETHROW( deeOutOfMemory );
	}
	if( pthread_cond_init( &pCondition, NULL ) != 0 ){
		DETHROW( deeOutOfMemory );
	}
#endif
	
	// windows
#ifdef OS_W32
	pEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	if( ! pEvent ){
		DETHROW( deeOutOfMemory );
	}
	InitializeCriticalSection( &pCSWaitCounter );
#endif
}

deBarrier::~deBarrier(){
	// unix, beos
#if defined OS_UNIX || defined OS_BEOS
	int result = pthread_cond_destroy( &pCondition );
	switch( result ){
	case 0:
		break;
		
	case EBUSY:
		printf( "Barrier %p: Condition still in use!\n", this );
		break;
		
	case EINVAL:
		printf( "Barrier %p: Invalid condition!\n", this );
		break;
		
	default:
		printf( "Barrier %p: Condition cleanup failed ( %i )\n", this, result );
	}
	
	result = pthread_mutex_destroy( &pMutex );
	switch( result ){
	case 0:
		break;
		
	case EBUSY:
		printf( "Barrier %p: Mutex still locked!\n", this );
		break;
		
	case EINVAL:
		printf( "Barrier %p: Invalid mutex!\n", this );
		break;
		
	default:
		printf( "Barrier %p: Mutex cleanup failed ( %i )\n", this, result );
	}
#endif
	
	// windows
#ifdef OS_W32
	if( pEvent ){
		CloseHandle( pEvent );
	}
	DeleteCriticalSection( &pCSWaitCounter );
#endif
}



// Management
///////////////

void deBarrier::Wait(){
	// unix, beos
#if defined OS_UNIX || defined OS_BEOS
	if( pthread_mutex_lock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	
	DBGBARRIER( "Wait() in" );
	if( pOpen ){
		DBGBARRIER( "Wait() slip" );
		if( pthread_mutex_unlock( &pMutex ) != 0 ){
			DETHROW( deeInvalidAction );
		}
		return;
	}
	
	pCounter++;
	
	if( pCounter == pThreshold ){
		DBGBARRIER( "Wait() open" );
		pOpen = true;
		
		if( pthread_cond_broadcast( &pCondition ) != 0 ){
			pthread_mutex_unlock( &pMutex );
			DETHROW( deeInvalidAction );
		}
		
	}else{
		DBGBARRIER( "Wait() wait" );
		while( ! pOpen ){
			if( pthread_cond_wait( &pCondition, &pMutex ) != 0 ){
				pthread_mutex_unlock( &pMutex );
				DETHROW( deeInvalidAction );
			}
		}
	}
	
	pCounter--;
	
	if( pCounter == 0 ){
		pOpen = false;
	}
	DBGBARRIER( "Wait() out" );
	
	if( pthread_mutex_unlock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
#endif
	
	// windows
#ifdef OS_W32
	EnterCriticalSection( &pCSWaitCounter );
	
	if( pOpen ){
		LeaveCriticalSection( &pCSWaitCounter );
		return;
	}
	
	pCounter++;
	
	if( pCounter == pThreshold ){
		pOpen = true;
		
		int i;
		for( i=0; i<pCounter; i++ ){
			LeaveCriticalSection( &pCSWaitCounter );
			if( ! SetEvent( pEvent ) ){
				DETHROW( deeInvalidAction );
			}
			EnterCriticalSection( &pCSWaitCounter );
		}
		
	}else{
		while( ! pOpen ){
			LeaveCriticalSection( &pCSWaitCounter );
			if( WaitForSingleObject( pEvent, INFINITE ) != WAIT_OBJECT_0 ){
				DETHROW( deeInvalidAction );
			}
			EnterCriticalSection( &pCSWaitCounter );
		}
	}
	
	pCounter--;
	
	if( pCounter == 0 ){
		pOpen = false;
	}
	
	LeaveCriticalSection( &pCSWaitCounter );
#endif
}
}

void deBarrier::Open(){
	// unix, beos
#if defined OS_UNIX || defined OS_BEOS
	if( pthread_mutex_lock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	
	if( pCounter == 0 ){
		DBGBARRIER( "Open() skip" );
		if( pthread_mutex_unlock( &pMutex ) != 0 ){
			DETHROW( deeInvalidAction );
		}
		return;
	}
	
	DBGBARRIER( "Open() open" );
	pOpen = true;
	
	if( pthread_cond_broadcast( &pCondition ) != 0 ){
		pthread_mutex_unlock( &pMutex );
		DETHROW( deeInvalidAction );
	}
	
	if( pthread_mutex_unlock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
#endif
	
	// windows
#ifdef OS_W32
	EnterCriticalSection( &pCSWaitCounter );
	
	if( pCounter == 0 ){
		LeaveCriticalSection( &pCSWaitCounter );
		return;
	}
	
	pOpen = true;
	
	int i;
	for( i=0; i<pCounter; i++ ){
		LeaveCriticalSection( &pCSWaitCounter );
		if( ! SetEvent( pEvent ) ){
			DETHROW( deeInvalidAction );
		}
		EnterCriticalSection( &pCSWaitCounter );
	}
	
	LeaveCriticalSection( &pCSWaitCounter );
#endif
}

void deBarrier::SetThreshold( int threshold ){
	if( threshold < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	// unix, beos
#if defined OS_UNIX || defined OS_BEOS
	if( pthread_mutex_lock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	
	pThreshold = threshold;
	
	DBGBARRIER( "SetThreshold() in" );
	if( pCounter >= pThreshold ){
		DBGBARRIER( "SetThreshold() open" );
		pOpen = true;
		
		if( pthread_cond_broadcast( &pCondition ) != 0 ){
			pthread_mutex_unlock( &pMutex );
			DETHROW( deeInvalidAction );
		}
	}
	
	if( pthread_mutex_unlock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
#endif
	
	// windows
#ifdef OS_W32
	EnterCriticalSection( &pCSWaitCounter );
	
	pThreshold = threshold;
	
	if( pCounter >= pThreshold ){
		pOpen = true;
		
		int i;
		for( i=0; i<pCounter; i++ ){
			LeaveCriticalSection( &pCSWaitCounter );
			if( ! SetEvent( pEvent ) ){
				DETHROW( deeInvalidAction );
			}
			EnterCriticalSection( &pCSWaitCounter );
		}
	}
	
	LeaveCriticalSection( &pCSWaitCounter );
#endif
}
