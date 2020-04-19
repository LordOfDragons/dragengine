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
