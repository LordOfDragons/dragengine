/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
