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
	btCollisionConfiguration *collisionConfiguration) :
btCollisionDispatcher(collisionConfiguration),
pCollisionFiltering(collisionFiltering){
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
bool debpCollisionDispatcher::needsCollision(const btCollisionObject *body0, const btCollisionObject *body1){
	return btCollisionDispatcher::needsCollision(body0, body1);
		//&& pCollisionFiltering.CanBodyHitBody( *body0, *body1 );
}
*/

/*
bool debpCollisionDispatcher::needsResponse(const btCollisionObject *body0, const btCollisionObject *body1){
	return btCollisionDispatcher::needsResponse(body0, body1);
}
*/
