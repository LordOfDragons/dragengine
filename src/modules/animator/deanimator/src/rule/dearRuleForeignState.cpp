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

#include "dearRuleForeignState.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/deEngine.h>



// Class dearRuleForeignState
///////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat("Rule Foreign State = %iys", (int)(timer.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleForeignState::dearRuleForeignState(dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleForeignState &rule) :
dearRule(instance, animator, firstLink, rule),

pForeignState(rule),
pForeignBone(-1),
pForeignVPS(-1),

pTargetPosition(rule.GetTargetPosition(), firstLink),
pTargetOrientation(rule.GetTargetOrientation(), firstLink),
pTargetSize(rule.GetTargetSize(), firstLink),
pTargetVPS(rule.GetTargetVertexPositionSet(), firstLink),

pScalePosition(rule.GetScalePosition()),
pScaleOrientation(rule.GetScaleOrientation()),
pScaleSize(rule.GetScaleSize()),
pScaleVPS(rule.GetScaleVertexPositionSet()),
pSourceCoordFrame(rule.GetSourceCoordinateFrame()),
pDestCoordFrame(rule.GetDestCoordinateFrame()),
pLockX(!rule.GetModifyX()),
pLockY(!rule.GetModifyY()),
pLockZ(!rule.GetModifyZ()),
pLockNone(!pLockX && !pLockY && !pLockZ),
pEnablePosition(rule.GetEnablePosition()),
pEnableOrientation(rule.GetEnableOrientation()),
pEnableSize(rule.GetEnableSize()),
pEnableVPS(rule.GetEnableVertexPositionSet())
{
	RuleChanged();
}

dearRuleForeignState::~dearRuleForeignState(){
}



// Management
///////////////

