/* 
 * Drag[en]gine Game Engine
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

#include "deSkinPropertyNodeGroup.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeGroup
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeGroup::deSkinPropertyNodeGroup(){
}

deSkinPropertyNodeGroup::~deSkinPropertyNodeGroup(){
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deSkinPropertyNode* )pNodes.GetAt( i );
	}
}



// Management
///////////////

int deSkinPropertyNodeGroup::GetNodeCount() const{
	return pNodes.GetCount();
}

deSkinPropertyNode *deSkinPropertyNodeGroup::GetNodeAt( int index ) const{
	return ( deSkinPropertyNode* )pNodes.GetAt( index );
}

void deSkinPropertyNodeGroup::AddNode( deSkinPropertyNode *node ){
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	pNodes.Add( node );
}



// Visiting
/////////////

void deSkinPropertyNodeGroup::Visit( deSkinPropertyNodeVisitor &visitor ){
	visitor.VisitGroup( *this );
}
