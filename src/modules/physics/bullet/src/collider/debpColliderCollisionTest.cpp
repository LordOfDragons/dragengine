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

#include "debpColliderCollisionTest.h"
#include "debpCollider.h"
#include "debpColliderComponent.h"
#include "../component/debpComponent.h"
#include "../world/debpWorld.h"
#include "../dePhysicsBullet.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deColliderCollisionTest.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>



// Class debpColliderCollisionTest
////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderCollisionTest::debpColliderCollisionTest(
debpCollider &parentCollider, deColliderCollisionTest &collisionTest ) :
pParentCollider( parentCollider ),
pCollisionTest( collisionTest ),
pSortByDistance( false ){
}

debpColliderCollisionTest::~debpColliderCollisionTest(){
}



// Management
///////////////

void debpColliderCollisionTest::CollisionTestChanged(){
}

void debpColliderCollisionTest::Update(){
	if( ! pCollisionTest.GetEnabled() ){
		return;
	}
	
	const deCollider &parentCollider = pParentCollider.GetCollider();
	const decQuaternion &orientation = parentCollider.GetOrientation();
	const decDMatrix &matrix = pParentCollider.GetMatrix();
	
	// cast position altered by bone if existing. for the time being this is just
	// done by preparing the matrices in the parent collider component if existing.
	// this will be optimized later
	decDVector position( pCollisionTest.GetOrigin() );
	decVector direction( pCollisionTest.GetDirection() );
	
	if( ! pCollisionTest.GetBone().IsEmpty() ){
		deComponent * const component = pCollisionTest.GetComponent();
		
		if( component ){
			deRig * const rig = component->GetRig();
			
			if( rig ){
				const int boneIndex = rig->IndexOfBoneNamed( pCollisionTest.GetBone() );
				
				if( boneIndex != -1 ){
					( ( debpComponent* )component->GetPeerPhysics() )->PrepareBone( boneIndex );
					
					const decMatrix &boneMatrix = component->GetBoneAt( boneIndex ).GetMatrix();
					
					position = decDVector( boneMatrix * pCollisionTest.GetOrigin() );
					
					if( pCollisionTest.GetLocalDirection() ){
						direction = boneMatrix.TransformNormal( direction );
					}
				}
			}
		}
	}
	
	if( pCollisionTest.GetLocalDirection() ){
		position = matrix * position;
		direction = matrix.TransformNormal( direction );
		
	}else{
		position += matrix.GetPosition();
	}
	
	// store the used test parameters in case somebody needs them
	pCollisionTest.SetTestOrigin( position );
	pCollisionTest.SetTestDirection( direction );
	
	// test collision and store the result
	deCollider * const collider = pCollisionTest.GetCollider();
	
	pCollisionInfoCount = 0;
	
	if( collider ){
		if( direction.IsZero() ){
			ColliderHits( position, orientation );
			
		}else{
			ColliderMoveHits( position, orientation, direction );
		}
		
	}else{
		RayHits( position, direction );
	}
	
	SetCollisionTestResult();
// 	pParentCollider.GetBullet()->LogInfoFormat( "CT: p(%f,%f,%f) d(%f,%f,%f) c=%p hc=%i hd=%f ts=(%f,%f,%f)\n",
// 		position.x, position.y, position.z, direction.x, direction.y, direction.z, collider,
// 		pHasCollision, pHitDistance, pCollisionTest.GetTouchSensor()->GetPosition().x,
// 		pCollisionTest.GetTouchSensor()->GetPosition().y, pCollisionTest.GetTouchSensor()->GetPosition().z );
}

void debpColliderCollisionTest::SetCollisionTestResult(){
	pCollisionTest.RemoveAllCollisionInfo();
	
	int i;
	for( i=0; i<pCollisionInfoCount; i++ ){
		pCollisionTest.AddCollisionInfo( ( deCollisionInfo* )pCollisionInfo.GetAt( i ) );
	}
}



