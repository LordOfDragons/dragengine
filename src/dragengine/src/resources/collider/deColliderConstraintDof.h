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

#ifndef _DECOLLIDERCONSTRAINTDOF_H_
#define _DECOLLIDERCONSTRAINTDOF_H_

#include "../../dragengine_export.h"


/**
 * \brief Collider Constraint Degree of Freedom.
 * 
 * Defines a degree of freedom constraint for a collider. A degree of freedom can be
 * either locked, limited or free depending on the values of the lower and upper
 * limits. If the upper limit is equal to the lower limit the degree of freedom is
 * locked. For performance and stability reason it is best to set as many degrees of
 * freedom to locked as possible. If the upper limit is larger than the lower limit
 * the degree of freedom is limited and is kept between those limits. If the upper
 * limit is lower than the lower limit the degree of freedom is free in which case
 * any movement is valid. A free degree of freedom is second best to locked in terms
 * of performance and stability. To avoid issues with floating point values physics
 * modules are required to use a safe zone of FLOAT_SAFE_EPSYLON between the two
 * limit values before calling a degree of freedom free instead of locked. Hence to
 * mark a degree of freedom as free best practise is to make the upper limit smaller
 * than the lower limit by a good amount. If you want to be safe use the setLocked,
 * setLimited and setFree functions. By default a degree of freedom is locked.
 * 
 * Joints in the real world exhibit friction while moving. This situation is modelled
 * using the static and kinematic friction. The static friction defines the force that
 * is needed to get a join moving in a certain direction. The physical formula is
 * staticFrictionForce = normalForce * staticFrictionCoefficient. Calculating normalForce
 * for a join is usually difficult so the the static friction force is used instead.
 * This is simple to retrieve from real world objects by measuring the force required
 * to get an object moving. While moving the kinematic friction is used. This is again
 * physically defined as kinematicFriction = normalForce * kinematicFrictionCoefficient.
 * Here too the normalForce is a problem. For this reason the force acting on the joint
 * is used instead. This allows to use the kinematicFrictionCoefficient as kinematic
 * friction. Hence the static friction is the force neede to get a joint moving and
 * the kinematic friction the percentage of the applying force absorbed by the inner
 * friction. Once the movement force drops below the static friction force the movement
 * grinds to a halt again.
 * 
 * In addition a degree of freedom can be turned into a spring. A degree of freedom
 * can be limited or free while being a spring or not. The spring is defined using
 * the constants found in Hooke's Law therefore F=-k*x . The stiffness spring constant
 * equals to the k constant defining the percentage of the distance stretched away
 * from the equilibrium position applied as force. Hence k is given as [N/m]. The
 * spring damping determines the amount of energy lost over time. A stiffness of 0
 * disables spring behavior.
 */
class DE_DLL_EXPORT deColliderConstraintDof{
private:
	float pLowerLimit;
	float pUpperLimit;
	float pStaticFriction;
	float pKinematicFriction;
	float pSpringStiffness;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new collider constraint degree of freedom. */
	deColliderConstraintDof();
	
	/** \brief Create new collider constraint degree of freedom as a copy of another constraint degree of freedom. */
	deColliderConstraintDof(const deColliderConstraintDof &dof);
	
	/** \brief Clean up collider constraint degree of freedom. */
	~deColliderConstraintDof();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Lower limit. */
	inline float GetLowerLimit() const{return pLowerLimit;}
	
	/** \brief Set lower limit. */
	void SetLowerLimit(float lowerLimit);
	
	/** \brief Upper limit. */
	inline float GetUpperLimit() const{return pUpperLimit;}
	
	/** \brief Set upper limit. */
	void SetUpperLimit(float upperLimit);
	
	/** \brief Set limits to locked. Both the upper and lower limit are set to the same value. */
	void SetLocked(float value);
	
	/** \brief Set limits to limited. Ensures the upper value is not less than the lower value. */
	void SetLimited(float lower, float upper);
	
	/** \brief Set limits to free. Sets lower to 1 and upper to 0. */
	void SetFree();
	
	/** \brief Static friction force. */
	inline float GetStaticFriction() const{return pStaticFriction;}
	
	/** \brief Set static friction force. */
	void SetStaticFriction(float friction);
	
	/** \brief Kinematic friction factor. */
	inline float GetKinematicFriction() const{return pKinematicFriction;}
	
	/** \brief Set kinematic friction factor. */
	void SetKinematicFriction(float friction);
	
	/** \brief Spring stiffness. */
	inline float GetSpringStiffness() const{return pSpringStiffness;}
	
	/** \brief Set spring stiffness. */
	void SetSpringStiffness(float stiffness);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set degree of freedom from another degree of freedom. */
	deColliderConstraintDof &operator=(const deColliderConstraintDof &dof);
	/*@}*/
};

#endif
