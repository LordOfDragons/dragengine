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

#include "debpClosestRayResultCallback.h"
#include "../debpCollisionObject.h"
#include "../collider/debpCollider.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/common/exceptions.h>



// Class debpClosestRayResultCallback
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpClosestRayResultCallback::debpClosestRayResultCallback( const btVector3 &rayFromWorld,
const btVector3 &rayToWorld, const decCollisionFilter *collisionFilter ) :
ClosestRayResultCallback( rayFromWorld, rayToWorld ),
pCollisionFilter( collisionFilter ){
}



// Overwriting
////////////////

bool debpClosestRayResultCallback::needsCollision( btBroadphaseProxy *proxy0 ) const{
	if( ! btCollisionWorld::RayResultCallback::needsCollision( proxy0 ) ){
		return false;
	}
	
	const btCollisionObject &collisionObject = *( ( btCollisionObject* )proxy0->m_clientObject );
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )collisionObject.getUserPointer() );
	
	if( colObj.IsOwnerCollider() ){
		return pCollisionFilter->Collides( colObj.GetOwnerCollider()->GetCollider().GetCollisionFilter() );
		
	}else if( colObj.IsOwnerHTSector() ){
		return pCollisionFilter->Collides( colObj.GetOwnerHTSector()->GetHeightTerrain()
			->GetHeightTerrain()->GetCollisionFilter() );
	}
	
	// all other combinations score no collision
	return false;
}
