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

#ifndef _DEBPBPCONSTRAINTPOINT2POINT_H_
#define _DEBPBPCONSTRAINTPOINT2POINT_H_

#include "debpBPConstraintBase.h"

#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>

class debpColliderConstraint;


/**
 * \brief Bullet physics point to point constraint.
 */
class debpBPConstraintPoint2Point : public btPoint2PointConstraint, debpBPConstraintBase{
private:
	debpColliderConstraint &pConstraint;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constraint. */
	debpBPConstraintPoint2Point( debpColliderConstraint &constraint, btRigidBody &rbA,
		btRigidBody &rbB, const btVector3 &pivotInA, const btVector3 &pivotInB );
	
	/** \brief Create constraint. */
	debpBPConstraintPoint2Point( debpColliderConstraint &constraint, btRigidBody &rbA,
		const btVector3 &pivotInA, const btVector3 &pivotInB );
	
	/** \brief Clean up constraint. */
	virtual ~debpBPConstraintPoint2Point();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Bullet internal method used by the constraint solver.
	 * \details Modified to support Drag[en]gine features.
	 */
	virtual void getInfo2( btConstraintInfo2 *info );
	/*@}*/
};

#endif
