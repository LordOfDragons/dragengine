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

#include "dearRuleBoneTransformator.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
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
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat("Rule Bone Transformator = %iys", (int)(timer.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif


// Constructors and Destructors
/////////////////////////////////

dearRuleBoneTransformator::dearRuleBoneTransformator(dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleBoneTransformator &rule) :
dearRule(instance, animator, firstLink, rule),

pBoneTransformator(rule),

pTargetBone(-1),
pInputBone(-1),

pTargetTranslation(rule.GetTargetTranslation(), firstLink),
pTargetRotation(rule.GetTargetRotation(), firstLink),
pTargetScaling(rule.GetTargetScaling(), firstLink),

pCoordinateFrame(rule.GetCoordinateFrame()),
pEnablePosition(rule.GetEnablePosition()),
pEnableOrientation(rule.GetEnableOrientation()),
pEnableSize(rule.GetEnableSize()),
pUseAxis(rule.GetUseAxis()),

pMinTranslation(rule.GetMinimumTranslation()),
pMaxTranslation(rule.GetMaximumTranslation()),
pMinRotation(rule.GetMinimumRotation()),
pMaxRotation(rule.GetMaximumRotation()),
pMinScaling(rule.GetMinimumScaling()),
pMaxScaling(rule.GetMaximumScaling()),
pAxis(rule.GetAxis()),
pMinAngle(rule.GetMinimumAngle()),
pMaxAngle(rule.GetMaximumAngle()),
pInputSource(rule.GetInputSource())
{
	RuleChanged();
}

dearRuleBoneTransformator::~dearRuleBoneTransformator(){
}



// Management
///////////////

void dearRuleBoneTransformator::Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist){
DEBUG_RESET_TIMERS;
	if(! GetEnabled()){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if(blendFactor < FLOAT_SAFE_EPSILON){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const dearAnimatorInstance &instance = GetInstance();
	const int boneCount = GetBoneMappingCount();
	int i;
	
	// prepare transformation matrix
	
	decVector translation, rotation, scaling;
	decMatrix transformMatrix;
	
	switch(pInputSource){
	case deAnimatorRuleBoneTransformator::eisTargetBlend:{
		const float valueTranslation = decMath::clamp(pTargetTranslation.GetValue(instance, 0.0f), 0.0f, 1.0f);
		const float valueRotation = decMath::clamp(pTargetRotation.GetValue(instance, 0.0f), 0.0f, 1.0f);
		const float valueScaling = decMath::clamp(pTargetScaling.GetValue(instance, 0.0f), 0.0f, 1.0f);
		
		translation = pMinTranslation * (1.0f - valueTranslation) + pMaxTranslation * valueTranslation;
		scaling = pMinScaling * (1.0f - valueScaling) + pMaxScaling * valueScaling;
		
		if(pUseAxis){
			const float angle = pMinAngle * (1.0f - valueRotation) + pMaxAngle * valueRotation;
			
			transformMatrix = decMatrix::CreateScale(scaling).
				QuickMultiply(decMatrix::CreateRotationAxis(pAxis, angle)).
				QuickMultiply(decMatrix::CreateTranslation(translation));
			
		}else{
			rotation = pMinRotation * (1.0f - valueRotation) + pMaxRotation * valueRotation;
			
			transformMatrix.SetSRT(scaling, rotation, translation);
		}
		}break;
		
	case deAnimatorRuleBoneTransformator::eisTargetDirect:
		pTargetTranslation.GetVector(instance, translation);
		scaling.Set(1.0f, 1.0f, 1.0f);
		pTargetScaling.GetVector(instance, scaling);
		
		if(pUseAxis){
			const float valueRotation = decMath::clamp(pTargetRotation.GetValue(instance, 0.0f), 0.0f, 1.0f);
			const float angle = pMinAngle * (1.0f - valueRotation) + pMaxAngle * valueRotation;
			
			transformMatrix = decMatrix::CreateScale(scaling).
				QuickMultiply(decMatrix::CreateRotationAxis(pAxis, angle)).
				QuickMultiply(decMatrix::CreateTranslation(translation));
			
		}else{
			pTargetRotation.GetVector(instance, rotation);
			
			transformMatrix.SetSRT(scaling, rotation, translation);
		}
		break;
		
	case deAnimatorRuleBoneTransformator::eisBoneState:{
		if(pInputBone == -1){
			break;
		}
		
		const dearBoneState &bstate = *stalist.GetStateAt(pInputBone);
		
		if(pUseAxis){
			const float valueRotation = decMath::clamp(pTargetRotation.GetValue(instance, 0.0f), 0.0f, 1.0f);
			const float angle = pMinAngle * (1.0f - valueRotation) + pMaxAngle * valueRotation;
			
			transformMatrix = decMatrix::CreateScale(bstate.GetScale()).
				QuickMultiply(decMatrix::CreateRotationAxis(pAxis, angle)).
				QuickMultiply(decMatrix::CreateTranslation(bstate.GetPosition()));
			
		}else{
			transformMatrix.SetWorld(bstate.GetPosition(), bstate.GetOrientation(), bstate.GetScale());
		}
		}break;
		
	case deAnimatorRuleBoneTransformator::eisBoneStateInverse:{
		if(pInputBone == -1){
			break;
		}
		
		const dearBoneState &bstate = *stalist.GetStateAt(pInputBone);
		
		transformMatrix = decMatrix::CreateWorld(bstate.GetPosition(),
			bstate.GetOrientation(), bstate.GetScale()).QuickInvert();
		
		if(pUseAxis){
			const float valueRotation = decMath::clamp(pTargetRotation.GetValue(instance, 0.0f), 0.0f, 1.0f);
			const float angle = pMinAngle * (1.0f - valueRotation) + pMaxAngle * valueRotation;
			
			transformMatrix = decMatrix::CreateScale(transformMatrix.GetScale()).
				QuickMultiply(decMatrix::CreateRotationAxis(pAxis, angle)).
				QuickMultiply(decMatrix::CreateTranslation(transformMatrix.GetPosition()));
		}
		}break;
	}
	
	if(pCoordinateFrame == deAnimatorRuleBoneTransformator::ecfTargetBone && pTargetBone != -1){
		dearBoneState &bstate = *stalist.GetStateAt(pTargetBone);
		bstate.UpdateMatrices();
		transformMatrix = bstate.GetInverseGlobalMatrix().QuickMultiply(transformMatrix)
			.QuickMultiply(bstate.GetGlobalMatrix());
	}
	
	// step through all bones and apply transformation
	for(i=0; i<boneCount; i++){
		const int animatorBone = GetBoneMappingFor(i);
		if(animatorBone == -1){
			continue;
		}
		
		dearBoneState &bstate = *stalist.GetStateAt(animatorBone);
		bstate.UpdateMatrices();
		
		if(pCoordinateFrame == deAnimatorRuleBoneTransformator::ecfBoneLocal){
			const decMatrix m(bstate.GetLocalMatrix().QuickMultiply(transformMatrix));
			
			bstate.BlendWith(m.GetPosition(), m.ToQuaternion(), m.GetScale(),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
			
		}else{
			const decMatrix m(bstate.GetGlobalMatrix().QuickMultiply(transformMatrix)
				.QuickMultiply(bstate.GetInverseGlobalMatrix())
				.QuickMultiply(bstate.GetLocalMatrix()));
			
			bstate.BlendWith(m.GetPosition(), m.ToQuaternion(), m.GetScale(),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleBoneTransformator::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateTargetBone();
}



// Private Functions
//////////////////////

void dearRuleBoneTransformator::pUpdateTargetBone(){
	const dearBoneStateList &list = GetInstance().GetBoneStateList();
	pTargetBone = list.IndexOfStateNamed(pBoneTransformator.GetTargetBone());
	pInputBone = list.IndexOfStateNamed(pBoneTransformator.GetInputBone());
}
