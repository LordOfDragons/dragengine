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

#include "dedsLocomotion.h"

#include <libdscript/exceptions.h>

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderCollisionTest.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Inline functions
/////////////////////

static inline float climFromAdjustTime(float adjustTime){
	if(adjustTime > 0.001f){
		return powf(4.0f, 1.0f - logf(adjustTime) / logf(2.0f));
			// log(1/adjustTime) = log(1) - log(adjustTime) = -log(adjustTime)
		
	}else{
		return 0.0f;
	}
}



// Class dedsLocomotion
/////////////////////////

// Constructor, destructor
////////////////////////////

dedsLocomotion::dedsLocomotion() :
pCanTurn(true),
pAlwaysLimitLeftRight(false),

pLimitLookUp(90.0f),
pLimitLookDown(-90.0f),

pLimitLookLeft(-90.0f),
pLimitLookRight(90.0f),
pTurnAdjustLookHorizontal(true),

pAnalogMovingVertical(0.0f),
pTurnHorizontal(0.0f),

pAnalogMovingSpeed(0.0f),

pAdjustTimeOrientation(1.0f),
pCLimAccelTimeOrientation(0.0f),
pOrientation(0.0f),
pTurningSpeed(0.0f),

pIsMoving(false),
pMovingSpeed(0.0f),
pMovingOrientation(0.0f),
pMovingDirection(0.0f),

pIsTurningIP(false),
pAdjustTimeTurnIP(1.0f),
pTurnIP(0.0f),
pResetTimeTurnIP(false),
pReverseTimeTurnIP(false),

pCanTurnInPlace(true),
pLimitTurnInPlaceLeft(-70.0f),
pLimitTurnInPlaceRight(70.0f),

pResetTimeWalk(false),

pCanTilt(false),
pTiltMode(etmNone),
pCCTTiltSingle(NULL),
pCCTTiltFrontLeft(NULL),
pCCTTiltFrontRight(NULL),
pCCTTiltBackLeft(NULL),
pCCTTiltBackRight(NULL),

pLimitTiltUp(30.0f),
pLimitTiltDown(-30.0f),

pLimitTiltLeft(-30.0f),
pLimitTiltRight(30.0f),

pTiltOffset(0.0f),

pControllerMappings(NULL),
pControllerMappingCount(0),
pControllerMappingSize(0),

pAICollider(NULL),
pUpdateAIColliderAngularVelocity(false)
{
	pLookVertical.SetAdjustTime(0.0f);
	pLookVertical.SetAdjustRange(90.0f);
	
	pLookHorizontal.SetAdjustTime(0.0f);
	pLookHorizontal.SetAdjustRange(90.0f);
	
	pAnalogMovingHorizontal.SetAdjustTime(0.0f);
	pAnalogMovingHorizontal.SetAdjustRange(90.0f);
	
	pLinearVelocity.SetAdjustTime(0.5f);
	pLinearVelocity.SetAdjustRange(4.0f);
	
	pStance.SetAdjustTime(0.5f);
	pStance.SetAdjustRange(1.0f);
	
	pTiltVertical.SetAdjustTime(0.5f);
	pTiltVertical.SetAdjustRange(30.0f);
	
	pTiltHorizontal.SetAdjustTime(0.5f);
	pTiltHorizontal.SetAdjustRange(30.0f);
	
	pCLimAccelTimeOrientation = climFromAdjustTime(pAdjustTimeOrientation);
}


dedsLocomotion::~dedsLocomotion(){
	pCleanUp();
}



// Management
///////////////

void dedsLocomotion::SetCanTurn(bool canTurn){
	pCanTurn = canTurn;
}

void dedsLocomotion::SetAlwaysLimitLeftRight(bool alwaysLimitLeftRight){
	pAlwaysLimitLeftRight = alwaysLimitLeftRight;
}



void dedsLocomotion::SetLimitLookUp(float limit){
	pLimitLookUp = limit;
	SetLookVertical(pLookVertical.GetValue());
	SetLookVerticalGoal(pLookVertical.GetGoal());
}

void dedsLocomotion::SetLimitLookDown(float limit){
	pLimitLookDown = limit;
	SetLookVertical(pLookVertical.GetValue());
	SetLookVerticalGoal(pLookVertical.GetGoal());
}

void dedsLocomotion::SetLookVerticalGoal(float goal){
	pLookVertical.SetGoal(decMath::clamp(goal, pLimitLookDown, pLimitLookUp));
}

void dedsLocomotion::SetLookVertical(float value){
	pLookVertical.SetValue(decMath::clamp(value, pLimitLookDown, pLimitLookUp));
}



void dedsLocomotion::SetLimitLookLeft(float limit){
	pLimitLookLeft = limit;
}

void dedsLocomotion::SetLimitLookRight(float limit){
	pLimitLookRight = limit;
}

void dedsLocomotion::SetTurnAdjustLookHorizontal(bool turnAdjust){
	pTurnAdjustLookHorizontal = turnAdjust;
}



void dedsLocomotion::SetAnalogMovingVertical(float value){
	pAnalogMovingVertical = decMath::clamp(value, -90.0f, 90.0f);
// 	pAnalogMovingVertical = decMath::clamp( value, pLimitLookLeft, pLimitLookRight );
}

void dedsLocomotion::SetTurnHorizontal(float value){
	pTurnHorizontal = value;
}



void dedsLocomotion::SetAnalogMovingSpeed(float value){
	pAnalogMovingSpeed = value;
}



void dedsLocomotion::SetAdjustTimeOrientation(float time){
	pAdjustTimeOrientation = decMath::max(time, 0.0f);
	pCLimAccelTimeOrientation = climFromAdjustTime(pAdjustTimeOrientation);
}

