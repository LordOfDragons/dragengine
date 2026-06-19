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

#ifndef _AEMCRULEPROPERTIES_H_
#define _AEMCRULEPROPERTIES_H_

#include "aeMCPRule.h"
#include "aeMCPRuleAnimation.h"
#include "aeMCPRuleAnimationDifference.h"
#include "aeMCPRuleAnimationSelect.h"
#include "aeMCPRuleBoneTransformator.h"
#include "aeMCPRuleForeignState.h"
#include "aeMCPRuleGroup.h"
#include "aeMCPRuleInverseKinematic.h"
#include "aeMCPRuleLimit.h"
#include "aeMCPRuleMirror.h"
#include "aeMCPRuleStateManipulator.h"
#include "aeMCPRuleStateSnapshot.h"
#include "aeMCPRuleSubAnimator.h"
#include "aeMCPRuleTrackTo.h"

#include <deigde/meta/property/igdeMetaPropertyGroup.h>

class aeWindowMain;


class aeMCRuleProperties{
public:
	deTObjectReference<aeMCPRuleName> name = deTObjectReference<aeMCPRuleName>::New();
	deTObjectReference<aeMCPRuleBlendMode> blendMode = deTObjectReference<aeMCPRuleBlendMode>::New();
	deTObjectReference<aeMCPRuleBlendFactor> blendFactor = deTObjectReference<aeMCPRuleBlendFactor>::New();
	deTObjectReference<aeMCPRuleInvertBlendFactor> invertBlendFactor = deTObjectReference<aeMCPRuleInvertBlendFactor>::New();
	deTObjectReference<aeMCPRuleEnabled> enabled = deTObjectReference<aeMCPRuleEnabled>::New();
	deTObjectReference<aeMCPRuleListBones> listBones = deTObjectReference<aeMCPRuleListBones>::New();
	deTObjectReference<aeMCPRuleListVertexPositionSets> listVertexPositionSets = deTObjectReference<aeMCPRuleListVertexPositionSets>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			name,
			blendMode,
			blendFactor,
			invertBlendFactor,
			enabled,
			listBones,
			listVertexPositionSets));
	
	deTObjectReference<aeMCPRules> rules = deTObjectReference<aeMCPRules>::New();
	deTObjectReference<aeMCPRule> rule;
	deTObjectReference<igdeMetaPropertyGroup> group = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupRules", "@Animator.WPRule.Rules", "@Animator.WPRule.Rules.ToolTip",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, rules, rule));
	
	aeMCRuleProperties(aeWindowMain &windowMain) :
		rule(deTObjectReference<aeMCPRule>::New(windowMain)){}
};


class aeMCRuleAnimationProperties{
public:
	deTObjectReference<aeMCPRuleAnimationMoveName> moveName = deTObjectReference<aeMCPRuleAnimationMoveName>::New();
	deTObjectReference<aeMCPRuleAnimationMoveTime> moveTime = deTObjectReference<aeMCPRuleAnimationMoveTime>::New();
	deTObjectReference<aeMCPRuleAnimationEnablePosition> enablePosition = deTObjectReference<aeMCPRuleAnimationEnablePosition>::New();
	deTObjectReference<aeMCPRuleAnimationEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleAnimationEnableOrientation>::New();
	deTObjectReference<aeMCPRuleAnimationEnableSize> enableSize = deTObjectReference<aeMCPRuleAnimationEnableSize>::New();
	deTObjectReference<aeMCPRuleAnimationEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleAnimationEnableVertexPositionSet>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleAnimationProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			moveName,
			moveTime,
			enablePosition,
			enableOrientation,
			enableSize,
			enableVertexPositionSet);
	}
};


