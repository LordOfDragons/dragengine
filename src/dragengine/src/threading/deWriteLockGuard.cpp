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

#include "deReadWriteLock.h"
#include "deWriteLockGuard.h"
#include "../common/exceptions.h"



// Class deWriteLockGuard
///////////////////////////

// Constructor, destructor
////////////////////////////

deWriteLockGuard::deWriteLockGuard( deReadWriteLock &rwlock ) :
pRWLock( rwlock ),
pLocked( false )
{
	   WriteLock();
}

deWriteLockGuard::~deWriteLockGuard(){
	if( pLocked ){
		      WriteUnlock();
	}
}



// Management
///////////////

void deWriteLockGuard::WriteLock(){
	if( pLocked ){
		DETHROW( deeInvalidAction );
	}
	
	pRWLock.WriteLock();
	pLocked = true;
}

bool deWriteLockGuard::TryWriteLock(){
	if( pLocked ){
		DETHROW( deeInvalidAction );
	}
	
	pLocked = pRWLock.TryWriteLock();
	return pLocked;
}

void deWriteLockGuard::WriteUnlock(){
	if( ! pLocked ){
		DETHROW( deeInvalidAction );
	}
	
	pRWLock.WriteUnlock();
	pLocked = false;
}
