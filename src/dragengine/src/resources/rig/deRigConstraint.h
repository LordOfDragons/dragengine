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

#ifndef _DERIGCONSTRAINT_H_
#define _DERIGCONSTRAINT_H_

#include "../collider/deColliderConstraint.h"


/**
 * \brief Rig Constraint.
 *
 * Defines a constraint for a rig bone. The workings of this class is
 * similar to the deColliderConstraint class with a few differences.
 * The reference coordinate frame is defined relative to the owning
 * bone. The parent coordinate frame is the same as the reference
 * coordinate frame but relative to the parent bone. In addition an
 * offset can be specified which displaces the actual bone coordinate
 * frame relative to the reference coordinate frame. This is useful
 * for spring constraints where the equilibrium position is not equal
 * to the origin of the reference coordinate frame. In contrary to the
 * deColliderConstraint the parent bone has to be a valid rig bone.
 * Another difference is that in this class limits and spring stiffness
 * can only be accessed using vectors instead of per degree of freedom
 * as in the deColliderConstraint class. This has been done since the
 * rig constraint is loaded from a file and does not require fine
 * grained accessors and mutators.
 * 
 * A constraint can be set to break if an impulse larger than a threshold value affects
 * the constraint object along a degree of freedom. The physics module is responsible
 * to translate these settings into a useful simulation configuration and can vary.
 * By default a degree of freedom does not break.
 */
class DE_DLL_EXPORT deRigConstraint{
private:
	decVector pRefPosition;
	decQuaternion pRefOrientation;
	decVector pBoneOffset;
	
	deColliderConstraintDof pDof[ 6 ];
	
	float pDampingLinear;
	float pDampingAngular;
	float pDampingSpring;
	
	bool pIsRope;
	
	float pBreakingThreshold;
	
	int pParentBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new rig constraint. */
	deRigConstraint();
	
	/** \brief Clean up rig constraint. */
	~deRigConstraint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position of the reference coordinate frame. */
	inline const decVector &GetReferencePosition() const{ return pRefPosition; }
	
	/** \brief Set position of the reference coordinate frame. */
	void SetReferencePosition( const decVector &position );
	
	/** \brief Orientation of the reference coordinate frame. */
	inline const decQuaternion &GetReferenceOrientation() const{ return pRefOrientation; }
	
	/** \brief Set orientation of the reference coordinate frame. */
	void SetReferenceOrientation( const decQuaternion &orientation );
	
	/**
	 * Retrieves the offset of the bone constraint position
	 * relative to the reference coordinate frame.
	 */
	inline const decVector &GetBoneOffset() const{ return pBoneOffset; }
	
	/**
	 * \brief Set the offset of the bone constraint position
	 * relative to the reference coordinate frame.
	 */
	void SetBoneOffset( const decVector &offset );
	
	/** \brief Retrieves a degree of freedom. */
	deColliderConstraintDof &GetDof( deColliderConstraint::eDegreesOfFreedom dof );
	const deColliderConstraintDof &GetDof( deColliderConstraint::eDegreesOfFreedom dof ) const;
	
	/** \brief X-axis linear degree of freedom. */
	inline deColliderConstraintDof &GetDofLinearX(){ return pDof[ deColliderConstraint::edofLinearX ]; }
	inline const deColliderConstraintDof &GetDofLinearX() const{ return pDof[ deColliderConstraint::edofLinearX ]; }
	
	/** \brief Y-axis linear degree of freedom. */
	inline deColliderConstraintDof &GetDofLinearY(){ return pDof[ deColliderConstraint::edofLinearY ]; }
	inline const deColliderConstraintDof &GetDofLinearY() const{ return pDof[ deColliderConstraint::edofLinearY ]; }
	
	/** \brief Z-axis linear degree of freedom. */
	inline deColliderConstraintDof &GetDofLinearZ(){ return pDof[ deColliderConstraint::edofLinearZ ]; }
	inline const deColliderConstraintDof &GetDofLinearZ() const{ return pDof[ deColliderConstraint::edofLinearZ ]; }
	
	/** \brief X-axis angular degree of freedom. */
	inline deColliderConstraintDof &GetDofAngularX(){ return pDof[ deColliderConstraint::edofAngularX ]; }
	inline const deColliderConstraintDof &GetDofAngularX() const{ return pDof[ deColliderConstraint::edofAngularX ]; }
	
	/** \brief Y-axis angular degree of freedom. */
	inline deColliderConstraintDof &GetDofAngularY(){ return pDof[ deColliderConstraint::edofAngularY ]; }
	inline const deColliderConstraintDof &GetDofAngularY() const{ return pDof[ deColliderConstraint::edofAngularY ]; }
	
	/** \brief Z-axis angular degree of freedom. */
	inline deColliderConstraintDof &GetDofAngularZ(){ return pDof[ deColliderConstraint::edofAngularZ ]; }
	inline const deColliderConstraintDof &GetDofAngularZ() const{ return pDof[ deColliderConstraint::edofAngularZ ]; }
	
	/** \brief Linear damping. */
	inline float GetLinearDamping() const{ return pDampingLinear; }
	
	/** \brief Set linear damping. */
	void SetLinearDamping( float damping );
	
	/** \brief Angular damping. */
	inline float GetAngularDamping() const{ return pDampingAngular; }
	
	/** \brief Set angular damping. */
	void SetAngularDamping( float damping );
	
	/** \brief Spring damping. */
	inline float GetSpringDamping() const{ return pDampingSpring; }
	
	/** \brief Set spring damping. */
	void SetSpringDamping( float damping );
	
	/** \brief Determines if this constraint is a rope. */
	inline bool GetIsRope() const{ return pIsRope; }
	
	/** \brief Sets if this constraint is a rope. */
	void SetIsRope( bool isRope );
	
	/** \brief Breaking impulse threshold or 0 if disabled. */
	inline float GetBreakingThreshold() const{ return pBreakingThreshold; }
	
	/** \brief Set breaking impulse threshold or 0 to disable. */
	void SetBreakingThreshold( float impulseThreshold );
	
	/** \brief Parent bone index. */
	inline int GetParentBone() const{ return pParentBone; }
	
	/** \brief Set parent bone index. */
	void SetParentBone( int bone );
	/*@}*/
};

#endif