void dedsLocomotion::SetOrientation(float orientation){
	pOrientation = decMath::normalize(orientation, 360.0f);
	SetOrientationQuaternion(decQuaternion::CreateFromEulerY(pOrientation * DEG2RAD));
}

void dedsLocomotion::SetOrientationQuaternion(const decQuaternion &orientation){
	pOrientationQuaternion = orientation;
	//pCollider->SetOrientation( orientation );
}

void dedsLocomotion::SetTurningSpeed(float speed){
	pTurningSpeed = speed;
	SetAngularVelocity(decVector(0.0f, speed, 0.0f));
}

void dedsLocomotion::SetAngularVelocity(const decVector &velocity){
	pAngularVelocity = velocity;
	//pCollider->SetAngularVelocity( velocity * DEG2RAD );
}



void dedsLocomotion::SetIsMoving(bool isMoving){
	pIsMoving = isMoving;
}

void dedsLocomotion::SetMovingSpeed(float speed){
	pMovingSpeed = speed;
}

void dedsLocomotion::SetMovingOrientation(float orientation){
	pMovingOrientation = decMath::normalize(orientation, 360.0f);
}

void dedsLocomotion::SetMovingDirection(float direction){
	pMovingDirection = decMath::normalize(direction, -180.0f, 180.0f);
}



void dedsLocomotion::SetIsTurningIP(bool turnInPlace){
	pIsTurningIP = turnInPlace;
}

void dedsLocomotion::SetAdjustTimeTurnIP(float time){
	pAdjustTimeTurnIP = decMath::max(time, 0.0f);
}

void dedsLocomotion::SetTurnIP(float value){
	pTurnIP = value;
}

void dedsLocomotion::SetResetTimeTurnIP(bool reset){
	pResetTimeTurnIP = reset;
}

void dedsLocomotion::SetReverseTimeTurnIP(bool reverse){
	pReverseTimeTurnIP = reverse;
}



void dedsLocomotion::SetCanTurnInPlace(bool canTurnInPlace){
	pCanTurnInPlace = canTurnInPlace;
}

void dedsLocomotion::SetLimitTurnInPlaceLeft(float limit){
	pLimitTurnInPlaceLeft = limit;
}

void dedsLocomotion::SetLimitTurnInPlaceRight(float limit){
	pLimitTurnInPlaceRight = limit;
}



void dedsLocomotion::SetResetTimeWalk(bool reset){
	pResetTimeWalk = reset;
}



void dedsLocomotion::SetCanTilt(bool canTilt){
	pCanTilt = canTilt;
}

void dedsLocomotion::SetTiltMode(eTiltModes mode){
	if(mode < etmNone || mode > etmWeighted){
		DSTHROW(dueInvalidParam);
	}
	pTiltMode = mode;
}

void dedsLocomotion::SetCCTTiltSingle(deColliderCollisionTest *collisionTest){
	if(collisionTest == pCCTTiltSingle){
		return;
	}
	
	if(pCCTTiltSingle){
		pCCTTiltSingle->FreeReference();
	}
	
	pCCTTiltSingle = collisionTest;
	
	if(collisionTest){
		collisionTest->AddReference();
	}
}

void dedsLocomotion::SetCCTTiltFrontLeft(deColliderCollisionTest *collisionTest){
	if(collisionTest == pCCTTiltFrontLeft){
		return;
	}
	
	if(pCCTTiltFrontLeft){
		pCCTTiltFrontLeft->FreeReference();
	}
	
	pCCTTiltFrontLeft = collisionTest;
	
	if(collisionTest){
		collisionTest->AddReference();
	}
}

void dedsLocomotion::SetCCTTiltFrontRight(deColliderCollisionTest *collisionTest){
	if(collisionTest == pCCTTiltFrontRight){
		return;
	}
	
	if(pCCTTiltFrontRight){
		pCCTTiltFrontRight->FreeReference();
	}
	
	pCCTTiltFrontRight = collisionTest;
	
	if(collisionTest){
		collisionTest->AddReference();
	}
}

void dedsLocomotion::SetCCTTiltBackLeft(deColliderCollisionTest *collisionTest){
	if(collisionTest == pCCTTiltBackLeft){
		return;
	}
	
	if(pCCTTiltBackLeft){
		pCCTTiltBackLeft->FreeReference();
	}
	
	pCCTTiltBackLeft = collisionTest;
	
	if(collisionTest){
		collisionTest->AddReference();
	}
}

void dedsLocomotion::SetCCTTiltBackRight(deColliderCollisionTest *collisionTest){
	if(collisionTest == pCCTTiltBackRight){
		return;
	}
	
	if(pCCTTiltBackRight){
		pCCTTiltBackRight->FreeReference();
	}
	
	pCCTTiltBackRight = collisionTest;
	
	if(collisionTest){
		collisionTest->AddReference();
	}
}



void dedsLocomotion::SetLimitTiltUp(float limit){
	pLimitTiltUp = limit;
	SetTiltVertical(pTiltVertical.GetValue());
	SetTiltVerticalGoal(pTiltVertical.GetGoal());
}

void dedsLocomotion::SetLimitTiltDown(float limit){
	pLimitTiltDown = limit;
	SetTiltVertical(pTiltVertical.GetValue());
	SetTiltVerticalGoal(pTiltVertical.GetGoal());
}

void dedsLocomotion::SetTiltVertical(float tiltVertical){
	pTiltVertical.SetValue(decMath::clamp(tiltVertical, pLimitTiltDown, pLimitTiltUp));
}

void dedsLocomotion::SetTiltVerticalGoal(float tiltVertical){
	pTiltVertical.SetGoal(decMath::clamp(tiltVertical, pLimitTiltDown, pLimitTiltUp));
}



void dedsLocomotion::SetLimitTiltLeft(float limit){
	pLimitTiltLeft = limit;
	SetTiltHorizontal(pTiltHorizontal.GetValue());
	SetTiltHorizontalGoal(pTiltHorizontal.GetGoal());
}

