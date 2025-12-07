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

#ifndef _DEBPCLOSESTRAYRESULTCALLBACK_H_
#define _DEBPCLOSESTRAYRESULTCALLBACK_H_

#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

class decCollisionFilter;



/**
 * \brief Modification of the btCollisionWorld::ClosestRayResultCallback struct used for ray casts with bullet.
 */
class debpClosestRayResultCallback : public btCollisionWorld::ClosestRayResultCallback{
private:
	const decCollisionFilter *pCollisionFilter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new callback. */
	debpClosestRayResultCallback(const btVector3 &rayFromWorld,
	const btVector3 &rayToWorld, const decCollisionFilter *collisionFilter);
	/*@}*/
	
	/** \name Overwriting */
	/*@{*/
	/** \brief Determine if a collision between two collision objects is possible. */
	virtual bool needsCollision(btBroadphaseProxy *proxy0) const;
	/*@}*/
};

#endif
