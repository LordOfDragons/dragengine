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

#include "debpOverlapFilterCallback.h"
#include "debpSharedCollisionFiltering.h"

#include <dragengine/common/exceptions.h>



// Class debpOverlapFilterCallback
////////////////////////////////////

// Constructor, Destructor
////////////////////////////

debpOverlapFilterCallback::debpOverlapFilterCallback( debpSharedCollisionFiltering &collisionFiltering ) :
pCollisionFiltering( collisionFiltering ){
}

debpOverlapFilterCallback::~debpOverlapFilterCallback(){
}



// Management
///////////////

bool debpOverlapFilterCallback::needBroadphaseCollision( btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1 ) const{
	// bullet broadphase test
	if( ( proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask ) == 0
	||  ( proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask ) == 0 ){
		return false;
	}
	
	// the broadphase collision callback is done early and affects the addition of collision
	// pairs. the collision dispatch does only take collision pairs into account. for this
	// reason the tests here only have the goal to prevent collision pairs to be generated
	// for objects that never can collide at all like two touch sensors. the layer mask and
	// group number is also used. these are a bit more tricky since they can theoretically
	// change. in general though they are not supposed to change for objects during their
	// lifetime. with this assumption a lot of collision pairs can be avoided speeding up
	// collision testing. should a layer mask or group number still change it is required
	// to dirty the object to force a broadphase update
	// 
	// see btHashedOverlappingPairCache::needsBroadphaseCollision() in btOverlappingPairCache.h
	return pCollisionFiltering.CanBodyHitBody(
		*( ( btCollisionObject*)proxy0->m_clientObject ),
		*( ( btCollisionObject* )proxy1->m_clientObject ) );
}