void dedsLocomotion::SetLimitTiltRight(float limit){
	pLimitTiltRight = limit;
	SetTiltHorizontal(pTiltHorizontal.GetValue());
	SetTiltHorizontalGoal(pTiltHorizontal.GetGoal());
}

void dedsLocomotion::SetTiltHorizontal(float tiltHorizontal){
	pTiltHorizontal.SetValue(decMath::clamp(tiltHorizontal, pLimitTiltLeft, pLimitTiltRight));
}

void dedsLocomotion::SetTiltHorizontalGoal(float tiltHorizontal){
	pTiltHorizontal.SetGoal(decMath::clamp(tiltHorizontal, pLimitTiltLeft, pLimitTiltRight));
}



void dedsLocomotion::SetTiltOffset(float offset){
	pTiltOffset = offset;
}



const dedsLControllerMapping &dedsLocomotion::GetControllerMappingAt(int index) const{
	if(index < 0 || index >= pControllerMappingCount){
		DSTHROW(dueInvalidParam);
	}
	
	return pControllerMappings[index];
}

int dedsLocomotion::IndexOfControllerMappingWith(deAnimatorInstance *animatorInstance,
int controller) const{
	int i;
	
	for(i=0; i<pControllerMappingCount; i++){
		if(pControllerMappings[i].GetAnimatorInstance() == animatorInstance
		&& pControllerMappings[i].GetController() == controller){
			return i;
		}
	}
	
	return -1;
}

void dedsLocomotion::AddControllerMapping(deAnimatorInstance *animatorInstance,
int controller, dedsLControllerMapping::eAttributes attribute){
	int i;
	
	for(i=0; i<pControllerMappingCount; i++){
		if(pControllerMappings[i].GetAnimatorInstance() == animatorInstance
		&& pControllerMappings[i].GetController() == controller){
			pControllerMappings[i].SetAttribute(attribute);
			return;
		}
	}
	
	if(pControllerMappingCount == pControllerMappingSize){
		const int newSize = pControllerMappingSize + 1;
		dedsLControllerMapping * const newArray = new dedsLControllerMapping[newSize];
		if(pControllerMappings){
			for(i=0; i<pControllerMappingSize; i++){
				newArray[i] = pControllerMappings[i];
			}
			delete [] pControllerMappings;
		}
		
		pControllerMappings = newArray;
		pControllerMappingSize = newSize;
	}
	
	dedsLControllerMapping &mapping = pControllerMappings[pControllerMappingCount++];
	mapping.SetAnimatorInstance(animatorInstance);
	mapping.SetController(controller);
	mapping.SetAttribute(attribute);
}

void dedsLocomotion::RemoveControllerMapping(deAnimatorInstance *animatorInstance, int controller){
	int i;
	
	for(i=0; i<pControllerMappingCount; i++){
		if(pControllerMappings[i].GetAnimatorInstance() == animatorInstance
		&& pControllerMappings[i].GetController() != controller){
			continue;
		}
		
		int j;
		for(j=i+1; j<pControllerMappingCount; j++){
			pControllerMappings[j - 1] = pControllerMappings[j];
		}
		pControllerMappingCount--;
		break;
	}
}

void dedsLocomotion::RemoveAllControllerMappings(){
	pControllerMappingCount = 0;
}



void dedsLocomotion::SetAICollider(deCollider *collider){
	if(collider == pAICollider){
		return;
	}
	
	if(pAICollider){
		pAICollider->FreeReference();
	}
	
	pAICollider = collider;
	
	if(collider){
		collider->AddReference();
	}
}

void dedsLocomotion::SetUpdateAIColliderAngularVelocity(bool updateAngularVelocity){
	pUpdateAIColliderAngularVelocity = updateAngularVelocity;
}



void dedsLocomotion::CancelInput(){
	pAnalogMovingHorizontal.Reset();
	pAnalogMovingSpeed = 0.0f;
	pTurnHorizontal = 0.0f;
	pLookHorizontal.SetGoal(pLookHorizontal.GetValue());
	pLookVertical.SetGoal(pLookVertical.GetValue());
	pStance.SetGoal(pStance.GetValue());
}

void dedsLocomotion::CancelMovement(){
	CancelInput();
	
	pLinearVelocity.Reset();
	pMovingSpeed = 0.0f;
	pMovingOrientation = 0.0f;
	pMovingDirection = 0.0f;
	pResetTimeWalk = false;
}

void dedsLocomotion::CancelMotion(){
	CancelMovement();
	CancelTurnInPlace();
	
	SetTurningSpeed(0.0f);
	pTiltHorizontal.SetGoal(pTiltHorizontal.GetValue());
	pTiltVertical.SetGoal(pTiltVertical.GetValue());
}

void dedsLocomotion::CancelTurnInPlace(){
	pTurnIP = 0.0f;
	pIsTurningIP = false;
	pReverseTimeTurnIP = false;
	pResetTimeTurnIP = false;
}

void dedsLocomotion::ForceBodyAdjustment(){
	if(! pCanTurn){
		return;
	}
	
	pTurnHorizontal += pLookHorizontal.GetGoal();
	
	if(pCanTurnInPlace){
		pTurnHorizontal += pTurnIP;
	}
	pIsTurningIP = false;
	pTurnIP = 0.0f;
	pResetTimeTurnIP = false;
}



void dedsLocomotion::UpdateLooking(float elapsed){
	pLookHorizontal.Update(elapsed);
	pLookVertical.Update(elapsed);
	pAnalogMovingHorizontal.Update(elapsed);
}

