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

#include "deMutex.h"
#include "deMutexGuard.h"
#include "../common/exceptions.h"



// Class deMutexGuard
///////////////////////

// Constructor, destructor
////////////////////////////

deMutexGuard::deMutexGuard( deMutex &mutex ) :
pMutex( mutex ),
pLocked( false )
{
	Lock();
}

deMutexGuard::~deMutexGuard(){
	if( pLocked ){
		Unlock();
	}
}



// Management
///////////////

void deMutexGuard::Lock(){
	if( pLocked ){
		DETHROW( deeInvalidAction );
	}
	
	pMutex.Lock();
	pLocked = true;
}

bool deMutexGuard::TryLock(){
	if( pLocked ){
		DETHROW( deeInvalidAction );
	}
	
	pLocked = pMutex.TryLock();
	return pLocked;
}

void deMutexGuard::Unlock(){
	if( ! pLocked ){
		DETHROW( deeInvalidAction );
	}
	
	pMutex.Unlock();
	pLocked = false;
}
