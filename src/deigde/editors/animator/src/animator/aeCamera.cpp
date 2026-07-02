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

#include "aeCamera.h"
#include "aeAnimator.h"
#include "locomotion/aeAnimatorLocomotion.h"
#include "../gui/aeWindowMain.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>



// Class aeCamera
///////////////////

// Constructor, destructor
////////////////////////////

aeCamera::aeCamera(aeAnimator &animator, deEngine *engine) :
igdeCamera(animator.GetEnvironment(), engine),
pMPAttachToBone(animator.GetWindowMain().GetMCAnimatorProperties().cameraAttachToBone, animator.GetMetaContextView()),
pMPBone(animator.GetWindowMain().GetMCAnimatorProperties().cameraAttachBone, animator.GetMetaContextView()),
pMPRelativePosition(animator.GetWindowMain().GetMCAnimatorProperties().cameraAttachRelativePosition, animator.GetMetaContextView()),
pMPRelativeRotation(animator.GetWindowMain().GetMCAnimatorProperties().cameraAttachRelativeRotation, animator.GetMetaContextView())
{
	pAnimator = &animator;
	
	pFreeDistance = 0.0f;
	pDirty = false;
	
	pMPAttachToBone.onValueChanged = [this](){
		pDirty = true;
		pAnimator->NotifyCameraChanged();
	};
	
	pMPBone.onValueChanged = [this](){
		pDirty = true;
		pAnimator->NotifyCameraChanged();
	};
	
	pMPRelativePosition.onValueChanged = [this](){
		pDirty = true;
		pAnimator->NotifyCameraChanged();
	};
	pMPRelativeRotation.onValueChanged = pMPRelativePosition.onValueChanged;
}

aeCamera::~aeCamera(){
}



// Management
///////////////

void aeCamera::SetBone(const char *value){
	pMPBone = value;
}

void aeCamera::SetFreePosition(const decDVector &freePosition){
	if(!freePosition.IsEqualTo(pFreePosition)){
		pFreePosition = freePosition;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetFreeOrientation(const decVector &freeOrientation){
	if(!freeOrientation.IsEqualTo(pFreeOrientation)){
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

void aeCamera::SetRelativePosition(const decVector &value){
	pMPRelativePosition = value;
}

void aeCamera::SetRelativeOrientation(const decVector &value){
	pMPRelativeRotation = value;
}

void aeCamera::SetAttachToBone(bool value){
	pMPAttachToBone = value;
}

void aeCamera::Update(){
	aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	
	if(pDirty || pMPAttachToBone || locomotion.GetEnabled()){
		if(pMPAttachToBone){
			decMatrix matrix(decMatrix::CreateWorld(pMPRelativePosition, pMPRelativeRotation));
			deComponent *engComponent = pAnimator->GetEngineComponent();
			
			if(engComponent){
				decMatrix compMat;
				
				engComponent->PrepareBones();
				
				if(!pMPBone->IsEmpty() && engComponent->GetRig()){
					const int index = engComponent->GetRig()->IndexOfBoneNamed(pMPBone);
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
	pMPBone = "";
	pFreePosition.Set(0.0, 1.5, 0.0);
	pFreeOrientation.Set(0.0f, 180.0f, 0.0f);
	pMPRelativePosition = decVector();
	pMPRelativeRotation = decQuaternion();
	pMPAttachToBone = false;
	
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