void dedsLocomotion::ApplyStates(){
	pResetTimeWalk = false;
	pReverseTimeTurnIP = false;
	pResetTimeTurnIP = false;
	
	SetOrientation(pOrientation + pTurnHorizontal);
	pLookHorizontal.SetValue(pLookHorizontal.GetGoal());
	pAnalogMovingHorizontal.SetValue(pAnalogMovingHorizontal.GetGoal());
	
	pTurnHorizontal = 0.0f;
	pLookVertical.SetValue(pLookVertical.GetGoal());
	pTurningSpeed = 0.0f;
	pTurnIP = 0.0f;
	
	SetMovingOrientation(pOrientation + pAnalogMovingHorizontal.GetValue());
	
	pLinearVelocity.SetValue(decMatrix::CreateRotationY(pMovingOrientation).TransformView() * pAnalogMovingSpeed);
	pMovingSpeed = pAnalogMovingSpeed;
	pIsMoving = fabsf(pAnalogMovingSpeed) > 0.001f;
// 	printf( "ApplyStates: mo=%g o=%g md=%g amlr=%g tlr=%g lv=(%g,%g,%g) ls=%g\n", pMovingOrientation, pOrientation, pMovingDirection,
// 		pAnalogMovingHorizontal.GetValue(), pTurnHorizontal, pLinearVelocity.GetValue().x,
// 		pLinearVelocity.GetValue().y, pLinearVelocity.GetValue().z, pMovingSpeed );
}

void dedsLocomotion::UpdateLocomotion(float elapsed){
	if(elapsed < 1e-5f){
		return;
	}
	
	pResetTimeWalk = false;
	pReverseTimeTurnIP = false;
	pResetTimeTurnIP = false;
	
	UpdateIsMoving();
	UpdateOrientation(elapsed);
	UpdateLinearVelocity(elapsed);
	UpdateStance(elapsed);
}

void dedsLocomotion::UpdateIsMoving(){
	const bool isMoving = fabsf(pAnalogMovingSpeed) > 0.001f;
	if(isMoving == pIsMoving){
		return;
	}
	
	if(isMoving){
		if(pMovingSpeed < 0.001f){
			pResetTimeWalk = true;
		}
		
		pIsTurningIP = false;
		pTurnIP = 0.0f;
		
	}else{
		pResetTimeTurnIP = true;
		//pAnimator->ResetControllersWith( eaTimeStand );
	}
	
	pIsMoving = isMoving;
}

void dedsLocomotion::UpdateOrientation(float elapsed){
	if(! pCanTurn){
		if(pLookHorizontal.GetValue() > pLimitLookRight){
			pLookHorizontal.SetValue(pLimitLookRight);
			pLookHorizontal.SetGoal(pLimitLookRight);
			
		}else if(pLookHorizontal.GetValue() < pLimitLookLeft){
			pLookHorizontal.SetValue(pLimitLookLeft);
			pLookHorizontal.SetGoal(pLimitLookLeft);
		}
		
		pTurnHorizontal = 0.0f;
		SetTurningSpeed(0.0f);
		
		return;
	}
	
	float adjustOrientation = 0.0f;
	
	if(pIsMoving){
		UpdateOrientationMoving(elapsed, adjustOrientation);
		
	}else{
		UpdateOrientationNotMoving(elapsed, adjustOrientation);
	}
	
	CheckLookingRangeViolation(adjustOrientation);
	AdjustOrientation(adjustOrientation);
	SetTurningSpeed(adjustOrientation / elapsed);
}

void dedsLocomotion::UpdateOrientationMoving(float elapsed, float &adjustOrientation){
	// adjust value for no smoothing
	adjustOrientation = pTurnHorizontal;
	
	// smooth value if required
	if(pAdjustTimeOrientation > 0.001f){
#if 1
		const float limitTurnSpeed = 90.0f / pAdjustTimeOrientation;
		adjustOrientation *= decMath::min(elapsed / (pAdjustTimeOrientation * 0.25f), 1.0f);
		
		const float limitAccel = pCLimAccelTimeOrientation * 90.0f * elapsed; // scale = 90 degrees
		const float oldTurnSpeed = decMath::clamp(pTurningSpeed, -limitTurnSpeed, limitTurnSpeed);
		
		float sdiff = (adjustOrientation / elapsed) - oldTurnSpeed;
		
		if(sdiff > limitAccel && adjustOrientation > 0.0f){ // old < goal(=old+adjust)
			adjustOrientation = (oldTurnSpeed + limitAccel) * elapsed;
			
		}else if(sdiff < -limitAccel && adjustOrientation < 0.0f){ // old > goal(=old+adjust)
			adjustOrientation = (oldTurnSpeed - limitAccel) * elapsed;
		}
#else
		const float limitTurnSpeed = 90.0f / pAdjustTimeOrientation;
		const float limitTurnAccel = limitTurnSpeed * 3.5f;
		const float adjustFactor = 2.5f / pAdjustTimeOrientation * elapsed;
		
		adjustOrientation *= decMath::min(adjustFactor, 1.0f);
		
		// clamp turning speed to the desired turning speed
		const float maxSpeedFactor = limitTurnSpeed * elapsed;
		adjustOrientation = decMath::clamp(adjustOrientation, -maxSpeedFactor, maxSpeedFactor);
		
		// clamp turning acceleration to the desired turning acceleration
		const float turningSpeed = decMath::clamp(pTurningSpeed, -limitTurnSpeed, limitTurnSpeed);
		const float speedDiff = adjustOrientation / elapsed - turningSpeed;
		const float maxSpeedDiff = limitTurnAccel * elapsed;
		
		if(speedDiff > maxSpeedDiff){
			adjustOrientation = (turningSpeed + maxSpeedDiff) * elapsed;
			
		}else if(speedDiff < -maxSpeedDiff){
			adjustOrientation = (turningSpeed - maxSpeedDiff) * elapsed;
		}
#endif
	}
}

