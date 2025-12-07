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
#include <stdlib.h>
#include <string.h>

#include "deRigBone.h"
#include "deRigConstraint.h"
#include "../../common/exceptions.h"



// Class deRigBone
////////////////////

// Constructor, destructor
////////////////////////////

deRigBone::deRigBone(const char *name) :
pName(name),
pParent(-1),
pMass(1.0f),
pDynamic(false),
pIKLimitsLower(TWO_PI, TWO_PI, TWO_PI),
pIKLimitsUpper(0.0f, 0.0f, 0.0f),
pIKResistance(0.0f, 0.0f, 0.0f),
pConstraints(NULL),
pConstraintCount(0),
pConstraintSize(0)
{
	if(!name[0]){
		DETHROW(deeInvalidParam);
	}
	
	pIKLocked[0] = false;
	pIKLocked[1] = false;
	pIKLocked[2] = false;
}

deRigBone::~deRigBone(){
	if(pConstraints){
		while(pConstraintCount > 0){
			delete pConstraints[pConstraintCount - 1];
			pConstraintCount--;
		}
		delete [] pConstraints;
	}
}



// Management
///////////////

void deRigBone::SetParent(int boneIndex){
	if(boneIndex < -1){
		DETHROW(deeInvalidParam);
	}
	
	pParent = boneIndex;
}

void deRigBone::SetPosition(const decVector &position){
	pPos = position;
}

void deRigBone::SetRotation(const decVector &rotation){
	pRot = rotation;
}

void deRigBone::SetCentralMassPoint(const decVector &cmp){
	pCMP = cmp;
}

void deRigBone::SetMatrices(const decMatrix &matrix){
	pMatrix = matrix;
	pInvMatrix = pMatrix.QuickInvert();
}

void deRigBone::SetDynamic(bool dynamic){
	pDynamic = dynamic;
}

void deRigBone::SetMass(float mass){
	pMass = decMath::max(mass, 0.0f);
}



// Inverse kinematics
///////////////////////

void deRigBone::SetIKLimits(const decVector &lower, const decVector &upper){
	pIKLimitsLower = lower;
	pIKLimitsUpper = upper;
}

void deRigBone::SetIKResistance(const decVector &resistance){
	pIKResistance = resistance.Clamped(decVector(), decVector(1.0f, 1.0f, 1.0f));
}

void deRigBone::SetIKLockedX(bool locked){
	pIKLocked[0] = locked;
}

void deRigBone::SetIKLockedY(bool locked){
	pIKLocked[1] = locked;
}

void deRigBone::SetIKLockedZ(bool locked){
	pIKLocked[2] = locked;
}



// Shapes
///////////

void deRigBone::SetShapes(const decShapeList &shapes){
	const int count = shapes.GetCount();
	int i;
	
	pShapes = shapes;
	
	pShapeProperties.RemoveAll();
	for(i=0; i<count; i++){
		pShapeProperties.Add("");
	}
}

void deRigBone::SetShapeProperties(const decStringList &properties){
	if(properties.GetCount() != pShapes.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pShapeProperties = properties;
}



// Constraints
////////////////

deRigConstraint &deRigBone::GetConstraintAt(int index) const{
	if(index < 0 || index >= pConstraintCount){
		DETHROW(deeInvalidParam);
	}
	
	return *pConstraints[index];
}

void deRigBone::AddConstraint(deRigConstraint *constraint){
	if(pHasConstraint(constraint)){
		DETHROW(deeInvalidParam);
	}
	
	if(pConstraintCount == pConstraintSize){
		const int newSize = pConstraintSize * 3 / 2 + 1;
		deRigConstraint ** const newArray = new deRigConstraint*[newSize];
		if(pConstraints){
			memcpy(newArray, pConstraints, sizeof(deRigConstraint*) * pConstraintSize);
			delete [] pConstraints;
		}
		pConstraints = newArray;
		pConstraintSize = newSize;
	}
	
	pConstraints[pConstraintCount] = constraint;
	pConstraintCount++;
}



// Private Functions
//////////////////////

bool deRigBone::pHasConstraint(deRigConstraint *constraint) const{
	if(!constraint){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(constraint == pConstraints[i]){
			return true;
		}
	}
	
	return false;
}
