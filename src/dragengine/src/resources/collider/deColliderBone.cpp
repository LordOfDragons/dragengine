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

// includes
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deColliderBone.h"
#include "deCollider.h"
#include "../../common/exceptions.h"



// class deColliderBone
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderBone::deColliderBone(deCollider *collider, int index) :
pCollider(collider),
pIndex(index),
pMass(1.0f),
pDynamic(false)
{
	if(! collider || index < 0){
		DETHROW(deeInvalidParam);
	}
}

deColliderBone::~deColliderBone(){
}



// Management
///////////////

void deColliderBone::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	pCollider->pNotifyBonePositionChanged(pIndex);
}

void deColliderBone::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	pCollider->pNotifyBoneOrientationChanged(pIndex);
}

void deColliderBone::SetLinearVelocity(const decVector &linearVelocity){
	if(linearVelocity.IsEqualTo(pLinearVelocity)){
		return;
	}
	
	pLinearVelocity = linearVelocity;
	pCollider->pNotifyBoneLinearVelocityChanged(pIndex);
}

void deColliderBone::SetAngularVelocity(const decVector &angularVelocity){
	if(angularVelocity.IsEqualTo(pAngularVelocity)){
		return;
	}
	
	pAngularVelocity = angularVelocity;
	pCollider->pNotifyBoneAngularVelocityChanged(pIndex);
}

void deColliderBone::UpdateMatrix(){
	pMatrix.SetWorld(pPosition, pOrientation);
	pInvMatrix = pMatrix.QuickInvert();
}

void deColliderBone::SetMass(float mass){
	if(mass < 0.0f){
		mass = 0.0f;
	}
	
	if(fabs(mass - pMass) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMass = mass;
	pCollider->pNotifyBonePropertiesChanged(pIndex);
}

void deColliderBone::SetDynamic(bool dynamic){
	if(pDynamic == dynamic){
		return;
	}
	
	pDynamic = dynamic;
	pCollider->pNotifyBoneDynamicChanged(pIndex);
}
