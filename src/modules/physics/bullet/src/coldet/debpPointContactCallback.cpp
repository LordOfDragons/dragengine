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

#include "debpPointContactCallback.h"
#include "../debpCollisionObject.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpPointContactCallback
//////////////////////////////////

// Constructors
/////////////////

debpPointContactCallback::debpPointContactCallback( const btVector3 &point,
const decCollisionFilter &collisionFilter, deBaseScriptingCollider &listener,
debpCollisionDetection &colDet ) :
pPoint( point ),
pCollisionFilter( collisionFilter ),
pListener( listener ),
pColDet( colDet ){
}



// Bullet
///////////

bool debpPointContactCallback::process( const btBroadphaseProxy *proxy ){
	btCollisionObject * const bpColObj = ( btCollisionObject* )proxy->m_clientObject;
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )bpColObj->getUserPointer() );
	bool callListener = false;
	
	// test against collider
	if( colObj.IsOwnerCollider() ){
		debpCollider * const collider = colObj.GetOwnerCollider();
		deCollider * const engCollider = &collider->GetCollider();
		
		if( pCollisionFilter.CollidesNot( engCollider->GetCollisionFilter() ) ){
			return true;
		}
		if( ! pListener.CanHitCollider( NULL, engCollider ) ){
			return true;
		}
		if( ! pColDet.GetBulletShapeCollision().IsPointInside( *bpColObj, pPoint ) ){
			return true;
		}
		
		pColDet.GetCollisionInfo()->SetCollider( engCollider, colObj.GetOwnerBone(),
			( int )( intptr_t )bpColObj->getCollisionShape()->getUserPointer() - 1,
			-1 /* convexResult.m_localShapeInfo->m_triangleIndex; // problem... bullet index not our index */ );
		callListener = true;
		
	// test against height terrain
	}else if( colObj.IsOwnerHTSector() ){
		const decCollisionFilter &cfHT = colObj.GetOwnerHTSector()->GetHeightTerrain()->
			GetHeightTerrain()->GetCollisionFilter();
			
		if( ! pCollisionFilter.Collides( cfHT ) ){
			return true;
		}
		
		//if( pColDet.ColliderHitsHeightTerrain( pCollider, colObj.GetOwnerHTSector(), pResult ) ){
		// how should this test look like?
		return true;
		
		const debpHTSector &htsector = *colObj.GetOwnerHTSector();
		pColDet.GetCollisionInfo()->SetHTSector( htsector.GetHeightTerrain()->GetHeightTerrain(),
			htsector.GetSector() );
		callListener = true;
	}
	
	// call listener if requested
	deCollisionInfo * const colInfo = pColDet.GetCollisionInfo();
	
	if( callListener ){
		colInfo->SetDistance( 0.0f ); // not used
		colInfo->SetNormal( decVector( 0.0f, 1.0f, 0.0f ) ); // not used
		pListener.CollisionResponse( NULL, colInfo );
	}
	
	// the return value seems to be not used. we use it the same way as in the sweep case
	return colInfo->GetStopTesting() ? BT_ZERO : BT_ONE;
}
