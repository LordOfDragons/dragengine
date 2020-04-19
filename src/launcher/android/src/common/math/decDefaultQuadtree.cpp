/* 
 * Drag[en]gine Android Launcher
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

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decDefaultQuadtree.h"
#include "../exceptions.h"



// Class decDefaultQuadtree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

decDefaultQuadtree::decDefaultQuadtree( const decVector2 &center, const decVector2 &halfSize ) : decQuadtree( center, halfSize ){
	pElements = NULL;
	pElementCount = 0;
	pElementSize = 0;
}

decDefaultQuadtree::~decDefaultQuadtree(){
	RemoveAllElements();
	if( pElements ) delete [] pElements;
}



// Management
///////////////

decQuadtree *decDefaultQuadtree::CreateQuadtree( int quadrant ) const{
	decVector2 halfSize = GetHalfSize() * 0.5f;
	const decVector2 &center = GetCenter();
	decQuadtree *node = ( decDefaultQuadtree* )NULL;
	decVector2 nc;
	
	// determine the smallest and largest coordinates
	if( ( quadrant & 2 ) == 2 ){
		nc.x = center.x + halfSize.x;
	}else{
		nc.x = center.x - halfSize.x;
	}
	if( ( quadrant & 1 ) == 1 ){
		nc.y = center.y + halfSize.y;
	}else{
		nc.y = center.y - halfSize.y;
	}
	
	// create child node
	node = ( decDefaultQuadtree* )new decDefaultQuadtree( nc, halfSize );
	if( ! node ) DETHROW( deeOutOfMemory );
	return node;
}

void decDefaultQuadtree::ClearNodeContent(){
	RemoveAllElements();
}

decDefaultQuadtree *decDefaultQuadtree::InsertIntoTree( void *element, const decVector2 &boxCenter, const decVector2 &boxHalfSize, int maxDepth ){
	decQuadtree *curNode = this;
	decQuadtree *nextNode;
	int d;
	
	for( d=0; d<maxDepth; d++ ){
		nextNode = curNode->GetNodeAtBox( boxCenter, boxHalfSize );
		if( ! nextNode ) break;
		curNode = nextNode;
	}
	
	( ( decDefaultQuadtree* )curNode )->AddElement( element );
	return ( decDefaultQuadtree* )curNode;
}



// Elements
/////////////

void *decDefaultQuadtree::GetElementAt( int index ) const{
	if( index < 0 || index >= pElementCount ) DETHROW( deeInvalidParam );
	return pElements[ index ];
}

int decDefaultQuadtree::IndexOfElement( void *element ) const{
	int i;
	for( i=0; i<pElementCount; i++ ){
		if( pElements[ i ] == element ) return i;
	}
	return -1;
}

void decDefaultQuadtree::AddElement( void *element ){
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

void decDefaultQuadtree::RemoveElement( void *element ){
	int i, index = IndexOfElement( element );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pElementCount; i++ ) pElements[ i - 1 ] = pElements[ i ];
	pElements[ pElementCount - 1 ] = NULL;
	pElementCount--;
}

void decDefaultQuadtree::RemoveAllElements(){
	pElementCount = 0;
}
