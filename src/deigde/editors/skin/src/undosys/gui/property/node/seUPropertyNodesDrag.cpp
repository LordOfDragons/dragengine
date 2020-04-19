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

#include "seUPropertyNodesDrag.h"
#include "../../../../skin/property/node/sePropertyNode.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodesDrag
///////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodesDrag::seUPropertyNodesDrag( const sePropertyNodeList &nodes ) :
pNodes( nodes )
{
	SetShortInfo( "Drag nodes" );
}

seUPropertyNodesDrag::~seUPropertyNodesDrag(){
}



// Management
///////////////

void seUPropertyNodesDrag::SetDistance( const decPoint &distance ){
	pDistance = distance;
}

void seUPropertyNodesDrag::Undo(){
	const decPoint3 distance( pDistance.x, pDistance.y, 0 );
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		sePropertyNode &node = *pNodes.GetAt( i );
		node.SetPosition( node.GetPosition() - distance );
	}
}

void seUPropertyNodesDrag::Redo(){
	const decPoint3 distance( pDistance.x, pDistance.y, 0 );
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		sePropertyNode &node = *pNodes.GetAt( i );
		node.SetPosition( node.GetPosition() + distance );
	}
}
