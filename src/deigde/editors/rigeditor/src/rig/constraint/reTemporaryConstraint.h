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

// include only once
#ifndef _RETEMPORARYCONSTRAINT_H_
#define _RETEMPORARYCONSTRAINT_H_

// includes
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/collider/deColliderComponent.h>

// predefinitions
class reRig;
class reRigBone;
class deColliderConstraint;
class deEngine;



/**
 * @brief Temporary Constraint.
 *
 * Temporary Constraint.
 */
class reTemporaryConstraint{
private:
	deColliderComponent::Ref pEngSimCollider;
	deColliderConstraint *pEngConstraint;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pLinearLowerLimits;
	decVector pLinearUpperLimits;
	decVector pAngularLowerLimits;
	decVector pAngularUpperLimits;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new temporary constraint. */
	reTemporaryConstraint(reRig *rig, reRigBone *bone, const decDVector &position, const decQuaternion &orientation);
	/** Cleans up the temporary constraint. */
	~reTemporaryConstraint();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the engine constraint. */
	inline deColliderConstraint *GetEngineConstraint() const{ return pEngConstraint; }
	
	/** Retrieves the constraint position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the constraint position. */
	void SetPosition(const decDVector &position);
	/** Retrieves the constraint orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** Sets the constraint orientation. */
	void SetOrientation(const decQuaternion &orientation);
	/** Retrieves the lower linear limits. */
	inline const decVector &GetLinearLowerLimits() const{ return pLinearLowerLimits; }
	/** Sets the lower linear limits. */
	void SetLinearLowerLimits(const decVector &lowerLimits);
	/** Retrieves the upper linear limits. */
	inline const decVector &GetLinearUpperLimits() const{ return pLinearUpperLimits; }
	/** Sets the upper linear limits. */
	void SetLinearUpperLimits(const decVector &upperLimits);
	/** Retrieves the lower angular limits. */
	inline const decVector &GetAngularLowerLimits() const{ return pAngularLowerLimits; }
	/** Sets the lower angular limits. */
	void SetAngularLowerLimits(const decVector &lowerLimits);
	/** Retrieves the upper angular limits. */
	inline const decVector &GetAngularUpperLimits() const{ return pAngularUpperLimits; }
	/** Sets the upper angular limits. */
	void SetAngularUpperLimits(const decVector &upperLimits);
	/*@}*/
	
private:
	void pNotifyConstraintChanged();
};

// end of include only once
#endif