class aeMCRuleAnimationDifferenceProperties{
public:
	deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveName> leadingMoveName = deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveName>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveTime> leadingMoveTime = deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveTime>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveName> referenceMoveName = deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveName>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveTime> referenceMoveTime = deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveTime>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceUseSameMove> useSameMove = deTObjectReference<aeMCPRuleAnimationDifferenceUseSameMove>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceUseComponentSpace> useComponentSpace = deTObjectReference<aeMCPRuleAnimationDifferenceUseComponentSpace>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceEnablePosition> enablePosition = deTObjectReference<aeMCPRuleAnimationDifferenceEnablePosition>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleAnimationDifferenceEnableOrientation>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceEnableSize> enableSize = deTObjectReference<aeMCPRuleAnimationDifferenceEnableSize>::New();
	deTObjectReference<aeMCPRuleAnimationDifferenceEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleAnimationDifferenceEnableVertexPositionSet>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleAnimationDifferenceProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			leadingMoveName,
			leadingMoveTime,
			referenceMoveName,
			referenceMoveTime,
			useSameMove,
			useComponentSpace,
			enablePosition,
			enableOrientation,
			enableSize,
			enableVertexPositionSet);
	}
};


class aeMCRuleAnimationSelectProperties{
public:
	deTObjectReference<aeMCPRuleAnimationSelectMoves> moves = deTObjectReference<aeMCPRuleAnimationSelectMoves>::New();
	deTObjectReference<aeMCPRuleAnimationSelectEnablePosition> enablePosition = deTObjectReference<aeMCPRuleAnimationSelectEnablePosition>::New();
	deTObjectReference<aeMCPRuleAnimationSelectEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleAnimationSelectEnableOrientation>::New();
	deTObjectReference<aeMCPRuleAnimationSelectEnableSize> enableSize = deTObjectReference<aeMCPRuleAnimationSelectEnableSize>::New();
	deTObjectReference<aeMCPRuleAnimationSelectEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleAnimationSelectEnableVertexPositionSet>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleAnimationSelectProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			moves,
			enablePosition,
			enableOrientation,
			enableSize,
			enableVertexPositionSet);
	}
};


class aeMCRuleBoneTransformatorProperties{
public:
	deTObjectReference<aeMCPRuleBoneTransformatorMinTranslation> minTranslation = deTObjectReference<aeMCPRuleBoneTransformatorMinTranslation>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorMaxTranslation> maxTranslation = deTObjectReference<aeMCPRuleBoneTransformatorMaxTranslation>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorMinRotation> minRotation = deTObjectReference<aeMCPRuleBoneTransformatorMinRotation>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorMaxRotation> maxRotation = deTObjectReference<aeMCPRuleBoneTransformatorMaxRotation>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorMinScaling> minScaling = deTObjectReference<aeMCPRuleBoneTransformatorMinScaling>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorMaxScaling> maxScaling = deTObjectReference<aeMCPRuleBoneTransformatorMaxScaling>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorAxis> axis = deTObjectReference<aeMCPRuleBoneTransformatorAxis>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorMinAngle> minAngle = deTObjectReference<aeMCPRuleBoneTransformatorMinAngle>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorMaxAngle> maxAngle = deTObjectReference<aeMCPRuleBoneTransformatorMaxAngle>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorEnablePosition> enablePosition = deTObjectReference<aeMCPRuleBoneTransformatorEnablePosition>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleBoneTransformatorEnableOrientation>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorEnableSize> enableSize = deTObjectReference<aeMCPRuleBoneTransformatorEnableSize>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorUseAxis> useAxis = deTObjectReference<aeMCPRuleBoneTransformatorUseAxis>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorTargetBone> targetBone = deTObjectReference<aeMCPRuleBoneTransformatorTargetBone>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorInputBone> inputBone = deTObjectReference<aeMCPRuleBoneTransformatorInputBone>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorCoordinateFrame> coordinateFrame = deTObjectReference<aeMCPRuleBoneTransformatorCoordinateFrame>::New();
	deTObjectReference<aeMCPRuleBoneTransformatorInputSource> inputSource = deTObjectReference<aeMCPRuleBoneTransformatorInputSource>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleBoneTransformatorProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
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
			inputSource);
	}
};


