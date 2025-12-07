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
	const debpDCollisionVolume &TransformBase(const debpShape *shape, const decDMatrix &transformation);
	/** Transforms a shape relative to the current state. */
	const debpDCollisionVolume &TransformCurrent(const debpShape *shape, const decDMatrix &transformation);
	
	/** Transforms an engine sphere shape. */
	const debpDCollisionSphere &TransformShapeSphere(const decShapeSphere &sphere, const decDMatrix &transformation);
	/** Transforms an engine box shape. */
	const debpDCollisionBox &TransformShapeBox(const decShapeBox &box, const decDMatrix &transformation);
	/** Transforms an engine cylinder shape. */
	const debpDCollisionCylinder &TransformShapeCylinder(const decShapeCylinder &cylinder, const decDMatrix &transformation);
	/** Transforms an engine capsule shape. */
	const debpDCollisionCapsule &TransformShapeCapsule(const decShapeCapsule &capsule, const decDMatrix &transformation);
	
	/** Transforms an engine collision sphere. */
	const debpDCollisionSphere &TransformCollisionSphere(const debpDCollisionSphere &sphere, const decDMatrix &transformation);
	/** Transforms an engine collision box. */
	const debpDCollisionBox &TransformCollisionBox(const debpDCollisionBox &box, const decDMatrix &transformation);
	/** Transforms an engine collision cylinder. */
	const debpDCollisionCylinder &TransformCollisionCylinder(const debpDCollisionCylinder &cylinder, const decDMatrix &transformation);
	/** Transforms an engine collision capsule. */
	const debpDCollisionCapsule &TransformCollisionCapsule(const debpDCollisionCapsule &capsule, const decDMatrix &transformation);
	/*@}*/
};

// end of include only once
#endif
