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
#include <string.h>

#include "dearRuleLimit.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/deEngine.h>



// Class dearRuleBoneRotator
//////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat("Rule Limit = %iys", (int)(timer.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleLimit::dearRuleLimit(dearAnimatorInstance &instance, const dearAnimator &animator,
	int firstLink, const deAnimatorRuleLimit &rule) :
dearRule(instance, animator, firstLink, rule),
pLimit(rule),

pEnablePositionXMin(rule.GetEnablePositionXMin()),
pEnablePositionXMax(rule.GetEnablePositionXMax()),
pEnablePositionYMin(rule.GetEnablePositionYMin()),
pEnablePositionYMax(rule.GetEnablePositionYMax()),
pEnablePositionZMin(rule.GetEnablePositionZMin()),
pEnablePositionZMax(rule.GetEnablePositionZMax()),
pEnablePositionAny(
	pEnablePositionXMin || pEnablePositionXMax ||
	pEnablePositionYMin || pEnablePositionYMax ||
	pEnablePositionZMin || pEnablePositionZMax),

pEnableRotationXMin(rule.GetEnableRotationXMin()),
pEnableRotationXMax(rule.GetEnableRotationXMax()),
pEnableRotationYMin(rule.GetEnableRotationYMin()),
pEnableRotationYMax(rule.GetEnableRotationYMax()),
pEnableRotationZMin(rule.GetEnableRotationZMin()),
pEnableRotationZMax(rule.GetEnableRotationZMax()),
pEnableRotationAny(
	pEnableRotationXMin || pEnableRotationXMax ||
	pEnableRotationYMin || pEnableRotationYMax ||
	pEnableRotationZMin || pEnableRotationZMax),

pEnableScalingXMin(rule.GetEnableScalingXMin()),
pEnableScalingXMax(rule.GetEnableScalingXMax()),
pEnableScalingYMin(rule.GetEnableScalingYMin()),
pEnableScalingYMax(rule.GetEnableScalingYMax()),
pEnableScalingZMin(rule.GetEnableScalingZMin()),
pEnableScalingZMax(rule.GetEnableScalingZMax()),
pEnableScalingAny(
	pEnableScalingXMin || pEnableScalingXMax ||
	pEnableScalingYMin || pEnableScalingYMax ||
	pEnableScalingZMin || pEnableScalingZMax),

pEnableVPSMin(rule.GetEnableVertexPositionSetMin()),
pEnableVPSMax(rule.GetEnableVertexPositionSetMax()),
pEnableVPSAny(pEnableVPSMin || pEnableVPSMax),

pEnabledAny(pEnablePositionAny || pEnableRotationAny || pEnableScalingAny || pEnableVPSAny),

pCoordinateFrame(rule.GetCoordinateFrame()),
pMinPosition(rule.GetMinimumPosition()),
pMaxPosition(rule.GetMaximumPosition()),
pMinRotation(rule.GetMinimumRotation()),
pMaxRotation(rule.GetMaximumRotation()),
pMinScaling(rule.GetMinimumScaling()),
pMaxScaling(rule.GetMaximumScaling()),
pMinVPS(rule.GetMinimumVertexPositionSet()),
pMaxVPS(rule.GetMaximumVertexPositionSet())
{
	RuleChanged();
}

dearRuleLimit::~dearRuleLimit(){
}



// Management
///////////////