class aeMCRuleForeignStateProperties{
public:
	deTObjectReference<aeMCPRuleForeignStateForeignBone> foreignBone = deTObjectReference<aeMCPRuleForeignStateForeignBone>::New();
	deTObjectReference<aeMCPRuleForeignStateForeignVertexPositionSet> foreignVertexPositionSet = deTObjectReference<aeMCPRuleForeignStateForeignVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleForeignStateScalePosition> scalePosition = deTObjectReference<aeMCPRuleForeignStateScalePosition>::New();
	deTObjectReference<aeMCPRuleForeignStateScaleOrientation> scaleOrientation = deTObjectReference<aeMCPRuleForeignStateScaleOrientation>::New();
	deTObjectReference<aeMCPRuleForeignStateScaleSize> scaleSize = deTObjectReference<aeMCPRuleForeignStateScaleSize>::New();
	deTObjectReference<aeMCPRuleForeignStateScaleVertexPositionSet> scaleVertexPositionSet = deTObjectReference<aeMCPRuleForeignStateScaleVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleForeignStateEnablePosition> enablePosition = deTObjectReference<aeMCPRuleForeignStateEnablePosition>::New();
	deTObjectReference<aeMCPRuleForeignStateEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleForeignStateEnableOrientation>::New();
	deTObjectReference<aeMCPRuleForeignStateEnableSize> enableSize = deTObjectReference<aeMCPRuleForeignStateEnableSize>::New();
	deTObjectReference<aeMCPRuleForeignStateEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleForeignStateEnableVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleForeignStateSourceCoordinateFrame> sourceCoordinateFrame = deTObjectReference<aeMCPRuleForeignStateSourceCoordinateFrame>::New();
	deTObjectReference<aeMCPRuleForeignStateDestCoordinateFrame> destCoordinateFrame = deTObjectReference<aeMCPRuleForeignStateDestCoordinateFrame>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleForeignStateProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
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
			destCoordinateFrame);
	}
};


class aeMCRuleGroupProperties{
public:
	deTObjectReference<aeMCPRuleGroupEnablePosition> enablePosition = deTObjectReference<aeMCPRuleGroupEnablePosition>::New();
	deTObjectReference<aeMCPRuleGroupEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleGroupEnableOrientation>::New();
	deTObjectReference<aeMCPRuleGroupEnableSize> enableSize = deTObjectReference<aeMCPRuleGroupEnableSize>::New();
	deTObjectReference<aeMCPRuleGroupEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleGroupEnableVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleGroupUseCurrentState> useCurrentState = deTObjectReference<aeMCPRuleGroupUseCurrentState>::New();
	deTObjectReference<aeMCPRuleGroupApplicationType> applicationType = deTObjectReference<aeMCPRuleGroupApplicationType>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleGroupProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			enablePosition,
			enableOrientation,
			enableSize,
			enableVertexPositionSet,
			useCurrentState,
			applicationType);
	}
};


class aeMCRuleInverseKinematicProperties{
public:
	deTObjectReference<aeMCPRuleInverseKinematicGoalPosition> goalPosition = deTObjectReference<aeMCPRuleInverseKinematicGoalPosition>::New();
	deTObjectReference<aeMCPRuleInverseKinematicGoalOrientation> goalOrientation = deTObjectReference<aeMCPRuleInverseKinematicGoalOrientation>::New();
	deTObjectReference<aeMCPRuleInverseKinematicLocalPosition> localPosition = deTObjectReference<aeMCPRuleInverseKinematicLocalPosition>::New();
	deTObjectReference<aeMCPRuleInverseKinematicLocalOrientation> localOrientation = deTObjectReference<aeMCPRuleInverseKinematicLocalOrientation>::New();
	deTObjectReference<aeMCPRuleInverseKinematicAdjustOrientation> adjustOrientation = deTObjectReference<aeMCPRuleInverseKinematicAdjustOrientation>::New();
	deTObjectReference<aeMCPRuleInverseKinematicUseSolverBone> useSolverBone = deTObjectReference<aeMCPRuleInverseKinematicUseSolverBone>::New();
	deTObjectReference<aeMCPRuleInverseKinematicSolverBone> solverBone = deTObjectReference<aeMCPRuleInverseKinematicSolverBone>::New();
	deTObjectReference<aeMCPRuleInverseKinematicReachRange> reachRange = deTObjectReference<aeMCPRuleInverseKinematicReachRange>::New();
	deTObjectReference<aeMCPRuleInverseKinematicReachBone> reachBone = deTObjectReference<aeMCPRuleInverseKinematicReachBone>::New();
	deTObjectReference<aeMCPRuleInverseKinematicReachCenter> reachCenter = deTObjectReference<aeMCPRuleInverseKinematicReachCenter>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleInverseKinematicProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
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
			reachCenter);
	}
};


