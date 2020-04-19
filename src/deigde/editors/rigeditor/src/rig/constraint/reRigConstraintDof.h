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
 * @brief Rig constraint degree of freedom working object.
 */
class reRigConstraintDof{
private:
	/** Parent rig constraint. */
	reRigConstraint *pRigConstraint;
	/** Dof index. */
	deColliderConstraint::eDegreesOfFreedom pDofIndex;
	
	/** Lower limit. */
	float pLowerLimit;
	/** Upper limit. */
	float pUpperLimit;
	/** Static friction. */
	float pStaticFriction;
	/** Kinetic friction. */
	float pKinematicFriction;
	/** Spring stiffness. */
	float pSpringStiffness;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig constraint dof. */
	reRigConstraintDof( reRigConstraint *constraint, deColliderConstraint::eDegreesOfFreedom dofIndex );
	/** Cleans up the rig constraint dof. */
	~reRigConstraintDof();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig constraint. */
	inline reRigConstraint *GetRigConstraint() const{ return pRigConstraint; }
	/** Retrieves the dof index. */
	inline int GetDofIndex() const{ return pDofIndex; }
	
	/** Retrieves the lower limit. */
	inline float GetLowerLimit() const{ return pLowerLimit; }
	/** Sets the lower limit. */
	void SetLowerLimit( float lowerLimit );
	/** Retrieves the upper limit. */
	inline float GetUpperLimit() const{ return pUpperLimit; }
	/** Sets the upper limit. */
	void SetUpperLimit( float upperLimit );
	
	/** Retrieves the static friction coefficient. */
	inline float GetStaticFriction() const{ return pStaticFriction; }
	/** Sets the static friction coefficient. */
	void SetStaticFriction( float friction );
	/** Retrieves the kinematic friction coefficient. */
	inline float GetKinematicFriction() const{ return pKinematicFriction; }
	/** Sets the kinematic friction coefficient. */
	void SetKinematicFriction( float friction );
	
	/** Retrieves the spring stiffness. */
	inline float GetSpringStiffness() const{ return pSpringStiffness; }
	/** Sets the spring stiffness. */
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
	
private:
	void pCleanUp();
};

#endif
