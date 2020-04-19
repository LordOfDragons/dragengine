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

#include "seUPropertyNodesRotate.h"
#include "../../../../skin/property/node/sePropertyNode.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"
#include "../../../../skin/property/node/sePropertyNodeList.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodesRotate
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodesRotate::seUPropertyNodesRotate( const sePropertyNodeList &nodes,
const decVector2 &pivot, const decVector2 &origin ) :
pNodes( NULL ),
pCount( 0 ),
pPivot( pivot ),
pOrigin( origin )
{
	const int count = nodes.GetCount();
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Rotate nodes" );
	
	try{
		pNodes = new sNode[ count ];
		
		for( pCount=0; pCount<count; pCount++ ){
			sePropertyNode * const node = nodes.GetAt( pCount );
			if( ! node->GetParent() ){
				DETHROW( deeInvalidParam );
			}
			
			pNodes[ pCount ].node = node;
			node->AddReference();
			pNodes[ pCount ].position = node->GetPosition();
			pNodes[ pCount ].rotation = node->GetRotation();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

seUPropertyNodesRotate::~seUPropertyNodesRotate(){
	pCleanUp();
}



// Management
///////////////

void seUPropertyNodesRotate::SetTarget( const decVector2 &target ){
	if( target.IsEqualTo( pTarget ) ){
		return;
	}
	
	pTarget = target;
	
	if( target.IsEqualTo( pOrigin, 0.5f ) ){
		pTransform.SetIdentity();
		return;
	}
	
	const decVector2 piv2org( pOrigin - pPivot );
	const float angleOrigin = atan2f( piv2org.y, piv2org.x );
	
	const decVector2 piv2tar( target - pPivot );
	const float angleTarget = atan2f( piv2tar.y, piv2tar.x );
	
	/*if( GetNodeCount() == 1 ){
		rotation = GetNodeAt( 0 ).rotation;
		shearing = GetNodeAt( 0 ).shearing;
		
		SetTransform(
			decTexMatrix2::CreateTranslation()
			decTexMatrix2::CreateTranslation( -pPivot )
			* decTexMatrix2::CreateScale( fscale )
			* decTexMatrix2::CreateTranslation( pPivot ) );
		
	}else{*/
		pTransform = decTexMatrix2::CreateTranslation( -pPivot )
			* decTexMatrix2::CreateRotation( angleTarget - angleOrigin )
			* decTexMatrix2::CreateTranslation( pPivot );
	//}
}

void seUPropertyNodesRotate::Undo(){
	int i;
	
	for( i=0; i<pCount; i++ ){
		sePropertyNode &node = *pNodes[ i ].node;
		node.SetPosition( pNodes[ i ].position );
		node.SetRotation( pNodes[ i ].rotation );
	}
}

void seUPropertyNodesRotate::Redo(){
	if( pTransform.IsEqualTo( decTexMatrix2() ) ){
		return;
	}
	
	int i;
	for( i=0; i<pCount; i++ ){
		sePropertyNode &node = *pNodes[ i ].node;
		
		const decTexMatrix2 matrix( node.CreateScreenTransformMatrix() * pTransform
			* node.GetParent()->CreateScreenTransformMatrix().Invert().ToTexMatrix2() );
		
		float rotation = matrix.GetRotation() / DEG2RAD;
		if( node.GetSize().x < 0 ){
			// node size x is negative. flip result to get correct result
			rotation += 180.0f;
		}
		
		node.SetRotation( rotation );
		
		const decPoint position( ( matrix.GetPosition() - node.CreateCanvasTransformMatrix().GetPosition() ).Round() );
		node.SetPosition( decPoint3( position.x, position.y, pNodes[ i ].position.z ) );
	}
}



// Private Functions
//////////////////////

void seUPropertyNodesRotate::pCleanUp(){
	if( pNodes ){
		int i;
		for( i=0; i<pCount; i++ ){
			pNodes[ i ].node->FreeReference();
		}
		delete [] pNodes;
	}
}
