/* 
 * Drag[en]gine OpenAL Audio Module
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
#include <string.h>
#include <stdlib.h>

#include "deoalDefaultOctree.h"
#include <dragengine/common/exceptions.h>



// Class deoalDefaultOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalDefaultOctree::deoalDefaultOctree( const decVector &center, const decVector &halfSize ) : deoalOctree( center, halfSize ){
	pElements = NULL;
	pElementCount = 0;
	pElementSize = 0;
}

deoalDefaultOctree::~deoalDefaultOctree(){
	RemoveAllElements();
	if( pElements ) delete [] pElements;
}



// Management
///////////////

deoalOctree *deoalDefaultOctree::CreateOctree( int octant ) const{
	decVector halfSize = GetHalfSize() * 0.5f;
	const decVector &center = GetCenter();
	deoalOctree *node = ( deoalDefaultOctree* )NULL;
	decVector nc;
	
	// determine the smallest and largest coordinates
	if( ( octant & 4 ) == 4 ){
		nc.x = center.x + halfSize.x;
	}else{
		nc.x = center.x - halfSize.x;
	}
	if( ( octant & 2 ) == 2 ){
		nc.y = center.y + halfSize.y;
	}else{
		nc.y = center.y - halfSize.y;
	}
	if( ( octant & 1 ) == 1 ){
		nc.z = center.z + halfSize.z;
	}else{
		nc.z = center.z - halfSize.z;
	}
	
	// create child node
	node = ( deoalDefaultOctree* )new deoalDefaultOctree( nc, halfSize );
	if( ! node ) DETHROW( deeOutOfMemory );
	return node;
}

void deoalDefaultOctree::ClearNodeContent(){
	RemoveAllElements();
}

deoalDefaultOctree *deoalDefaultOctree::InsertIntoTree( void *element, const decVector &boxCenter, const decVector &boxHalfSize, int maxDepth ){
	deoalOctree *curNode = this;
	deoalOctree *nextNode;
	int d;
	
	for( d=0; d<maxDepth; d++ ){
		nextNode = curNode->GetNodeAtBox( boxCenter, boxHalfSize );
		if( ! nextNode ) break;
		curNode = nextNode;
	}
	
	( ( deoalDefaultOctree* )curNode )->AddElement( element );
	return ( deoalDefaultOctree* )curNode;
}



// Elements
/////////////

void *deoalDefaultOctree::GetElementAt( int index ) const{
	if( index < 0 || index >= pElementCount ) DETHROW( deeInvalidParam );
	return pElements[ index ];
}

int deoalDefaultOctree::IndexOfElement( void *element ) const{
	int i;
	for( i=0; i<pElementCount; i++ ){
		if( pElements[ i ] == element ) return i;
	}
	return -1;
}

void deoalDefaultOctree::AddElement( void *element ){
	if( ! element ) DETHROW( deeInvalidParam );
	
	if( pElementCount == pElementSize ){
		int i, newSize = pElementSize * 3 / 2 + 1;
		void **newArray = new void*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pElements ){
			for( i=0; i<pElementSize; i++ ) newArray[ i ] = pElements[ i ];
			delete [] pElements;
		}
		pElements = newArray;
		pElementSize = newSize;
	}
	
	pElements[ pElementCount ] = element;
	pElementCount++;
}

void deoalDefaultOctree::RemoveElement( void *element ){
	int i, index = IndexOfElement( element );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pElementCount; i++ ) pElements[ i - 1 ] = pElements[ i ];
	pElements[ pElementCount - 1 ] = NULL;
	pElementCount--;
}

void deoalDefaultOctree::RemoveAllElements(){
	pElementCount = 0;
}
