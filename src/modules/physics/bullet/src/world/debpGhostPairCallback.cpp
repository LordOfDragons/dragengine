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

#include "debpCollisionWorld.h"
#include "debpGhostPairCallback.h"
#include "../debpCollisionObject.h"
#include "../debpGhostObject.h"
#include "../collider/debpCollider.h"
#include "../touchsensor/debpTouchSensor.h"

#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/collider/deCollider.h>



// Class debpGhostPairCallback
////////////////////////////////

// Constructor, Destructor
////////////////////////////

debpGhostPairCallback::debpGhostPairCallback(){
}

debpGhostPairCallback::~debpGhostPairCallback(){
}



// Management
///////////////

btBroadphasePair *debpGhostPairCallback::addOverlappingPair( btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1 ){
	// collision pairs added here passed the broad phase filter. they have to near phase
	// filtered if the touch sensor is tracking objects entering and leaving shapes
	const btCollisionObject &btColObj0 = *( ( btCollisionObject*)proxy0->m_clientObject );
	const btCollisionObject &btColObj1 = *( ( btCollisionObject* )proxy1->m_clientObject );
	const debpCollisionObject &colObj0 = *( ( debpCollisionObject* )btColObj0.getUserPointer() );
	const debpCollisionObject &colObj1 = *( ( debpCollisionObject* )btColObj1.getUserPointer() );
	
	// test first proxy for being a touch sensor
	if( colObj0.IsOwnerTouchSensor() ){
		const debpTouchSensor &touchSensor = *colObj0.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->addOverlappingObjectInternal( proxy1, proxy0 );
		
// 		printf( "GPC+: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj1, colObj1.IsOwnerCollider() ? 'C' : ( colObj1.IsOwnerHTSector() ? 'H' : 'T' ) );
		
	// for all other ghost objects use the default behavior
	}else if( btColObj0.getInternalType() == btCollisionObject::CO_GHOST_OBJECT ){ 
		( ( btGhostObject* )&btColObj0 )->addOverlappingObjectInternal( proxy1, proxy0 );
	}
	
	// test second proxy for being a touch sensor
	if( colObj1.IsOwnerTouchSensor() ){
		const debpTouchSensor &touchSensor = *colObj1.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->addOverlappingObjectInternal( proxy0, proxy1 );
		
// 		printf( "GPC+: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj0, colObj0.IsOwnerCollider() ? 'C' : ( colObj0.IsOwnerHTSector() ? 'H' : 'T' ) );
		
	// for all other ghost objects use the default behavior
	}else if( btColObj1.getInternalType() == btCollisionObject::CO_GHOST_OBJECT ){ 
		( ( btGhostObject* )&btColObj1 )->addOverlappingObjectInternal( proxy0, proxy1 );
	}
	
	// return value is not used by bullet code. return NULL for future safety
	return NULL;
}

void *debpGhostPairCallback::removeOverlappingPair( btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1, btDispatcher *dispatcher ){
	const btCollisionObject &btColObj0 = *( ( btCollisionObject*)proxy0->m_clientObject );
	const btCollisionObject &btColObj1 = *( ( btCollisionObject* )proxy1->m_clientObject );
	const debpCollisionObject &colObj0 = *( ( debpCollisionObject* )btColObj0.getUserPointer() );
	const debpCollisionObject &colObj1 = *( ( debpCollisionObject* )btColObj1.getUserPointer() );
	
	// test first proxy for being a touch sensor
	if( colObj0.IsOwnerTouchSensor() ){
		debpTouchSensor &touchSensor = *colObj0.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->removeOverlappingObjectInternal( proxy1, dispatcher, proxy0 );
		
// 		printf( "GPC-: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj1, colObj1.IsOwnerCollider() ? 'C' : ( colObj1.IsOwnerHTSector() ? 'H' : 'T' ) );
		
		if( touchSensor.GetTouchSensor().GetTrackEnterLeave() ){
			if( colObj1.IsOwnerCollider() ){
				debpCollider * const collider = colObj1.GetOwnerCollider();
				
				if( touchSensor.GetTouchingColliders().Has( collider ) ){
// 					printf( "GPC: ts=%p s=%p => remove collider %p\n", touchSensor.GetTouchSensor(), &touchSensor, &colObj1 );
					touchSensor.GetTouchingColliders().Remove( collider );
					touchSensor.GetLeavingColliders().Add( collider );
				}
				
			//}else if( colObj1.IsOwnerHTSector() ){
			//	// TODO
			}
		}
		
	// for all other ghost objects use the default behavior
	}else if( btColObj0.getInternalType() == btCollisionObject::CO_GHOST_OBJECT ){ 
		( ( btGhostObject* )&btColObj0 )->removeOverlappingObjectInternal( proxy1, dispatcher, proxy0 );
	}
	
	// test second proxy for being a touch sensor
	if( colObj1.IsOwnerTouchSensor() ){
		debpTouchSensor &touchSensor = *colObj1.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->removeOverlappingObjectInternal( proxy0, dispatcher, proxy1 );
		
// 		printf( "GPC-: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj0, colObj0.IsOwnerCollider() ? 'C' : ( colObj0.IsOwnerHTSector() ? 'H' : 'T' ) );
		
		if( touchSensor.GetTouchSensor().GetTrackEnterLeave() ){
			if( colObj0.IsOwnerCollider() ){
				debpCollider * const collider = colObj0.GetOwnerCollider();
				
				if( touchSensor.GetTouchingColliders().Has( collider ) ){
// 					printf( "GPC: ts=%p s=%p => remove collider %p\n", touchSensor.GetTouchSensor(), &touchSensor, &colObj1 );
					touchSensor.GetTouchingColliders().Remove( collider );
					touchSensor.GetLeavingColliders().Add( collider );
				}
				
			//}else if( colObj1.IsOwnerHTSector() ){
			//	// TODO
			}
		}
		
	// for all other ghost objects use the default behavior
	}else if( btColObj1.getInternalType() == btCollisionObject::CO_GHOST_OBJECT ){ 
		( ( btGhostObject* )&btColObj1 )->removeOverlappingObjectInternal( proxy1, dispatcher, proxy0 );
	}
	
	// return NULL... no idea why
	return NULL;
}

void debpGhostPairCallback::removeOverlappingPairsContainingProxy( btBroadphaseProxy *proxy0, btDispatcher *dispatcher ){
	// bullet throws an assertion so this is most probably not supposed to be called yet
	DETHROW( deeInvalidAction );
}
