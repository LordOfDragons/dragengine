/* 
 * Drag[en]gine IGDE Rig Editor
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reRigPush.h"
#include "reRigPushList.h"
#include "dragengine/common/exceptions.h"



// Class reRigPushList
////////////////////////

// Constructor, destructor
////////////////////////////

reRigPushList::reRigPushList(){
	pPushes = NULL;
	pPushCount = 0;
	pPushSize = 0;
}

reRigPushList::~reRigPushList(){
	RemoveAllPushes();
	if( pPushes ) delete [] pPushes;
}



// Management
///////////////

reRigPush *reRigPushList::GetPushAt( int index ) const{
	if( index < 0 || index >= pPushCount ) DETHROW( deeInvalidParam );
	
	return pPushes[ index ];
}

int reRigPushList::IndexOfPush( reRigPush *push ) const{
	if( ! push ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pPushCount; i++ ){
		if( push == pPushes[ i ] ) return i;
	}
	
	return -1;
}

bool reRigPushList::HasPush( reRigPush *push ) const{
	if( ! push ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pPushCount; i++ ){
		if( push == pPushes[ i ] ) return true;
	}
	
	return false;
}

void reRigPushList::AddPush( reRigPush *push ){
	if( HasPush( push ) ) DETHROW( deeInvalidParam );
	
	if( pPushCount == pPushSize ){
		int newSize = pPushSize * 3 / 2 + 1;
		reRigPush **newArray = new reRigPush*[ newSize ];
		if( pPushes ){
			memcpy( newArray, pPushes, sizeof( reRigPush* ) * pPushSize );
			delete [] pPushes;
		}
		pPushes = newArray;
		pPushSize = newSize;
	}
	
	pPushes[ pPushCount ] = push;
	pPushCount++;
	
	push->AddReference();
}

void reRigPushList::RemovePush( reRigPush *push ){
	int i, index = IndexOfPush( push );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pPushCount; i++ ){
		pPushes[ i - 1 ] = pPushes[ i ];
	}
	pPushCount--;
	
	push->FreeReference();
}

void reRigPushList::RemoveAllPushes(){
	while( pPushCount > 0 ){
		pPushCount--;
		pPushes[ pPushCount ]->FreeReference();
	}
}
