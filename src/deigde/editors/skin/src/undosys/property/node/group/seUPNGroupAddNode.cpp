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

#include "seUPNGroupAddNode.h"
#include "../../../../skin/property/seProperty.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupAddNode
////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupAddNode::seUPNGroupAddNode(
sePropertyNodeGroup *node, sePropertyNode *child ) :
pNode( NULL ),
pChild( NULL )
{
	if( ! node || ! node->GetProperty() || ! child || child->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Node group add node" );
	
	pNode = node;
	node->AddReference();
	
	pChild = child;
	child->AddReference();
}

seUPNGroupAddNode::~seUPNGroupAddNode(){
	if( pChild ){
		pChild->FreeReference();
	}
	if( pNode ){
		pNode->FreeReference();
	}
}



// Management
///////////////

void seUPNGroupAddNode::Undo(){
	pNode->RemoveNode( pChild );
}

void seUPNGroupAddNode::Redo(){
	pNode->AddNode( pChild );
	
	pNode->GetProperty()->GetNodeSelection().RemoveAll();
	pNode->GetProperty()->GetNodeSelection().Add( pChild );
}
