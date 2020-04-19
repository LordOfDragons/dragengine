/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalModelOctree.h"
#include "deoalModelOctreeVisitor.h"
#include "../deoalModelFace.h"
#include "../../utils/collision/deoalCollisionBox.h"
#include "../../utils/collision/deoalCollisionVolume.h"
#include "../../utils/collision/deoalCollisionTriangle.h"
#include "../../utils/collision/deoalCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoalModelOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalModelOctree::deoalModelOctree( const decVector &center, const decVector &halfExtend ) :
deoalOctree( center, halfExtend ){
}

deoalModelOctree::~deoalModelOctree(){
}



// Management
///////////////

deoalOctree *deoalModelOctree::CreateOctree( int octant ) const{
	const decVector halfExtend( GetHalfSize() * 0.5f );
	const decVector &center = GetCenter();
	decVector nc;
	
	// determine the smallest and largest coordinates
	if( ( octant & 4 ) == 4 ){
		nc.x = center.x + halfExtend.x;
		
	}else{
		nc.x = center.x - halfExtend.x;
	}
	
	if( ( octant & 2 ) == 2 ){
		nc.y = center.y + halfExtend.y;
		
	}else{
		nc.y = center.y - halfExtend.y;
	}
	
	if( ( octant & 1 ) == 1 ){
		nc.z = center.z + halfExtend.z;
		
	}else{
		nc.z = center.z - halfExtend.z;
	}
	
	// create child node
	return new deoalModelOctree( nc, halfExtend );
}

void deoalModelOctree::ClearNodeContent(){
	ClearFaces();
}

void deoalModelOctree::ClearFaces(){
	int i;
	
	pFaces.RemoveAll();
	
	for( i=0; i<8; i++ ){
		deoalModelOctree * const node = ( deoalModelOctree* )GetNodeAt( i );
		if( node ){
			node->ClearFaces();
		}
	}
}

void deoalModelOctree::InsertFaceIntoTree( deoalModelFace *face, int maxDepth ){
	if( ! face || maxDepth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const decVector &minExtend = face->GetMinExtend();
	const decVector &maxExtend = face->GetMaxExtend();
	const decVector boxCenter( ( minExtend + maxExtend ) * 0.5f );
	const decVector boxHalfExtend( ( maxExtend - minExtend ) * 0.5f );
	
	pGetNodeFor( boxCenter, boxHalfExtend, maxDepth )->pFaces.Add( face );
}

void deoalModelOctree::InsertFaceOverlap( deoalModelFace *face, int depth ){
	if( ! face || depth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	deoalCollisionTriangle triangle( face->GetVertex1(), face->GetVertex2(),
		face->GetVertex3(), face->GetNormal() );
	
	pInsertFaceOverlap( face, triangle, depth );
}

int deoalModelOctree::GetFaceCount() const{
	return pFaces.GetCount();
}

deoalModelFace *deoalModelOctree::GetFaceAt( int index ) const{
	return ( deoalModelFace* )pFaces.GetAt( index );
}



// Private Functions
//////////////////////

deoalModelOctree *deoalModelOctree::pGetNodeFor( const decVector &center,
const decVector &halfExtend, int maxDepth ){
	deoalModelOctree *curNode = this;
	int i;
	
	for( i=0; i<maxDepth; i++ ){
		deoalModelOctree * const nextNode = ( deoalModelOctree* )curNode->GetNodeAtBox( center, halfExtend );
		if( ! nextNode ){
			break;
		}
		curNode = nextNode;
	}
	
	return curNode;
}

deoalModelOctree *deoalModelOctree::pGetNodeFor( const decVector &position, int maxDepth ){
	deoalModelOctree *curNode = this;
	int i;
	
	for( i=0; i<maxDepth; i++ ){
		const int octant = curNode->FindOctantAtPoint( position );
		if( octant == deoalOctree::eoNotFound ){
			break;
		}
		
		deoalModelOctree *nextNode = ( deoalModelOctree* )curNode->GetNodeAt( octant );
		if( ! nextNode ){
			nextNode = ( deoalModelOctree* )curNode->CreateOctree( octant );
			curNode->SetNodeAt( octant, nextNode );
		}
		
		curNode = nextNode;
	}
	
	return curNode;
}

void deoalModelOctree::pInsertFaceOverlap( deoalModelFace *face,
deoalCollisionTriangle &triangle, int depth ){
	if( depth == 0 ){
		pFaces.Add( face );
		return;
	}
	
	const decVector halfExtend( GetHalfSize() * 0.5f );
	const decVector &center = GetCenter();
	deoalModelOctree *node;
	decVector nodeCenter;
	
	int i;
	for( i=0; i<8; i++ ){
		node = ( deoalModelOctree* )GetNodeAt( i );
		
		if( node ){
			nodeCenter = node->GetCenter();
			
		}else{
			if( ( i & 4 ) == 4 ){
				nodeCenter.x = center.x + halfExtend.x;
				
			}else{
				nodeCenter.x = center.x - halfExtend.x;
			}
			
			if( ( i & 2 ) == 2 ){
				nodeCenter.y = center.y + halfExtend.y;
				
			}else{
				nodeCenter.y = center.y - halfExtend.y;
			}
			
			if( ( i & 1 ) == 1 ){
				nodeCenter.z = center.z + halfExtend.z;
				
			}else{
				nodeCenter.z = center.z - halfExtend.z;
			}
		}
		
		deoalCollisionBox box( nodeCenter, halfExtend );
		if( ! triangle.BoxHitsTriangle( &box ) ){
			continue;
		}
		
		if( ! node ){
			node = new deoalModelOctree( nodeCenter, halfExtend );
			SetNodeAt( i, node );
		}
		
		node->pInsertFaceOverlap( face, triangle, depth - 1 );
	}
}
