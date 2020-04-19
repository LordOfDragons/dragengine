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
#ifndef _DEBPSHAPETRANSFORM_H_
#define _DEBPSHAPETRANSFORM_H_

// includes
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionCylinder.h"
#include "../coldet/collision/debpDCollisionCapsule.h"

// predefinitions
class debpShape;
class debpDCollisionVolume;
class decShapeBox;
class decShapeSphere;
class decShapeCylinder;
class decShapeCapsule;



/**
 * @brief Shape Transform Helper.
 * Helper class for transforming shapes into collision volumes in an
 * efficient way. This class stores a copy of all possible collision
 * volumes to avoid memory allocation and deallocation. With each
 * transformation the matching collision volume is updated and
 * returned. This collision volume is valid until the next time a
 * transformation is done. The transformation is done either from the
 * base shape state or the current shape state.
 */
class debpShapeTransform{
private:
	debpDCollisionSphere pSphere;
	debpDCollisionBox pBox;
	debpDCollisionCylinder pCylinder;
	debpDCollisionCapsule pCapsule;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape transform helper. */
	debpShapeTransform();
	/** Cleans up the shape transform helper. */
	~debpShapeTransform();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Transforms a shape relative to the base state. */
	const debpDCollisionVolume &TransformBase( const debpShape *shape, const decDMatrix &transformation );
	/** Transforms a shape relative to the current state. */
	const debpDCollisionVolume &TransformCurrent( const debpShape *shape, const decDMatrix &transformation );
	
	/** Transforms an engine sphere shape. */
	const debpDCollisionSphere &TransformShapeSphere( const decShapeSphere &sphere, const decDMatrix &transformation );
	/** Transforms an engine box shape. */
	const debpDCollisionBox &TransformShapeBox( const decShapeBox &box, const decDMatrix &transformation );
	/** Transforms an engine cylinder shape. */
	const debpDCollisionCylinder &TransformShapeCylinder( const decShapeCylinder &cylinder, const decDMatrix &transformation );
	/** Transforms an engine capsule shape. */
	const debpDCollisionCapsule &TransformShapeCapsule( const decShapeCapsule &capsule, const decDMatrix &transformation );
	
	/** Transforms an engine collision sphere. */
	const debpDCollisionSphere &TransformCollisionSphere( const debpDCollisionSphere &sphere, const decDMatrix &transformation );
	/** Transforms an engine collision box. */
	const debpDCollisionBox &TransformCollisionBox( const debpDCollisionBox &box, const decDMatrix &transformation );
	/** Transforms an engine collision cylinder. */
	const debpDCollisionCylinder &TransformCollisionCylinder( const debpDCollisionCylinder &cylinder, const decDMatrix &transformation );
	/** Transforms an engine collision capsule. */
	const debpDCollisionCapsule &TransformCollisionCapsule( const debpDCollisionCapsule &capsule, const decDMatrix &transformation );
	/*@}*/
};

// end of include only once
#endif
