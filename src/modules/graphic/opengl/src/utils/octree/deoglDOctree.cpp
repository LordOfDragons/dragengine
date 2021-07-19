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

#include "deoglDOctree.h"
#include "deoglDOctreeVisitor.h"
#include "../collision/deoglDCollisionBox.h"
#include "../collision/deoglDCollisionDetection.h"
#include <dragengine/common/exceptions.h>



// Class deoglDOctree
////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglDOctree::deoglDOctree( const decDVector &center, const decDVector &halfSize ) :
pCenter( center ),
pHalfSize( halfSize ),
pMinExtend( center - halfSize ),
pMaxExtend( center + halfSize ),
pParent( NULL )
{
	int i;
	for( i=0; i<8; i++ ){
		pNodes[ i ] = NULL;
	}
}

deoglDOctree::~deoglDOctree(){
	int i;
	
	for( i=0; i<8; i++ ){
		if( pNodes[ i ] ) delete pNodes[ i ];
	}
}



// Management
///////////////

void deoglDOctree::SetParent( deoglDOctree *parent ){
	pParent = parent;
}

deoglDOctree *deoglDOctree::GetNodeAt( int octant ) const{
	if( octant < 0 || octant > 7 ) DETHROW( deeInvalidParam );
	return pNodes[ octant ];
}

void deoglDOctree::SetNodeAt( int octant, deoglDOctree *node ){
	if( octant < 0 || octant > 7 ) DETHROW( deeInvalidParam );
	if( pNodes[ octant ] != node ){
		if( pNodes[ octant ] ) delete pNodes[ octant ];
		pNodes[ octant ] = node;
	}
}

deoglDOctree *deoglDOctree::GetNodeAtBox( const decDVector &minExtend, const decDVector &maxExtend ){
	const int octant = FindOctantAtBox( minExtend, maxExtend );
	
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

deoglDOctree *deoglDOctree::FindNodeAtBox( const decDVector &minExtend, const decDVector &maxExtend ) const{
	int octant = FindOctantAtBox( minExtend, maxExtend );
	
	// if we found no matching octant return NULL
	if( octant == eoNotFound ) return NULL;
	
	// return the node that we found
	return pNodes[ octant ];
}

int deoglDOctree::FindOctantAtBox( const decDVector &minExtend, const decDVector &maxExtend ) const{
	int octant = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit. if an
	// axis does not split no octant is found.
	if( minExtend.x >= pCenter.x ){
		octant |= 4;
		
	}else if( maxExtend.x >= pCenter.x ){
		return eoNotFound;
	}
	
	if( minExtend.y >= pCenter.y ){
		octant |= 2;
		
	}else if( maxExtend.y >= pCenter.y ){
		return eoNotFound;
	}
	
	if( minExtend.z >= pCenter.z ){
		octant |= 1;
		
	}else if( maxExtend.z >= pCenter.z ){
		return eoNotFound;
	}
	
	// return the found octant
	return octant;
}

bool deoglDOctree::ContainsBox( const decDVector &minExtend, const decDVector &maxExtend ) const{
	return minExtend >= pMinExtend && maxExtend <= pMaxExtend;
}

int deoglDOctree::FindOctantAtPoint( const decDVector &point ) const{
	int bitmask = 0;
	
	// determine the bit mask of the box compared to the center of the node.
	// a positive axis becomes a 1 bit and a negative axis a 0 bit.
	if( point.x >= pCenter.x ) bitmask |= 4;
	if( point.y >= pCenter.y ) bitmask |= 2;
	if( point.z >= pCenter.z ) bitmask |= 1;
	
	// return the found octant
	return bitmask;
}

bool deoglDOctree::ContainsPoint( const decDVector &point ) const{
	return point >= pMinExtend &&  point < pMaxExtend;
}



void deoglDOctree::VisitNodes( deoglDOctreeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	int i;
	
	// visit
	visitor->VisitNode( this, deoglDCollisionDetection::eirInside );
	
	// visit each child node
	for( i=0; i<8; i++ ){
		if( pNodes[ i ] ) pNodes[ i ]->VisitNodes( visitor );
	}
}

void deoglDOctree::VisitNodesColliding( deoglDOctreeVisitor *visitor, deoglDCollisionVolume *volume ){
	if( ! visitor || ! volume ) DETHROW( deeInvalidParam );
	deoglDCollisionBox colBox( pCenter, pHalfSize );
	int i;
	
	// exit if this node is not in the collision volume
	if( ! volume->BoxHitsVolume( &colBox ) ) return;
	
	// visit
	visitor->VisitNode( this, deoglDCollisionDetection::eirPartial );
	
	// test each child node
	for( i=0; i<8; i++ ){
		if( pNodes[ i ] ) pNodes[ i ]->VisitNodesColliding( visitor, volume );
	}
}

void deoglDOctree::VisitNodesColliding( deoglDOctreeVisitor *visitor, const decDVector &boxMinExtend, const decDVector &boxMaxExtend ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	int i, result;
	
	result = deoglDCollisionDetection::AABoxIntersectsAABox( pMinExtend, pMaxExtend, boxMinExtend, boxMaxExtend );
	
	if( result == deoglDCollisionDetection::eirOutside ) return;
	
	visitor->VisitNode( this, result );
	
	if( result == deoglDCollisionDetection::eirInside ){
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

void deoglDOctree::ClearTree( bool clearNodes ){
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