class aeMCRuleLimitProperties{
public:
	deTObjectReference<aeMCPRuleLimitMinPosition> minPosition = deTObjectReference<aeMCPRuleLimitMinPosition>::New();
	deTObjectReference<aeMCPRuleLimitMaxPosition> maxPosition = deTObjectReference<aeMCPRuleLimitMaxPosition>::New();
	deTObjectReference<aeMCPRuleLimitMinRotation> minRotation = deTObjectReference<aeMCPRuleLimitMinRotation>::New();
	deTObjectReference<aeMCPRuleLimitMaxRotation> maxRotation = deTObjectReference<aeMCPRuleLimitMaxRotation>::New();
	deTObjectReference<aeMCPRuleLimitMinScaling> minScaling = deTObjectReference<aeMCPRuleLimitMinScaling>::New();
	deTObjectReference<aeMCPRuleLimitMaxScaling> maxScaling = deTObjectReference<aeMCPRuleLimitMaxScaling>::New();
	deTObjectReference<aeMCPRuleLimitMinVertexPositionSet> minVertexPositionSet = deTObjectReference<aeMCPRuleLimitMinVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleLimitMaxVertexPositionSet> maxVertexPositionSet = deTObjectReference<aeMCPRuleLimitMaxVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleLimitTargetBone> targetBone = deTObjectReference<aeMCPRuleLimitTargetBone>::New();
	deTObjectReference<aeMCPRuleLimitCoordinateFrame> coordinateFrame = deTObjectReference<aeMCPRuleLimitCoordinateFrame>::New();
	deTObjectReference<aeMCPRuleLimitEnablePositionXMin> enablePositionXMin = deTObjectReference<aeMCPRuleLimitEnablePositionXMin>::New();
	deTObjectReference<aeMCPRuleLimitEnablePositionXMax> enablePositionXMax = deTObjectReference<aeMCPRuleLimitEnablePositionXMax>::New();
	deTObjectReference<aeMCPRuleLimitEnablePositionYMin> enablePositionYMin = deTObjectReference<aeMCPRuleLimitEnablePositionYMin>::New();
	deTObjectReference<aeMCPRuleLimitEnablePositionYMax> enablePositionYMax = deTObjectReference<aeMCPRuleLimitEnablePositionYMax>::New();
	deTObjectReference<aeMCPRuleLimitEnablePositionZMin> enablePositionZMin = deTObjectReference<aeMCPRuleLimitEnablePositionZMin>::New();
	deTObjectReference<aeMCPRuleLimitEnablePositionZMax> enablePositionZMax = deTObjectReference<aeMCPRuleLimitEnablePositionZMax>::New();
	deTObjectReference<aeMCPRuleLimitEnableRotationXMin> enableRotationXMin = deTObjectReference<aeMCPRuleLimitEnableRotationXMin>::New();
	deTObjectReference<aeMCPRuleLimitEnableRotationXMax> enableRotationXMax = deTObjectReference<aeMCPRuleLimitEnableRotationXMax>::New();
	deTObjectReference<aeMCPRuleLimitEnableRotationYMin> enableRotationYMin = deTObjectReference<aeMCPRuleLimitEnableRotationYMin>::New();
	deTObjectReference<aeMCPRuleLimitEnableRotationYMax> enableRotationYMax = deTObjectReference<aeMCPRuleLimitEnableRotationYMax>::New();
	deTObjectReference<aeMCPRuleLimitEnableRotationZMin> enableRotationZMin = deTObjectReference<aeMCPRuleLimitEnableRotationZMin>::New();
	deTObjectReference<aeMCPRuleLimitEnableRotationZMax> enableRotationZMax = deTObjectReference<aeMCPRuleLimitEnableRotationZMax>::New();
	deTObjectReference<aeMCPRuleLimitEnableScalingXMin> enableScalingXMin = deTObjectReference<aeMCPRuleLimitEnableScalingXMin>::New();
	deTObjectReference<aeMCPRuleLimitEnableScalingXMax> enableScalingXMax = deTObjectReference<aeMCPRuleLimitEnableScalingXMax>::New();
	deTObjectReference<aeMCPRuleLimitEnableScalingYMin> enableScalingYMin = deTObjectReference<aeMCPRuleLimitEnableScalingYMin>::New();
	deTObjectReference<aeMCPRuleLimitEnableScalingYMax> enableScalingYMax = deTObjectReference<aeMCPRuleLimitEnableScalingYMax>::New();
	deTObjectReference<aeMCPRuleLimitEnableScalingZMin> enableScalingZMin = deTObjectReference<aeMCPRuleLimitEnableScalingZMin>::New();
	deTObjectReference<aeMCPRuleLimitEnableScalingZMax> enableScalingZMax = deTObjectReference<aeMCPRuleLimitEnableScalingZMax>::New();
	deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMin> enableVertexPositionSetMin = deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMin>::New();
	deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMax> enableVertexPositionSetMax = deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMax>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleLimitProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
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
	}
};