void dedsLocomotion::UpdateOrientationNotMoving(float elapsed, float &adjustOrientation){
	// adjust orientation is the remaining turn left right
	adjustOrientation = pTurnHorizontal;
	
	// smooth value if required
	if(pAdjustTimeOrientation > 0.001f){
		const float limitTurnSpeed = 90.0f / pAdjustTimeOrientation;
		const float limitTurnAccel = limitTurnSpeed * 3.5f;
		const float adjustFactor = 2.5f / pAdjustTimeOrientation * elapsed;
		
		adjustOrientation *= decMath::min(adjustFactor, 1.0f);
		
		// clamp turning speed to the desired turning speed
		const float maxSpeedFactor = limitTurnSpeed * elapsed;
		adjustOrientation = decMath::clamp(adjustOrientation, -maxSpeedFactor, maxSpeedFactor);
		
		// clamp turning acceleration to the desired turning acceleration
		const float turningSpeed = decMath::clamp(pTurningSpeed, -limitTurnSpeed, limitTurnSpeed);
		const float speedDiff = adjustOrientation / elapsed - turningSpeed;
		const float maxSpeedDiff = limitTurnAccel * elapsed;
		
		if(speedDiff > maxSpeedDiff){
			adjustOrientation = (turningSpeed + maxSpeedDiff) * elapsed;
			
		}else if(speedDiff < -maxSpeedDiff){
			adjustOrientation = (turningSpeed - maxSpeedDiff) * elapsed;
		}
	}
	
	// turn in place rotation if required
	if(pCanTurnInPlace){
		if(pLookHorizontal.GetGoal() > pLimitTurnInPlaceRight){
			if(pIsTurningIP){
				if(pTurnIP < 0.0f){
					pTurnIP += 90.0f;
					pReverseTimeTurnIP = true;
				}
				
			}else{
				pIsTurningIP = true;
				pTurnIP = 90.0f;
				pResetTimeTurnIP = true;
			}
			
		}else if(pLookHorizontal.GetGoal() < pLimitTurnInPlaceLeft){
			if(pIsTurningIP){
				if(pTurnIP > 0.0f){
					pTurnIP -= 90.0f;
					pReverseTimeTurnIP = true;
				}
				
			}else{
				pIsTurningIP = true;
				pTurnIP = -90.0f;
				pResetTimeTurnIP = true;
			}
		}
		
	}else{
		pIsTurningIP = false;
		pTurnIP = 0.0f;
		pResetTimeTurnIP = false;
	}
	
	if(pIsTurningIP){
		if(pTurnIP > 0.0f){
			adjustOrientation = 90.0f * elapsed / pAdjustTimeTurnIP;
			
			if(adjustOrientation > pTurnIP){
				adjustOrientation = pTurnIP;
				pTurnIP = 0.0f;
				pIsTurningIP = false;
				
			}else{
				pTurnIP -= adjustOrientation;
			}
			
		}else{
			adjustOrientation = -90.0f * elapsed / pAdjustTimeTurnIP;
			
			if(adjustOrientation < pTurnIP){
				adjustOrientation = pTurnIP;
				pTurnIP = 0.0f;
				pIsTurningIP = false;
				
			}else{
				pTurnIP -= adjustOrientation;
			}
		}
	}
}

void dedsLocomotion::CheckLookingRangeViolation(float &adjustOrientation){
	// if we can turn the body keep the looking always inside the limits.
	// if leaving adjust the adjustOrientation to satisfy the limits again.
	if(pCanTurn && !pAlwaysLimitLeftRight){
		if(pLookHorizontal.GetGoal() - adjustOrientation > pLimitLookRight){
			adjustOrientation = pLookHorizontal.GetGoal() - pLimitLookRight;
			
		}else if(pLookHorizontal.GetGoal() - adjustOrientation < pLimitLookLeft){
			adjustOrientation = pLookHorizontal.GetGoal() - pLimitLookLeft;
		}
		
	// if we can not turn clamp the looking to the limits
	}else{
		if(pLookHorizontal.GetValue() - adjustOrientation > pLimitLookRight){
			pLookHorizontal.SetValue(pLimitLookRight);
			pLookHorizontal.SetGoal(pLimitLookRight);
			
		}else if(pLookHorizontal.GetValue() - adjustOrientation < pLimitLookLeft){
			pLookHorizontal.SetValue(pLimitLookLeft);
			pLookHorizontal.SetGoal(pLimitLookLeft);
		}
	}
}

void dedsLocomotion::UpdateLinearVelocity(float elapsed){
	if(pIsMoving){
		SetMovingOrientation(pOrientation + pAnalogMovingHorizontal.GetValue());
	}
	
	pLinearVelocity.SetGoal(decMatrix::CreateRotationY(pMovingOrientation * DEG2RAD)
		.TransformView() * pAnalogMovingSpeed);
	pLinearVelocity.Update(elapsed);
	// TODO update collider if attached
	
	const decVector &linearVelocity = pLinearVelocity.GetValue();
	pMovingSpeed = linearVelocity.Length();
	
	if(pMovingSpeed > 0.001f){ // otherwise undefined orientation
		SetMovingOrientation(-atan2f(linearVelocity.x, linearVelocity.z) * RAD2DEG);
	}
	
	SetMovingDirection(pMovingOrientation - pOrientation);
// 	printf( "UpdateLinearVelocity: mo=%g o=%g md=%g amlr=%g tlr=%g lv=(%g,%g,%g) ls=%g\n",
// 		pMovingOrientation, pOrientation, pMovingDirection, pAnalogMovingHorizontal.GetValue(),
// 		pTurnHorizontal, linearVelocity.x, linearVelocity.y, linearVelocity.z, pMovingSpeed );
}

