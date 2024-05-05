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
