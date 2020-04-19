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

#include "seUPNGroupNodes.h"
#include "../../../../skin/property/seProperty.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupNodes
//////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupNodes::seUPNGroupNodes( const sePropertyNodeList &nodes ) :
pParentGroup( NULL ),
pNodeGroup( NULL ),
pNodeCount( 0 ),
pNodes( NULL )
{
	const int count = nodes.GetCount();
	int i;
	
	sePropertyNodeGroup * const parentGroup = nodes.GetAt( 0 )->GetParent();
	if( ! parentGroup || ! parentGroup->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	for( i=0; i<count; i++ ){
		if( nodes.GetAt( i )->GetParent() != parentGroup ){
			DETHROW( deeInvalidParam );
		}
	}
	
	SetShortInfo( "Group nodes" );
	
	decVector2 minBounds;
	decVector2 maxBounds;
	for( i=0; i<count; i++ ){
		const sePropertyNode &childNode = *nodes.GetAt( i );
		const decTexMatrix2 childTransform( childNode.CreateParentTransformMatrix() );
		const decPoint3 &childSize = childNode.GetSize();
		const decVector2 absSize( ( float )abs( childSize.x ), ( float )abs( childSize.y ) );
		
		const decVector2 p1( childTransform.GetPosition() );
		const decVector2 p2( childTransform * decVector2( absSize.x, 0.0f ) );
		const decVector2 p3( childTransform * decVector2( 0.0f, absSize.y ) );
		const decVector2 p4( childTransform * absSize );
		
		const decVector2 smallest( p1.Smallest( p2 ).Smallest( p3 ).Smallest( p4 ) );
		const decVector2 largest( p1.Largest( p2 ).Largest( p3 ).Largest( p4 ) );
		
		if( i == 0 ){
			minBounds = smallest;
			maxBounds = largest;
			
		}else{
			minBounds.SetSmallest( smallest );
			maxBounds.SetLargest( largest );
		}
	}
	
	pOffset.Set( minBounds.x, minBounds.y, 0.0f );
	
	try{
		// store nodes sorted by index in the parent group
		pNodes = new sNode[ count ];
		for( pNodeCount=0; pNodeCount<count; pNodeCount++ ){
			sePropertyNode * const node = nodes.GetAt( pNodeCount );
			pNodes[ pNodeCount ].position = node->GetPosition();
			pNodes[ pNodeCount ].index = parentGroup->IndexOfNode( node );
			pNodes[ pNodeCount ].node = node;
			node->AddReference();
		}
		
		for( i=1; i<pNodeCount; i++ ){
			if( pNodes[ i ].index >= pNodes[ i - 1 ].index ){
				continue;
			}
			
			const sNode temp( pNodes[ i - 1 ] );
			pNodes[ i - 1 ] = pNodes[ i ];
			pNodes[ i ] = temp;
			
			if( i > 1 ){
				i -= 2;
			}
		}
		
		// create node group
		pNodeGroup = new sePropertyNodeGroup( parentGroup->GetEngine() );
		decPoint rounded( minBounds.Round() );
		pNodeGroup->SetPosition( decPoint3( rounded.x, rounded.y, parentGroup->GetPosition().z ) );
		
		rounded = ( maxBounds - minBounds ).Round();
		pNodeGroup->SetSize( decPoint3( rounded.x, rounded.y, 1 ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pParentGroup = parentGroup;
	parentGroup->AddReference();
}

seUPNGroupNodes::~seUPNGroupNodes(){
	pCleanUp();
}



// Management
///////////////

void seUPNGroupNodes::Undo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	pNodeGroup->RemoveAllNodes();
	pParentGroup->RemoveNode( pNodeGroup );
	
	int i;
	for( i=0; i<pNodeCount; i++ ){
		pNodes[ i ].node->SetPosition( pNodes[ i ].position );
		pParentGroup->InsertNode( pNodes[ i ].index, pNodes[ i ].node );
		selection.Add( pNodes[ i ].node );
	}
}

void seUPNGroupNodes::Redo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	int i;
	for( i=0; i<pNodeCount; i++ ){
		pParentGroup->RemoveNode( pNodes[ i ].node );
		pNodes[ i ].node->SetPosition( ( decVector( pNodes[ i ].position ) - pOffset ).Round() );
		pNodeGroup->AddNode( pNodes[ i ].node );
	}
	
	pParentGroup->AddNode( pNodeGroup );
	
	selection.Add( pNodeGroup );
}



// Private Functions
//////////////////////

void seUPNGroupNodes::pCleanUp(){
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