void debpColliderCollisionTest::Reset(){
	pCollisionInfoCount = 0;
	pSortByDistance = false;
}

void debpColliderCollisionTest::RayHits( const decDVector &position, const decVector &direction ){
	deTouchSensor * const touchSensor = pCollisionTest.GetTouchSensor();
	
	pSortByDistance = true;
	
	if( touchSensor ){
		touchSensor->RayHits( position, direction, this );
		
	}else{
		pParentCollider.GetParentWorld()->RayHits( position, direction, this, pCollisionTest.GetCollisionFilter() );
	}
}

void debpColliderCollisionTest::ColliderHits( const decDVector &position ){
	ColliderHits( position, decQuaternion() );
}

void debpColliderCollisionTest::ColliderHits( const decDVector &position, const decQuaternion &orientation ){
	deCollider * const collider = pCollisionTest.GetCollider();
	if( ! collider ){
		return;
	}
	
	collider->SetPosition( position );
	collider->SetOrientation( orientation );
	
	pSortByDistance = false;
	
	deTouchSensor * const touchSensor = pCollisionTest.GetTouchSensor();
	
	if( touchSensor ){
		touchSensor->ColliderHits( collider, this );
		
	}else{
		pParentCollider.GetParentWorld()->ColliderHits( collider, this );
	}
}

void debpColliderCollisionTest::ColliderMoveHits( const decDVector &position, const decVector &direction ){
	ColliderMoveHits( position, decQuaternion(), direction );
}

void debpColliderCollisionTest::ColliderMoveHits( const decDVector &position, const decQuaternion &orientation,
const decVector &direction ){
	deCollider * const collider = pCollisionTest.GetCollider();
	if( ! collider ){
		return;
	}
	
	collider->SetPosition( position );
	collider->SetOrientation( orientation );
	
	pSortByDistance = true;
	
	deTouchSensor * const touchSensor = pCollisionTest.GetTouchSensor();
	
	if( touchSensor ){
		touchSensor->ColliderMoveHits( collider, direction, this );
		
	}else{
		pParentCollider.GetParentWorld()->ColliderMoveHits( collider, direction, this );
	}
}

void debpColliderCollisionTest::ColliderRotateHits( const decDVector &position, const decVector &rotation ){
	ColliderRotateHits( position, decQuaternion(), rotation );
}

void debpColliderCollisionTest::ColliderRotateHits( const decDVector &position, const decQuaternion &orientation,
const decVector &rotation ){
	deCollider * const collider = pCollisionTest.GetCollider();
	if( ! collider ){
		return;
	}
	
	collider->SetPosition( position );
	collider->SetOrientation( orientation );
	
	pSortByDistance = true;
	
	deTouchSensor * const touchSensor = pCollisionTest.GetTouchSensor();
	
	if( touchSensor ){
		touchSensor->ColliderRotateHits( collider, rotation, this );
		
	}else{
		pParentCollider.GetParentWorld()->ColliderRotateHits( collider, rotation, this );
	}
}

void debpColliderCollisionTest::ColliderMoveRotateHits( const decDVector &position, const decVector &direction,
const decVector &rotation ){
	ColliderMoveRotateHits( position, decQuaternion(), direction, rotation );
}

void debpColliderCollisionTest::ColliderMoveRotateHits( const decDVector &position, const decQuaternion &orientation,
const decVector &direction, const decVector &rotation ){
	deCollider * const collider = pCollisionTest.GetCollider();
	if( ! collider ){
		return;
	}
	
	collider->SetPosition( position );
	collider->SetOrientation( orientation );
	
	pSortByDistance = true;
	
	deTouchSensor * const touchSensor = pCollisionTest.GetTouchSensor();
	
	if( touchSensor ){
		touchSensor->ColliderMoveRotateHits( collider, direction, rotation, this );
		
	}else{
		pParentCollider.GetParentWorld()->ColliderMoveRotateHits( collider, direction, rotation, this );
	}
}



// Notifications
//////////////////

