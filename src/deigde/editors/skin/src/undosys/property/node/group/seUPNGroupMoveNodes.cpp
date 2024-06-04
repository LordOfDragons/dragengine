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
#include <string.h>
#include <stdlib.h>

#include "seUPNGroupMoveNodes.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupMoveNodes
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupMoveNodes::seUPNGroupMoveNodes( sePropertyNodeGroup *node, const sePropertyNodeList &children ) :
pNode( NULL ),
pChildren( NULL ),
pCount( 0 )
{
	if( ! node || ! node->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = children.GetCount();
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pChildren = new sNode[ count ];
		
		for( pCount=0; pCount<count; pCount++ ){
			pChildren[ pCount ].node = children.GetAt( pCount );
			pChildren[ pCount ].node->AddReference();
			pChildren[ pCount ].index = node->IndexOfNode( pChildren[ pCount ].node );
		}
		
		int i;
		for( i=1; i<count; i++ ){
			if( pChildren[ i ].index >= pChildren[ i - 1 ].index ){
				continue;
			}
			
			const sNode temp( pChildren[ i - 1 ] );
			pChildren[ i - 1 ] = pChildren[ i ];
			pChildren[ i ] = temp;
			if( i > 1 ){
				i--;
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pNode = node;
	node->AddReference();
}

seUPNGroupMoveNodes::~seUPNGroupMoveNodes(){
	pCleanUp();
}



// Management
///////////////

bool seUPNGroupMoveNodes::HasAnyEffect() const{
	return pCount > 0;
}



// Private Functions
//////////////////////

void seUPNGroupMoveNodes::pCleanUp(){
	pClearChildNodes();
	if( pNode ){
		pNode->FreeReference();
	}
}

void seUPNGroupMoveNodes::pClearChildNodes(){
	if( ! pChildren ){
		return;
	}
	
	int i;
	for( i=0; i<pCount; i++ ){
		pChildren[ i ].node->FreeReference();
	}
	delete [] pChildren;
	pChildren = NULL;
	
	pCount = 0;
}
