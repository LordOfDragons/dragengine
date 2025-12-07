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

#include "dearRuleTrackTo.h"
#include "../dearAnimatorInstance.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../component/dearComponent.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/component/deComponent.h>



// Class dearRuleTrackTo
//////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat("Rule Track To = %iys", (int)(timer.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleTrackTo::dearRuleTrackTo(dearAnimatorInstance &instance, const dearAnimator &animator,
	int firstLink, const deAnimatorRuleTrackTo &rule) :
dearRule(instance, animator, firstLink, rule),
pTrackTo(rule),

pTrackBone(-1),

pTargetPosition(rule.GetTargetPosition(), firstLink),
pTargetUp(rule.GetTargetUp(), firstLink),

pLockedAxis(rule.GetLockedAxis()),
pUpTarget(rule.GetUpTarget()),
pTrackAxis(rule.GetTrackAxis()),
pUpAxis(rule.GetUpAxis())
{
	RuleChanged();
}

dearRuleTrackTo::~dearRuleTrackTo(){
}



// Management
///////////////

void dearRuleTrackTo::Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist){
DEBUG_RESET_TIMERS;
	if(!GetEnabled()){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if(blendFactor < FLOAT_SAFE_EPSILON){
		return;
	}
	
	const dearAnimatorInstance &instance = GetInstance();
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const int boneCount = GetBoneMappingCount();
	int i;
	
	// controller affected values
	decVector position;
	pTargetPosition.GetVector(instance, position);
	
	decVector up;
	pTargetUp.GetVector(instance, up);
	
	
	// determine the position to track to
	decVector trackPosition;
	
	if(pTrackBone == -1){
		trackPosition = position;
		
	}else{
		dearBoneState &bstate = *stalist.GetStateAt(pTrackBone);
		bstate.UpdateMatrices();
		trackPosition = bstate.GetGlobalMatrix().GetPosition();
	}
	
	// if the track axis is a negative axis this is the same as using the respective positive axis
	// but using the negated track vector. removes a bunch of lines of code
	deAnimatorRuleTrackTo::eTrackAxis trackAxis = pTrackAxis;
	bool negativeTrackAxis = false;
	
	if(trackAxis == deAnimatorRuleTrackTo::etaNegX){
		trackAxis = deAnimatorRuleTrackTo::etaPosX;
		negativeTrackAxis = true;
		
	}else if(trackAxis == deAnimatorRuleTrackTo::etaNegY){
		trackAxis = deAnimatorRuleTrackTo::etaPosY;
		negativeTrackAxis = true;
		
	}else if(trackAxis == deAnimatorRuleTrackTo::etaNegZ){
		trackAxis = deAnimatorRuleTrackTo::etaPosZ;
		negativeTrackAxis = true;
	}
	
	// determine the up vector to align the bone rotations to
	deAnimatorRuleTrackTo::eTrackAxis upAxis = pUpAxis;
	decVector alignUpVector;
	
	if(pLockedAxis == deAnimatorRuleTrackTo::elaNone){
		switch(pUpTarget){
		case deAnimatorRuleTrackTo::eutWorldX:
			if(GetInstance().GetComponent()){
				alignUpVector = GetInstance().GetComponent()->GetMatrix().TransformRight();
				
			}else{
				alignUpVector.Set(1.0f, 0.0f, 0.0f);
			}
			break;
			
		case deAnimatorRuleTrackTo::eutWorldY:
			if(GetInstance().GetComponent()){
				alignUpVector = GetInstance().GetComponent()->GetMatrix().TransformUp();
				
			}else{
				alignUpVector.Set(0.0f, 1.0f, 0.0f);
			}
			break;
			
		case deAnimatorRuleTrackTo::eutWorldZ:
			if(GetInstance().GetComponent()){
				alignUpVector = GetInstance().GetComponent()->GetMatrix().TransformView();
				
			}else{
				alignUpVector.Set(0.0f, 0.0f, 1.0f);
			}
			break;
			
		case deAnimatorRuleTrackTo::eutComponentX:
			alignUpVector.Set(1.0f, 0.0f, 0.0f);
			break;
			
		case deAnimatorRuleTrackTo::eutComponentY:
			alignUpVector.Set(0.0f, 1.0f, 0.0f);
			break;
			
		case deAnimatorRuleTrackTo::eutComponentZ:
			alignUpVector.Set(0.0f, 0.0f, 1.0f);
			break;
			
		case deAnimatorRuleTrackTo::eutTrackBoneX:
			if(pTrackBone == -1){
				alignUpVector.Set(1.0f, 0.0f, 0.0f);
				
			}else{
				alignUpVector = stalist.GetStateAt(pTrackBone)->GetGlobalMatrix().TransformRight();
			}
			break;
			
		case deAnimatorRuleTrackTo::eutTrackBoneY:
			if(pTrackBone == -1){
				alignUpVector.Set(0.0f, 1.0f, 0.0f);
				
			}else{
				alignUpVector = stalist.GetStateAt(pTrackBone)->GetGlobalMatrix().TransformUp();
			}
			break;
			
		case deAnimatorRuleTrackTo::eutTrackBoneZ:
			if(pTrackBone == -1){
				alignUpVector.Set(0.0f, 0.0f, 1.0f);
				
			}else{
				alignUpVector = stalist.GetStateAt(pTrackBone)->GetGlobalMatrix().TransformView();
			}
			break;
			
		case deAnimatorRuleTrackTo::eutController:{
			alignUpVector = up;
			
			const float vlen = alignUpVector.Length();
			if(vlen < 1e-5f){
				alignUpVector.Set(0.0f, 1.0f, 0.0f);
				
			}else{
				alignUpVector /= vlen;
			}
			}break;
		}
		
		// if the up axis is a negative axis this is the same as using the respective positive axis
		// but using the negated align up vector. removes a bunch of lines of code
		if(upAxis == deAnimatorRuleTrackTo::etaNegX){
			upAxis = deAnimatorRuleTrackTo::etaPosX;
			alignUpVector = -alignUpVector;
			
		}else if(upAxis == deAnimatorRuleTrackTo::etaNegY){
			upAxis = deAnimatorRuleTrackTo::etaPosY;
			alignUpVector = -alignUpVector;
			
		}else if(upAxis == deAnimatorRuleTrackTo::etaNegZ){
			upAxis = deAnimatorRuleTrackTo::etaPosZ;
			alignUpVector = -alignUpVector;
		}
	}
	
	// rotate bones
	for(i=0; i<boneCount; i++){
		const int boneIndex = GetBoneMappingFor(i);
		if(boneIndex == -1){
			continue;
		}
		
		dearBoneState &bstate = *stalist.GetStateAt(boneIndex);
		
		bstate.UpdateMatrices();
		const decVector bonePosition(bstate.GetGlobalMatrix().GetPosition());
		
		// determine the track normal
		decVector axisTrack(trackPosition - bonePosition);
		const float vlen = axisTrack.Length();
		if(vlen < 1e-5f){
			continue; // track position equals to bone position, no tracking possible
		}
		axisTrack /= vlen;
		
		if(negativeTrackAxis){
			axisTrack = -axisTrack;
		}
		
		// calculate the new bone coordinate system
		decVector axisX;
		decVector axisY;
		decVector axisZ;
		
		if(pLockedAxis == deAnimatorRuleTrackTo::elaNone){
			if(trackAxis == deAnimatorRuleTrackTo::etaPosX){
				axisX = axisTrack;
				
				if(upAxis == deAnimatorRuleTrackTo::etaPosX){
					continue; // no valid tracking configuration
					
				}else if(upAxis == deAnimatorRuleTrackTo::etaPosY){
					axisZ = axisX % alignUpVector;
					const float vlen2 = axisZ.Length();
					if(vlen2 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisZ /= vlen2;
					
					axisY = axisZ % axisX;
					const float vlen3 = axisY.Length();
					if(vlen3 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisY /= vlen3;
					
				}else if(upAxis == deAnimatorRuleTrackTo::etaPosZ){
					axisY = alignUpVector % axisX;
					const float vlen2 = axisY.Length();
					if(vlen2 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisY /= vlen2;
					
					axisZ = axisX % axisY;
					const float vlen3 = axisZ.Length();
					if(vlen3 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisZ /= vlen3;
				}
				
			}else if(trackAxis == deAnimatorRuleTrackTo::etaPosY){
				axisY = axisTrack;
				
				if(upAxis == deAnimatorRuleTrackTo::etaPosX){
					axisZ = alignUpVector % axisY;
					const float vlen2 = axisZ.Length();
					if(vlen2 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisZ /= vlen2;
					
					axisX = axisY % axisZ;
					const float vlen3 = axisX.Length();
					if(vlen3 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisX /= vlen3;
					
				}else if(upAxis == deAnimatorRuleTrackTo::etaPosY){
					continue; // no valid tracking configuration
					
				}else if(upAxis == deAnimatorRuleTrackTo::etaPosZ){
					axisX = axisY % alignUpVector;
					const float vlen2 = axisX.Length();
					if(vlen2 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisX /= vlen2;
					
					axisZ = axisX % axisY;
					const float vlen3 = axisZ.Length();
					if(vlen3 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisZ /= vlen3;
				}
				
			}else if(trackAxis == deAnimatorRuleTrackTo::etaPosZ){
				axisZ = axisTrack;
				
				if(upAxis == deAnimatorRuleTrackTo::etaPosX){
					axisY = axisZ % alignUpVector;
					const float vlen2 = axisY.Length();
					if(vlen2 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisY /= vlen2;
					
					axisX = axisY % axisZ;
					const float vlen3 = axisX.Length();
					if(vlen3 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisX /= vlen3;
					
				}else if(upAxis == deAnimatorRuleTrackTo::etaPosY){
					axisX = alignUpVector % axisZ;
					const float vlen2 = axisX.Length();
					if(vlen2 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisX /= vlen2;
					
					axisY = axisZ % axisX;
					const float vlen3 = axisY.Length();
					if(vlen3 < 1e-5f){
						continue; // no valid tracking configuration
					}
					axisY /= vlen3;
					
				}else if(upAxis == deAnimatorRuleTrackTo::etaPosZ){
					continue; // no valid tracking configuration
				}
			}
			
		}else if(pLockedAxis == deAnimatorRuleTrackTo::elaX){
			axisX = bstate.GetGlobalMatrix().TransformRight();
			
			if(trackAxis == deAnimatorRuleTrackTo::etaPosY){
				axisZ = axisX % axisTrack;
				const float vlen2 = axisZ.Length();
				if(vlen2 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisZ /= vlen2;
				
				axisY = axisZ % axisX;
				const float vlen3 = axisY.Length();
				if(vlen3 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisY /= vlen3;
				
			}else if(trackAxis == deAnimatorRuleTrackTo::etaPosZ){
				axisY = axisTrack % axisX;
				const float vlen2 = axisY.Length();
				if(vlen2 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisY /= vlen2;
				
				axisZ = axisX % axisY;
				const float vlen3 = axisZ.Length();
				if(vlen3 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisZ /= vlen3;
				
			}else{
				continue; // no valid tracking configuration
			}
			
		}else if(pLockedAxis == deAnimatorRuleTrackTo::elaY){
			axisY = bstate.GetGlobalMatrix().TransformUp();
			
			if(trackAxis == deAnimatorRuleTrackTo::etaPosX){
				axisZ = axisTrack % axisY;
				const float vlen2 = axisZ.Length();
				if(vlen2 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisZ /= vlen2;
				
				axisX = axisY % axisZ;
				const float vlen3 = axisX.Length();
				if(vlen3 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisX /= vlen3;
				
			}else if(trackAxis == deAnimatorRuleTrackTo::etaPosZ){
				axisX = axisY % axisTrack;
				const float vlen2 = axisX.Length();
				if(vlen2 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisX /= vlen2;
				
				axisZ = axisX % axisY;
				const float vlen3 = axisZ.Length();
				if(vlen3 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisZ /= vlen3;
				
			}else{
				continue; // no valid tracking configuration
			}
			
		}else if(pLockedAxis == deAnimatorRuleTrackTo::elaZ){
			axisZ = bstate.GetGlobalMatrix().TransformView();
			
			if(trackAxis == deAnimatorRuleTrackTo::etaPosX){
				axisY = axisZ % axisTrack;
				const float vlen2 = axisY.Length();
				if(vlen2 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisY /= vlen2;
				
				axisX = axisY % axisZ;
				const float vlen3 = axisX.Length();
				if(vlen3 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisX /= vlen3;
				
			}else if(trackAxis == deAnimatorRuleTrackTo::etaPosY){
				axisX = axisTrack % axisZ;
				const float vlen2 = axisX.Length();
				if(vlen2 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisX /= vlen2;
				
				axisY = axisZ % axisX;
				const float vlen3 = axisY.Length();
				if(vlen3 < 1e-5f){
					continue; // no valid tracking configuration
				}
				axisY /= vlen3;
				
			}else{
				continue; // no valid tracking configuration
			}
			
		}else{
			continue; // no valid tracking configuration
		}
		
		// calculate the bone parameters for the new coordinate system
		decMatrix globalMatrix;
		globalMatrix.a11 = axisX.x;
		globalMatrix.a21 = axisX.y;
		globalMatrix.a31 = axisX.z;
		globalMatrix.a12 = axisY.x;
		globalMatrix.a22 = axisY.y;
		globalMatrix.a32 = axisY.z;
		globalMatrix.a13 = axisZ.x;
		globalMatrix.a23 = axisZ.y;
		globalMatrix.a33 = axisZ.z;
		globalMatrix.a14 = bonePosition.x;
		globalMatrix.a24 = bonePosition.y;
		globalMatrix.a34 = bonePosition.z;
		
		if(bstate.GetParentState()){
			const decMatrix m(globalMatrix
				.QuickMultiply(bstate.GetParentState()->GetInverseGlobalMatrix())
				.QuickMultiply(bstate.GetInverseRigLocalMatrix()));
			
			bstate.BlendWith(decVector(), m.ToQuaternion(), blendMode, blendFactor, false, true);
			
		}else{
			const decMatrix m(globalMatrix.QuickMultiply(bstate.GetInverseRigLocalMatrix()));
			
			bstate.BlendWith(decVector(), m.ToQuaternion(), blendMode, blendFactor, false, true);
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleTrackTo::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateTrackBone();
}



// Private Functions
//////////////////////

void dearRuleTrackTo::pUpdateTrackBone(){
	pTrackBone = GetInstance().GetBoneStateList().IndexOfStateNamed(pTrackTo.GetTrackBone());
}