void dedsLocomotion::UpdateStance(float elapsed){
	pStance.Update(elapsed);
}

void dedsLocomotion::UpdatePostLocomotion(float elapsed){
	UpdateTilt(elapsed);
	
	int i;
	for(i=0; i<pControllerMappingCount; i++){
		pControllerMappings[i].ApplyPost(*this, elapsed);
	}
}

void dedsLocomotion::UpdateTilt(float elapsed){
	if(pCanTilt){
		switch(pTiltMode){
		case etmNone:
			return;
			
		case etmSingle:
			UpdateTiltSingleCast(elapsed);
			break;
			
		case etmWeighted:
			UpdateTiltWeightCast(elapsed);
			break;
		}
		
	}else{
		pTiltVertical.SetGoal(0.0f);
		pTiltHorizontal.SetGoal(0.0f);
	}
	
	pTiltVertical.Update(elapsed);
	pTiltHorizontal.Update(elapsed);
}

void dedsLocomotion::UpdateTiltSingleCast(float elapsed){
	if(! pCCTTiltSingle || ! pAICollider){
		return;
	}
	
	if(pCCTTiltSingle->GetCollisionInfoCount() > 0){
		const deCollisionInfo &info = *pCCTTiltSingle->GetCollisionInfoAt(0);
		
		const decMatrix matrix(decMatrix::CreateFromQuaternion(pAICollider->GetOrientation().Conjugate()));
		const decVector normal(matrix.TransformNormal(info.GetNormal()));
		
		SetTiltHorizontalGoal(atan2f(normal.x, normal.y) / DEG2RAD);
		SetTiltVerticalGoal(atan2f(normal.z, normal.y) / DEG2RAD);
		SetTiltOffset(pCCTTiltSingle->GetOrigin().y
			- info.GetDistance() * pCCTTiltSingle->GetDirection().Length());
		
	}else{
		SetTiltHorizontalGoal(0.0f);
		SetTiltVerticalGoal(0.0f);
		SetTiltOffset(0.0f);
	}
}

void dedsLocomotion::UpdateTiltWeightCast(float elapsed){
	if(! pCCTTiltFrontLeft || ! pCCTTiltFrontRight || ! pCCTTiltBackLeft || ! pCCTTiltBackRight || ! pAICollider){
		return;
	}
	
	const float distFrontLeft = pCCTTiltFrontLeft->GetDirection().Length();
	const float distFrontRight = pCCTTiltFrontRight->GetDirection().Length();
	const float distBackLeft = pCCTTiltBackLeft->GetDirection().Length();
	const float distBackRight = pCCTTiltBackRight->GetDirection().Length();
	
	const float originFrontLeft = pCCTTiltFrontLeft->GetOrigin().y;
	const float originFrontRight = pCCTTiltFrontRight->GetOrigin().y;
	const float originBackLeft = pCCTTiltBackLeft->GetOrigin().y;
	const float originBackRight = pCCTTiltBackRight->GetOrigin().y;
	
	float heightFrontLeft = originFrontLeft - distFrontLeft;
	float heightFrontRight = originFrontRight - distFrontRight;
	float heightBackLeft = originBackLeft - distBackLeft;
	float heightBackRight = originBackRight - distBackRight;
	
	if(pCCTTiltFrontLeft->GetCollisionInfoCount() > 0){
		heightFrontLeft = originFrontLeft
			- pCCTTiltFrontLeft->GetCollisionInfoAt(0)->GetDistance() * distFrontLeft;
	}
	if(pCCTTiltFrontRight->GetCollisionInfoCount() > 0){
		heightFrontRight = originFrontRight
			- pCCTTiltFrontRight->GetCollisionInfoAt(0)->GetDistance() * distFrontRight;
	}
	if(pCCTTiltBackLeft->GetCollisionInfoCount() > 0){
		heightBackLeft = originBackLeft
			- pCCTTiltBackLeft->GetCollisionInfoAt(0)->GetDistance() * distBackLeft;
	}
	if(pCCTTiltBackRight->GetCollisionInfoCount() > 0){
		heightBackRight = originBackRight
			- pCCTTiltBackRight->GetCollisionInfoAt(0)->GetDistance() * distBackRight;
	}
	
	// the spread calculation is not the best in the world since it assumes front and back have the same offset.
	// a potential solution would be to average the front and back spread. in general though the spread is the
	// same front and back so this solution should work in most of the time.
	const float spreadHorizontal = decMath::max(pCCTTiltFrontRight->GetOrigin().x - pCCTTiltFrontLeft->GetOrigin().x, 0.01f);
	const float spreadFrontBack = decMath::max(pCCTTiltFrontLeft->GetOrigin().z - pCCTTiltBackLeft->GetOrigin().z, 0.01f);
	
	// calculate the tilt values. this is "(v1-v2)*0.5 + (v3-v4)*0.5".
	const float hdiffHorizontal = (heightFrontLeft - heightFrontRight) * 0.5f + (heightBackLeft - heightBackRight) * 0.5f;
	SetTiltHorizontalGoal(atanf(hdiffHorizontal / spreadHorizontal) / DEG2RAD);
	
	const float hdiffVertical = (heightFrontLeft - heightBackLeft) * 0.5f + (heightFrontRight - heightBackRight) * 0.5f;
	SetTiltVerticalGoal(atanf(hdiffVertical / spreadFrontBack) / DEG2RAD);
	
	// tilt offset from samples
	SetTiltOffset((heightFrontLeft + heightFrontRight + heightBackLeft + heightBackRight) * 0.25f);
}

void dedsLocomotion::UpdateAnimatorInstance(float elapsed){
	int i;
	for(i=0; i<pControllerMappingCount; i++){
		pControllerMappings[i].Apply(*this, elapsed);
	}
	
	pReverseTimeTurnIP = false;
	pResetTimeTurnIP = false;
	pResetTimeWalk = false;
}

