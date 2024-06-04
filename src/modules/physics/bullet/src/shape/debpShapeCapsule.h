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
	virtual void UpdateWithMatrix( const decDMatrix &transformation, const decDVector &scale );
	/** Prints out on the console some debugging information about the shape. */
	virtual void PrintDebug( dePhysicsBullet &module );
	/*@}*/
};

// end of include only once
#endif
