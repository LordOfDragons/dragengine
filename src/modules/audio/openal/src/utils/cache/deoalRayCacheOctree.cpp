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

#include "deoalRayCacheOctree.h"
#include "deoalRayCacheRay.h"
#include "../collision/deoalCollisionBox.h"
#include "../collision/deoalCollisionVolume.h"
#include "../collision/deoalCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayCacheOctree
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayCacheOctree::deoalRayCacheOctree( const decVector &center, const decVector &halfSize ) :
deoalOctree( center, halfSize ){
}

deoalRayCacheOctree::~deoalRayCacheOctree(){
	RemoveAllRays();
}



// Management
///////////////

deoalOctree *deoalRayCacheOctree::CreateOctree( int octant ) const{
	const decVector halfSize( GetHalfSize() * 0.5f );
	const decVector &center = GetCenter();
	decVector nc;
	
	if( ( octant & 4 ) == 4 ){
		nc.x = center.x + halfSize.x;
		
	}else{
		nc.x = center.x - halfSize.x;
	}
	
	if( ( octant & 2 ) == 2 ){
		nc.y = center.y + halfSize.y;
		
	}else{
		nc.y = center.y - halfSize.y;
	}
	
	if( ( octant & 1 ) == 1 ){
		nc.z = center.z + halfSize.z;
		
	}else{
		nc.z = center.z - halfSize.z;
	}
	
	return new deoalRayCacheOctree( nc, halfSize );
}

void deoalRayCacheOctree::ClearNodeContent(){
	RemoveAllRays();
}

void deoalRayCacheOctree::ClearRays(){
	RemoveAllRays();
	
	int i;
	for( i=0; i<8; i++ ){
		deoalOctree * const node = GetNodeAt( i );
		if( node ){
			( ( deoalRayCacheOctree* )node )->ClearRays();
		}
	}
}

void deoalRayCacheOctree::InsertRayIntoTree( deoalRayCacheRay *ray, int maxDepth ){
	if( ! ray || maxDepth < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pGetNodeFor( ray->GetOrigin(), maxDepth )->AddRay( ray );
}



// Rays
/////////

int deoalRayCacheOctree::GetRayCount() const{
	return pRays.GetCount();
}

deoalRayCacheRay *deoalRayCacheOctree::GetRayAt( int index ) const{
	return ( deoalRayCacheRay* )pRays.GetAt( index );
}

int deoalRayCacheOctree::IndexOfRay( deoalRayCacheRay *ray ) const{
	return pRays.IndexOf( ray );
}

void deoalRayCacheOctree::AddRay( deoalRayCacheRay *ray ){
	if( ! ray || ray->GetOctreeNode() ){
		DETHROW( deeInvalidParam );
	}
	
	pRays.Add( ray );
	ray->SetOctreeNode( this );
}

void deoalRayCacheOctree::RemoveRay( deoalRayCacheRay *ray ){
	if( ! ray || ray->GetOctreeNode() != this ){
		DETHROW( deeInvalidParam );
	}
	
	pRays.RemoveFrom( pRays.IndexOf( ray ) );
	ray->SetOctreeNode( NULL );
}

void deoalRayCacheOctree::RemoveAllRays(){
	const int count = pRays.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalRayCacheRay* )pRays.GetAt( i ) )->SetOctreeNode( NULL );
	}
	pRays.RemoveAll();
}



	
// Private Functions
//////////////////////

deoalRayCacheOctree *deoalRayCacheOctree::pGetNodeFor( const decVector &position, int maxDepth ){
	deoalOctree *curNode = this;
	int i;
	
	for( i=0; i<maxDepth; i++ ){
		const int octant = curNode->FindOctantAtPoint( position );
		if( octant == deoalOctree::eoNotFound ){
			break;
		}
		
		deoalOctree *nextNode = curNode->GetNodeAt( octant );
		if( ! nextNode ){
			nextNode = curNode->CreateOctree( octant );
			curNode->SetNodeAt( octant, nextNode );
		}
		
		curNode = nextNode;
	}
	
	return ( deoalRayCacheOctree* )curNode;
}
