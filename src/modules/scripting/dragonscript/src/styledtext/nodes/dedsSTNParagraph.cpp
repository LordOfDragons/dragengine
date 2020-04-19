/* 
 * Drag[en]gine DragonScript Script Module
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

#include "dedsSTNParagraph.h"
#include "dedsStyledTextNode.h"

#include <libdscript/exceptions.h>



// Class dedsSTNParagraph
///////////////////////////

// Constructor, destructor
////////////////////////////


dedsSTNParagraph::dedsSTNParagraph(){
	pNodes = NULL;
	pNodeSize = 0;
	pNodeCount = 0;
}

dedsSTNParagraph::~dedsSTNParagraph(){
	RemoveAllNodes();
	if( pNodes ) delete [] pNodes;
}



// Management
///////////////

dedsStyledTextNode* dedsSTNParagraph::GetNodeAt( int index ) const{
	if( index < 0 || index >= pNodeCount ) DSTHROW( dueInvalidParam );
	
	return pNodes[ index ];
}

int dedsSTNParagraph::IndexOfNode( dedsStyledTextNode* node ) const{
	if( ! node ) DSTHROW( dueInvalidParam );
	
	int n;
	
	for( n=0; n<pNodeCount; n++ ){
		if( node == pNodes[ n ] ){
			return n;
		}
	}
	
	return -1;
}

bool dedsSTNParagraph::HasNode( dedsStyledTextNode* node ) const{
	if( ! node ) DSTHROW( dueInvalidParam );
	
	int n;
	
	for( n=0; n<pNodeCount; n++ ){
		if( node == pNodes[ n ] ){
			return true;
		}
	}
	
	return false;
}

void dedsSTNParagraph::AddNode( dedsStyledTextNode* node ){
	if( HasNode( node ) ) DSTHROW( dueInvalidParam );
	
	if( pNodeCount == pNodeSize ){
		int newSize = pNodeSize + 10;
		dedsStyledTextNode **newArray = new dedsStyledTextNode*[ newSize ];
		if( ! newArray ) DSTHROW( dueOutOfMemory );
		if( pNodes ){
			memcpy( newArray, pNodes, sizeof( dedsStyledTextNode* ) * pNodeSize );
			delete [] pNodes;
		}
		pNodes = newArray;
		pNodeSize = newSize;
	}
	
	pNodes[ pNodeCount ] = node;
	node->AddReference();
	pNodeCount++;
}

void dedsSTNParagraph::RemoveNode( dedsStyledTextNode* node ){
	int n, index = IndexOfNode( node );
	
	if( index == -1 ) DSTHROW( dueInvalidParam );
	
	for( n=index+1; n<pNodeCount; n++ ){
		pNodes[ n - 1 ] = pNodes[ n ];
	}
	pNodeCount--;
}

void dedsSTNParagraph::RemoveAllNodes(){
	while( pNodeCount > 0 ){
		pNodeCount--;
		pNodes[ pNodeCount ]->FreeReference();
	}
}
