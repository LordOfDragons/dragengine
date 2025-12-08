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

#include "reCamera.h"
#include "reRig.h"
#include "bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class reCamera
///////////////////

// Constructor, destructor
////////////////////////////

reCamera::reCamera(reRig *rig, deEngine *engine) : igdeCamera(engine){
	if(!rig) DETHROW(deeInvalidParam);
	
	pRig = rig;
	
	pFreeDistance = 0.0f;
	pBone = NULL;
	pDirty = false;
	pAttachToBone = false;
}

reCamera::~reCamera(){
}



// Management
///////////////

void reCamera::SetBone(reRigBone *bone){
	if(bone != pBone){
		if(pBone) pBone->FreeReference();
		
		pBone = bone;
		
		if(bone) bone->AddReference();
		
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetFreePosition(const decDVector &freePosition){
	if(!freePosition.IsEqualTo(pFreePosition)){
		pFreePosition = freePosition;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetFreeOrientation(const decVector &freeOrientation){
	if(!freeOrientation.IsEqualTo(pFreeOrientation)){
		pFreeOrientation = freeOrientation;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetFreeDistance(float freeDistance){
	if(fabsf(freeDistance - pFreeDistance) > 1e-5f){
		pFreeDistance = freeDistance;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetRelativePosition(const decDVector &relativePosition){
	if(!relativePosition.IsEqualTo(pRelPosition)){
		pRelPosition = relativePosition;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetRelativeOrientation(const decVector &relativeOrientation){
	if(!relativeOrientation.IsEqualTo(pRelOrientation)){
		pRelOrientation = relativeOrientation;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetAttachToBone(bool attachToBone){
	if(pAttachToBone != attachToBone){
		pAttachToBone = attachToBone;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::Update(){
	if(pDirty || pAttachToBone){
		if(pAttachToBone){
			decDMatrix matrix = decDMatrix::CreateRT(decDVector(pRelOrientation * DEG2RAD), pRelPosition);
			
			if(pBone){
				matrix *= decDMatrix(pBone->GetPoseMatrix());
			}
			
			SetPosition(matrix.GetPosition());
			SetOrientation(matrix.GetEulerAngles().ToVector() * RAD2DEG);
			SetDistance(0.0f);
			
		}else{
			SetPosition(pFreePosition);
			SetOrientation(pFreeOrientation);
			SetDistance(pFreeDistance);
		}
		
		pDirty = false;
		
		pRig->NotifyCameraViewChanged();
	}
}



void reCamera::Reset(){
	igdeCamera::Reset();
	
	pFreeDistance = 5.0f;
	if(pBone){
		pBone = NULL;
	}
	pFreePosition.Set(0.0, 1.0, 0.0);
	pFreeOrientation.Set(0.0f, 180.0f, 0.0f);
	pRelPosition.Set(0.0, 0.0, 0.0);
	pRelOrientation.Set(0.0f, 0.0f, 0.0f);
	pAttachToBone = false;
	
	SetExposure(1.0f);
	SetLowestIntensity(20.0f);
	SetHighestIntensity(20.0f);
	
	pDirty = true;
	
	pRig->NotifyCameraChanged();
}

void reCamera::ParameterChanged(){
	igdeCamera::ParameterChanged();
	
	pRig->NotifyCameraChanged();
}

void reCamera::AdaptionChanged(){
	igdeCamera::AdaptionChanged();
	
	pRig->NotifyCameraChanged();
}