void debpColliderCollisionTest::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	// find index to insert information at if sorting by distance is used.
	int index = pCollisionInfoCount;
	
	if( pSortByDistance ){
		const float distance = info->GetDistance();
		
		for( index=0; index<pCollisionInfoCount; index++ ){
			const deCollisionInfo &checkInfo = *( ( deCollisionInfo* )pCollisionInfo.GetAt( index ) );
			if( distance < checkInfo.GetDistance() ){
				break;
			}
		}
	}
	
	// we need to also check if the same collision exists already. this can happen if a
	// collider has multiple shapes (for itself or a bone). the collision routine can
	// potentially test collision with each individual shape in the shape list. we want to
	// make sure we add a collision for each affected shape only once. if the collision is
	// duplicate the existing one has to be updated and potentially moved
	int moveFromIndex = pCollisionInfoCount;
	
	if( info->GetCollider() ){
		int i;
		
		for( i=0; i<pCollisionInfoCount; i++ ){
			const deCollisionInfo &checkInfo = *( ( deCollisionInfo* )pCollisionInfo.GetAt( i ) );
			if( checkInfo.GetCollider() != info->GetCollider() ){
				continue;
			}
			if( checkInfo.GetBone() != info->GetBone() ){
				continue;
			}
			if( checkInfo.GetShape() != info->GetShape() ){
				continue;
			}
			if( checkInfo.GetFace() != info->GetFace() ){
				continue;
			}
			
			// same collision
			if( pSortByDistance ){
				// if the distance is better use it as the info to move otherwise
				// exit. index is the index of the first collision info having a distance larger
				// than the current collision distance. if i is less than index then the current
				// collision is farther away and we have to discard it. otherwise it is a farther
				// away collision of the same object we improve
				if( i < index ){
					return;
				}
				
				moveFromIndex = i;
				break;
				
			}else{
				// if no sorting by distance is used forget the new result
				return;
			}
		}
	}
	
	// create new collision info if list is not large enough. only done if we do not update
	// an already existing info
	if( moveFromIndex == pCollisionInfoCount && pCollisionInfoCount == pCollisionInfo.GetCount() ){
		deCollisionInfo * const newInfo = new deCollisionInfo;
		
		try{
			pCollisionInfo.Add( newInfo );
			
		}catch( const deException & ){
			newInfo->FreeReference();
			throw;
		}
		
		newInfo->FreeReference();
	}
	
	// move collision info to the right index if required
	if( index != moveFromIndex ){
		pCollisionInfo.Move( moveFromIndex, index );
	}
	
	// fill information at index with information
	deCollisionInfo * const updateInfo = ( deCollisionInfo* )pCollisionInfo.GetAt( index );
	
	if( info->GetCollider() ){
		updateInfo->SetCollider( info->GetCollider(), info->GetBone(), info->GetShape(), info->GetFace() );
		
	}else if( info->GetHeightTerrain() ){
		updateInfo->SetHTSector( info->GetHeightTerrain(), info->GetHTSector() );
		
	}else{
		updateInfo->Clear();
	}
	
	updateInfo->SetDistance( info->GetDistance() );
	updateInfo->SetNormal( info->GetNormal() );
	
	// increment count if no present collision info has been improved
	if( moveFromIndex == pCollisionInfoCount ){
		pCollisionInfoCount++;
	}
}

bool debpColliderCollisionTest::CanHitCollider( deCollider *owner, deCollider *collider ){
	// no collision with parent collider
	deCollider * const parentCollider = &pParentCollider.GetCollider();
	if( collider == parentCollider ){
		return false;
	}
	
	// no collision with ignore colliders in the parent collider
	if( parentCollider->HasIgnoreCollider( collider ) ){
		return false;
	}
	
	// otherwise ask parent collider collision listener
	deBaseScriptingCollider * const listener = parentCollider->GetPeerScripting();
	if( listener ){
		return listener->CanHitCollider( NULL, collider );
	}
	
	// otherwise accept the collision
	return true;
}