void dedsLocomotion::UpdateAICollider(){
	if(pAICollider){
		pAICollider->SetLinearVelocity(pLinearVelocity.GetValue());
		
		if(pUpdateAIColliderAngularVelocity){
			pAICollider->SetAngularVelocity(pAngularVelocity * DEG2RAD);
		}
	}
}

void dedsLocomotion::AdjustOrientation(float angle){
	SetOrientation(pOrientation + angle);
	
	pLookHorizontal.SetValue(pLookHorizontal.GetValue() - angle);
	pAnalogMovingHorizontal.SetValue(pAnalogMovingHorizontal.GetValue() - angle);
	
	if(pTurnAdjustLookHorizontal){
		pLookHorizontal.SetGoal(pLookHorizontal.GetGoal() - angle);
		pAnalogMovingHorizontal.SetGoal(pAnalogMovingHorizontal.GetGoal() - angle);
	}
	
	//SetTurnHorizontal( pTurnHorizontal - angle );
	if(angle > 0.0f){
		SetTurnHorizontal(decMath::max(pTurnHorizontal - angle, 0.0f));
		
	}else{
		SetTurnHorizontal(decMath::min(pTurnHorizontal - angle, 0.0f));
	}
}



void dedsLocomotion::ReadFromFile(decBaseFileReader &reader){
	switch(reader.ReadByte()){
	case 1:
		ReadFromFileV1(reader);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
		break;
	}
}

void dedsLocomotion::ReadFromFileV1(decBaseFileReader &reader){
	const int flags = reader.ReadUShort();
	pCanTurn = (flags & 0x1) == 0x1;
	pTurnAdjustLookHorizontal = (flags & 0x2) == 0x2;
	pIsMoving = (flags & 0x4) == 0x4;
	pIsTurningIP = (flags & 0x8) == 0x8;
	pResetTimeTurnIP = (flags & 0x10) == 0x10;
	pReverseTimeTurnIP = (flags & 0x20) == 0x20;
	pCanTurnInPlace = (flags & 0x40) == 0x40;
	pResetTimeWalk = (flags & 0x80) == 0x80;
	pCanTilt = (flags & 0x100) == 0x100;
	pUpdateAIColliderAngularVelocity = (flags & 0x200) == 0x200;
	
	pLimitLookUp = reader.ReadFloat();
	pLimitLookDown = reader.ReadFloat();
	pLookVertical.SetValue(reader.ReadFloat());
	pLookVertical.SetGoal(reader.ReadFloat());
	pLookVertical.SetAdjustRange(reader.ReadFloat());
	pLookVertical.SetAdjustTime(reader.ReadFloat());
	pLookVertical.SetChangeSpeed(reader.ReadFloat());
	
	pLimitLookLeft = reader.ReadFloat();
	pLimitLookRight = reader.ReadFloat();
	pLookHorizontal.SetValue(reader.ReadFloat());
	pLookHorizontal.SetGoal(reader.ReadFloat());
	pLookHorizontal.SetAdjustRange(reader.ReadFloat());
	pLookHorizontal.SetAdjustTime(reader.ReadFloat());
	pLookHorizontal.SetChangeSpeed(reader.ReadFloat());
	
	pAnalogMovingHorizontal.SetValue(reader.ReadFloat());
	pAnalogMovingHorizontal.SetGoal(reader.ReadFloat());
	pAnalogMovingHorizontal.SetAdjustRange(reader.ReadFloat());
	pAnalogMovingHorizontal.SetAdjustTime(reader.ReadFloat());
	pAnalogMovingHorizontal.SetChangeSpeed(reader.ReadFloat());
	pTurnHorizontal = reader.ReadFloat();
	
	pAnalogMovingSpeed = reader.ReadFloat();
	
	pAdjustTimeOrientation = reader.ReadFloat();
	pCLimAccelTimeOrientation = reader.ReadFloat();
	pOrientation = reader.ReadFloat();
	pOrientationQuaternion.SetFromEulerY(pOrientation * DEG2RAD);
	pTurningSpeed = reader.ReadFloat();
	pAngularVelocity = reader.ReadVector();
	
	pMovingSpeed = reader.ReadFloat();
	pMovingOrientation = reader.ReadFloat();
	pMovingDirection = reader.ReadFloat();
	
	pLinearVelocity.SetValue(reader.ReadVector());
	pLinearVelocity.SetGoal(reader.ReadVector());
	pLinearVelocity.SetAdjustRange(reader.ReadFloat());
	pLinearVelocity.SetAdjustTime(reader.ReadFloat());
	pLinearVelocity.SetChangeSpeed(reader.ReadFloat());
	
	pStance.SetValue(reader.ReadFloat());
	pStance.SetGoal(reader.ReadFloat());
	pStance.SetAdjustRange(reader.ReadFloat());
	pStance.SetAdjustTime(reader.ReadFloat());
	pStance.SetChangeSpeed(reader.ReadFloat());
	
	pAdjustTimeTurnIP = reader.ReadFloat();
	pTurnIP = reader.ReadFloat();
	
	pLimitTurnInPlaceLeft = reader.ReadFloat();
	pLimitTurnInPlaceRight = reader.ReadFloat();
	
	pTiltMode = (eTiltModes)reader.ReadByte();
	
	pLimitTiltUp = reader.ReadFloat();
	pLimitTiltDown = reader.ReadFloat();
	pTiltVertical.SetValue(reader.ReadFloat());
	pTiltVertical.SetGoal(reader.ReadFloat());
	pTiltVertical.SetAdjustRange(reader.ReadFloat());
	pTiltVertical.SetAdjustTime(reader.ReadFloat());
	pTiltVertical.SetChangeSpeed(reader.ReadFloat());
	
	pLimitTiltLeft = reader.ReadFloat();
	pLimitTiltRight = reader.ReadFloat();
	pTiltHorizontal.SetValue(reader.ReadFloat());
	pTiltHorizontal.SetGoal(reader.ReadFloat());
	pTiltHorizontal.SetAdjustRange(reader.ReadFloat());
	pTiltHorizontal.SetAdjustTime(reader.ReadFloat());
	pTiltHorizontal.SetChangeSpeed(reader.ReadFloat());
	
	pTiltOffset = reader.ReadFloat();
}

