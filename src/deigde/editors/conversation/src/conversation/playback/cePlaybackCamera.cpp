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

#include "cePlaybackCamera.h"
#include "../actor/ceConversationActor.h"
#include "../target/ceTarget.h"

#include <deigde/gui/igdeCamera.h>

#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/exceptions.h>



// Class cePlaybackCamera
///////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackCamera::cePlaybackCamera(cePlayback &playback) :
pPlayback(playback)
{
	pCameraTarget = NULL;
	pLookAtTarget = NULL;
	pCameraDistanceFrom = 0.0f;
	pCameraDistanceTo = 0.0f;
	pTiltFrom = 0.0f;
	pTiltTo = 0.0f;
	pFovFrom = 90.0f;
	pFovTo = 90.0f;
	pElapsed = 0.0f;
	pDuration = 0.0f;
	pLockUpAxis = true;
	pAlignTargets = false;
	pRelativeToLookAt = false;
	pLockCameraTarget = true;
	pLockLookAtTarget = true;
	pHasLockedCameraMatrix = false;
	pHasLockedLookAtMatrix = false;
}

cePlaybackCamera::~cePlaybackCamera(){
	SetCameraTarget(NULL);
	SetLookAtTarget(NULL);
}



// Management
///////////////

void cePlaybackCamera::SetCoordinateSystem(const decMatrix &matrix){
	pCoordSys = matrix;
}



void cePlaybackCamera::SetCameraTarget(ceTarget *target){
	if(target != pCameraTarget){
		
		pCameraTarget = target;
	}
}

void cePlaybackCamera::SetLookAtTarget(ceTarget *target){
	if(target != pLookAtTarget){
		
		pLookAtTarget = target;
	}
}

void cePlaybackCamera::SetOffsetCameraFrom(const decVector &position){
	pOffsetCameraFrom = position;
}

void cePlaybackCamera::SetOffsetCameraTo(const decVector &position){
	pOffsetCameraTo = position;
}

void cePlaybackCamera::SetCameraOrbitFrom(const decVector &rotation){
	pCameraOrbitFrom = rotation;
}

void cePlaybackCamera::SetCameraOrbitTo(const decVector &rotation){
	pCameraOrbitTo = rotation;
}

void cePlaybackCamera::SetCameraDistanceFrom(float distance){
	pCameraDistanceFrom = distance;
}

void cePlaybackCamera::SetCameraDistanceTo(float distance){
	pCameraDistanceTo = distance;
}

void cePlaybackCamera::SetLookAtFrom(const decVector &position){
	pOffsetLookAtFrom = position;
}

void cePlaybackCamera::SetLookAtTo(const decVector &position){
	pOffsetLookAtTo = position;
}

void cePlaybackCamera::SetPositionFrom(const decVector &position){
	pPositionFrom = position;
}

void cePlaybackCamera::SetPositionTo(const decVector &position){
	pPositionTo = position;
}

void cePlaybackCamera::SetRotationFrom(const decVector &rotation){
	pRotationFrom = rotation;
}

void cePlaybackCamera::SetRotationTo(const decVector &rotation){
	pRotationTo = rotation;
}

void cePlaybackCamera::SetTiltFrom(float tilt){
	pTiltFrom = tilt;
}

void cePlaybackCamera::SetTiltTo(float tilt){
	pTiltTo = tilt;
}

void cePlaybackCamera::SetFovFrom(float fov){
	pFovFrom = fov;
}

void cePlaybackCamera::SetFovTo(float fov){
	pFovTo = fov;
}

void cePlaybackCamera::SetLockUpAxis(bool lockUpAxis){
	pLockUpAxis = lockUpAxis;
}

void cePlaybackCamera::SetAlignTargets(bool billboardTargets){
	pAlignTargets = billboardTargets;
}

void cePlaybackCamera::SetRelativeToLookAt(bool relativeToLookAt){
	pRelativeToLookAt = relativeToLookAt;
}

void cePlaybackCamera::SetLockCameraTarget(bool lock){
	pLockCameraTarget = lock;
	pHasLockedCameraMatrix = false;
}

void cePlaybackCamera::SetLockLookAtTarget(bool lock){
	pLockLookAtTarget = lock;
	pHasLockedLookAtMatrix = false;
}



