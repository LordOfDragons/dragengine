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

#ifndef _DEBPDCOLLISIONVOLUMEVISITOR_H_
#define _DEBPDCOLLISIONVOLUMEVISITOR_H_

#include <dragengine/common/math/decMath.h>

class debpDCollisionSphere;
class debpDCollisionCylinder;
class debpDCollisionCapsule;
class debpDCollisionBox;
class debpDCollisionTriangle;
class debpDCollisionFrustum;



/**
 * @brief Visitor for collision volumes.
 *
 * This class is provided mainly for easily identifying collision volumes
 * and carrying out actions depending on their type. The collision volume
 * system is not mandatory for module programmers hence this class to
 * allow them to easily use their own system. The default implementation
 * of all functions is to do nothing at all.
 */
class debpDCollisionVolumeVisitor{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision volume visitor. */
	debpDCollisionVolumeVisitor();
	/** Cleans up the collision volume visitor. */
	virtual ~debpDCollisionVolumeVisitor();
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** Visits a collision sphere. */
	virtual void VisitSphere(debpDCollisionSphere *sphere);
	/** Visits a collision cylinder. */
	virtual void VisitCylinder(debpDCollisionCylinder *cylinder);
	/** Visits a collision capsule. */
	virtual void VisitCapsule(debpDCollisionCapsule *capsule);
	/** Visits a collision box. */
	virtual void VisitBox(debpDCollisionBox *box);
	/** Visits a collision triangle. */
	virtual void VisitTriangle(debpDCollisionTriangle *triangle);
	/** Visits a collision frustum. */
	virtual void VisitFrustum(debpDCollisionFrustum *frustum);
	/*@}*/
	
};


// end of include only once
#endif
