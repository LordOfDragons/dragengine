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
#include <string.h>
#include "deDecalList.h"
#include "../decal/deDecal.h"
#include "../../common/exceptions.h"



// Class deDecalList
//////////////////////

// Constructor, destructor
////////////////////////////

deDecalList::deDecalList(){
	pDecals = NULL;
	pDecalCount = 0;
	pDecalSize = 0;
}

deDecalList::~deDecalList(){
	RemoveAllDecals();
	if( pDecals ) delete [] pDecals;
}



// Management
///////////////

deDecal *deDecalList::GetDecalAt( int index ) const{
	if( index < 0 || index >= pDecalCount ) DETHROW( deeInvalidParam );
	
	return pDecals[ index ];
}

bool deDecalList::HasDecal( deDecal *decal ) const{
	if( ! decal ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pDecalCount; i++ ){
		if( decal == pDecals[ i ] ){
			return true;
		}
	}
	
	return false;
}

int deDecalList::IndexOfDecal( deDecal *decal ) const{
	if( ! decal ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pDecalCount; i++ ){
		if( decal == pDecals[ i ] ){
			return i;
		}
	}
	
	return -1;
}

void deDecalList::AddDecal( deDecal *decal ){
	if( HasDecal( decal ) ) DETHROW( deeInvalidParam );
	
	if( pDecalCount == pDecalSize ){
		int newSize = pDecalSize * 3 / 2 + 1;
		deDecal **newArray = new deDecal*[ newSize ];
		if( pDecals ){
			memcpy( newArray, pDecals, sizeof( deDecal* ) * pDecalSize );
			delete [] pDecals;
		}
		pDecals = newArray;
		pDecalSize = newSize;
	}
	
	pDecals[ pDecalCount ] = decal;
	pDecalCount++;
	
	decal->AddReference();
}

void deDecalList::RemoveAllDecals(){
	while( pDecalCount > 0 ){
		pDecalCount--;
		pDecals[ pDecalCount ]->FreeReference();
	}
}
