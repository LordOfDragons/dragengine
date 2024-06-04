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

#include "seUPNUngroupNodes.h"
#include "../../../../skin/property/seProperty.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNUngroupNodes
////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNUngroupNodes::seUPNUngroupNodes( sePropertyNodeGroup *nodeGroup ) :
pParentGroup( NULL ),
pNodeGroup( NULL ),
pIndex( 0 ),
pNodeCount( 0 ),
pNodes( 0 )
{
	if( ! nodeGroup ){
		DETHROW( deeInvalidParam );
	}
	
	sePropertyNodeGroup * const parentGroup = nodeGroup->GetParent();
	if( ! parentGroup ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Ungroup nodes" );
	
	pIndex = parentGroup->IndexOfNode( nodeGroup );
	
	const int count = nodeGroup->GetNodeCount();
	pNodes = new sNode[ count ];
	for( pNodeCount=0; pNodeCount<count; pNodeCount++ ){
		sePropertyNode * const node = nodeGroup->GetNodeAt( pNodeCount);
		pNodes[ pNodeCount ].position = node->GetPosition();
		pNodes[ pNodeCount ].size = node->GetSize();
		pNodes[ pNodeCount ].rotation = node->GetRotation();
		pNodes[ pNodeCount ].shearing = node->GetShearing();
		pNodes[ pNodeCount ].node = node;
		node->AddReference();
	}
	
	pParentGroup = parentGroup;
	parentGroup->AddReference();
	
	pNodeGroup = nodeGroup;
	nodeGroup->AddReference();
}

seUPNUngroupNodes::~seUPNUngroupNodes(){
	if( pNodeGroup ){
		pNodeGroup->FreeReference();
	}
	if( pParentGroup ){
		pParentGroup->FreeReference();
	}
	if( pNodes ){
		int i;
		for( i=0; i<pNodeCount; i++ ){
			pNodes[ i ].node->FreeReference();
		}
		delete [] pNodes;
	}
}



// Management
///////////////

void seUPNUngroupNodes::Undo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	int i;
	for( i=0; i<pNodeCount; i++ ){
		pParentGroup->RemoveNode( pNodes[ i ].node );
		pNodes[ i ].node->SetPosition( pNodes[ i ].position );
		pNodes[ i ].node->SetSize( pNodes[ i ].size );
		pNodes[ i ].node->SetRotation( pNodes[ i ].rotation );
		pNodes[ i ].node->SetShearing( pNodes[ i ].shearing );
		pNodeGroup->AddNode( pNodes[ i ].node );
	}
	
	pParentGroup->InsertNode( pIndex, pNodeGroup );
	
	selection.Add( pNodeGroup );
}

void seUPNUngroupNodes::Redo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	const decTexMatrix2 matrix( pNodeGroup->CreateParentTransformMatrix() );
	
	pParentGroup->RemoveNode( pNodeGroup );
	pNodeGroup->RemoveAllNodes();
	
	int i;
	for( i=0; i<pNodeCount; i++ ){
		pNodes[ i ].node->SetFromMatrix( pNodes[ i ].node->CreateParentTransformMatrix() * matrix,
			pNodes[ i ].size, pNodes[ i ].rotation );
		pParentGroup->AddNode( pNodes[ i ].node );
		selection.Add( pNodes[ i ].node );
	}
}
