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

#include "debpCollisionDispatcher.h"
#include "debpSharedCollisionFiltering.h"
#include "debpWorld.h"

#include <dragengine/common/exceptions.h>



// Class debpCollisionDispatcher
//////////////////////////////////

// Constructor, Destructor
////////////////////////////

debpCollisionDispatcher::debpCollisionDispatcher(
	debpSharedCollisionFiltering &collisionFiltering,
	btCollisionConfiguration *collisionConfiguration ) :
btCollisionDispatcher( collisionConfiguration ),
pCollisionFiltering( collisionFiltering ){
}

debpCollisionDispatcher::~debpCollisionDispatcher(){
}



// Overloads
//////////////

/*
// this is disabled because debpOverlapFilterCallback is already doing the same filtering.
// nobody else than btCollisionPairCallback::processOverlap in btCollisionDispatcher.cpp
// does use btCollisionDispatcher::needsCollision. this allows us to do the filtering only
// inside the broadphase and avoid doing it in the narrow phase.
// 
// WARNING
// this change has a dangerous side-effect. if collision filter or ignored collision objects
// are changed they will not have effect until the overlapping pair is removed. this can
// result in strange problems where objects keep on colliding although the changed collision
// parameters should prevent them from from colliding. colliders should best remove the rigid
// bodies from the dynamic world and re-add them to trigger a broadphase update.
bool debpCollisionDispatcher::needsCollision( const btCollisionObject *body0, const btCollisionObject *body1 ){
	return btCollisionDispatcher::needsCollision( body0, body1 );
		//&& pCollisionFiltering.CanBodyHitBody( *body0, *body1 );
}
*/

/*
bool debpCollisionDispatcher::needsResponse( const btCollisionObject *body0, const btCollisionObject *body1 ){
	return btCollisionDispatcher::needsResponse( body0, body1 );
}
*/