class aeMCRuleMirrorProperties{
public:
	deTObjectReference<aeMCPRuleMirrorMirrorAxis> mirrorAxis = deTObjectReference<aeMCPRuleMirrorMirrorAxis>::New();
	deTObjectReference<aeMCPRuleMirrorMirrorBone> mirrorBone = deTObjectReference<aeMCPRuleMirrorMirrorBone>::New();
	deTObjectReference<aeMCPRuleMirrorEnablePosition> enablePosition = deTObjectReference<aeMCPRuleMirrorEnablePosition>::New();
	deTObjectReference<aeMCPRuleMirrorEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleMirrorEnableOrientation>::New();
	deTObjectReference<aeMCPRuleMirrorEnableSize> enableSize = deTObjectReference<aeMCPRuleMirrorEnableSize>::New();
	deTObjectReference<aeMCPRuleMirrorEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleMirrorEnableVertexPositionSet>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleMirrorProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			mirrorAxis,
			mirrorBone,
			enablePosition,
			enableOrientation,
			enableSize,
			enableVertexPositionSet);
	}
};


class aeMCRuleStateManipulatorProperties{
public:
	deTObjectReference<aeMCPRuleStateManipulatorMinPosition> minPosition = deTObjectReference<aeMCPRuleStateManipulatorMinPosition>::New();
	deTObjectReference<aeMCPRuleStateManipulatorMaxPosition> maxPosition = deTObjectReference<aeMCPRuleStateManipulatorMaxPosition>::New();
	deTObjectReference<aeMCPRuleStateManipulatorMinRotation> minRotation = deTObjectReference<aeMCPRuleStateManipulatorMinRotation>::New();
	deTObjectReference<aeMCPRuleStateManipulatorMaxRotation> maxRotation = deTObjectReference<aeMCPRuleStateManipulatorMaxRotation>::New();
	deTObjectReference<aeMCPRuleStateManipulatorMinSize> minSize = deTObjectReference<aeMCPRuleStateManipulatorMinSize>::New();
	deTObjectReference<aeMCPRuleStateManipulatorMaxSize> maxSize = deTObjectReference<aeMCPRuleStateManipulatorMaxSize>::New();
	deTObjectReference<aeMCPRuleStateManipulatorMinVertexPositionSet> minVertexPositionSet = deTObjectReference<aeMCPRuleStateManipulatorMinVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleStateManipulatorMaxVertexPositionSet> maxVertexPositionSet = deTObjectReference<aeMCPRuleStateManipulatorMaxVertexPositionSet>::New();
	deTObjectReference<aeMCPRuleStateManipulatorEnablePosition> enablePosition = deTObjectReference<aeMCPRuleStateManipulatorEnablePosition>::New();
	deTObjectReference<aeMCPRuleStateManipulatorEnableRotation> enableRotation = deTObjectReference<aeMCPRuleStateManipulatorEnableRotation>::New();
	deTObjectReference<aeMCPRuleStateManipulatorEnableSize> enableSize = deTObjectReference<aeMCPRuleStateManipulatorEnableSize>::New();
	deTObjectReference<aeMCPRuleStateManipulatorEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleStateManipulatorEnableVertexPositionSet>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleStateManipulatorProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
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
			enableVertexPositionSet);
	}
};


