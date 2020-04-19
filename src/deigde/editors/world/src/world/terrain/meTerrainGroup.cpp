/* 
 * Drag[en]gine IGDE World Editor
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "meTerrainGroup.h"
#include "../object/meObject.h"

#include "dragengine/common/exceptions.h"


// class meTerrainGroup
/////////////////////////

// constructor, destructor
////////////////////////////

meTerrainGroup::meTerrainGroup( const char *name ){
	pName = NULL;
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	try{
		SetName( name );
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meTerrainGroup::~meTerrainGroup(){
	pCleanUp();
}



// management
///////////////

void meTerrainGroup::SetName( const char *name ){
	if( ! name || ! name[ 0 ] ) DETHROW( deeInvalidParam );
	char *newStr = new char[ strlen( name ) + 1 ];
	if( ! newStr ) DETHROW( deeOutOfMemory );
	strcpy( newStr, name );
	if( pName ) delete [] pName;
	pName = newStr;
}



// onj management
///////////////////////

meObject *meTerrainGroup::GetObject( int index ) const{
	if( index < 0 || index >= pObjectCount ) DETHROW( deeOutOfBoundary );
	return pObjects[ index ];
}

int meTerrainGroup::FindObject( meObject *onj ) const{
	if( ! onj ) return -1;
	int i;
	for( i=0; i<pObjectCount; i++ ){
		if( pObjects[ i ] == onj ) return i;
	}
	return -1;
}

bool meTerrainGroup::HasObject( meObject *onj ) const{
	return FindObject( onj ) != -1;
}

void meTerrainGroup::AddObject( meObject *onj ){
	if( ! onj ) DETHROW( deeInvalidParam );
	if( HasObject( onj ) ) return;
	if( pObjectCount == pObjectSize ){
		int i, newSize = pObjectCount * 3 / 2 + 1;
		meObject **newArray = new meObject*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pObjects ){
			for( i=0; i<pObjectCount; i++ ) newArray[ i ] = pObjects[ i ];
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	pObjects[ pObjectCount ] = onj;
	pObjectCount++;
}

void meTerrainGroup::RemoveObject( meObject *onj ){
	int i, index = FindObject( onj );
	if( index == -1 ) return;
	for( i=index+1; i<pObjectCount; i++ ) pObjects[ i - 1 ] = pObjects[ i ];
	pObjectCount--;
}

void meTerrainGroup::RemoveAllObjects(){
	pObjectCount = 0;
}



// private functions
//////////////////////

void meTerrainGroup::pCleanUp(){
	RemoveAllObjects();
	if( pObjects ) delete [] pObjects;
	if( pName) delete [] pName;
}