void dearRuleLimit::Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist){
DEBUG_RESET_TIMERS;
	if(! GetEnabled() || ! pEnabledAny){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if(blendFactor < FLOAT_SAFE_EPSILON){
		return;
	}
	
	// prepare for applying the rule
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const int boneCount = GetBoneMappingCount();
	const int vpsCount = GetVPSMappingCount();
	int i;
	
	// for bone local a quick version can be used
	switch(pCoordinateFrame){
	case deAnimatorRuleLimit::ecfBoneLocal:
		for(i=0; i<boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone == -1){
				continue;
			}
			
			// calculate limit coordinate space matrix
			dearBoneState &bstate = *stalist.GetStateAt(animatorBone);
			
			bstate.UpdateMatrices();
			decMatrix boneMatrix(bstate.GetLocalMatrix());
			
			// get the parameters but only if the respective parameter has any
			// chance to change to avoid unrequired calculations
			decVector position;
			decVector rotation;
			decVector scaling;
			
			if(pEnablePositionAny){
				position = boneMatrix.GetPosition();
			}
			if(pEnableRotationAny){
				rotation = boneMatrix.GetEulerAngles();
			}
			if(pEnableScalingAny){
				scaling = boneMatrix.GetScale();
			}
			
			// apply limits. keep track if anything changes to avoid doing calculations if they have no effect
			bool hasChangedPosition = false;
			bool hasChangedRotation = false;
			bool hasChangedScaling = false;
			
			if(pEnablePositionXMin && position.x < pMinPosition.x){
				position.x = pMinPosition.x;
				hasChangedPosition = true;
			}
			if(pEnablePositionXMax && position.x > pMaxPosition.x){
				position.x = pMaxPosition.x;
				hasChangedPosition = true;
			}
			if(pEnablePositionYMin && position.y < pMinPosition.y){
				position.y = pMinPosition.y;
				hasChangedPosition = true;
			}
			if(pEnablePositionYMax && position.y > pMaxPosition.y){
				position.y = pMaxPosition.y;
				hasChangedPosition = true;
			}
			if(pEnablePositionZMin && position.z < pMinPosition.z){
				position.z = pMinPosition.z;
				hasChangedPosition = true;
			}
			if(pEnablePositionZMax && position.z > pMaxPosition.z){
				position.z = pMaxPosition.z;
				hasChangedPosition = true;
			}
			
			if(pEnableRotationXMin && rotation.x < pMinRotation.x){
				rotation.x = pMinRotation.x;
				hasChangedRotation = true;
			}
			if(pEnableRotationXMax && rotation.x > pMaxRotation.x){
				rotation.x = pMaxRotation.x;
				hasChangedRotation = true;
			}
			if(pEnableRotationYMin && rotation.y < pMinRotation.y){
				rotation.y = pMinRotation.y;
				hasChangedRotation = true;
			}
			if(pEnableRotationYMax && rotation.y > pMaxRotation.y){
				rotation.y = pMaxRotation.y;
				hasChangedRotation = true;
			}
			if(pEnableRotationZMin && rotation.z < pMinRotation.z){
				rotation.z = pMinRotation.z;
				hasChangedRotation = true;
			}
			if(pEnableRotationZMax && rotation.z > pMaxRotation.z){
				rotation.z = pMaxRotation.z;
				hasChangedRotation = true;
			}
			
			if(pEnableScalingXMin && scaling.x < pMinScaling.x){
				scaling.x = pMinScaling.x;
				hasChangedScaling = true;
			}
			if(pEnableScalingXMax && scaling.x > pMaxScaling.x){
				scaling.x = pMaxScaling.x;
				hasChangedScaling = true;
			}
			if(pEnableScalingYMin && scaling.y < pMinScaling.y){
				scaling.y = pMinScaling.y;
				hasChangedScaling = true;
			}
			if(pEnableScalingYMax && scaling.y > pMaxScaling.y){
				scaling.y = pMaxScaling.y;
				hasChangedScaling = true;
			}
			if(pEnableScalingZMin && scaling.z < pMinScaling.z){
				scaling.z = pMinScaling.z;
				hasChangedScaling = true;
			}
			if(pEnableScalingZMax && scaling.z > pMaxScaling.z){
				scaling.z = pMaxScaling.z;
				hasChangedScaling = true;
			}
			
			// if nothing changed there is no need to apply any calculations
			if(! hasChangedPosition && ! hasChangedRotation && ! hasChangedScaling){
				continue;
			}
			
			// apply the blending
			if(hasChangedRotation){
				bstate.BlendWith(position, decQuaternion::CreateFromEuler(rotation), scaling,
					blendMode, blendFactor, hasChangedPosition, true, hasChangedScaling);
				
			}else{
				bstate.BlendWith(position, decQuaternion(), scaling, blendMode, blendFactor,
					hasChangedPosition, false, hasChangedScaling);
			}
		}
		break;
		
	// for all other coordinate frames a complex version has to be used to be safe
	case deAnimatorRuleLimit::ecfComponent:
	case deAnimatorRuleLimit::ecfTargetBone:
		// prepare transformation matrix
		decMatrix matrix1, matrix2;
		
		if(pCoordinateFrame == deAnimatorRuleLimit::ecfTargetBone && pTargetBone != -1){
			dearBoneState &bstate = *stalist.GetStateAt(pTargetBone);
			bstate.UpdateMatrices();
			matrix1 = bstate.GetInverseGlobalMatrix();
			matrix2 = bstate.GetGlobalMatrix();
		}
		
		// step through all bones and apply limitation
		for(i=0; i<boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone == -1){
				continue;
			}
			
			// calculate limit coordinate space matrix
			dearBoneState &bstate = *stalist.GetStateAt(animatorBone);
			decMatrix boneMatrix;
			
			bstate.UpdateMatrices();
			
			switch(pCoordinateFrame){
			case deAnimatorRuleLimit::ecfComponent:
				boneMatrix = bstate.GetGlobalMatrix();
				break;
				
			case deAnimatorRuleLimit::ecfBoneLocal:
			case deAnimatorRuleLimit::ecfTargetBone:
				if(pTargetBone == -1){
					boneMatrix = bstate.GetLocalMatrix();
					
				}else{
					boneMatrix = bstate.GetGlobalMatrix().QuickMultiply(matrix1);
				}
				break;
			}
			
			// get the parameters but only if the respective parameter has any
			// chance to change to avoid unrequired calculations
			decVector position(boneMatrix.GetPosition());
			decVector rotation(boneMatrix.GetEulerAngles());
			decVector scaling(boneMatrix.GetScale());
			
			// apply limits. keep track if anything changes to avoid doing calculations if they have no effect
			bool hasChanged = false;
			
			if(pEnablePositionXMin && position.x < pMinPosition.x){
				position.x = pMinPosition.x;
				hasChanged = true;
			}
			if(pEnablePositionXMax && position.x > pMaxPosition.x){
				position.x = pMaxPosition.x;
				hasChanged = true;
			}
			if(pEnablePositionYMin && position.y < pMinPosition.y){
				position.y = pMinPosition.y;
				hasChanged = true;
			}
			if(pEnablePositionYMax && position.y > pMaxPosition.y){
				position.y = pMaxPosition.y;
				hasChanged = true;
			}
			if(pEnablePositionZMin && position.z < pMinPosition.z){
				position.z = pMinPosition.z;
				hasChanged = true;
			}
			if(pEnablePositionZMax && position.z > pMaxPosition.z){
				position.z = pMaxPosition.z;
				hasChanged = true;
			}
			
			if(pEnableRotationXMin && rotation.x < pMinRotation.x){
				rotation.x = pMinRotation.x;
				hasChanged = true;
			}
			if(pEnableRotationXMax && rotation.x > pMaxRotation.x){
				rotation.x = pMaxRotation.x;
				hasChanged = true;
			}
			if(pEnableRotationYMin && rotation.y < pMinRotation.y){
				rotation.y = pMinRotation.y;
				hasChanged = true;
			}
			if(pEnableRotationYMax && rotation.y > pMaxRotation.y){
				rotation.y = pMaxRotation.y;
				hasChanged = true;
			}
			if(pEnableRotationZMin && rotation.z < pMinRotation.z){
				rotation.z = pMinRotation.z;
				hasChanged = true;
			}
			if(pEnableRotationZMax && rotation.z > pMaxRotation.z){
				rotation.z = pMaxRotation.z;
				hasChanged = true;
			}
			
			if(pEnableScalingXMin && scaling.x < pMinScaling.x){
				scaling.x = pMinScaling.x;
				hasChanged = true;
			}
			if(pEnableScalingXMax && scaling.x > pMaxScaling.x){
				scaling.x = pMaxScaling.x;
				hasChanged = true;
			}
			if(pEnableScalingYMin && scaling.y < pMinScaling.y){
				scaling.y = pMinScaling.y;
				hasChanged = true;
			}
			if(pEnableScalingYMax && scaling.y > pMaxScaling.y){
				scaling.y = pMaxScaling.y;
				hasChanged = true;
			}
			if(pEnableScalingZMin && scaling.z < pMinScaling.z){
				scaling.z = pMinScaling.z;
				hasChanged = true;
			}
			if(pEnableScalingZMax && scaling.z > pMaxScaling.z){
				scaling.z = pMaxScaling.z;
				hasChanged = true;
			}
			
			// if nothing changed there is no need to apply any calculations
			if(! hasChanged){
				continue;
			}
			
			// apply transformation
			decQuaternion orientation;
			
			switch(pCoordinateFrame){
			case deAnimatorRuleLimit::ecfComponent:
				boneMatrix = bstate.CalcLocalFromGlobal(decMatrix::CreateSRT(scaling, rotation, position));
				position = boneMatrix.GetPosition();
				orientation = boneMatrix.ToQuaternion();
				scaling = boneMatrix.GetScale();
				break;
				
			case deAnimatorRuleLimit::ecfBoneLocal:
			case deAnimatorRuleLimit::ecfTargetBone:
				if(pTargetBone != -1){
					boneMatrix = bstate.CalcLocalFromGlobal(decMatrix::CreateSRT(scaling, rotation, position).QuickMultiply(matrix2));
					position = boneMatrix.GetPosition();
					orientation = boneMatrix.ToQuaternion();
					scaling = boneMatrix.GetScale();
				}
				break;
			}
			
			// apply the blending
			bstate.BlendWith(position, orientation, scaling, blendMode, blendFactor, true, true, true);
		}
		break;
	}
	
	// vertex position sets
	for(i=0; i<vpsCount; i++){
		const int animatorVps = GetVPSMappingFor(i);
		if(animatorVps == -1){
			continue;
		}
		
		dearVPSState &vpsState = vpsstalist.GetStateAt(animatorVps);
		
		float weight = 0.0f;
		if(pEnableVPSAny){
			weight = vpsState.GetWeight();
		}
		
		bool hasChanged = false;
		
		if(pEnableVPSMin && weight < pMinVPS){
			weight = pMinVPS;
			hasChanged = true;
		}
		if(pEnableVPSMax && weight > pMaxVPS){
			weight = pMaxVPS;
			hasChanged = true;
		}
		
		if(hasChanged){
			vpsState.BlendWith(weight, blendMode, blendFactor, true);
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleLimit::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateTargetBone();
}



// Private Functions
//////////////////////

void dearRuleLimit::pUpdateTargetBone(){
	pTargetBone = GetInstance().GetBoneStateList().IndexOfStateNamed(pLimit.GetTargetBone());
}
