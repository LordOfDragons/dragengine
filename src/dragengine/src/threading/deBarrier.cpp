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

#include <stdio.h>
#include <stdlib.h>

#ifdef OS_UNIX
#include <errno.h>
#endif

#if defined OS_UNIX || defined OS_BEOS
#include <sys/time.h>
#endif

#include "deBarrier.h"
#include "../common/exceptions.h"

#ifdef OS_W32
#include "../common/math/decMath.h"
#endif



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
	InitializeConditionVariable( &pConditionVariable );
	InitializeCriticalSection( &pCriticalSection );
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
	// there seems to be no function to delete a condition variable
	DeleteCriticalSection( &pCriticalSection );
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
	EnterCriticalSection( &pCriticalSection );
	
	if( pOpen ){
		LeaveCriticalSection( &pCriticalSection );
		return;
	}
	
	pCounter++;
	
	if( pCounter == pThreshold ){
		pOpen = true;
		WakeAllConditionVariable( &pConditionVariable );
		
	}else{
		while( ! pOpen ){
			if( ! SleepConditionVariableCS( &pConditionVariable, &pCriticalSection, INFINITE ) ){
				DETHROW( deeInvalidAction );
			}
		}
	}
	
	pCounter--;
	
	if( pCounter == 0 ){
		pOpen = false;
	}
	
	LeaveCriticalSection( &pCriticalSection );
#endif
}

bool deBarrier::TryWait( int timeout ){
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
		return true;
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
		timespec ts;
		
		clock_gettime( CLOCK_REALTIME, &ts );
		ts.tv_sec += timeout / 1000;
		ts.tv_nsec += ( long )( timeout % 1000 ) * 1000000L;
		if( ts.tv_nsec >= 1000000000L ){
			ts.tv_sec++;
			ts.tv_nsec %= 1000000000L;
		}
		
		while( ! pOpen ){
			switch( pthread_cond_timedwait( &pCondition, &pMutex, &ts ) ){
			case 0:
				break;
				
			case ETIMEDOUT:
				if( pOpen ){
					break; // prevent return false even though barrier opened
				}
				
				pCounter--;
				if( pCounter == 0 ){
					pOpen = false;
				}
				pthread_mutex_unlock( &pMutex );
				return false;
				
			default:
				if( pOpen ){
					break; // this should never be possible but better safe than sorry
				}
				
				pCounter--;
				if( pCounter == 0 ){
					pOpen = false;
				}
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
	return true;
#endif
	
	// windows
#ifdef OS_W32
	EnterCriticalSection( &pCriticalSection );
	
	if( pOpen ){
		LeaveCriticalSection( &pCriticalSection );
		return true;
	}
	
	pCounter++;
	
	if( pCounter == pThreshold ){
		pOpen = true;
		WakeAllConditionVariable( &pConditionVariable );
		
	}else{
		const ULONGLONG timeStart = GetTickCount64();
		int elapsed = 0;
		
		while( ! pOpen ){
			const int sleepTimeout = decMath::max( timeout - elapsed, 0 );
			if( ! SleepConditionVariableCS( &pConditionVariable, &pCriticalSection, sleepTimeout ) ){
				if( pOpen ){
					break; // prevent return false even though barrier opened
				}
				
				const bool timedOut = GetLastError() == ERROR_TIMEOUT;
				
				pCounter--;
				if( pCounter == 0 ){
					pOpen = false;
				}
				LeaveCriticalSection( &pCriticalSection );
				
				if( timedOut ){
					return false;
					
				}else{
					DETHROW( deeInvalidAction );
				}
			}
			
			if( ! pOpen ){
				elapsed = ( int )( GetTickCount64() - timeStart );
				if( elapsed >= timeout ){
					pCounter--;
					if( pCounter == 0 ){
						pOpen = false;
					}
					LeaveCriticalSection( &pCriticalSection );
					return false;
				}
			}
		}
	}
	
	pCounter--;
	
	if( pCounter == 0 ){
		pOpen = false;
	}
	
	LeaveCriticalSection( &pCriticalSection );
	return true;
#endif
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
	EnterCriticalSection( &pCriticalSection );
	
	if( pCounter == 0 ){
		LeaveCriticalSection( &pCriticalSection );
		return;
	}
	
	pOpen = true;
	WakeAllConditionVariable( &pConditionVariable );
	
	LeaveCriticalSection( &pCriticalSection );
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
	EnterCriticalSection( &pCriticalSection );
	
	pThreshold = threshold;
	
	if( pCounter >= pThreshold ){
		pOpen = true;
		WakeAllConditionVariable( &pConditionVariable );
	}
	
	LeaveCriticalSection( &pCriticalSection );
#endif
}
