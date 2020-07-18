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

// include only once
#ifndef _DEBPSHAPESPHERE_H_
#define _DEBPSHAPESPHERE_H_

// includes
#include "debpShape.h"
#include "../coldet/collision/debpDCollisionSphere.h"

// predefinitions
class decShapeSphere;



/**
 * @brief Bullet Sphere Shape.
 *
 * Bullet counterpart of an engine sphere shape. The bullet shape contains
 * the original engine shape and the collision volume used for collision
 * testing. The collision volume can be updated using a transformation
 * matrix. A type is held to easily identify the shape without requiring
 * a visitor.
 */
class debpShapeSphere : public debpShape{
private:
	decShapeSphere *pSSphere;
	debpDCollisionSphere pCSphere;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	debpShapeSphere( decShapeSphere *shape );
	/** Cleans up the shape. */
	virtual ~debpShapeSphere();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the sphere shape. */
	inline decShapeSphere *GetShapeSphere() const{ return pSSphere; }
	/** Retrieves the sphere collision volume. */
	inline const debpDCollisionSphere &GetCollisionSphere() const{ return pCSphere; }
	
	/** Updates the collision volume using a transformation matrix. */
	virtual void UpdateWithMatrix( const decDMatrix &transformation );
	/** Prints out on the console some debugging information about the shape. */
	virtual void PrintDebug( dePhysicsBullet &module );
	/*@}*/
};

// end of include only once
#endif
