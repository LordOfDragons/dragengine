/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpUnstuckCollider.h"
#include "../../collider/debpCollider.h"
#include "../../world/debpWorld.h"

#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Class debpUnstuckCollider
//////////////////////////////

// Constructor, destructor
////////////////////////////

debpUnstuckCollider::debpUnstuckCollider( debpWorld &world ) :
pWorld( world ),

pStuckCollider( NULL ),

pNearbyColliders( NULL ),
pNearbyColliderCount( 0 ),
pNearbyColliderSize( 0 ),

pBlockerNormals( NULL ),
pBlockerNormalCount( 0 ),
pBlockerNormalSize( 0 )
{
	(void)pWorld; // will be used in the future
}

debpUnstuckCollider::~debpUnstuckCollider(){
	if( pBlockerNormals ){
		delete [] pBlockerNormals;
	}
	if( pNearbyColliders ){
		delete [] pNearbyColliders;
	}
}



// Management
///////////////

void debpUnstuckCollider::SetStuckCollider( debpCollider *collider ){
	pStuckCollider = collider;
}

void debpUnstuckCollider::SetDisplacement( const decDVector &displacement ){
	pDisplacement = displacement;
}

void debpUnstuckCollider::Reset(){
	pStuckCollider = NULL;
	pNearbyColliderCount = 0;
	pBlockerNormalCount = 0;
}

void debpUnstuckCollider::FindNearbyColliders(){
	pNearbyColliderCount = 0;
	
	// TODO visit world to find colliders
}

void debpUnstuckCollider::FindBlockerNormals(){
	pBlockerNormalCount = 0;
	
	// TODO test nearby colliders for collision with the stuck collider and
	// store the normals for them
}

bool debpUnstuckCollider::UnstuckCollider(){
	return true;
}



debpCollider *debpUnstuckCollider::GetNearbyColliderAt( int index ) const{
	if( index < 0 || index >= pNearbyColliderCount ) DETHROW( deeInvalidParam );
	
	return pNearbyColliders[ index ];
}

void debpUnstuckCollider::AddNearbyCollider( debpCollider *collider ){
	if( ! collider ) DETHROW( deeInvalidParam );
	
	if( pNearbyColliderCount == pNearbyColliderSize ){
		debpCollider **newArray = new debpCollider*[ pNearbyColliderSize + 1 ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pNearbyColliders ){
			memcpy( newArray, pNearbyColliders, sizeof( debpCollider* ) * pNearbyColliderSize );
			delete [] pNearbyColliders;
		}
		pNearbyColliders = newArray;
		pNearbyColliderSize++;
	}
	
	pNearbyColliders[ pNearbyColliderCount++ ] = collider;
}

void debpUnstuckCollider::RemoveAllNearbyColliders(){
	pNearbyColliderCount = 0;
}



const decDVector& debpUnstuckCollider::GetBlockerNormalAt( int index ) const{
	if( index < 0 || index >= pBlockerNormalCount ) DETHROW( deeInvalidParam );
	
	return pBlockerNormals[ index ];
}

void debpUnstuckCollider::AddBlockerNormal( const decDVector &normal ){
	if( pBlockerNormalCount == pBlockerNormalSize ){
		decDVector *newArray = new decDVector[ pBlockerNormalSize + 1 ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pBlockerNormals ){
			memcpy( newArray, pBlockerNormals, sizeof( decDVector ) * pBlockerNormalSize );
			delete [] pBlockerNormals;
		}
		pBlockerNormals = newArray;
		pBlockerNormalSize++;
	}
	
	pBlockerNormals[ pBlockerNormalCount++ ] = normal;
}

void debpUnstuckCollider::RemoveAllBlockerNormals(){
	pBlockerNormalCount = 0;
}
