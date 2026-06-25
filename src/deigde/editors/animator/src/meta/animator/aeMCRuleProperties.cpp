/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "aeMCRuleProperties.h"
#include "aeMCAnimatorProperties.h"

#include <deigde/meta/property/igdeMetaPropertyAdapters.h>


aeMCRuleProperties::aeMCRuleProperties(aeWindowMain &windowMain) :
	rule(deTObjectReference<aeMCPRule>::New(windowMain)){
}

void aeMCRuleProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += decTObjectOrderedSet<igdeMetaProperty>(devctag,
		name,
		blendMode,
		blendFactor,
		invertBlendFactor,
		enabled,
		affectedBones,
		affectedVertexPositionSets,
		targetBlendFactor);
	
	group->GetProperties() += decTObjectOrderedSet<igdeMetaProperty>(devctag,
		ruleTree,
		rule);
	
	igdeMetaPropertyAdapter::OnChanged(name, rules);
}


void aeMCRuleAnimationProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		moveName,
		moveTime,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet,
		targetMoveTime);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenMoveNames, moveName);
}


void aeMCRuleAnimationDifferenceProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		leadingMoveName,
		leadingMoveTime,
		referenceMoveName,
		referenceMoveTime,
		useComponentSpace,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet,
		targetLeadMoveTime,
		targetRefMoveTime);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenMoveNames, leadingMoveName);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenMoveNames, referenceMoveName);
}


void aeMCRuleAnimationSelectProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		moves,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet,
		targetMoveTime,
		targetSelect);
}


void aeMCRuleBoneTransformatorProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		minTranslation,
		maxTranslation,
		minRotation,
		maxRotation,
		minScaling,
		maxScaling,
		axis,
		minAngle,
		maxAngle,
		enablePosition,
		enableOrientation,
		enableSize,
		useAxis,
		targetBone,
		inputBone,
		coordinateFrame,
		inputSource,
		targetTranslation,
		targetRotation,
		targetScaling);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, targetBone);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, inputBone);
}


void aeMCRuleForeignStateProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		foreignBone,
		foreignVertexPositionSet,
		scalePosition,
		scaleOrientation,
		scaleSize,
		scaleVertexPositionSet,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet,
		sourceCoordinateFrame,
		destCoordinateFrame,
		targetPosition,
		targetOrientation,
		targetSize,
		targetVertexPositionSet);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, foreignBone);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenVPSNames, foreignVertexPositionSet);
}


void aeMCRuleGroupProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet,
		useCurrentState,
		applicationType,
		targetSelect);
	
	igdeMetaPropertyAdapter::OnChanged(rules, properties.rule.ruleTree);
}


void aeMCRuleInverseKinematicProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		goalPosition,
		goalOrientation,
		localPosition,
		localOrientation,
		adjustOrientation,
		useSolverBone,
		solverBone,
		reachRange,
		reachBone,
		reachCenter,
		targetGoalPosition,
		targetGoalOrientation,
		targetLocalPosition,
		targetLocalOrientation,
		targetReachRange,
		targetReachCenter);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, solverBone);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, reachBone);
}


void aeMCRuleLimitProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		minPosition,
		maxPosition,
		minRotation,
		maxRotation,
		minScaling,
		maxScaling,
		minVertexPositionSet,
		maxVertexPositionSet,
		targetBone,
		coordinateFrame,
		enablePositionXMin,
		enablePositionXMax,
		enablePositionYMin,
		enablePositionYMax,
		enablePositionZMin,
		enablePositionZMax,
		enableRotationXMin,
		enableRotationXMax,
		enableRotationYMin,
		enableRotationYMax,
		enableRotationZMin,
		enableRotationZMax,
		enableScalingXMin,
		enableScalingXMax,
		enableScalingYMin,
		enableScalingYMax,
		enableScalingZMin,
		enableScalingZMax,
		enableVertexPositionSetMin,
		enableVertexPositionSetMax);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, targetBone);
}


void aeMCRuleMirrorProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		mirrorAxis,
		mirrorBone,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, mirrorBone);
}


void aeMCRuleStateManipulatorProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		minPosition,
		maxPosition,
		minRotation,
		maxRotation,
		minSize,
		maxSize,
		minVertexPositionSet,
		maxVertexPositionSet,
		enablePosition,
		enableRotation,
		enableSize,
		enableVertexPositionSet,
		targetPosition,
		targetRotation,
		targetSize,
		targetVertexPositionSet);
}


void aeMCRuleStateSnapshotProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		useLastState,
		id,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet);
}


void aeMCRuleSubAnimatorProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		pathSubAnimator,
		enablePosition,
		enableOrientation,
		enableSize,
		enableVertexPositionSet);
}


void aeMCRuleTrackToProperties::Init(const aeMCAnimatorProperties &properties){
	metaProperties->GetData() += properties.rule.metaProperties->GetData();
	metaProperties->GetData() += igdeMetaProperty::List(devctag,
		trackBone,
		trackAxis,
		upAxis,
		upTarget,
		lockedAxis,
		targetPosition,
		targetUp);
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, trackBone);
}