const decCurveBezier &cePlaybackCamera::GetParameterCurve(int parameter) const{
	if(parameter < 0 || parameter >= EP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	return pCurve[parameter];
}

void cePlaybackCamera::SetParameterCurve(int parameter, const decCurveBezier &curve){
	if(parameter < 0 || parameter >= EP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	pCurve[parameter] = curve;
}

void cePlaybackCamera::SetParameterCurve(int parameter, float value){
	if(parameter < 0 || parameter >= EP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	pCurve[parameter].RemoveAllPoints();
	pCurve[parameter].SetInterpolationMode(decCurveBezier::eimConstant);
	pCurve[parameter].AddPoint(decCurveBezierPoint(decVector2(0.0f, value)));
}

void cePlaybackCamera::SetParameterCurve(int parameter, float valueFrom, float valueTo){
	if(parameter < 0 || parameter >= EP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	pCurve[parameter].RemoveAllPoints();
	pCurve[parameter].SetInterpolationMode(decCurveBezier::eimLinear);
	pCurve[parameter].AddPoint(decCurveBezierPoint(decVector2(0.0f, valueFrom)));
	pCurve[parameter].AddPoint(decCurveBezierPoint(decVector2(1.0f, valueTo)));
}



void cePlaybackCamera::SetDuration(float duration){
	if(duration < 0.0f){
		duration = 0.0f;
	}
	
	pDuration = duration;
}

void cePlaybackCamera::Rewind(){
	pElapsed = 0.0f;
	pHasLockedCameraMatrix = false;
	pHasLockedLookAtMatrix = false;
}

void cePlaybackCamera::Reset(){
	pCoordSys.SetIdentity();
	
	SetCameraTarget(NULL);
	pOffsetCameraFrom.SetZero();
	pOffsetCameraTo = pOffsetCameraFrom;
	pCameraOrbitFrom.SetZero();
	pCameraOrbitTo = pCameraOrbitFrom;
	pCameraDistanceFrom = 0.5f;
	pCameraDistanceTo = pCameraDistanceFrom;
	
	SetLookAtTarget(NULL);
	pOffsetLookAtFrom.SetZero();
	pOffsetLookAtTo = pOffsetLookAtFrom;
	
	pPositionFrom.SetZero();
	pPositionTo.SetZero();
	pRotationFrom.SetZero();
	pRotationTo.SetZero();
	pTiltFrom = 0.0f;
	pTiltTo = pTiltFrom;
	pFovFrom = 90.0f;
	pFovTo = pFovFrom;
	pAlignTargets = false;
	pRelativeToLookAt = false;
	
	SetParameterCurve(epPositionX, 0.0f);
	SetParameterCurve(epPositionY, 1.65f);
	SetParameterCurve(epPositionZ, -0.5f);
	SetParameterCurve(epLookAtX, 0.0f);
	SetParameterCurve(epLookAtY, 1.65f);
	SetParameterCurve(epLookAtZ, 0.0f);
	SetParameterCurve(epTilt, pTiltFrom);
	SetParameterCurve(epFov, pFovFrom);
	
	pElapsed = 0.0f;
	pDuration = 0.0f;
	
	pHasLockedCameraMatrix = false;
	pHasLockedLookAtMatrix = false;
}

void cePlaybackCamera::SetCoordSys1Actor(ceConversationActor &actor1){
	pCoordSys = decMatrix::CreateRotationY(180.0f * DEG2RAD)
		* decMatrix::CreateRT( actor1.GetOrientation() * DEG2RAD, actor1.GetPosition() );
	
	const decVector position = pCoordSys.GetPosition();
	const decVector view = pCoordSys.TransformView();
	const decVector up(0.0f, 1.0f, 0.0f);
	pCoordSys.SetWorld(position, (up % view) % up, up);
}

void cePlaybackCamera::SetCoordSys2Actors(ceConversationActor &actor1, ceConversationActor &actor2){
	const decVector &position1 = actor1.GetPosition();
	const decVector &position2 = actor2.GetPosition();
	decVector view;
	float len;
	
	view = position1 - position2;
	len = view.Length();
	
	if(len < 1e-6f){
		SetCoordSys1Actor(actor1);
		
	}else{
		view /= len;
		
		if(view.y >= 0.999f){
			// too close to the up vector. use a different up vector
			pCoordSys = decMatrix::CreateWorld((position1 + position2) * 0.5f, view, decVector(0.0f, 0.0f, 1.0f));
			
		}else{
			pCoordSys = decMatrix::CreateWorld((position1 + position2) * 0.5f, view, decVector(0.0f, 1.0f, 0.0f));
		}
	}
	
	const decVector position = pCoordSys.GetPosition();
	const decVector view2 = pCoordSys.TransformView();
	const decVector up(0.0f, 1.0f, 0.0f);
	pCoordSys.SetWorld(position, (up % view2) % up, up);
}

void cePlaybackCamera::Update(igdeCamera &camera, float elapsed, bool clamp){
	pElapsed += elapsed;
	
	if(clamp){
		if(pElapsed > pDuration){
			pElapsed = pDuration;
		}
		
	}else{
		if(pDuration < 1e-6f){
			pElapsed = 0.0f;
			
		}else{
			pElapsed = fmodf(pElapsed, pDuration);
		}
	}
	
	UpdateLinear(camera);
}

void cePlaybackCamera::UpdateConstant(igdeCamera &camera){
	UpdateWith(camera, pOffsetCameraFrom, pOffsetLookAtFrom, pPositionFrom,
		pRotationFrom, pCameraDistanceFrom, pCameraOrbitFrom, pFovFrom);
}

void cePlaybackCamera::UpdateLinear(igdeCamera &camera){
	decVector offsetCamera;
	decVector offsetLookAt;
	decVector position;
	decVector rotation;
	float cameraDistance;
	decVector cameraOrbit;
	float fov;
	
	if(pElapsed <= 0.0f || pDuration < 1e-6f){
		offsetCamera = pOffsetCameraFrom;
		offsetLookAt = pOffsetLookAtFrom;
		cameraOrbit = pCameraOrbitFrom;
		cameraDistance = pCameraDistanceFrom;
		position = pPositionFrom;
		rotation = pRotationFrom;
		fov = pFovFrom;
		
	}else if(pElapsed >= pDuration){
		offsetCamera = pOffsetCameraTo;
		offsetLookAt = pOffsetLookAtTo;
		cameraOrbit = pCameraOrbitTo;
		cameraDistance = pCameraDistanceTo;
		position = pPositionTo;
		rotation = pRotationTo;
		fov = pFovTo;
		
	}else{
		const float factorTo = pElapsed / pDuration;
		const float factorFrom = 1.0f - factorTo;
		
		offsetCamera = pOffsetCameraFrom * factorFrom + pOffsetCameraTo * factorTo;
		offsetLookAt = pOffsetLookAtFrom * factorFrom + pOffsetLookAtTo * factorTo;
		cameraOrbit = pCameraOrbitFrom * factorFrom + pCameraOrbitTo * factorTo;
		cameraDistance = pCameraDistanceFrom * factorFrom + pCameraDistanceTo * factorTo;
		position = pPositionFrom * factorFrom + pPositionTo * factorTo;
		rotation = pRotationFrom * factorFrom + pRotationTo * factorTo;
		fov = pFovFrom * factorFrom + pFovTo * factorTo;
	}
	
	UpdateWith(camera, offsetCamera, offsetLookAt, position, rotation, cameraDistance, cameraOrbit, fov);
}

void cePlaybackCamera::UpdateCurve(igdeCamera &camera){
	const decVector up = pCoordSys.TransformUp();
	
	decCurveBezierEvaluator evalPosX(pCurve[epPositionX]);
	decCurveBezierEvaluator evalPosY(pCurve[epPositionY]);
	decCurveBezierEvaluator evalPosZ(pCurve[epPositionZ]);
	decCurveBezierEvaluator evalLookAtX(pCurve[epLookAtX]);
	decCurveBezierEvaluator evalLookAtY(pCurve[epLookAtY]);
	decCurveBezierEvaluator evalLookAtZ(pCurve[epLookAtZ]);
	decCurveBezierEvaluator evalTilt(pCurve[epTilt]);
	decCurveBezierEvaluator evalFov(pCurve[epFov]);
	decMatrix targetMatrix;
	decVector orientation;
	decVector position;
	float evalPosition;
	decVector lookAt;
	decVector view;
	float len;
	
	if(pElapsed <= 0.0f || pDuration < 1e-6f){
		evalPosition = 0.0f;
		
	}else if(pElapsed >= pDuration){
		evalPosition = 1.0f;
		
	}else{
		evalPosition = pElapsed / pDuration;
	}
	
	position.x = evalPosX.EvaluateAt(evalPosition);
	position.y = evalPosY.EvaluateAt(evalPosition);
	position.z = evalPosZ.EvaluateAt(evalPosition);
	lookAt.x = evalLookAtX.EvaluateAt(evalPosition);
	lookAt.y = evalLookAtY.EvaluateAt(evalPosition);
	lookAt.z = evalLookAtZ.EvaluateAt(evalPosition);
	
	if(pCameraTarget){
		pCameraTarget->GetCoordinateSystem(pPlayback, targetMatrix);
		position = targetMatrix * position;
		
	}else{
		position = pCoordSys * position;
	}
	
	if(pLookAtTarget){
		pLookAtTarget->GetCoordinateSystem(pPlayback, targetMatrix);
		lookAt = targetMatrix * lookAt;
		
	}else{
		lookAt = pCoordSys * lookAt;
	}
	
	view = lookAt - position;
	len = view.Length();
	if(len > 1e-5f){
		orientation = decMatrix::CreateRotationZ(evalTilt.EvaluateAt(evalPosition) * DEG2RAD)
			* decMatrix::CreateWorld( position, view / len, up ).GetEulerAngles() * RAD2DEG;
	}
	
	camera.SetPosition(decDVector(position));
	camera.SetOrientation(orientation);
	camera.SetDistance(0.0f);
	camera.SetFov(evalFov.EvaluateAt(evalPosition));
}



void cePlaybackCamera::UpdateWith(igdeCamera &camera, const decVector &offsetCamera,
const decVector &offsetLookAt, const decVector &position, const decVector &rotation,
float cameraDistance, const decVector &cameraOrbit, float fov){
	decMatrix matrixTargetCamera;
	decMatrix matrixTargetLookAt;
	
	// calculate camera and look-at target matrices
	if(pHasLockedCameraMatrix){
		matrixTargetCamera = pLockedCameraMatrix;
		
	}else{
		if(pCameraTarget){
			pCameraTarget->GetCoordinateSystem(pPlayback, matrixTargetCamera);
			
		}else{
			matrixTargetCamera = pCoordSys;
		}
		
		if(pLockCameraTarget){
			pLockedCameraMatrix = matrixTargetCamera;
			pHasLockedCameraMatrix = true;
		}
	}
	
	if(pHasLockedLookAtMatrix){
		matrixTargetLookAt = pLockedLookAtMatrix;
		
	}else{
		if(pLookAtTarget){
			pLookAtTarget->GetCoordinateSystem(pPlayback, matrixTargetLookAt);
			
		}else{
			matrixTargetLookAt = pCoordSys;
		}
		
		if(pLockLookAtTarget){
			pLockedLookAtMatrix = matrixTargetLookAt;
			pHasLockedLookAtMatrix = true;
		}
	}
	
	// align target matrices if required
	if(pAlignTargets && pCameraTarget && pLookAtTarget){
		decVector line = matrixTargetLookAt.GetPosition() - matrixTargetCamera.GetPosition();
		const float len = line.Length();
		
		if(len > 1e-5f){
			line /= len;
			
			const decVector tp1 = matrixTargetCamera.GetPosition();
			const decVector tu1 = matrixTargetCamera.TransformUp();
			if(pLockUpAxis){
				matrixTargetCamera.SetWorld(tp1, (tu1 % line) % tu1, tu1);
				
			}else{
				matrixTargetCamera.SetWorld(tp1, line, tu1);
			}
			
			const decVector tp2 = matrixTargetLookAt.GetPosition();
			const decVector tu2 = matrixTargetLookAt.TransformUp();
			if(pLockUpAxis){
				matrixTargetLookAt.SetWorld(tp2, (tu2 % -line) % tu2, tu2);
				
			}else{
				matrixTargetLookAt.SetWorld(tp2, -line, tu2);
			}
		}
	}
	
	// determine the final camera and look-at position
	const decVector lookAtPosition = matrixTargetLookAt * offsetLookAt;
	decVector cameraPosition;
	
	if(pRelativeToLookAt){
		cameraPosition = (decMatrix::CreateTranslation(0.0, 0.0, -cameraDistance)
			* decMatrix::CreateRT( cameraOrbit * DEG2RAD, offsetLookAt )
			* matrixTargetLookAt ).GetPosition();
		
	}else{
		cameraPosition = (decMatrix::CreateTranslation(0.0, 0.0, -cameraDistance)
			* decMatrix::CreateRT( cameraOrbit * DEG2RAD, offsetCamera )
			* matrixTargetCamera ).GetPosition();
	}
	
	// align the up axis to the conversation coordinate system up axis
	decMatrix matrixCamera = decMatrix::CreateRT(rotation * DEG2RAD, position);
	const decVector view = lookAtPosition - cameraPosition;
	
	if(view.Length() > FLOAT_SAFE_EPSILON){
		const decVector up = pCoordSys.TransformUp();
		matrixCamera *= decMatrix::CreateWorld(cameraPosition, view, up);
		
	}else{
		matrixCamera *= decMatrix::CreateTranslation(cameraPosition);
	}
	
	camera.SetPosition(decDVector(matrixCamera.GetPosition()));
	camera.SetOrientation(matrixCamera.GetEulerAngles() * RAD2DEG);
	camera.SetDistance(0.0f);
	camera.SetFov(fov);
}
