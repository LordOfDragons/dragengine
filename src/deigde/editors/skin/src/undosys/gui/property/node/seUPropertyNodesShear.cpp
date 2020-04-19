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

#include "seUPropertyNodeData.h"
#include "seUPropertyNodesShear.h"
#include "../../../../skin/property/node/sePropertyNode.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"
#include "../../../../skin/property/node/sePropertyNodeList.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodesShear
////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodesShear::seUPropertyNodesShear( const sePropertyNodeList &nodes,
const decVector2 &pivot, const decVector2 &origin, float rotation, float shearing ) :
pPivot( pivot ),
pOrigin( origin ),
pRotation( rotation ),
pShearing( shearing )
{
	SetShortInfo( "Resize nodes" );
	
	const int count = nodes.GetCount();
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			pAddNodes( nodes.GetAt( i ) );
		}
		
	}catch( const deException & ){
		pNodes.RemoveAll();
		throw;
	}
}

seUPropertyNodesShear::~seUPropertyNodesShear(){
}



// Management
///////////////

void seUPropertyNodesShear::SetDistance( const decVector2 &distance ){
	if( distance.IsEqualTo( pDistance ) ){
		return;
	}
	
	pDistance = distance;
	
	if( distance.Length() < 0.5f ){
		pTransform.SetIdentity();
		return;
	}
	
	const decTexMatrix2 matrix(
		decTexMatrix2::CreateRotation( -pRotation )
		* decTexMatrix2::CreateShear( -pShearing, 0.0f ) );
	
	const decVector2 piv2org( matrix * ( pOrigin - pPivot ) );
	const decVector2 distance2( matrix * distance );
	decVector2 fshear;
	
	if( fabsf( piv2org.x ) > 0.5f ){
		fshear.y = distance2.y / piv2org.x;
	}
	if( fabsf( piv2org.y ) > 0.5f ){
		fshear.x = distance2.x / piv2org.y;
	}
	
	/*if( GetNodeCount() == 1 ){
		rotation = GetNodeAt( 0 ).rotation;
		shearing = GetNodeAt( 0 ).shearing;
		
		SetTransform(
			decTexMatrix2::CreateTranslation()
			decTexMatrix2::CreateTranslation( -pPivot )
			* decTexMatrix2::CreateScale( fscale )
			* decTexMatrix2::CreateTranslation( pPivot ) );
		
	}else{*/
		pTransform =
			decTexMatrix2::CreateTranslation( -pPivot )
			* matrix
			* decTexMatrix2::CreateShear( fshear )
			* decTexMatrix2::CreateShear( pShearing, 0.0f )
			* decTexMatrix2::CreateRotation( pRotation )
			* decTexMatrix2::CreateTranslation( pPivot );
	//}
}

void seUPropertyNodesShear::Undo(){
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const seUPropertyNodeData &data = *( ( seUPropertyNodeData* )pNodes.GetAt( i ) );
		sePropertyNode &node = *data.GetNode();
		
		node.SetPosition( data.GetPosition() );
		node.SetSize( data.GetSize() );
		node.SetRotation( data.GetRotation() );
		node.SetShearing( data.GetShearing() );
	}
}

void seUPropertyNodesShear::Redo(){
	if( pTransform.IsEqualTo( decTexMatrix2() ) ){
		return;
	}
	
	const int count = pNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const seUPropertyNodeData &data = *( ( seUPropertyNodeData* )pNodes.GetAt( i ) );
		sePropertyNode &node = *data.GetNode();
		
		node.SetFromMatrix( data.GetMatrix() * pTransform * data.CreateInverseParentMatrix(),
			data.GetSize(), data.GetRotation() );
	}
}



// Private Functions
//////////////////////

void seUPropertyNodesShear::pAddNodes( sePropertyNode *node ){
	seUPropertyNodeData *data = NULL;
	
	try{
		data = new seUPropertyNodeData( node );
		pNodes.Add( data );
		data->FreeReference();
		data = NULL;
		
		if( node->GetMask() ){
			pAddNodes( node->GetMask() );
		}
		
		if( node->GetNodeType() == sePropertyNode::entGroup ){
			const sePropertyNodeGroup &nodeGroup = ( sePropertyNodeGroup& )*node;
			const int count = nodeGroup.GetNodeCount();
			int i;
			for( i=0; i<count; i++ ){
				pAddNodes( nodeGroup.GetNodeAt( i ) );
			}
		}
		
	}catch( const deException & ){
		if( data ){
			data->FreeReference();
		}
		throw;
	}
}
