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

#include "seUPNGroupNodesDown.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupNodesDown
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupNodesDown::seUPNGroupNodesDown( sePropertyNodeGroup *node, const sePropertyNodeList &children ) :
seUPNGroupMoveNodes( node, children )
{
	SetShortInfo( "Move nodes down" );
	
	if( pCount > 0 && pChildren[ 0 ].index == 0 ){
		pClearChildNodes(); // has no effect
	}
}

seUPNGroupNodesDown::~seUPNGroupNodesDown(){
}



// Management
///////////////

void seUPNGroupNodesDown::Undo(){
	int i;
	for( i=pCount-1; i>=0; i-- ){
		pNode->MoveNode( pChildren[ i ].node, pChildren[ i ].index );
	}
}

void seUPNGroupNodesDown::Redo(){
	int i;
	for( i=0; i<pCount; i++ ){
		pNode->MoveNode( pChildren[ i ].node, pChildren[ i ].index - 1 );
	}
}
