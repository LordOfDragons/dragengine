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

#ifndef _DEBPBPCONSTRAINT6DOF_H_
#define _DEBPBPCONSTRAINT6DOF_H_

#include "debpBPConstraintBase.h"

#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>

class debpColliderConstraint;



/**
 * \brief Bullet physics 6-Dof constraint adapter.
 */
class debpBPConstraint6Dof : public btGeneric6DofConstraint, debpBPConstraintBase{
private:
	debpColliderConstraint &pConstraint;
	int pIndexMotorX;
	int pIndexMotorY;
	int pIndexMotorZ;
	
	btScalar pOrgLimitLinearLower[ 3 ];
	btScalar pOrgLimitLinearUpper[ 3 ];
	btScalar pOrgLimitAngularLower[ 3 ];
	btScalar pOrgLimitAngularUpper[ 3 ];
	
	bool pHasJointFrictionLinear[ 3 ];
	bool pHasJointFrictionAngular[ 3 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constraint. */
	debpBPConstraint6Dof( debpColliderConstraint &constraint, btRigidBody &rbA,
		btRigidBody &rbB, const btTransform &frameInA, const btTransform &frameInB );
	
	/** \brief Create constraint. */
	debpBPConstraint6Dof( debpColliderConstraint &constraint, btRigidBody &rbA,
		const btTransform &frameInA, const btTransform &frameInB );
	
	/** \brief Clean up constraint. */
	virtual ~debpBPConstraint6Dof();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set motor indices. */
	void SetMotorIndices( int x, int y, int z );
	
	/** \brief Save limits for special use. */
	void SaveLimits();
	
	/** \brief Set if linear joint friction is present. */
	void SetHasJointFrictionLinear( int index, bool enabled );
	
	/** \brief Set if angular joint friction is present. */
	void SetHasJointFrictionAngular( int index, bool enabled );
	
	/**
	 * \brief Prepare constraint for next simulation step.
	 * \details From debpBPConstraintBase.
	 */
	virtual void PrepareForStep();
	
	
	
	/**
	 * \brief Determine number of rows required for constraint friction.
	 * 
	 * This call is used by subclasses of constraint solvers to add constraint friction. This composes
	 * of static and kinematic joint friction.
	 */
	virtual int GetConstraintFrictionCount();
	
	/**
	 * \brief Get constraint friction forces.
	 * 
	 * This call is used by subclasses of constraint solvers to add constraint friction. This composes
	 * of static and kinematic joint friction.
	 */
	virtual void GetConstraintFriction( btTypedConstraint::btConstraintInfo2 &info );
	
	
	
	/**
	 * \brief Bullet internal method used by the constraint solver.
	 * \details Modified to support Drag[en]gine features.
	 */
	virtual void getInfo1( btConstraintInfo1 *info );
	
	/**
	 * \brief Bullet internal method used by the constraint solver.
	 * \details Modified to support Drag[en]gine features.
	 */
	virtual void getInfo2( btConstraintInfo2 *info );
	/*@}*/
	
	
	
private:
	btScalar pConstraintInvMass() const;
	btScalar pFrictionFactor( btScalar timeStep ) const;
	btScalar pGetMassA() const;
	btScalar pGetMassB() const;
};

#endif