void dearRuleForeignState::Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist){
DEBUG_RESET_TIMERS;
	if(!GetEnabled()){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if(blendFactor < FLOAT_SAFE_EPSILON){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const dearAnimatorInstance &instance = GetInstance();
	const int boneCount = GetBoneMappingCount();
	const int vpsCount = GetVPSMappingCount();
	int i;
	
	// controller affected values
	const float scalePosition = pTargetPosition.GetValue(instance, pScalePosition);
	const float scaleOrientation = pTargetOrientation.GetValue(instance, pScaleOrientation);
	const float scaleSize = pTargetSize.GetValue(instance, pScaleSize);
	const float scaleVPS = pTargetVPS.GetValue(instance, pScaleVPS);
	
	// calculate the foreign state
	decVector scale(1.0f, 1.0f, 1.0f);
	decQuaternion orientation;
	float weight = 0.0f;
	decVector position;
	decMatrix matrix;
	
	if(pForeignBone != -1){
		dearBoneState &boneState = *stalist.GetStateAt(pForeignBone);
		
		switch(pSourceCoordFrame){
		case deAnimatorRuleForeignState::ecfBoneLocal:
			switch(pDestCoordFrame){
			case deAnimatorRuleForeignState::ecfBoneLocal:
				if(pEnablePosition){
					position = boneState.GetPosition() * scalePosition;
				}
				if(pEnableOrientation){
					orientation = decQuaternion().Slerp(boneState.GetOrientation(), scaleOrientation);
				}
				if(pEnableSize){
					scale += (boneState.GetScale() - scale) * scaleSize;
				}
				break;
				
			case deAnimatorRuleForeignState::ecfComponent:
				boneState.UpdateMatrices();
				matrix = boneState.GetGlobalMatrix();
				break;
			}
			break;
			
		case deAnimatorRuleForeignState::ecfComponent:
			boneState.UpdateMatrices();
			matrix = boneState.GetGlobalMatrix();
			
			if(pDestCoordFrame == deAnimatorRuleForeignState::ecfBoneLocal){
				if(pEnablePosition){
					position = matrix.GetPosition() * scalePosition;
				}
				if(pEnableOrientation){
					orientation = decQuaternion().Slerp(matrix.ToQuaternion(), scaleOrientation);
				}
				if(pEnableSize){
					scale += (matrix.GetScale() - scale) * scaleSize;
				}
			}
			break;
		}
	}
	
	if(pForeignVPS != -1 && pEnableVPS){
		weight = vpsstalist.GetStateAt(pForeignVPS).GetWeight() * scaleVPS;
	}
	
	// apply state.
	// 
	// NOTE do not modify "position", "orientation" and "scale" beyond this point
	
	decVector modifyPosition, modifyScale;
	decQuaternion modifyOrientation;
	
	for(i=0; i<boneCount; i++){
		const int animatorBone = GetBoneMappingFor(i);
		if(animatorBone == -1){
			continue;
		}
		
		dearBoneState &boneState = *stalist.GetStateAt(animatorBone);
		
		switch(pDestCoordFrame){
		case deAnimatorRuleForeignState::ecfBoneLocal:
			if(pLockNone){
				boneState.BlendWith(position, orientation, scale, blendMode,
					blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
				
			}else{
				if(pEnablePosition){
					const decVector &lockPosition = boneState.GetPosition();
					modifyPosition = position;
					
					if(pLockX){
						modifyPosition.x = lockPosition.x;
					}
					if(pLockY){
						modifyPosition.y = lockPosition.y;
					}
					if(pLockZ){
						modifyPosition.z = lockPosition.z;
					}
				}
				
				if(pEnableOrientation){
					const decVector lockRotation(boneState.GetOrientation().GetEulerAngles());
					decVector rotation(orientation.GetEulerAngles());
					
					if(pLockX){
						rotation.x = lockRotation.x;
					}
					if(pLockY){
						rotation.y = lockRotation.y;
					}
					if(pLockZ){
						rotation.z = lockRotation.z;
					}
					
					modifyOrientation = decQuaternion::CreateFromEuler(rotation);
				}
				
				if(pEnableSize){
					const decVector &lockScale = boneState.GetScale();
					modifyScale = scale;
					
					if(pLockX){
						modifyScale.x = lockScale.x;
					}
					if(pLockY){
						modifyScale.y = lockScale.y;
					}
					if(pLockZ){
						modifyScale.z = lockScale.z;
					}
				}
				
				boneState.BlendWith(modifyPosition, modifyOrientation, modifyScale,
					blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
			}
			break;
			
		case deAnimatorRuleForeignState::ecfComponent:
			boneState.UpdateMatrices();
			
			const decMatrix m(matrix.QuickMultiply(boneState.GetInverseGlobalMatrix())
				.QuickMultiply(boneState.GetLocalMatrix()));
			
			if(pLockNone){
				boneState.BlendWith(m.GetPosition(), m.ToQuaternion(), m.GetScale(),
					blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
				
			}else{
				if(pEnablePosition){
					const decVector &lockPosition = boneState.GetPosition();
					modifyPosition = m.GetPosition();
					
					if(pLockX){
						modifyPosition.x = lockPosition.x;
					}
					if(pLockY){
						modifyPosition.y = lockPosition.y;
					}
					if(pLockZ){
						modifyPosition.z = lockPosition.z;
					}
				}
				
				if(pEnableOrientation){
					const decVector lockRotation(boneState.GetOrientation().GetEulerAngles());
					decVector rotation(m.GetEulerAngles());
					
					if(pLockX){
						rotation.x = lockRotation.x;
					}
					if(pLockY){
						rotation.y = lockRotation.y;
					}
					if(pLockZ){
						rotation.z = lockRotation.z;
					}
					
					modifyOrientation = decQuaternion::CreateFromEuler(rotation);
				}
				
				if(pEnableSize){
					const decVector &lockScale = boneState.GetScale();
					modifyScale = m.GetScale();
					
					if(pLockX){
						modifyScale.x = lockScale.x;
					}
					if(pLockY){
						modifyScale.y = lockScale.y;
					}
					if(pLockZ){
						modifyScale.z = lockScale.z;
					}
				}
				
				boneState.BlendWith(modifyPosition, modifyOrientation, modifyScale,
					blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
			}
			break;
		}
	}
	
	for(i=0; i<vpsCount; i++){
		const int animatorVps = GetVPSMappingFor(i);
		if(animatorVps == -1){
			continue;
		}
		vpsstalist.GetStateAt(animatorVps).BlendWith(weight, blendMode, blendFactor, pEnableVPS);
	}
DEBUG_PRINT_TIMER;
}



void dearRuleForeignState::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateForeignBone();
}



// Private Functions
//////////////////////

void dearRuleForeignState::pUpdateForeignBone(){
	pForeignBone = GetInstance().GetBoneStateList().IndexOfStateNamed(pForeignState.GetForeignBone());
	pForeignVPS = GetInstance().GetVPSStateList().IndexOfStateNamed(pForeignState.GetForeignVertexPositionSet());
}
