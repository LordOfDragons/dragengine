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
