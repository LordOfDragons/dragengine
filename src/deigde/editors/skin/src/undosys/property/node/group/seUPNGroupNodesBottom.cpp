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

#include "seUPNGroupNodesBottom.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupNodesBottom
////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupNodesBottom::seUPNGroupNodesBottom( sePropertyNodeGroup *node, const sePropertyNodeList &children ) :
seUPNGroupMoveNodes( node, children )
{
	SetShortInfo( "Move nodes to bottom" );
	
	if( pCount > 0 && pChildren[ 0 ].index == 0 ){
		pClearChildNodes(); // has no effect
	}
}

seUPNGroupNodesBottom::~seUPNGroupNodesBottom(){
}



// Management
///////////////

void seUPNGroupNodesBottom::Undo(){
	int i;
	for( i=pCount-1; i>=0; i-- ){
		pNode->MoveNode( pChildren[ i ].node, pChildren[ i ].index );
	}
}

void seUPNGroupNodesBottom::Redo(){
	int i;
	for( i=0; i<pCount; i++ ){
		pNode->MoveNode( pChildren[ i ].node, i );
	}
}