class aeMCRuleStateSnapshotProperties{
public:
	deTObjectReference<aeMCPRuleStateSnapshotUseLastState> useLastState = deTObjectReference<aeMCPRuleStateSnapshotUseLastState>::New();
	deTObjectReference<aeMCPRuleStateSnapshotID> id = deTObjectReference<aeMCPRuleStateSnapshotID>::New();
	deTObjectReference<aeMCPRuleStateSnapshotEnablePosition> enablePosition = deTObjectReference<aeMCPRuleStateSnapshotEnablePosition>::New();
	deTObjectReference<aeMCPRuleStateSnapshotEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleStateSnapshotEnableOrientation>::New();
	deTObjectReference<aeMCPRuleStateSnapshotEnableSize> enableSize = deTObjectReference<aeMCPRuleStateSnapshotEnableSize>::New();
	deTObjectReference<aeMCPRuleStateSnapshotEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleStateSnapshotEnableVertexPositionSet>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleStateSnapshotProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			useLastState,
			id,
			enablePosition,
			enableOrientation,
			enableSize,
			enableVertexPositionSet);
	}
};


class aeMCRuleSubAnimatorProperties{
public:
	deTObjectReference<aeMCPRuleSubAnimatorPathSubAnimator> pathSubAnimator = deTObjectReference<aeMCPRuleSubAnimatorPathSubAnimator>::New();
	deTObjectReference<aeMCPRuleSubAnimatorEnablePosition> enablePosition = deTObjectReference<aeMCPRuleSubAnimatorEnablePosition>::New();
	deTObjectReference<aeMCPRuleSubAnimatorEnableOrientation> enableOrientation = deTObjectReference<aeMCPRuleSubAnimatorEnableOrientation>::New();
	deTObjectReference<aeMCPRuleSubAnimatorEnableSize> enableSize = deTObjectReference<aeMCPRuleSubAnimatorEnableSize>::New();
	deTObjectReference<aeMCPRuleSubAnimatorEnableVertexPositionSet> enableVertexPositionSet = deTObjectReference<aeMCPRuleSubAnimatorEnableVertexPositionSet>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCRuleSubAnimatorProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			pathSubAnimator,
			enablePosition,
			enableOrientation,
			enableSize,
			enableVertexPositionSet);
	}
};


class aeMCRuleTrackToProperties{
public:
	deTObjectReference<aeMCPRuleTrackToTrackBone> trackBone = deTObjectReference<aeMCPRuleTrackToTrackBone>::New();
	deTObjectReference<aeMCPRuleTrackToTrackAxis> trackAxis = deTObjectReference<aeMCPRuleTrackToTrackAxis>::New();
	deTObjectReference<aeMCPRuleTrackToUpAxis> upAxis = deTObjectReference<aeMCPRuleTrackToUpAxis>::New();
	deTObjectReference<aeMCPRuleTrackToUpTarget> upTarget = deTObjectReference<aeMCPRuleTrackToUpTarget>::New();
	deTObjectReference<aeMCPRuleTrackToLockedAxis> lockedAxis = deTObjectReference<aeMCPRuleTrackToLockedAxis>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	aeMCRuleTrackToProperties(const aeMCRuleProperties &ruleProperties){
		metaProperties->GetData() += ruleProperties.metaProperties->GetData();
		metaProperties->GetData() += igdeMetaProperty::List(devctag,
			trackBone,
			trackAxis,
			upAxis,
			upTarget,
			lockedAxis);
	}
};

#endif
