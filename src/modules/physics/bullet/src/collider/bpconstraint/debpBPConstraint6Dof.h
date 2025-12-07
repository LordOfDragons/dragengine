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

#ifndef _DEBPBPCONSTRAINT6DOF_H_
#define _DEBPBPCONSTRAINT6DOF_H_

#include "debpBPConstraintBase.h"

#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>

class debpColliderConstraint;



/**
 * Bullet physics 6-Dof constraint adapter.
 */
class debpBPConstraint6Dof : public btGeneric6DofConstraint, debpBPConstraintBase{
private:
	debpColliderConstraint &pConstraint;
	int pIndexMotorX;
	int pIndexMotorY;
	int pIndexMotorZ;
	
	btScalar pOrgLimitLinearLower[3];
	btScalar pOrgLimitLinearUpper[3];
	btScalar pOrgLimitAngularLower[3];
	btScalar pOrgLimitAngularUpper[3];
	
	bool pHasJointFrictionLinear[3];
	bool pHasJointFrictionAngular[3];
	
	btScalar pDamping;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create constraint. */
	debpBPConstraint6Dof(debpColliderConstraint &constraint, btRigidBody &rbA,
		btRigidBody &rbB, const btTransform &frameInA, const btTransform &frameInB);
	
	/** Create constraint. */
	debpBPConstraint6Dof(debpColliderConstraint &constraint, btRigidBody &rbA,
		const btTransform &frameInA, const btTransform &frameInB);
	
	/** Clean up constraint. */
	~debpBPConstraint6Dof() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set motor indices. */
	void SetMotorIndices(int x, int y, int z);
	
	/** Save limits for special use. */
	void SaveLimits();
	
	/** Set if linear joint friction is present. */
	void SetHasJointFrictionLinear(int index, bool enabled);
	
	/** Set if angular joint friction is present. */
	void SetHasJointFrictionAngular(int index, bool enabled);
	
	/** Damping. */
	inline btScalar GetDamping() const{ return pDamping; }
	
	/** Set damping. */
	void SetDamping(btScalar damping);
	
	/**
	 * Prepare constraint for next simulation step.
	 * \details From debpBPConstraintBase.
	 */
	void PrepareForStep() override;
	
	
	
	/**
	 * Determine number of rows required for constraint friction.
	 * 
	 * This call is used by subclasses of constraint solvers to add constraint friction. This composes
	 * of static and kinematic joint friction.
	 */
	int GetConstraintFrictionCount() override;
	
	/**
	 * Get constraint friction forces.
	 * 
	 * This call is used by subclasses of constraint solvers to add constraint friction. This composes
	 * of static and kinematic joint friction.
	 */
	virtual void GetConstraintFriction(btTypedConstraint::btConstraintInfo2 &info);
	
	
	
	/**
	 * Bullet internal method used by the constraint solver.
	 * \details Modified to support Drag[en]gine features.
	 */
	virtual void getInfo1(btConstraintInfo1 *info);
	
	/**
	 * Bullet internal method used by the constraint solver.
	 * \details Modified to support Drag[en]gine features.
	 */
	virtual void getInfo2(btConstraintInfo2 *info);
	/*@}*/
	
	
	
private:
	btScalar pConstraintInvMass() const;
	btScalar pFrictionFactor(btScalar timeStep) const;
	btScalar pGetMassA() const;
	btScalar pGetMassB() const;
};

#endif