void dedsLocomotion::WriteToFile(decBaseFileWriter &writer) const {
	writer.WriteByte(1); // version
	
	int flags = 0;
	if(pCanTurn){
		flags |= 0x1;
	}
	if(pTurnAdjustLookHorizontal){
		flags |= 0x2;
	}
	if(pIsMoving){
		flags |= 0x4;
	}
	if(pIsTurningIP){
		flags |= 0x8;
	}
	if(pResetTimeTurnIP){
		flags |= 0x10;
	}
	if(pReverseTimeTurnIP){
		flags |= 0x20;
	}
	if(pCanTurnInPlace){
		flags |= 0x40;
	}
	if(pResetTimeWalk){
		flags |= 0x80;
	}
	if(pCanTilt){
		flags |= 0x100;
	}
	if(pUpdateAIColliderAngularVelocity){
		flags |= 0x200;
	}
	writer.WriteUShort(flags);
	
	writer.WriteFloat(pLimitLookUp);
	writer.WriteFloat(pLimitLookDown);
	writer.WriteFloat(pLookVertical.GetValue());
	writer.WriteFloat(pLookVertical.GetGoal());
	writer.WriteFloat(pLookVertical.GetAdjustRange());
	writer.WriteFloat(pLookVertical.GetAdjustTime());
	writer.WriteFloat(pLookVertical.GetChangeSpeed());
	
	writer.WriteFloat(pLimitLookLeft);
	writer.WriteFloat(pLimitLookRight);
	writer.WriteFloat(pLookHorizontal.GetValue());
	writer.WriteFloat(pLookHorizontal.GetGoal());
	writer.WriteFloat(pLookHorizontal.GetAdjustRange());
	writer.WriteFloat(pLookHorizontal.GetAdjustTime());
	writer.WriteFloat(pLookHorizontal.GetChangeSpeed());
	
	writer.WriteFloat(pAnalogMovingHorizontal.GetValue());
	writer.WriteFloat(pAnalogMovingHorizontal.GetGoal());
	writer.WriteFloat(pAnalogMovingHorizontal.GetAdjustRange());
	writer.WriteFloat(pAnalogMovingHorizontal.GetAdjustTime());
	writer.WriteFloat(pAnalogMovingHorizontal.GetChangeSpeed());
	writer.WriteFloat(pTurnHorizontal);
	
	writer.WriteFloat(pAnalogMovingSpeed);
	
	writer.WriteFloat(pAdjustTimeOrientation);
	writer.WriteFloat(pCLimAccelTimeOrientation);
	writer.WriteFloat(pOrientation);
	writer.WriteFloat(pTurningSpeed);
	writer.WriteVector(pAngularVelocity);
	
	writer.WriteFloat(pMovingSpeed);
	writer.WriteFloat(pMovingOrientation);
	writer.WriteFloat(pMovingDirection);
	
	writer.WriteVector(pLinearVelocity.GetValue());
	writer.WriteVector(pLinearVelocity.GetGoal());
	writer.WriteFloat(pLinearVelocity.GetAdjustRange());
	writer.WriteFloat(pLinearVelocity.GetAdjustTime());
	writer.WriteFloat(pLinearVelocity.GetChangeSpeed());
	
	writer.WriteFloat(pStance.GetValue());
	writer.WriteFloat(pStance.GetGoal());
	writer.WriteFloat(pStance.GetAdjustRange());
	writer.WriteFloat(pStance.GetAdjustTime());
	writer.WriteFloat(pStance.GetChangeSpeed());
	
	writer.WriteFloat(pAdjustTimeTurnIP);
	writer.WriteFloat(pTurnIP);
	
	writer.WriteFloat(pLimitTurnInPlaceLeft);
	writer.WriteFloat(pLimitTurnInPlaceRight);
	
	writer.WriteByte(pTiltMode);
	
	writer.WriteFloat(pLimitTiltUp);
	writer.WriteFloat(pLimitTiltDown);
	writer.WriteFloat(pTiltVertical.GetValue());
	writer.WriteFloat(pTiltVertical.GetGoal());
	writer.WriteFloat(pTiltVertical.GetAdjustRange());
	writer.WriteFloat(pTiltVertical.GetAdjustTime());
	writer.WriteFloat(pTiltVertical.GetChangeSpeed());
	
	writer.WriteFloat(pLimitTiltLeft);
	writer.WriteFloat(pLimitTiltRight);
	writer.WriteFloat(pTiltHorizontal.GetValue());
	writer.WriteFloat(pTiltHorizontal.GetGoal());
	writer.WriteFloat(pTiltHorizontal.GetAdjustRange());
	writer.WriteFloat(pTiltHorizontal.GetAdjustTime());
	writer.WriteFloat(pTiltHorizontal.GetChangeSpeed());
	
	writer.WriteFloat(pTiltOffset);
}



// Private functions
//////////////////////

void dedsLocomotion::pCleanUp(){
	SetAICollider(NULL);
	
	SetCCTTiltSingle(NULL);
	SetCCTTiltFrontLeft(NULL);
	SetCCTTiltFrontRight(NULL);
	SetCCTTiltBackLeft(NULL);
	SetCCTTiltBackRight(NULL);
	
	if(pControllerMappings){
		delete [] pControllerMappings;
	}
}
