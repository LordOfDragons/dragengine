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

#include <stdio.h>
#include <string.h>

#include "dearBoneState.h"
#include "dearIKWorkState.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRigBone.h>



// Class dearIKWorkState
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearIKWorkState::dearIKWorkState() :
pBoneStateIndex(0),
pAxisTypeX(eatFree),
pAxisTypeY(eatFree),
pAxisTypeZ(eatFree),
pHasLimits(false),
pDampening(1.0f, 1.0f, 1.0f),
pHasDampening(false),
pWeight(1.0f),
pLength(0.0f){
}

dearIKWorkState::~dearIKWorkState(){
}



// Management	
	/** Transform global matrix and update inverse global matrix. */
	void TransformGlobalMatrixInverse(const decMatrix &matrix);

///////////////

void dearIKWorkState::SetBoneStateIndex(int index){
	pBoneStateIndex = index;
}

void dearIKWorkState::SetGlobalMatrix(const decMatrix &matrix){
	pGlobalMatrix = matrix;
}

void dearIKWorkState::SetInverseGlobalMatrix(const decMatrix &matrix){
	pInvGlobalMatrix = matrix;
}

void dearIKWorkState::TransformGlobalMatrix(const decMatrix &matrix, bool updateInverse){
	pGlobalMatrix = pGlobalMatrix.QuickMultiply(matrix);
	if(updateInverse){
		pInvGlobalMatrix = pGlobalMatrix.QuickInvert();
	}
}

void dearIKWorkState::SetAxisTypeX(eAxisTypes type){
	pAxisTypeX = type;
}

void dearIKWorkState::SetAxisTypeY(eAxisTypes type){
	pAxisTypeY = type;
}

void dearIKWorkState::SetAxisTypeZ(eAxisTypes type){
	pAxisTypeZ = type;
}

void dearIKWorkState::SetHasLimits(bool hasLimits){
	pHasLimits = hasLimits;
}

void dearIKWorkState::SetDampening(const decVector &dampening){
	pDampening = dampening.Clamped(decVector(0.0f, 0.0f, 0.0f), decVector(1.0f, 1.0f, 1.0f));
	pHasDampening = !pDampening.IsEqualTo(decVector(1.0f, 1.0f, 1.0f), 0.01f);
}

void dearIKWorkState::UpdateLimits(const deRigBone &bone){
	pLimitLower = bone.GetIKLimitsLower();
	pLimitUpper = bone.GetIKLimitsUpper();
	
	if(pAxisTypeX != eatLimited){
		pLimitUpper.x = pLimitLower.x = 0.0f;
	}
	if(pAxisTypeY != eatLimited){
		pLimitUpper.y = pLimitLower.y = 0.0f;
	}
	if(pAxisTypeZ != eatLimited){
		pLimitUpper.z = pLimitLower.z = 0.0f;
	}
	
	pLimitZeroQuatInv.SetFromEuler((pLimitLower + pLimitUpper) * 0.5f);
	pLimitZeroQuat = pLimitZeroQuatInv.Conjugate();
	
	pLimitUpper = (pLimitUpper - pLimitLower) * 0.5f;
	pLimitLower = -pLimitUpper;
}

void dearIKWorkState::SetLockedRotation(const decVector &rotation){
	pLockedRotation = rotation;
}

void dearIKWorkState::SetWeight(float weight){
	pWeight = weight;
}

void dearIKWorkState::SetLength(float length){
	pLength = length;
}

void dearIKWorkState::SetEndPosition(const decVector &position){
	pEndPosition = position;
	pLength = position.Length();
}

decVector dearIKWorkState::GetGlobalEnd() const{
	return pGlobalMatrix * pEndPosition;
}

void dearIKWorkState::SetRigLocalRotation(const decQuaternion &rotation){
	pRigLocalRot = rotation;
	pInvRigLocalRot = rotation.Conjugate();
}

void dearIKWorkState::SetLastGlobalOrientation(const decQuaternion &orientation){
	pLastGlobalOrientation = orientation;
	pInvLastGlobalOrientation = orientation.Conjugate();
}
