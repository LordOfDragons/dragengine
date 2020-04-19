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

#ifndef _DEBPBPCONSTRAINT6DOFSPRING_H_
#define _DEBPBPCONSTRAINT6DOFSPRING_H_

#include "debpBPConstraintBase.h"

#include <BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h>

class debpColliderConstraint;



/**
 * \brief Bullet physics 6-Dof spring constraint adapter.
 */
class debpBPConstraint6DofSpring : public btGeneric6DofSpringConstraint, debpBPConstraintBase{
private:
	debpColliderConstraint &pConstraint;
	int pIndexMotorX;
	int pIndexMotorY;
	int pIndexMotorZ;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constraint. */
	debpBPConstraint6DofSpring( debpColliderConstraint &constraint, btRigidBody &rbA,
		btRigidBody &rbB, const btTransform &frameInA, const btTransform &frameInB );
	
	/** \brief Create constraint. */
	debpBPConstraint6DofSpring( debpColliderConstraint &constraint, btRigidBody &rbA,
		const btTransform &frameInA, const btTransform &frameInB );
	
	/** \brief Clean up constraint. */
	virtual ~debpBPConstraint6DofSpring();
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set motor indices. */
	void SetMotorIndices( int x, int y, int z );
	
	/**
	 * \brief Bullet internal method used by the constraint solver.
	 * \details Modified to support Drag[en]gine features.
	 */
	virtual void getInfo2( btConstraintInfo2 *info );
	/*@}*/
};

#endif
