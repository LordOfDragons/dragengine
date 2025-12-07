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
#include <string.h>
#include "aeCamera.h"
#include "aeAnimator.h"
#include "locomotion/aeAnimatorLocomotion.h"

#include "dragengine/deEngine.h"
#include "dragengine/common/exceptions.h"
#include "dragengine/resources/component/deComponent.h"
#include "dragengine/resources/component/deComponentBone.h"
#include "dragengine/resources/rig/deRig.h"



// Class aeCamera
///////////////////

// Constructor, destructor
////////////////////////////

aeCamera::aeCamera(aeAnimator *animator, deEngine *engine) : igdeCamera(engine){
	if(! animator) DETHROW(deeInvalidParam);
	
	pAnimator = animator;
	
	pFreeDistance = 0.0f;
	pDirty = false;
	pAttachToBone = false;
}

aeCamera::~aeCamera(){
}



// Management
///////////////

void aeCamera::SetBone(const char *bone){
	if(pBone == bone){
		return;
	}
	
	pBone = bone;
	pDirty = true;
	
	pAnimator->NotifyCameraChanged();
}

void aeCamera::SetFreePosition(const decDVector &freePosition){
	if(! freePosition.IsEqualTo(pFreePosition)){
		pFreePosition = freePosition;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetFreeOrientation(const decVector &freeOrientation){
	if(! freeOrientation.IsEqualTo(pFreeOrientation)){
		pFreeOrientation = freeOrientation;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetFreeDistance(float freeDistance){
	if(fabsf(freeDistance - pFreeDistance) > 1e-5f){
		pFreeDistance = freeDistance;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetRelativePosition(const decVector &relativePosition){
	if(! relativePosition.IsEqualTo(pRelPosition)){
		pRelPosition = relativePosition;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetRelativeOrientation(const decVector &relativeOrientation){
	if(! relativeOrientation.IsEqualTo(pRelOrientation)){
		pRelOrientation = relativeOrientation;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetAttachToBone(bool attachToBone){
	if(pAttachToBone != attachToBone){
		pAttachToBone = attachToBone;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::Update(){
	aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	
	if(pDirty || pAttachToBone || locomotion.GetEnabled()){
		if(pAttachToBone){
			decMatrix matrix(decMatrix::CreateRT(pRelOrientation * DEG2RAD, pRelPosition));
			deComponent *engComponent = pAnimator->GetEngineComponent();
			
			if(engComponent){
				decMatrix compMat;
				
				engComponent->PrepareBones();
				
				if(! pBone.IsEmpty() && engComponent->GetRig()){
					const int index = engComponent->GetRig()->IndexOfBoneNamed(pBone);
					if(index != -1){
						compMat = engComponent->GetBoneAt(index).GetMatrix();
					}
				}
				
				compMat = compMat.QuickMultiply(engComponent->GetMatrix());
				
				matrix = matrix.QuickMultiply(compMat.Normalized());
			}
			
			SetPosition(matrix.GetPosition());
			SetOrientation(matrix.GetEulerAngles() * RAD2DEG);
			SetDistance(0.0f);
			
			pDirty = false;
			
		}else if(locomotion.GetEnabled()){
			const float orientation = locomotion.GetOrientation().GetValue();
			const float lookLeftRight = locomotion.GetLookLeftRight().GetValue();
			decDMatrix matrix(decDMatrix::CreateRT(decDVector(pFreeOrientation * DEG2RAD), pFreePosition)
				.QuickMultiply(decDMatrix::CreateRotationY((orientation + lookLeftRight) * DEG2RAD))
				.QuickMultiply(decDMatrix::CreateTranslation(locomotion.GetPosition())));
			
			SetPosition(matrix.GetPosition());
			SetOrientation(matrix.GetEulerAngles().ToVector() * RAD2DEG);
			SetDistance(pFreeDistance);
			
			pDirty = true; // so that when we stop the testing an update is done to recover
			
		}else{
			SetPosition(pFreePosition);
			SetOrientation(pFreeOrientation);
			SetDistance(pFreeDistance);
			
			pDirty = false;
		}
		
		pAnimator->NotifyCameraViewChanged();
	}
}



void aeCamera::Reset(){
	igdeCamera::Reset();
	
	pFreeDistance = 5.0f;
	pBone.Empty();
	pFreePosition.Set(0.0, 1.5, 0.0);
	pFreeOrientation.Set(0.0f, 180.0f, 0.0f);
	pRelPosition.Set(0.0f, 0.0f, 0.0f);
	pRelOrientation.Set(0.0f, 0.0f, 0.0f);
	pAttachToBone = false;
	
	SetExposure(1.0f); // 2.0f
	SetLowestIntensity(20.0f);
	SetHighestIntensity(20.0f);
	
//	SetAdaptionTime( 0.8f );
//	SetExposure( 1.0f );
//	SetLowestIntensity( 0.1f );
//	SetHighestIntensity( 2500.0f );
	
	pDirty = true;
	
	pAnimator->NotifyCameraChanged();
}

void aeCamera::ParameterChanged(){
	igdeCamera::ParameterChanged();
	
	pAnimator->NotifyCameraChanged();
}

void aeCamera::AdaptionChanged(){
	igdeCamera::AdaptionChanged();
	
	pAnimator->NotifyCameraChanged();
}
