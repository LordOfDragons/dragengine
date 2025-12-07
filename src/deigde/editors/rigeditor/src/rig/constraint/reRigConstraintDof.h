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
	reRigConstraintDof(reRigConstraint &constraint, deColliderConstraint::eDegreesOfFreedom dofIndex);
	
	/** Clean up rig constraint dof. */
	~reRigConstraintDof();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent rig constraint. */
	inline reRigConstraint &GetRigConstraint() const{return pRigConstraint;}
	
	/** Dof index. */
	inline deColliderConstraint::eDegreesOfFreedom GetDofIndex() const{return pDofIndex;}
	
	/** Equality threshold to use for lower and upper limit. */
	float GetThresholdLimits() const;
	
	/** Lower limit. */
	inline float GetLowerLimit() const{return pLowerLimit;}
	
	/** Set lower limit. */
	void SetLowerLimit(float lowerLimit);
	
	/** Upper limit. */
	inline float GetUpperLimit() const{return pUpperLimit;}
	
	/** Set upper limit. */
	void SetUpperLimit(float upperLimit);
	
	/** Static friction coefficient. */
	inline float GetStaticFriction() const{return pStaticFriction;}
	
	/** Set static friction coefficient. */
	void SetStaticFriction(float friction);
	
	/** Kinematic friction coefficient. */
	inline float GetKinematicFriction() const{return pKinematicFriction;}
	
	/** Set kinematic friction coefficient. */
	void SetKinematicFriction(float friction);
	
	/** Spring stiffness. */
	inline float GetSpringStiffness() const{return pSpringStiffness;}
	
	/** Set spring stiffness. */
	void SetSpringStiffness(float stiffness);
	
	/** Update engine constraint dof. */
	void UpdateEngineDof(deColliderConstraintDof &engDof) const;
	
	/** Set from engine constraint dof. */
	void SetFromEngineDof(const deColliderConstraintDof &engDof);
	
	/** Set parameters from another degree of freedom. */
	void SetParametersFrom(const reRigConstraintDof &dof);
	
	/** Notify all that the constraint dof changed. */
	void NotifyAllConstraintDofChanged();
	
	/** Notify engine about change to the constraint dof. */
	void NotifyEngineConstraintChanged();
	/*@}*/
};

#endif
