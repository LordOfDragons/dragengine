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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "deColliderConstraint.h"
#include "deCollider.h"
#include "../../common/exceptions.h"



// Class deColliderConstraint
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderConstraint::deColliderConstraint() :
pDampingLinear(1.0f),
pDampingAngular(1.0f),
pDampingSpring(1.0f),
pIsRope(false),
pBreakingThreshold(0.0f),
pBone(-1)
{
	LockAll();
}

deColliderConstraint::deColliderConstraint(const deColliderConstraint &constraint) :
pTargetCollider(constraint.pTargetCollider),
pTargetBone(constraint.pTargetBone),
pPosition1(constraint.pPosition1),
pOrientation1(constraint.pOrientation1),
pPosition2(constraint.pPosition2),
pOrientation2(constraint.pOrientation2),
pDampingLinear(constraint.pDampingLinear),
pDampingAngular(constraint.pDampingAngular),
pDampingSpring(constraint.pDampingSpring),
pIsRope(constraint.pIsRope),
pBreakingThreshold(constraint.pBreakingThreshold),
pBone(constraint.pBone)
{
	int i;
	for(i=0; i<6; i++){
		pDof[i] = constraint.pDof[i];
	}
}

deColliderConstraint::~deColliderConstraint(){
}



// Management
///////////////

void deColliderConstraint::SetTargetCollider(deCollider *collider){
	pTargetCollider = collider;
}

void deColliderConstraint::SetTargetBone(const char *bone){
	pTargetBone = bone;
}



void deColliderConstraint::SetPosition1(const decVector &position){
	pPosition1 = position;
}

void deColliderConstraint::SetOrientation1(const decQuaternion &orientation){
	pOrientation1 = orientation;
}

void deColliderConstraint::SetPosition2(const decVector &position){
	pPosition2 = position;
}

void deColliderConstraint::SetOrientation2(const decQuaternion &orientation){
	pOrientation2 = orientation;
}



deColliderConstraintDof &deColliderConstraint::GetDof(eDegreesOfFreedom dof){
	if(dof < edofLinearX || dof > edofAngularZ){
		DETHROW(deeInvalidParam);
	}
	
	return pDof[dof];
}

const deColliderConstraintDof &deColliderConstraint::GetDof(eDegreesOfFreedom dof) const{
	if(dof < edofLinearX || dof > edofAngularZ){
		DETHROW(deeInvalidParam);
	}
	
	return pDof[dof];
}



void deColliderConstraint::SetLinearDamping(float damping){
	pDampingLinear = damping;
}

void deColliderConstraint::SetAngularDamping(float damping){
	pDampingAngular = damping;
}

void deColliderConstraint::SetSpringDamping(float damping){
	pDampingSpring = damping;
}



void deColliderConstraint::LockAll(){
	pDof[edofLinearX].SetLocked(0.0f);
	pDof[edofLinearY].SetLocked(0.0f);
	pDof[edofLinearZ].SetLocked(0.0f);
	
	pDof[edofAngularX].SetLocked(0.0f);
	pDof[edofAngularY].SetLocked(0.0f);
	pDof[edofAngularZ].SetLocked(0.0f);
}

void deColliderConstraint::SetToBallJoint(){
	pDof[edofLinearX].SetLocked(0.0f);
	pDof[edofLinearY].SetLocked(0.0f);
	pDof[edofLinearZ].SetLocked(0.0f);
	
	pDof[edofAngularX].SetFree();
	pDof[edofAngularY].SetFree();
	pDof[edofAngularZ].SetFree();
}

void deColliderConstraint::SetToHingeJoint(eDegreesOfFreedom dof, float lowerLimit, float upperLimit){
	if(dof < edofAngularX || dof > edofAngularZ){
		DETHROW(deeInvalidParam);
	}
	
	LockAll();
	
	pDof[dof].SetLimited(lowerLimit, upperLimit);
}

void deColliderConstraint::SetToPistonJoint(eDegreesOfFreedom dof, float lowerLimit, float upperLimit){
	if(dof < edofLinearX || dof > edofLinearZ){
		DETHROW(deeInvalidParam);
	}
	
	LockAll();
	
	pDof[dof].SetLimited(lowerLimit, upperLimit);
}

void deColliderConstraint::SetSpringLinear1D(eDegreesOfFreedom dof, float stiffness, float damping, float bendStiffness){
	if(dof < edofLinearX || dof > edofLinearZ){
		DETHROW(deeInvalidParam);
	}
	
	pDof[edofLinearX].SetFree();
	pDof[edofLinearY].SetFree();
	pDof[edofLinearZ].SetFree();
	
	pDof[edofAngularX].SetLocked(0.0f);
	pDof[edofAngularY].SetLocked(0.0f);
	pDof[edofAngularZ].SetLocked(0.0f);
	
	pDof[edofLinearX].SetSpringStiffness(bendStiffness);
	pDof[edofLinearY].SetSpringStiffness(bendStiffness);
	pDof[edofLinearZ].SetSpringStiffness(bendStiffness);
	
	pDof[dof].SetSpringStiffness(stiffness);
}



void deColliderConstraint::SetIsRope(bool isRope){
	pIsRope = isRope;
}

void deColliderConstraint::SetBreakingThreshold(float impulseThreshold){
	if(impulseThreshold < 0.0f){
		impulseThreshold = 0.0f;
	}
	pBreakingThreshold = impulseThreshold;
}

void deColliderConstraint::SetBone(int bone){
	if(bone < -1){
		DETHROW(deeInvalidParam);
	}
	
	pBone = bone;
}



// Operators
//////////////

deColliderConstraint &deColliderConstraint::operator=(const deColliderConstraint &constraint){
	pTargetCollider = constraint.pTargetCollider;
	pTargetBone = constraint.pTargetBone;
	
	pPosition1 = constraint.pPosition1;
	pOrientation1 = constraint.pOrientation1;
	pPosition2 = constraint.pPosition2;
	pOrientation2 = constraint.pOrientation2;
	
	pDof[edofLinearX] = constraint.GetDofLinearX();
	pDof[edofLinearY] = constraint.GetDofLinearY();
	pDof[edofLinearZ] = constraint.GetDofLinearZ();
	pDof[edofAngularX] = constraint.GetDofAngularX();
	pDof[edofAngularY] = constraint.GetDofAngularY();
	pDof[edofAngularZ] = constraint.GetDofAngularZ();
	
	pDampingLinear = constraint.pDampingLinear;
	pDampingAngular = constraint.pDampingAngular;
	pDampingSpring = constraint.pDampingSpring;
	
	pIsRope = constraint.pIsRope;
	
	pBreakingThreshold = constraint.pBreakingThreshold;
	
	pBone = constraint.pBone;
	
	return *this;
}
