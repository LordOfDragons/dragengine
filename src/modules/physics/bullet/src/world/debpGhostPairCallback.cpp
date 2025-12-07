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

btBroadphasePair *debpGhostPairCallback::addOverlappingPair(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1){
	// collision pairs added here passed the broad phase filter. they have to near phase
	// filtered if the touch sensor is tracking objects entering and leaving shapes
	const btCollisionObject &btColObj0 = *((btCollisionObject*)proxy0->m_clientObject);
	const btCollisionObject &btColObj1 = *((btCollisionObject*)proxy1->m_clientObject);
	const debpCollisionObject &colObj0 = *((debpCollisionObject*)btColObj0.getUserPointer());
	const debpCollisionObject &colObj1 = *((debpCollisionObject*)btColObj1.getUserPointer());
	
	// test first proxy for being a touch sensor
	if(colObj0.IsOwnerTouchSensor()){
		const debpTouchSensor &touchSensor = *colObj0.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->addOverlappingObjectInternal(proxy1, proxy0);
		
// 		printf( "GPC+: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj1, colObj1.IsOwnerCollider() ? 'C' : ( colObj1.IsOwnerHTSector() ? 'H' : 'T' ) );
		
	// for all other ghost objects use the default behavior
	}else if(btColObj0.getInternalType() == btCollisionObject::CO_GHOST_OBJECT){ 
		((btGhostObject*)&btColObj0)->addOverlappingObjectInternal(proxy1, proxy0);
	}
	
	// test second proxy for being a touch sensor
	if(colObj1.IsOwnerTouchSensor()){
		const debpTouchSensor &touchSensor = *colObj1.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->addOverlappingObjectInternal(proxy0, proxy1);
		
// 		printf( "GPC+: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj0, colObj0.IsOwnerCollider() ? 'C' : ( colObj0.IsOwnerHTSector() ? 'H' : 'T' ) );
		
	// for all other ghost objects use the default behavior
	}else if(btColObj1.getInternalType() == btCollisionObject::CO_GHOST_OBJECT){ 
		((btGhostObject*)&btColObj1)->addOverlappingObjectInternal(proxy0, proxy1);
	}
	
	// return value is not used by bullet code. return NULL for future safety
	return NULL;
}

void *debpGhostPairCallback::removeOverlappingPair(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1, btDispatcher *dispatcher){
	const btCollisionObject &btColObj0 = *((btCollisionObject*)proxy0->m_clientObject);
	const btCollisionObject &btColObj1 = *((btCollisionObject*)proxy1->m_clientObject);
	const debpCollisionObject &colObj0 = *((debpCollisionObject*)btColObj0.getUserPointer());
	const debpCollisionObject &colObj1 = *((debpCollisionObject*)btColObj1.getUserPointer());
	
	// test first proxy for being a touch sensor
	if(colObj0.IsOwnerTouchSensor()){
		debpTouchSensor &touchSensor = *colObj0.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->removeOverlappingObjectInternal(proxy1, dispatcher, proxy0);
		
// 		printf( "GPC-: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj1, colObj1.IsOwnerCollider() ? 'C' : ( colObj1.IsOwnerHTSector() ? 'H' : 'T' ) );
		
		if(touchSensor.GetTouchSensor().GetTrackEnterLeave()){
			if(colObj1.IsOwnerCollider()){
				debpCollider * const collider = colObj1.GetOwnerCollider();
				
				if(touchSensor.GetTouchingColliders().Has(collider)){
// 					printf( "GPC: ts=%p s=%p => remove collider %p\n", touchSensor.GetTouchSensor(), &touchSensor, &colObj1 );
					touchSensor.GetTouchingColliders().Remove(collider);
					touchSensor.GetLeavingColliders().Add(collider);
				}
				
			//}else if( colObj1.IsOwnerHTSector() ){
			//	// TODO
			}
		}
		
	// for all other ghost objects use the default behavior
	}else if(btColObj0.getInternalType() == btCollisionObject::CO_GHOST_OBJECT){ 
		((btGhostObject*)&btColObj0)->removeOverlappingObjectInternal(proxy1, dispatcher, proxy0);
	}
	
	// test second proxy for being a touch sensor
	if(colObj1.IsOwnerTouchSensor()){
		debpTouchSensor &touchSensor = *colObj1.GetOwnerTouchSensor();
		touchSensor.GetGhostObject()->GetGhostObject()->removeOverlappingObjectInternal(proxy0, dispatcher, proxy1);
		
// 		printf( "GPC-: ts=%p s=%p co=%p(%c)\n", touchSensor.GetTouchSensor(), &touchSensor,
// 			&colObj0, colObj0.IsOwnerCollider() ? 'C' : ( colObj0.IsOwnerHTSector() ? 'H' : 'T' ) );
		
		if(touchSensor.GetTouchSensor().GetTrackEnterLeave()){
			if(colObj0.IsOwnerCollider()){
				debpCollider * const collider = colObj0.GetOwnerCollider();
				
				if(touchSensor.GetTouchingColliders().Has(collider)){
// 					printf( "GPC: ts=%p s=%p => remove collider %p\n", touchSensor.GetTouchSensor(), &touchSensor, &colObj1 );
					touchSensor.GetTouchingColliders().Remove(collider);
					touchSensor.GetLeavingColliders().Add(collider);
				}
				
			//}else if( colObj1.IsOwnerHTSector() ){
			//	// TODO
			}
		}
		
	// for all other ghost objects use the default behavior
	}else if(btColObj1.getInternalType() == btCollisionObject::CO_GHOST_OBJECT){ 
		((btGhostObject*)&btColObj1)->removeOverlappingObjectInternal(proxy1, dispatcher, proxy0);
	}
	
	// return NULL... no idea why
	return NULL;
}

void debpGhostPairCallback::removeOverlappingPairsContainingProxy(btBroadphaseProxy *proxy0, btDispatcher *dispatcher){
	// bullet throws an assertion so this is most probably not supposed to be called yet
	DETHROW(deeInvalidAction);
}
