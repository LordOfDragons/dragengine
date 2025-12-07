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

#ifndef _DECOLLIDERCONSTRAINT_H_
#define _DECOLLIDERCONSTRAINT_H_

#include "deCollider.h"
#include "deColliderConstraintDof.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"


/**
 * \brief Collider Constraint.
 *
 * Defines a constraint for a collider. Constraints can be used for
 * colliders attached or colliders itself. If used on an attachement
 * the attached collider is constraint to the parent collider. If
 * used on a collider it is constraint to the static world. Constraints
 * are defined in a generic way as a 6-Dof ( degreees of freedom )
 * type constraint. Two coordinate systems are defined for the two
 * constraint partners. Each degree of freedom can be either locked,
 * limited or free. If limited the upper and lower limit can be set.
 * There are three linear and three angular degrees of freedom. The
 * linear degrees of freedom allow the constraint partner to move
 * translational relative to a given major axis whereas the angular
 * degrees of freedom allow rotation about a major axis.
 * 
 * A degree of freedom is considered locked if the upper and lower
 * limit are equal. It is considered free if the upper limit is
 * smaller than the lower limit. In all other cases the degree of
 * freedom is limited.
 * 
 * By default all degrees of freedom are locked.
 * 
 * In addition each degree of freedom can be turned into a spring
 * type degree of freedom. A degree of freedom can be limited or
 * free while being a spring or not. The spring is defined using
 * the constants found in Hooke's Law therefore F=-k*x . The stiffness
 * sprint constant equals to the k constant defining the percentage
 * of the distance stretched away from the equilibrium position
 * applied as force. Hence k is given as [N/m]. The spring damping
 * determines the amount of energy lost over time. A stiffness of
 * 0 disables spring behavior for a given degree of freedom.
 * 
 * A constraint can also be defined as a rope. If the constraint
 * is a rope no collision shape is required on the affected colliders
 * as the constraint itself becomes a rope shape.
 * 
 * A constraint can be set to break if an impulse larger than a threshold value affects
 * the constraint object along a degree of freedom. The physics module is responsible
 * to translate these settings into a useful simulation configuration and can vary.
 * By default a degree of freedom does not break.
 */
class DE_DLL_EXPORT deColliderConstraint{
public:
	/** \brief Degrees of freedom. */
	enum eDegreesOfFreedom{
		/** \brief Linear along the X-Axis. */
		edofLinearX,
		
		/** \brief Linear along the Y-Axis. */
		edofLinearY,
		
		/** \brief Linear along the Z-Axis. */
		edofLinearZ,
		
		/** \brief Angular around the X-Axis. */
		edofAngularX,
		
		/** \brief Angular around the Y-Axis. */
		edofAngularY,
		
		/** \brief Angular around the Z-Axis. */
		edofAngularZ
	};
	
	
	
private:
	deCollider::Ref pTargetCollider;
	decString pTargetBone;
	
	decVector pPosition1;
	decQuaternion pOrientation1;
	decVector pPosition2;
	decQuaternion pOrientation2;
	
	deColliderConstraintDof pDof[6];
	
	float pDampingLinear;
	float pDampingAngular;
	float pDampingSpring;
	
	bool pIsRope;
	
	float pBreakingThreshold;
	
	int pBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new collider constraint. */
	deColliderConstraint();
	
	/** \brief Create new collider constraint as a copy of another constraint. */
	deColliderConstraint(const deColliderConstraint &constraint);
	
	/** \brief Clean up collider constraint. */
	virtual ~deColliderConstraint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Target collider or NULL to constraint to the world. */
	inline deCollider *GetTargetCollider() const{ return pTargetCollider; }
	
	/** \brief Set target collider or NULL to constraint to the world. */
	void SetTargetCollider(deCollider *collider);
	
	/** \brief Name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	
	/** \brief Set name of the target bone. */
	void SetTargetBone(const char *bone);
	
	/** \brief Position of the reference coordinate system. */
	inline const decVector &GetPosition1() const{ return pPosition1; }
	
	/** \brief Set position of the reference coordinate system. */
	void SetPosition1(const decVector &position);
	
	/** \brief Orientation of the reference coordinate system. */
	inline const decQuaternion &GetOrientation1() const{ return pOrientation1; }
	
	/** \brief Set orientation of the reference coordinate system. */
	void SetOrientation1(const decQuaternion &orientation);
	
	/** \brief Position of the reference coordinate system. */
	inline const decVector &GetPosition2() const{ return pPosition2; }
	
	/** \brief Set position of the reference coordinate system. */
	void SetPosition2(const decVector &position);
	
