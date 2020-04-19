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
