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
#ifndef _DEBPSHAPECYLINDER_H_
#define _DEBPSHAPECYLINDER_H_

// includes
#include "debpShape.h"
#include "../coldet/collision/debpDCollisionCylinder.h"

// predefinitions
class decShapeCylinder;



/**
 * @brief Bullet Cylinder Shape.
 *
 * Bullet counterpart of an engine cylinder shape. The bullet shape contains
 * the original engine shape and the collision volume used for collision
 * testing. The collision volume can be updated using a transformation
 * matrix. A type is held to easily identify the shape without requiring
 * a visitor.
 */
class debpShapeCylinder : public debpShape{
private:
	decShapeCylinder *pSCylinder;
	debpDCollisionCylinder pCCylinder;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	debpShapeCylinder( decShapeCylinder *shape );
	/** Cleans up the shape. */
	virtual ~debpShapeCylinder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the cylinder shape. */
	inline decShapeCylinder *GetShapeCylinder() const{ return pSCylinder; }
	/** Retrieves the cylinder collision volume. */
	inline const debpDCollisionCylinder &GetCollisionCylinder() const{ return pCCylinder; }
	
	/** Updates the collision volume using a transformation matrix. */
	virtual void UpdateWithMatrix( const decDMatrix &transformation, const decDVector &scale );
	/** Prints out on the console some debugging information about the shape. */
	virtual void PrintDebug( dePhysicsBullet &module );
	/*@}*/
};

// end of include only once
#endif
