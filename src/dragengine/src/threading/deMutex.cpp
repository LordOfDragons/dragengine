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
#	include <errno.h>
#endif

#include "deMutex.h"
#include "../common/exceptions.h"



// Class deMutex
//////////////////

// Constructor, destructor
////////////////////////////

deMutex::deMutex()
#ifdef OS_W32
pMutex( NULL )
#endif
{
#if defined OS_UNIX || defined OS_BEOS
	/*
	const int result = pthread_mutex_init( &pMutex, NULL );
	if( result != 0 ){
		if( result == ENOMEM ) DETHROW( deeOutOfMemory );
		if( result == EAGAIN ) DETHROW_INFO( deeOutOfMemory, "lack of resources" );
		if( result == EPERM ) DETHROW_INFO( deeInvalidAction, "permission denied" );
		if( result == EBUSY ) DETHROW_INFO( deeInvalidAction, "double initialization" );
		DETHROW_INFO( deeInvalidAction, "unknown error" );
	}
	*/
	if( pthread_mutex_init( &pMutex, NULL ) != 0 ){
		DETHROW( deeOutOfMemory );
	}
#endif

#ifdef OS_W32
	pMutex = CreateMutex( NULL, FALSE, NULL );
	if( pMutex == NULL ){
		DETHROW( deeOutOfMemory );
	}
#endif
}

deMutex::~deMutex(){
#if defined OS_UNIX || defined OS_BEOS
	const int result = pthread_mutex_destroy( &pMutex );
	if( result != 0 ){
		if( result == EBUSY ) printf( "[MUTEX] %p: Still locked!\n", this );
		if( result == EINVAL ) printf( "[MUTEX] %p: Invalid mutex!\n", this );
		printf( "[MUTEX] %p: Cleanup failed ( %i )\n", this, result );
	}
#endif

#ifdef OS_W32
	if( pMutex != NULL ){
		CloseHandle( pMutex );
	}
#endif
}



// Management
///////////////

void deMutex::Lock(){
#if defined OS_UNIX || defined OS_BEOS
	if( pthread_mutex_lock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
#endif
	
#ifdef OS_W32
	if( WaitForSingleObject( pMutex, INFINITE ) != WAIT_OBJECT_0 ){
		DETHROW( deeInvalidAction );
	}
#endif
}

bool deMutex::TryLock(){
#if defined OS_UNIX || defined OS_BEOS
	switch( pthread_mutex_trylock( &pMutex ) ){
	case 0:
		return true;
		
	case EBUSY:
		return false;
		
	default:
		DETHROW( deeInvalidAction );
	}
#endif
	
#ifdef OS_W32
	switch( WaitForSingleObject( pMutex, 0 ) ){
	case WAIT_OBJECT_0:
		return true;
		
	case WAIT_TIMEOUT:
		return false;
		
	default:
		DETHROW( deeInvalidAction );
	}
#endif
}

bool deMutex::TryLock( int timeout ){
#if defined OS_UNIX || defined OS_BEOS
	timespec ts;
	ts.tv_nsec = ( long )( timeout % 1000 ) * 1000000000L;
	ts.tv_sec = timeout / 1000;
	
	switch( pthread_mutex_timedlock( &pMutex, &ts ) ){
	case 0:
		return true;
		
	case ETIMEDOUT:
		return false;
		
	default:
		DETHROW( deeInvalidAction );
	}
#endif
	
#ifdef OS_W32
	switch( WaitForSingleObject( pMutex, timeout ) ){
	case WAIT_OBJECT_0:
		return true;
		
	case WAIT_TIMEOUT:
		return false;
		
	default:
		DETHROW( deeInvalidAction );
	}
#endif
}

void deMutex::Unlock(){
#if defined OS_UNIX || defined OS_BEOS
	if( pthread_mutex_unlock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
#endif
	
#ifdef OS_W32
	if( ! ReleaseMutex( pMutex ) ){
		DETHROW( deeInvalidAction );
	}
#endif
}
