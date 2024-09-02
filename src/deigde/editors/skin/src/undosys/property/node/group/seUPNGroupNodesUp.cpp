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

#include "seUPNGroupNodesUp.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupNodesUp
////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupNodesUp::seUPNGroupNodesUp( sePropertyNodeGroup *node, const sePropertyNodeList &children ) :
seUPNGroupMoveNodes( node, children )
{
	SetShortInfo( "Move nodes up" );
	
	if( pCount > 0 && pChildren[ pCount - 1 ].index == node->GetNodeCount() - 1 ){
		pClearChildNodes(); // has no effect
	}
}

seUPNGroupNodesUp::~seUPNGroupNodesUp(){
}



// Management
///////////////

void seUPNGroupNodesUp::Undo(){
	int i;
	for( i=0; i<pCount; i++ ){
		pNode->MoveNode( pChildren[ i ].node, pChildren[ i ].index );
	}
}

void seUPNGroupNodesUp::Redo(){
	int i;
	for( i=pCount-1; i>=0; i-- ){
		pNode->MoveNode( pChildren[ i ].node, pChildren[ i ].index + 1 );
	}
}
