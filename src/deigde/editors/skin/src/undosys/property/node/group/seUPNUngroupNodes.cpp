/* 
 * Drag[en]gine IGDE Skin Editor
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
