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
	debpClosestRayResultCallback( const btVector3 &rayFromWorld,
	const btVector3 &rayToWorld, const decCollisionFilter *collisionFilter );
	/*@}*/
	
	/** \name Overwriting */
	/*@{*/
	/** \brief Determine if a collision between two collision objects is possible. */
	virtual bool needsCollision( btBroadphaseProxy *proxy0 ) const;
	/*@}*/
};

#endif
