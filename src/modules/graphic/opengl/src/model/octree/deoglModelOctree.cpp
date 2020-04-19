/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglModelOctree.h"
#include "deoglModelOctreeVisitor.h"
#include "../face/deoglModelFace.h"
#include "../../utils/collision/deoglCollisionBox.h"
#include "../../utils/collision/deoglCollisionVolume.h"
#include "../../utils/collision/deoglCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglModelOctree::deoglModelOctree( const decVector &center, const decVector &halfExtend ) : deoglOctree( center, halfExtend ){
}

deoglModelOctree::~deoglModelOctree(){
}



// Management
///////////////

deoglOctree *deoglModelOctree::CreateOctree( int octant ) const{
	const decVector halfExtend( GetHalfSize() * 0.5f );
	const decVector &center = GetCenter();
	deoglOctree *node = NULL;
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
	node = new deoglModelOctree( nc, halfExtend );
	if( ! node ) DETHROW( deeOutOfMemory );
	
	return node;
}

void deoglModelOctree::ClearNodeContent(){
	ClearFaces();
}

void deoglModelOctree::ClearFaces(){
	deoglOctree *node;
	int i;
	
	pFaces.RemoveAll();
	
	for( i=0; i<8; i++ ){
		node = GetNodeAt( i );
		if( node ){
			( ( deoglModelOctree* )node )->ClearFaces();
		}
	}
}

void deoglModelOctree::InsertFaceIntoTree( deoglModelFace *face, int maxDepth ){
	if( ! face || maxDepth < 0 ) DETHROW( deeInvalidParam );
	
	const decVector &minExtend = face->GetMinExtend();
	const decVector &maxExtend = face->GetMaxExtend();
	const decVector boxCenter = ( minExtend + maxExtend ) * 0.5f;
	const decVector boxHalfExtend = ( maxExtend - minExtend ) * 0.5f;
	
	pGetNodeFor( boxCenter, boxHalfExtend, maxDepth )->GetFaceList().Add( face );
}

void deoglModelOctree::VisitNodesCollidingVolume( deoglModelOctreeVisitor &visitor, deoglCollisionVolume &volume ){
	deoglCollisionBox box( GetCenter(), GetHalfSize() );
	const deoglOctree *node;
	int i;
	
	if( volume.BoxHitsVolume( &box ) ){
		visitor.VisitNode( this, deoglCollisionDetection::eirPartial );
		
		for( i=0; i<8; i++ ){
			node = GetNodeAt( i );
			
			if( node ){
				( ( deoglModelOctree* )node )->VisitNodesCollidingVolume( visitor, volume );
			}
		}
	}
}



// Private Functions
//////////////////////

deoglModelOctree *deoglModelOctree::pGetNodeFor( const decVector &center, const decVector &halfExtend, int maxDepth ){
	deoglOctree *curNode = this;
	deoglOctree *nextNode;
	int d;
	
	for( d=0; d<maxDepth; d++ ){
		nextNode = curNode->GetNodeAtBox( center, halfExtend );
		if( ! nextNode ) break;
		curNode = nextNode;
	}
	
	return ( deoglModelOctree* )curNode;
}

deoglModelOctree *deoglModelOctree::pGetNodeFor( const decVector &position, int maxDepth ){
	deoglOctree *curNode = this;
	deoglOctree *nextNode;
	int d, octant;
	
	for( d=0; d<maxDepth; d++ ){
		octant = curNode->FindOctantAtPoint( position );
		if( octant == deoglOctree::eoNotFound ) break;
		
		nextNode = curNode->GetNodeAt( octant );
		if( ! nextNode ){
			nextNode = curNode->CreateOctree( octant );
			curNode->SetNodeAt( octant, nextNode );
		}
		
		curNode = nextNode;
	}
	
	return ( deoglModelOctree* )curNode;
}
