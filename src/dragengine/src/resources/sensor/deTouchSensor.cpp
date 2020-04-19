/* 
 * Drag[en]gine Game Engine
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

#include "deTouchSensor.h"
#include "deTouchSensorManager.h"
#include "../world/deWorld.h"
#include "../../deEngine.h"
#include "../../common/shape/decShape.h"
#include "../../common/exceptions.h"
#include "../../resources/collider/deCollider.h"
#include "../../systems/modules/physics/deBasePhysicsTouchSensor.h"
#include "../../systems/modules/scripting/deBaseScriptingTouchSensor.h"



// Class deTouchSensor
////////////////////////

// Constructor, destructor
////////////////////////////

deTouchSensor::deTouchSensor( deTouchSensorManager *manager ) :
deResource( manager ),

pTrackEnterLeave( true ),
pEnabled( true ),

pPeerPhysics( NULL ),
pPeerScripting( NULL ),

pParentWorld( NULL ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL ){
}

deTouchSensor::~deTouchSensor(){
	if( pPeerPhysics ){
		delete pPeerPhysics;
	}
	if( pPeerScripting ){
		delete pPeerScripting;
	}
}



// Management
///////////////

void deTouchSensor::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	if( pPeerPhysics ){
		pPeerPhysics->PositionChanged();
	}
}

void deTouchSensor::SetOrientation( const decQuaternion &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	if( pPeerPhysics ){
		pPeerPhysics->OrientationChanged();
	}
}



void deTouchSensor::SetCollisionFilter( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = collisionFilter;
	
	if( pPeerPhysics ){
		pPeerPhysics->CollisionFilterChanged();
	}
}

void deTouchSensor::SetTrackEnterLeave( bool trackEnterLeave ){
	if( trackEnterLeave == pTrackEnterLeave ){
		return;
	}
	
	pTrackEnterLeave = trackEnterLeave;
	
	if( pPeerPhysics ){
		pPeerPhysics->TrackEnterLeaveChanged();
	}
}

void deTouchSensor::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	if( pPeerPhysics ){
		pPeerPhysics->EnabledChanged();
	}
}



void deTouchSensor::NotifyColliderEntered( deCollider *collider ){
	if( ! collider ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerScripting ){
		pPeerScripting->ColliderEntered( collider );
	}
}

void deTouchSensor::NotifyColliderLeft( deCollider *collider ){
	if( ! collider ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerScripting ){
		pPeerScripting->ColliderLeft( collider );
	}
}



bool deTouchSensor::IsEmpty() const{
	if( pPeerPhysics ){
		return pPeerPhysics->IsEmpty();
	}
	return true;
}

int deTouchSensor::GetColliderCount() const{
	if( pPeerPhysics ){
		return pPeerPhysics->GetColliderCount();
	}
	return 0;
}

deCollider *deTouchSensor::GetColliderAt( int collider ) const{
	if( pPeerPhysics ){
		return pPeerPhysics->GetColliderAt( collider );
	}
	DETHROW( deeInvalidParam );
}



// Colliders to ignore
////////////////////////

int deTouchSensor::GetIgnoreColliderCount() const{
	return pIgnoreColliders.GetCount();
}

deCollider *deTouchSensor::GetIgnoreColliderAt( int index ) const{
	return ( deCollider* )pIgnoreColliders.GetAt( index );
}

bool deTouchSensor::HasIgnoreCollider( deCollider *collider ) const{
	return pIgnoreColliders.Has( collider );
}

void deTouchSensor::AddIgnoreCollider( deCollider *collider ){
	pIgnoreColliders.Add( collider );
	
	if( pPeerPhysics ){
		pPeerPhysics->IgnoreCollidersChanged();
	}
}

void deTouchSensor::RemoveIgnoreCollider( deCollider *collider ){
	pIgnoreColliders.Remove( collider );
	
	if( pPeerPhysics ){
		pPeerPhysics->IgnoreCollidersChanged();
	}
}

void deTouchSensor::RemoveAllIgnoreColliders(){
	pIgnoreColliders.RemoveAll();
	
	if( pPeerPhysics ){
		pPeerPhysics->IgnoreCollidersChanged();
	}
}



// Collision Detection
////////////////////////

bool deTouchSensor::PointInside( const decDVector &point ){
	if( pPeerPhysics ){
		return pPeerPhysics->PointInside( point );
	}
	return false;
}

void deTouchSensor::AllHits( deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->AllHits( listener );
	}
}

void deTouchSensor::RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->RayHits( rayOrigin, rayDirection, listener );
	}
}

void deTouchSensor::ColliderHits( deCollider *collider, deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderHits( collider, listener );
	}
}

void deTouchSensor::ColliderMoveHits( deCollider *collider, const decVector &displacement,
deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderMoveHits( collider, displacement, listener );
	}
}

void deTouchSensor::ColliderRotateHits( deCollider *collider, const decVector &rotation,
deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderRotateHits( collider, rotation, listener );
	}
}

void deTouchSensor::ColliderMoveRotateHits( deCollider *collider,
const decVector &displacement, const decVector &rotation, deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderMoveRotateHits( collider, displacement, rotation, listener );
	}
}



// Shapes
///////////

void deTouchSensor::SetShape( const decShapeList &shape ){
	pShape = shape;
	if( pPeerPhysics ){
		pPeerPhysics->ShapeChanged();
	}
}



// System Peers
/////////////////

void deTouchSensor::SetPeerPhysics( deBasePhysicsTouchSensor *phyTS ){
	if( phyTS == pPeerPhysics ){
		return;
	}
	
	if( pPeerPhysics ){
		delete pPeerPhysics;
	}
	pPeerPhysics = phyTS;
}

void deTouchSensor::SetPeerScripting( deBaseScriptingTouchSensor *scrTS ){
	if( scrTS == pPeerScripting ){
		return;
	}
	
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	pPeerScripting = scrTS;
}



// Linked List
////////////////

void deTouchSensor::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deTouchSensor::SetLLWorldPrev( deTouchSensor *touchSensor ){
	pLLWorldPrev = touchSensor;
}

void deTouchSensor::SetLLWorldNext( deTouchSensor *touchSensor ){
	pLLWorldNext = touchSensor;
}
