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

#if defined OS_UNIX || defined OS_BEOS
#include <errno.h>
#endif

#include "deReadWriteLock.h"
#include "../common/exceptions.h"



// Class deReadWriteLock
//////////////////////////

// Constructor, destructor
////////////////////////////

deReadWriteLock::deReadWriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	if( pthread_rwlock_init( &pRWLock, NULL ) != 0 ){
		DETHROW( deeOutOfMemory );
	}
	#endif
	
	#ifdef OS_W32
	InitializeSRWLock( &pRWLock );
	#endif
}

deReadWriteLock::~deReadWriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	const int result = pthread_rwlock_destroy( &pRWLock );
	if( result != 0 ){
		switch( result ){
		case EBUSY:
			printf( "[RWLOCK] %p: Still locked!\n", this );
			break;
			
		case EINVAL:
			printf( "[RWLOCK] %p: Invalid lock!\n", this );
			break;
		}
		printf( "[RWLOCK] %p: Cleanup failed (%i)\n", this, result );
	}
	#endif
	
	#ifdef OS_W32
	// SRWLock does not require explicit deletion
	#endif
}



// Management
///////////////

void deReadWriteLock::ReadLock(){
	#if defined OS_UNIX || defined OS_BEOS
	if( pthread_rwlock_rdlock( &pRWLock ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	AcquireSRWLockShared( &pRWLock );
	#endif
}

bool deReadWriteLock::TryReadLock(){
	#if defined OS_UNIX || defined OS_BEOS
	switch( pthread_rwlock_tryrdlock( &pRWLock ) ){
	case 0:
		return true;
		
	case EBUSY:
		return false;
		
	default:
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	return TryAcquireSRWLockShared( &pRWLock ) != 0;
	#endif
}

void deReadWriteLock::ReadUnlock(){
	#if defined OS_UNIX || defined OS_BEOS
	if( pthread_rwlock_unlock( &pRWLock ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	ReleaseSRWLockShared( &pRWLock );
	#endif
}



void deReadWriteLock::WriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	if( pthread_rwlock_wrlock( &pRWLock ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	AcquireSRWLockExclusive( &pRWLock );
	#endif
}

bool deReadWriteLock::TryWriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	switch( pthread_rwlock_trywrlock( &pRWLock ) ){
	case 0:
		return true;
		
	case EBUSY:
		return false;
		
	default:
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	return TryAcquireSRWLockExclusive( &pRWLock ) != 0;
	#endif
}

void deReadWriteLock::WriteUnlock(){
	#if defined OS_UNIX || defined OS_BEOS
	if( pthread_rwlock_unlock( &pRWLock ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	ReleaseSRWLockExclusive( &pRWLock );
	#endif
}
