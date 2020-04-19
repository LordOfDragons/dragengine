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
#ifndef _DEBPSHAPECAPSULE_H_
#define _DEBPSHAPECAPSULE_H_

// includes
#include "debpShape.h"
#include "../coldet/collision/debpDCollisionCapsule.h"

// predefinitions
class decShapeCapsule;



/**
 * @brief Bullet Capsule Shape.
 *
 * Bullet counterpart of an engine capsule shape. The bullet shape contains
 * the original engine shape and the collision volume used for collision
 * testing. The collision volume can be updated using a transformation
 * matrix. A type is held to easily identify the shape without requiring
 * a visitor.
 */
class debpShapeCapsule : public debpShape{
private:
	decShapeCapsule *pSCapsule;
	debpDCollisionCapsule pCCapsule;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	debpShapeCapsule( decShapeCapsule *shape );
	/** Cleans up the shape. */
	virtual ~debpShapeCapsule();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the capsule shape. */
	inline decShapeCapsule *GetShapeCapsule() const{ return pSCapsule; }
	/** Retrieves the capsule collision volume. */
	inline const debpDCollisionCapsule &GetCollisionCapsule() const{ return pCCapsule; }
	
	/** Updates the collision volume using a transformation matrix. */
	virtual void UpdateWithMatrix( const decDMatrix &transformation );
	/** Prints out on the console some debugging informations about the shape. */
	virtual void PrintDebug( dePhysicsBullet &module );
	/*@}*/
};

// end of include only once
#endif
