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

#include "ceCameraShot.h"
#include "../ceConversation.h"

#include <dragengine/common/exceptions.h>



// Class ceCameraShot
///////////////////////

// Constructor, destructor
////////////////////////////

ceCameraShot::ceCameraShot(const char *name) :
pName(name)
{
	pConversation = nullptr;
	pActorCount = 1;
	pOffsetCameraFrom.Set(0.0f, 1.65f, 0.0f);
	pCameraDistanceFrom = 0.5f;
	pOffsetLookAtFrom.Set(0.0f, 1.65f, 0.0f);
	pTiltFrom = 0.0f;
	pFovFrom = 90.0f;
	pOffsetCameraTo = pOffsetCameraFrom;
	pCameraDistanceTo = pCameraDistanceFrom;
	pOffsetLookAtTo = pOffsetLookAtFrom;
	pTiltTo = pTiltFrom;
	pFovTo = pFovFrom;
	pLockUpAxis = false;
	pAlignTargets = false;
	pRelativeToLookAt = false;
	pLockCameraTarget = true;
	pLockLookAtTarget = true;
}

ceCameraShot::ceCameraShot(const ceCameraShot &cameraShot){
	int i;
	
	pConversation = nullptr;
	pName = cameraShot.pName;
	pActorCount = cameraShot.pActorCount;
	
	pCameraTarget = cameraShot.pCameraTarget;
	pOffsetCameraFrom = cameraShot.pOffsetCameraFrom;
	pOffsetCameraTo = cameraShot.pOffsetCameraTo;
	pCameraOrbitFrom = cameraShot.pCameraOrbitFrom;
	pCameraOrbitTo = cameraShot.pCameraOrbitTo;
	pCameraDistanceFrom = cameraShot.pCameraDistanceFrom;
	pCameraDistanceTo = cameraShot.pCameraDistanceTo;
	
	pLookAtTarget = cameraShot.pLookAtTarget;
	pOffsetLookAtFrom = cameraShot.pOffsetLookAtFrom;
	pOffsetLookAtTo = cameraShot.pOffsetLookAtTo;
	
	pPositionFrom = cameraShot.pPositionFrom;
	pPositionTo = cameraShot.pPositionTo;
	pRotationFrom = cameraShot.pRotationFrom;
	pRotationTo = cameraShot.pRotationTo;
	
	pTiltFrom = cameraShot.pTiltFrom;
	pTiltTo = cameraShot.pTiltTo;
	pFovFrom = cameraShot.pFovFrom;
	pFovTo = cameraShot.pFovTo;
	
	pLockUpAxis = cameraShot.pLockUpAxis;
	pAlignTargets = cameraShot.pAlignTargets;
	pRelativeToLookAt = cameraShot.pRelativeToLookAt;
	pLockCameraTarget = cameraShot.pLockCameraTarget;
	pLockLookAtTarget = cameraShot.pLockLookAtTarget;
	
	for(i=0; i<EP_COUNT; i++){
		pCurve[i] = cameraShot.pCurve[i];
	}
}

ceCameraShot::~ceCameraShot(){
}



// Management
///////////////

void ceCameraShot::SetConversation(ceConversation *conversation){
	pConversation = conversation;
}

void ceCameraShot::SetName(const char *name){
	DEASSERT_NOTNULL(name)
	
	if(pName == name){
		return;
	}
	
	if(pConversation){
		DEASSERT_FALSE(pConversation->GetCameraShotList().HasMatching(
			[&](const ceCameraShot &c){ return c.GetName() == name;}))
	}
	
	pName = name;
	
	if(pConversation){
		pConversation->NotifyCameraShotChanged(this);
	}
}

void ceCameraShot::SetActorCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pActorCount){
		pActorCount = count;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}