	/** \brief Orientation of the reference coordinate system. */
	inline const decQuaternion &GetOrientation2() const{ return pOrientation2; }
	
	/** \brief Set orientation of the reference coordinate system. */
	void SetOrientation2(const decQuaternion &orientation);
	
	/** \brief Retrieves a degree of freedom. */
	deColliderConstraintDof &GetDof(eDegreesOfFreedom dof);
	const deColliderConstraintDof &GetDof(eDegreesOfFreedom dof) const;
	
	/** \brief X-axis linear degree of freedom. */
	inline deColliderConstraintDof &GetDofLinearX(){ return pDof[edofLinearX]; }
	inline const deColliderConstraintDof &GetDofLinearX() const{ return pDof[edofLinearX]; }
	
	/** \brief Y-axis linear degree of freedom. */
	inline deColliderConstraintDof &GetDofLinearY(){ return pDof[edofLinearY]; }
	inline const deColliderConstraintDof &GetDofLinearY() const{ return pDof[edofLinearY]; }
	
	/** \brief Z-axis linear degree of freedom. */
	inline deColliderConstraintDof &GetDofLinearZ(){ return pDof[edofLinearZ]; }
	inline const deColliderConstraintDof &GetDofLinearZ() const{ return pDof[edofLinearZ]; }
	
	/** \brief X-axis angular degree of freedom. */
	inline deColliderConstraintDof &GetDofAngularX(){ return pDof[edofAngularX]; }
	inline const deColliderConstraintDof &GetDofAngularX() const{ return pDof[edofAngularX]; }
	
	/** \brief Y-axis angular degree of freedom. */
	inline deColliderConstraintDof &GetDofAngularY(){ return pDof[edofAngularY]; }
	inline const deColliderConstraintDof &GetDofAngularY() const{ return pDof[edofAngularY]; }
	
	/** \brief Z-axis angular degree of freedom. */
	inline deColliderConstraintDof &GetDofAngularZ(){ return pDof[edofAngularZ]; }
	inline const deColliderConstraintDof &GetDofAngularZ() const{ return pDof[edofAngularZ]; }
	
	/** \brief Linear damping. */
	inline float GetLinearDamping() const{ return pDampingLinear; }
	
	/** \brief Set linear damping. */
	void SetLinearDamping(float damping);
	
	/** \brief Angular damping. */
	inline float GetAngularDamping() const{ return pDampingAngular; }
	
	/** \brief Set angular damping. */
	void SetAngularDamping(float damping);
	
	/** \brief Spring damping. */
	inline float GetSpringDamping() const{ return pDampingSpring; }
	
	/** \brief Set spring damping. */
	void SetSpringDamping(float damping);
	
	/** \brief Locks all degrees of freedom. */
	void LockAll();
	
	/** \brief Locks all linear degrees of freedom and sets all angular degrees of freedom free. */
	void SetToBallJoint();
	
	/**
	 * \brief Set upper and lower limit for angular degree of freedom and locks all others (hinge joint).
	 */
	void SetToHingeJoint(eDegreesOfFreedom degreeOfFreedom, float lowerLimit, float upperLimit);
	
	/** \brief Set upper and lower limit for linear degree of freem and locks all others. */
	void SetToPistonJoint(eDegreesOfFreedom degreeOfFreedom, float lowerLimit, float upperLimit);
	
	/**
	 * \brief Set a one-dimensional linear spring with bending properties.
	 * 
	 * The given linear degree of freedom is set free with the given spring stiffness
	 * and damping. All other linear degrees of freedom are set free to with the given
	 * bending stiffness and damping. All angular degrees of freedom are locked.
	 */
	void SetSpringLinear1D(eDegreesOfFreedom degreeOfFreedom, float stiffness,
		float damping, float bendStiffness);
	
	/** \brief Determines if this constraint is a rope. */
	inline bool GetIsRope() const{ return pIsRope; }
	
	/** \brief Sets if this constraint is a rope. */
	void SetIsRope(bool isRope);
	
	/** \brief Breaking impulse threshold or 0 if disabled. */
	inline float GetBreakingThreshold() const{ return pBreakingThreshold; }
	
	/** \brief Set breaking impulse threshold or 0 to disable. */
	void SetBreakingThreshold(float impulseThreshold);
	
	/** \brief Bone index or -1 if constraint the entire collider. */
	inline int GetBone() const{ return pBone; }
	
	/** \brief Set bone index or -1 if constraint the entire collider. */
	void SetBone(int bone);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set constraint with parameters from another constraint. */
	deColliderConstraint &operator=(const deColliderConstraint &constraint);
	/*@}*/
};

#endif
