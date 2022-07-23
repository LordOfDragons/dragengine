/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RERIGCONSTRAINTDOF_H_
#define _RERIGCONSTRAINTDOF_H_

#include <dragengine/resources/collider/deColliderConstraint.h>

class reRigConstraint;



/**
 * Rig constraint degree of freedom working object.
 */
class reRigConstraintDof{
private:
	reRigConstraint &pRigConstraint;
	const deColliderConstraint::eDegreesOfFreedom pDofIndex;
	float pLowerLimit;
	float pUpperLimit;
	float pStaticFriction;
	float pKinematicFriction;
	float pSpringStiffness;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rig constraint dof. */
	reRigConstraintDof( reRigConstraint &constraint, deColliderConstraint::eDegreesOfFreedom dofIndex );
	
	/** Clean up rig constraint dof. */
	~reRigConstraintDof();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent rig constraint. */
	inline reRigConstraint &GetRigConstraint() const{ return pRigConstraint; }
	
	/** Dof index. */
	inline deColliderConstraint::eDegreesOfFreedom GetDofIndex() const{ return pDofIndex; }
	
	/** Equality threshold to use for lower and upper limit. */
	float GetThresholdLimits() const;
	
	/** Lower limit. */
	inline float GetLowerLimit() const{ return pLowerLimit; }
	
	/** Set lower limit. */
	void SetLowerLimit( float lowerLimit );
	
	/** Upper limit. */
	inline float GetUpperLimit() const{ return pUpperLimit; }
	
	/** Set upper limit. */
	void SetUpperLimit( float upperLimit );
	
	/** Static friction coefficient. */
	inline float GetStaticFriction() const{ return pStaticFriction; }
	
	/** Set static friction coefficient. */
	void SetStaticFriction( float friction );
	
	/** Kinematic friction coefficient. */
	inline float GetKinematicFriction() const{ return pKinematicFriction; }
	
	/** Set kinematic friction coefficient. */
	void SetKinematicFriction( float friction );
	
	/** Spring stiffness. */
	inline float GetSpringStiffness() const{ return pSpringStiffness; }
	
	/** Set spring stiffness. */
	void SetSpringStiffness( float stiffness );
	
	/** Update engine constraint dof. */
	void UpdateEngineDof( deColliderConstraintDof &engDof ) const;
	
	/** Set from engine constraint dof. */
	void SetFromEngineDof( const deColliderConstraintDof &engDof );
	
	/** Set parameters from another degree of freedom. */
	void SetParametersFrom( const reRigConstraintDof &dof );
	
	/** Notify all that the constraint dof changed. */
	void NotifyAllConstraintDofChanged();
	
	/** Notify engine about change to the constraint dof. */
	void NotifyEngineConstraintChanged();
	/*@}*/
};

#endif
