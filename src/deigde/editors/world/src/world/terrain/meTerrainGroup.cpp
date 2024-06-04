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
	DEASSERT_NOTNULL( name )
	DEASSERT_NOTNULL( name[ 0 ] )
	pName = name;
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
}
