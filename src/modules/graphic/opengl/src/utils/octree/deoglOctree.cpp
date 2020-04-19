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
#include <string.h>
#include <stdlib.h>

#include "deoglOctree.h"
#include "deoglOctreeVisitor.h"
#include "../collision/deoglCollisionBox.h"
#include "../collision/deoglCollisionDetection.h"
#include <dragengine/common/exceptions.h>



// Class deoglOctree
////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglOctree::deoglOctree( const decVector &center, const decVector &halfSize ){
	int i;
	
	for( i=0; i<8; i++ ) pNodes[ i ] = NULL;
	pCenter = center;
	pHalfSize = halfSize;
	pParent = NULL;
}

deoglOctree::~deoglOctree(){
	int i;
	
	for( i=0; i<8; i++ ){
		if( pNodes[ i ] ) delete pNodes[ i ];
	}
}



// Management
///////////////

void deoglOctree::SetParent( deoglOctree *parent ){
	pParent = parent;
}

deoglOctree *deoglOctree::GetNodeAt( int octant ) const{
	if( octant < 0 || octant > 7 ) DETHROW( deeInvalidParam );
	return pNodes[ octant ];
}

void deoglOctree::SetNodeAt( int octant, deoglOctree *node ){
	if( octant < 0 || octant > 7 ) DETHROW( deeInvalidParam );
	if( pNodes[ octant ] != node ){
		if( pNodes[ octant ] ) delete pNodes[ octant ];
		pNodes[ octant ] = node;
	}
}

deoglOctree *deoglOctree::GetNodeAtBox( const decVector &boxCenter, const decVector &boxHalfSize ){
	int octant = FindOctantAtBox( boxCenter, boxHalfSize );
	
	// if we found no matching octant return NULL
	if( octant == eoNotFound ) return NULL;
	
	// if the node does not exist create it
	if( ! pNodes[ octant ] ){
		pNodes[ octant ] = CreateOctree( octant );
		pNodes[ octant ]->SetParent( this );
	}
	
	// return the node that we found
	return pNodes[ octant ];
}

deoglOctree *deoglOctree::FindNodeAtBox( const decVector &boxCenter, const decVector &boxHalfSize ) const{
	int octant = FindOctantAtBox( boxCenter, boxHalfSize );
	
	// if we found no matching octant return NULL
	if( octant == eoNotFound ) return NULL;
	
	// return the node that we found
	return pNodes[ octant ];
}

int deoglOctree::FindOctantAtBox( const decVector &boxCenter, const decVector &boxHalfSize ) const{
	int octant = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit. if an
	// axis does not split no octant is found.
	if( boxCenter.x - boxHalfSize.x >= pCenter.x ){
		octant |= 4;
	}else if( boxCenter.x + boxHalfSize.x >= pCenter.x ){
		return eoNotFound;
	}
	if( boxCenter.y - boxHalfSize.y >= pCenter.y ){
		octant |= 2;
	}else if( boxCenter.y + boxHalfSize.y >= pCenter.y ){
		return eoNotFound;
	}
	if( boxCenter.z - boxHalfSize.z >= pCenter.z ){
		octant |= 1;
	}else if( boxCenter.z + boxHalfSize.z >= pCenter.z ){
		return eoNotFound;
	}
	
	// return the found octant
	return octant;
}

bool deoglOctree::ContainsBox( const decVector &boxCenter, const decVector &boxHalfSize ) const{
	return ( boxCenter - boxHalfSize ) >= ( pCenter - pHalfSize )
		&& ( boxCenter + boxHalfSize ) < ( pCenter + pHalfSize );
}

deoglOctree *deoglOctree::FindNodeAtPoint( const decVector &point ) const{
	int octant = FindOctantAtPoint( point );
	
	// if we found no matching octant return NULL
	if( octant == eoNotFound ) return NULL;
	
	// return the node that we found
	return pNodes[ octant ];
}

int deoglOctree::FindOctantAtPoint( const decVector &point ) const{
	int bitmask = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit.
	if( point.x >= pCenter.x ) bitmask |= 4;
	if( point.y >= pCenter.y ) bitmask |= 2;
	if( point.z >= pCenter.z ) bitmask |= 1;
	
	// return the found octant
	return bitmask;
}

bool deoglOctree::ContainsPoint( const decVector &point ) const{
	return point >= ( pCenter - pHalfSize ) &&  point < ( pCenter + pHalfSize );
}



deoglOctree *deoglOctree::SearchTreeForBox( const decVector &boxCenter, const decVector &boxHalfSize ) const{
	deoglOctree *nextNode = FindNodeAtBox( boxCenter, boxHalfSize );
	deoglOctree *curNode = ( deoglOctree* )this;
	
	while( nextNode ){
		curNode = nextNode;
		nextNode = curNode->FindNodeAtBox( boxCenter, boxHalfSize );
	}
	
	return curNode;
}

deoglOctree *deoglOctree::SearchTreeForPoint( const decVector &point ) const{
	deoglOctree *nextNode = FindNodeAtPoint( point );
	deoglOctree *curNode = ( deoglOctree* )this;
	
	while( nextNode ){
		curNode = nextNode;
		nextNode = curNode->FindNodeAtPoint( point );
	}
	
	return curNode;
}

void deoglOctree::VisitNodes( deoglOctreeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	int i;
	
	// visit
	visitor->VisitNode( this, deoglCollisionDetection::eirInside );
	
	// visit each child node
	for( i=0; i<8; i++ ){
		if( pNodes[ i ] ) pNodes[ i ]->VisitNodes( visitor );
	}
}

void deoglOctree::VisitNodesColliding( deoglOctreeVisitor *visitor, deoglCollisionVolume *volume ){
	if( ! visitor || ! volume ) DETHROW( deeInvalidParam );
	deoglCollisionBox colBox( pCenter, pHalfSize );
	int i;
	
	// exit if this node is not in the collision volume
	if( ! volume->BoxHitsVolume( &colBox ) ) return;
	
	// visit
	visitor->VisitNode( this, deoglCollisionDetection::eirPartial );
	
	// test each child node
	for( i=0; i<8; i++ ){
		if( pNodes[ i ] ) pNodes[ i ]->VisitNodesColliding( visitor, volume );
	}
}

void deoglOctree::VisitNodesColliding( deoglOctreeVisitor *visitor, const decVector &boxMinExtend, const decVector &boxMaxExtend ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	int i, result;
	
	result = deoglCollisionDetection::AABoxIntersectsAABox( pCenter - pHalfSize, pCenter + pHalfSize, boxMinExtend, boxMaxExtend );
	
	if( result == deoglCollisionDetection::eirOutside ) return;
	
	visitor->VisitNode( this, result );
	
	if( result == deoglCollisionDetection::eirInside ){
		for( i=0; i<8; i++ ){
			if( pNodes[ i ] ){
				pNodes[ i ]->VisitNodes( visitor );
			}
		}
		
	}else{
		for( i=0; i<8; i++ ){
			if( pNodes[ i ] ){
				pNodes[ i ]->VisitNodesColliding( visitor, boxMinExtend, boxMaxExtend );
			}
		}
	}
}

void deoglOctree::ClearTree( bool clearNodes ){
	int i;
	
	ClearNodeContent();
	
	for( i=0; i<8; i++ ){
		if( pNodes[ i ] ){
			pNodes[ i ]->ClearTree( clearNodes );
			if( clearNodes ){
				delete pNodes[ i ];
				pNodes[ i ] = NULL;
			}
		}
	}
}