void ceCameraShot::SetCameraTarget(const char *target){
	if(!target){
		DETHROW(deeInvalidParam);
	}
	
	if(pCameraTarget != target){
		pCameraTarget = target;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetOffsetCameraFrom(const decVector& offset){
	if(!offset.IsEqualTo(pOffsetCameraFrom)){
		pOffsetCameraFrom = offset;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetOffsetCameraTo(const decVector& offset){
	if(!offset.IsEqualTo(pOffsetCameraTo)){
		pOffsetCameraTo = offset;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}



void ceCameraShot::SetLookAtTarget(const char *target){
	if(!target){
		DETHROW(deeInvalidParam);
	}
	
	if(pLookAtTarget != target){
		pLookAtTarget = target;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetOffsetLookAtFrom(const decVector& offset){
	if(!offset.IsEqualTo(pOffsetLookAtFrom)){
		pOffsetLookAtFrom = offset;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetOffsetLookAtTo(const decVector& offset){
	if(!offset.IsEqualTo(pOffsetLookAtTo)){
		pOffsetLookAtTo = offset;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}



void ceCameraShot::SetCameraOrbitFrom(const decVector &rotation){
	if(!rotation.IsEqualTo(pCameraOrbitFrom)){
		pCameraOrbitFrom = rotation;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetCameraOrbitTo(const decVector &rotation){
	if(!rotation.IsEqualTo(pCameraOrbitTo)){
		pCameraOrbitTo = rotation;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetCameraDistanceFrom(float distance){
	if(fabsf(distance - pCameraDistanceFrom) > 1e-6f){
		pCameraDistanceFrom = distance;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetCameraDistanceTo(float distance){
	if(fabsf(distance - pCameraDistanceTo) > 1e-6f){
		pCameraDistanceTo = distance;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}



void ceCameraShot::SetPositionFrom(const decVector &position){
	if(!position.IsEqualTo(pPositionFrom)){
		pPositionFrom = position;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetPositionTo(const decVector &position){
	if(!position.IsEqualTo(pPositionTo)){
		pPositionTo = position;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetRotationFrom(const decVector &rotation){
	if(!rotation.IsEqualTo(pRotationFrom)){
		pRotationFrom = rotation;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetRotationTo(const decVector &rotation){
	if(!rotation.IsEqualTo(pRotationTo)){
		pRotationTo = rotation;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetTiltFrom(float tilt){
	if(fabsf(tilt - pTiltFrom) > 1e-6f){
		pTiltFrom = tilt;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetTiltTo(float tilt){
	if(fabsf(tilt - pTiltTo) > 1e-6f){
		pTiltTo = tilt;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetFovFrom(float fov){
	if(fabsf(fov - pFovFrom) > 1e-6f){
		pFovFrom = fov;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetFovTo(float fov){
	if(fabsf(fov - pFovTo) > 1e-6f){
		pFovTo = fov;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}



void ceCameraShot::SetLockUpAxis(bool lockUpAxis){
	if(lockUpAxis != pLockUpAxis){
		pLockUpAxis = lockUpAxis;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetAlignTargets(bool alignTargets){
	if(alignTargets != pAlignTargets){
		pAlignTargets = alignTargets;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetRelativeToLookAt(bool relativeToLookAt){
	if(relativeToLookAt != pRelativeToLookAt){
		pRelativeToLookAt = relativeToLookAt;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetLockCameraTarget(bool lock){
	if(lock != pLockCameraTarget){
		pLockCameraTarget = lock;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}

void ceCameraShot::SetLockLookAtTarget(bool lock){
	if(lock != pLockLookAtTarget){
		pLockLookAtTarget = lock;
		
		if(pConversation){
			pConversation->NotifyCameraShotChanged(this);
		}
	}
}



const decCurveBezier &ceCameraShot::GetParameterCurve(int parameter) const{
	if(parameter < 0 || parameter >= EP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	return pCurve[parameter];
}

void ceCameraShot::SetParameterCurve(int parameter, const decCurveBezier &curve){
	if(parameter < 0 || parameter >= EP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	pCurve[parameter] = curve;
	
	if(pConversation){
		pConversation->NotifyCameraShotChanged(this);
	}
}
