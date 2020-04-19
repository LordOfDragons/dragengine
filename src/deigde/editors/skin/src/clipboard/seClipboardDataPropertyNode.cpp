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
#include <stdlib.h>

#include "seClipboardDataPropertyNode.h"
#include "../skin/property/node/sePropertyNode.h"
#include "../skin/property/node/sePropertyNodeList.h"

#include <dragengine/common/exceptions.h>



// Class seClipboardDataPropertyNode
//////////////////////////////////////

const char * const seClipboardDataPropertyNode::TYPE_NAME = "property-node";

// Constructor, destructor
////////////////////////////

seClipboardDataPropertyNode::seClipboardDataPropertyNode( const sePropertyNodeList &nodes ) :
igdeClipboardData( TYPE_NAME )
{
	const int count = nodes.GetCount();
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	sePropertyNode *node = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			node = nodes.GetAt( i )->Copy();
			pNodes.Add( node );
			node->FreeReference();
			node = NULL;
		}
		
	}catch( const deException & ){
		if( node ){
			node->FreeReference();
		}
		pNodes.RemoveAll();
		throw;
	}
}

seClipboardDataPropertyNode::~seClipboardDataPropertyNode(){
}



// Management
///////////////

int seClipboardDataPropertyNode::GetCount() const{
	return pNodes.GetCount();
}

const sePropertyNode &seClipboardDataPropertyNode::GetAt( int index ) const{
	return *( ( const sePropertyNode * )pNodes.GetAt( index ) );
}
