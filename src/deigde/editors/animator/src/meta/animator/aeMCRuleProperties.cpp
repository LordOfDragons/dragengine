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


void aeMCRuleProperties::Init(aeWindowMain &windowMain){
	name = deTObjectReference<aeMCPRuleName>::New();
	blendMode = deTObjectReference<aeMCPRuleBlendMode>::New();
	blendFactor = deTObjectReference<aeMCPRuleBlendFactor>::New();
	invertBlendFactor = deTObjectReference<aeMCPRuleInvertBlendFactor>::New();
	enabled = deTObjectReference<aeMCPRuleEnabled>::New();
	affectedBones = deTObjectReference<aeMCPRuleAffectedBones>::New();
	affectedVertexPositionSets = deTObjectReference<aeMCPRuleAffectedVertexPositionSets>::New();
	targetBlendFactor = deTObjectReference<aeMCPRuleTargetBlendFactor>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			name,
			blendMode,
			blendFactor, targetBlendFactor, invertBlendFactor,
			enabled,
			affectedBones, affectedVertexPositionSets));
	
	ruleTree = deTObjectReference<aeMCPRuleTree>::New();
	rules = deTObjectReference<aeMCPRules>::New();
	rule = deTObjectReference<aeMCPRule>::New(windowMain);
	group = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupRules", "Animator.WPRule.Rules",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, ruleTree, rule));
	
	igdeMetaPropertyAdapter::OnChanged(name, rules);
	igdeMetaPropertyAdapter::OnChanged(rules, ruleTree);
}


void aeMCRuleAnimationProperties::Init(const aeMCAnimatorProperties &properties){
	moveName = deTObjectReference<aeMCPRuleAnimationMoveName>::New();
	moveTime = deTObjectReference<aeMCPRuleAnimationMoveTime>::New();
	enablePosition = deTObjectReference<aeMCPRuleAnimationEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleAnimationEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleAnimationEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleAnimationEnableVertexPositionSet>::New();
	targetMoveTime = deTObjectReference<aeMCPRuleAnimationTargetMoveTime>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			moveName,
			moveTime, targetMoveTime,
			enablePosition, enableOrientation, enableSize, enableVertexPositionSet));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenMoveNames, moveName);
}


void aeMCRuleAnimationDifferenceProperties::Init(const aeMCAnimatorProperties &properties){
	leadingMoveName = deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveName>::New();
	leadingMoveTime = deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveTime>::New();
	referenceMoveName = deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveName>::New();
	referenceMoveTime = deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveTime>::New();
	useComponentSpace = deTObjectReference<aeMCPRuleAnimationDifferenceUseComponentSpace>::New();
	enablePosition = deTObjectReference<aeMCPRuleAnimationDifferenceEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleAnimationDifferenceEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleAnimationDifferenceEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleAnimationDifferenceEnableVertexPositionSet>::New();
	targetLeadMoveTime = deTObjectReference<aeMCPRuleAnimationDifferenceTargetLeadMoveTime>::New();
	targetRefMoveTime = deTObjectReference<aeMCPRuleAnimationDifferenceTargetRefMoveTime>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			leadingMoveName, leadingMoveTime, targetLeadMoveTime,
			referenceMoveName, referenceMoveTime, targetRefMoveTime,
			useComponentSpace,
			enablePosition, enableOrientation, enableSize, enableVertexPositionSet));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenMoveNames, leadingMoveName);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenMoveNames, referenceMoveName);
}


void aeMCRuleAnimationSelectProperties::Init(const aeMCAnimatorProperties &properties){
	moves = deTObjectReference<aeMCPRuleAnimationSelectMoves>::New();
	enablePosition = deTObjectReference<aeMCPRuleAnimationSelectEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleAnimationSelectEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleAnimationSelectEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleAnimationSelectEnableVertexPositionSet>::New();
	targetMoveTime = deTObjectReference<aeMCPRuleAnimationSelectTargetMoveTime>::New();
	targetSelect = deTObjectReference<aeMCPRuleAnimationSelectTargetSelect>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			moves,
			targetSelect,
			targetMoveTime,
			enablePosition, enableOrientation, enableSize, enableVertexPositionSet));
}


void aeMCRuleBoneTransformatorProperties::Init(const aeMCAnimatorProperties &properties){
	minTranslation = deTObjectReference<aeMCPRuleBoneTransformatorMinTranslation>::New();
	maxTranslation = deTObjectReference<aeMCPRuleBoneTransformatorMaxTranslation>::New();
	minRotation = deTObjectReference<aeMCPRuleBoneTransformatorMinRotation>::New();
	maxRotation = deTObjectReference<aeMCPRuleBoneTransformatorMaxRotation>::New();
	minScaling = deTObjectReference<aeMCPRuleBoneTransformatorMinScaling>::New();
	maxScaling = deTObjectReference<aeMCPRuleBoneTransformatorMaxScaling>::New();
	axis = deTObjectReference<aeMCPRuleBoneTransformatorAxis>::New();
	minAngle = deTObjectReference<aeMCPRuleBoneTransformatorMinAngle>::New();
	maxAngle = deTObjectReference<aeMCPRuleBoneTransformatorMaxAngle>::New();
	enablePosition = deTObjectReference<aeMCPRuleBoneTransformatorEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleBoneTransformatorEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleBoneTransformatorEnableSize>::New();
	useAxis = deTObjectReference<aeMCPRuleBoneTransformatorUseAxis>::New();
	targetBone = deTObjectReference<aeMCPRuleBoneTransformatorTargetBone>::New();
	inputBone = deTObjectReference<aeMCPRuleBoneTransformatorInputBone>::New();
	coordinateFrame = deTObjectReference<aeMCPRuleBoneTransformatorCoordinateFrame>::New();
	inputSource = deTObjectReference<aeMCPRuleBoneTransformatorInputSource>::New();
	targetTranslation = deTObjectReference<aeMCPRuleBoneTransformatorTargetTranslation>::New();
	targetRotation = deTObjectReference<aeMCPRuleBoneTransformatorTargetRotation>::New();
	targetScaling = deTObjectReference<aeMCPRuleBoneTransformatorTargetScaling>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			minTranslation, maxTranslation, targetTranslation, enablePosition,
			minRotation, maxRotation, targetRotation, enableOrientation,
			minScaling, maxScaling, targetScaling, enableSize,
			axis, minAngle, maxAngle, useAxis,
			targetBone, inputBone,
			coordinateFrame,
			inputSource));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, targetBone);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, inputBone);
}


void aeMCRuleForeignStateProperties::Init(const aeMCAnimatorProperties &properties){
	foreignBone = deTObjectReference<aeMCPRuleForeignStateForeignBone>::New();
	foreignVertexPositionSet = deTObjectReference<aeMCPRuleForeignStateForeignVertexPositionSet>::New();
	scalePosition = deTObjectReference<aeMCPRuleForeignStateScalePosition>::New();
	scaleOrientation = deTObjectReference<aeMCPRuleForeignStateScaleOrientation>::New();
	scaleSize = deTObjectReference<aeMCPRuleForeignStateScaleSize>::New();
	scaleVertexPositionSet = deTObjectReference<aeMCPRuleForeignStateScaleVertexPositionSet>::New();
	enablePosition = deTObjectReference<aeMCPRuleForeignStateEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleForeignStateEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleForeignStateEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleForeignStateEnableVertexPositionSet>::New();
	sourceCoordinateFrame = deTObjectReference<aeMCPRuleForeignStateSourceCoordinateFrame>::New();
	destCoordinateFrame = deTObjectReference<aeMCPRuleForeignStateDestCoordinateFrame>::New();
	targetPosition = deTObjectReference<aeMCPRuleForeignStateTargetPosition>::New();
	targetOrientation = deTObjectReference<aeMCPRuleForeignStateTargetOrientation>::New();
	targetSize = deTObjectReference<aeMCPRuleForeignStateTargetSize>::New();
	targetVertexPositionSet = deTObjectReference<aeMCPRuleForeignStateTargetVertexPositionSet>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			foreignBone, foreignVertexPositionSet,
			scalePosition, targetPosition, enablePosition,
			scaleOrientation, targetOrientation, enableOrientation,
			scaleSize, targetSize, enableSize,
			scaleVertexPositionSet, targetVertexPositionSet, enableVertexPositionSet,
			sourceCoordinateFrame, destCoordinateFrame));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, foreignBone);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenVPSNames, foreignVertexPositionSet);
}


void aeMCRuleGroupProperties::Init(const aeMCAnimatorProperties &properties){
	rules = deTObjectReference<aeMCPRuleGroupRules>::New();
	enablePosition = deTObjectReference<aeMCPRuleGroupEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleGroupEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleGroupEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleGroupEnableVertexPositionSet>::New();
	useCurrentState = deTObjectReference<aeMCPRuleGroupUseCurrentState>::New();
	applicationType = deTObjectReference<aeMCPRuleGroupApplicationType>::New();
	targetSelect = deTObjectReference<aeMCPRuleGroupTargetSelect>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			enablePosition, enableOrientation, enableSize, enableVertexPositionSet,
			useCurrentState,
			applicationType,
			targetSelect));
	
	igdeMetaPropertyAdapter::OnChanged(properties.rule.name, rules);
	igdeMetaPropertyAdapter::OnChanged(rules, properties.rule.ruleTree);
}


void aeMCRuleInverseKinematicProperties::Init(const aeMCAnimatorProperties &properties){
	goalPosition = deTObjectReference<aeMCPRuleInverseKinematicGoalPosition>::New();
	goalOrientation = deTObjectReference<aeMCPRuleInverseKinematicGoalOrientation>::New();
	localPosition = deTObjectReference<aeMCPRuleInverseKinematicLocalPosition>::New();
	localOrientation = deTObjectReference<aeMCPRuleInverseKinematicLocalOrientation>::New();
	adjustOrientation = deTObjectReference<aeMCPRuleInverseKinematicAdjustOrientation>::New();
	useSolverBone = deTObjectReference<aeMCPRuleInverseKinematicUseSolverBone>::New();
	solverBone = deTObjectReference<aeMCPRuleInverseKinematicSolverBone>::New();
	reachRange = deTObjectReference<aeMCPRuleInverseKinematicReachRange>::New();
	reachBone = deTObjectReference<aeMCPRuleInverseKinematicReachBone>::New();
	reachCenter = deTObjectReference<aeMCPRuleInverseKinematicReachCenter>::New();
	targetGoalPosition = deTObjectReference<aeMCPRuleInverseKinematicTargetGoalPosition>::New();
	targetGoalOrientation = deTObjectReference<aeMCPRuleInverseKinematicTargetGoalOrientation>::New();
	targetLocalPosition = deTObjectReference<aeMCPRuleInverseKinematicTargetLocalPosition>::New();
	targetLocalOrientation = deTObjectReference<aeMCPRuleInverseKinematicTargetLocalOrientation>::New();
	targetReachRange = deTObjectReference<aeMCPRuleInverseKinematicTargetReachRange>::New();
	targetReachCenter = deTObjectReference<aeMCPRuleInverseKinematicTargetReachCenter>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			goalPosition, targetGoalPosition,
			goalOrientation, targetGoalOrientation,
			localPosition, targetLocalPosition,
			localOrientation, targetLocalOrientation,
			adjustOrientation,
			useSolverBone,
			solverBone,
			reachRange, targetReachRange, reachBone, reachCenter,
			targetReachCenter));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, solverBone);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, reachBone);
}


void aeMCRuleLimitProperties::Init(const aeMCAnimatorProperties &properties){
	minPosition = deTObjectReference<aeMCPRuleLimitMinPosition>::New();
	maxPosition = deTObjectReference<aeMCPRuleLimitMaxPosition>::New();
	minRotation = deTObjectReference<aeMCPRuleLimitMinRotation>::New();
	maxRotation = deTObjectReference<aeMCPRuleLimitMaxRotation>::New();
	minScaling = deTObjectReference<aeMCPRuleLimitMinScaling>::New();
	maxScaling = deTObjectReference<aeMCPRuleLimitMaxScaling>::New();
	minVertexPositionSet = deTObjectReference<aeMCPRuleLimitMinVertexPositionSet>::New();
	maxVertexPositionSet = deTObjectReference<aeMCPRuleLimitMaxVertexPositionSet>::New();
	targetBone = deTObjectReference<aeMCPRuleLimitTargetBone>::New();
	coordinateFrame = deTObjectReference<aeMCPRuleLimitCoordinateFrame>::New();
	enablePositionXMin = deTObjectReference<aeMCPRuleLimitEnablePositionXMin>::New();
	enablePositionXMax = deTObjectReference<aeMCPRuleLimitEnablePositionXMax>::New();
	enablePositionYMin = deTObjectReference<aeMCPRuleLimitEnablePositionYMin>::New();
	enablePositionYMax = deTObjectReference<aeMCPRuleLimitEnablePositionYMax>::New();
	enablePositionZMin = deTObjectReference<aeMCPRuleLimitEnablePositionZMin>::New();
	enablePositionZMax = deTObjectReference<aeMCPRuleLimitEnablePositionZMax>::New();
	enableRotationXMin = deTObjectReference<aeMCPRuleLimitEnableRotationXMin>::New();
	enableRotationXMax = deTObjectReference<aeMCPRuleLimitEnableRotationXMax>::New();
	enableRotationYMin = deTObjectReference<aeMCPRuleLimitEnableRotationYMin>::New();
	enableRotationYMax = deTObjectReference<aeMCPRuleLimitEnableRotationYMax>::New();
	enableRotationZMin = deTObjectReference<aeMCPRuleLimitEnableRotationZMin>::New();
	enableRotationZMax = deTObjectReference<aeMCPRuleLimitEnableRotationZMax>::New();
	enableScalingXMin = deTObjectReference<aeMCPRuleLimitEnableScalingXMin>::New();
	enableScalingXMax = deTObjectReference<aeMCPRuleLimitEnableScalingXMax>::New();
	enableScalingYMin = deTObjectReference<aeMCPRuleLimitEnableScalingYMin>::New();
	enableScalingYMax = deTObjectReference<aeMCPRuleLimitEnableScalingYMax>::New();
	enableScalingZMin = deTObjectReference<aeMCPRuleLimitEnableScalingZMin>::New();
	enableScalingZMax = deTObjectReference<aeMCPRuleLimitEnableScalingZMax>::New();
	enableVertexPositionSetMin = deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMin>::New();
	enableVertexPositionSetMax = deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMax>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			minPosition, enablePositionXMin, enablePositionYMin, enablePositionZMin,
			maxPosition, enablePositionXMax, enablePositionYMax, enablePositionZMax,
			minRotation, enableRotationXMin, enableRotationYMin, enableRotationZMin,
			maxRotation, enableRotationXMax, enableRotationYMax, enableRotationZMax,
			minScaling, enableScalingXMin, enableScalingYMin, enableScalingZMin,
			maxScaling, enableScalingXMax, enableScalingYMax, enableScalingZMax,
			minVertexPositionSet, enableVertexPositionSetMin,
			maxVertexPositionSet, enableVertexPositionSetMax,
			targetBone,
			coordinateFrame));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, targetBone);
}


void aeMCRuleMirrorMatchNameProperties::Init(const aeMCAnimatorProperties &properties, aeWindowMain &windowMain){
	first = deTObjectReference<aeMCPRuleMirrorMatchNameFirst>::New();
	second = deTObjectReference<aeMCPRuleMirrorMatchNameSecond>::New();
	type = deTObjectReference<aeMCPRuleMirrorMatchNameType>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		igdeMetaProperty::List(devctag, first, second, type));
	
	matchNames = deTObjectReference<aeMCPRuleMirrorMatchNames>::New();
	matchName = deTObjectReference<aeMCPRuleMirrorMatchName>::New(windowMain);
	groupMatchNames = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.rule_mirror.groupMatchNames", "Animator.WPAPanelRuleMirror.Pairs",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, matchNames, matchName));
	
	igdeMetaPropertyAdapter::OnChanged(first, matchNames);
	igdeMetaPropertyAdapter::OnChanged(second, matchNames);
	igdeMetaPropertyAdapter::OnChangedUsing<igdeMetaPropertySelection>(type, matchNames);
}


void aeMCRuleMirrorProperties::Init(const aeMCAnimatorProperties &properties, aeWindowMain &windowMain){
	mirrorAxis = deTObjectReference<aeMCPRuleMirrorMirrorAxis>::New();
	mirrorBone = deTObjectReference<aeMCPRuleMirrorMirrorBone>::New();
	enablePosition = deTObjectReference<aeMCPRuleMirrorEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleMirrorEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleMirrorEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleMirrorEnableVertexPositionSet>::New();
	
	matchName.Init(properties, windowMain);
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			mirrorAxis,
			mirrorBone,
			matchName.groupMatchNames,
			enablePosition, enableOrientation, enableSize,
			enableVertexPositionSet));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, mirrorBone);
}


void aeMCRuleStateManipulatorProperties::Init(const aeMCAnimatorProperties &properties){
	minPosition = deTObjectReference<aeMCPRuleStateManipulatorMinPosition>::New();
	maxPosition = deTObjectReference<aeMCPRuleStateManipulatorMaxPosition>::New();
	minRotation = deTObjectReference<aeMCPRuleStateManipulatorMinRotation>::New();
	maxRotation = deTObjectReference<aeMCPRuleStateManipulatorMaxRotation>::New();
	minSize = deTObjectReference<aeMCPRuleStateManipulatorMinSize>::New();
	maxSize = deTObjectReference<aeMCPRuleStateManipulatorMaxSize>::New();
	minVertexPositionSet = deTObjectReference<aeMCPRuleStateManipulatorMinVertexPositionSet>::New();
	maxVertexPositionSet = deTObjectReference<aeMCPRuleStateManipulatorMaxVertexPositionSet>::New();
	enablePosition = deTObjectReference<aeMCPRuleStateManipulatorEnablePosition>::New();
	enableRotation = deTObjectReference<aeMCPRuleStateManipulatorEnableRotation>::New();
	enableSize = deTObjectReference<aeMCPRuleStateManipulatorEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleStateManipulatorEnableVertexPositionSet>::New();
	targetPosition = deTObjectReference<aeMCPRuleStateManipulatorTargetPosition>::New();
	targetRotation = deTObjectReference<aeMCPRuleStateManipulatorTargetRotation>::New();
	targetSize = deTObjectReference<aeMCPRuleStateManipulatorTargetSize>::New();
	targetVertexPositionSet = deTObjectReference<aeMCPRuleStateManipulatorTargetVertexPositionSet>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			minPosition, maxPosition, targetPosition, enablePosition,
			minRotation, maxRotation, targetRotation, enableRotation,
			minSize, maxSize, targetSize, enableSize,
			minVertexPositionSet, maxVertexPositionSet, targetVertexPositionSet, enableVertexPositionSet));
}


void aeMCRuleStateSnapshotProperties::Init(const aeMCAnimatorProperties &properties){
	useLastState = deTObjectReference<aeMCPRuleStateSnapshotUseLastState>::New();
	id = deTObjectReference<aeMCPRuleStateSnapshotID>::New();
	enablePosition = deTObjectReference<aeMCPRuleStateSnapshotEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleStateSnapshotEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleStateSnapshotEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleStateSnapshotEnableVertexPositionSet>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			useLastState,
			id,
			enablePosition, enableOrientation, enableSize, enableVertexPositionSet));
}


void aeMCRuleSubAnimatorConnectionProperties::Init(const aeMCAnimatorProperties &properties, aeWindowMain &windowMain){
	target = deTObjectReference<aeMCPRuleSubAnimatorConnectionTarget>::New();
	controller = deTObjectReference<aeMCPRuleSubAnimatorConnectionController>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		igdeMetaProperty::List(devctag, target, controller));
	
	igdeMetaPropertyAdapter::OnChanged(properties.controller.controllers, controller);
	
	connections = deTObjectReference<aeMCPRuleSubAnimatorConnections>::New();
	connection = deTObjectReference<aeMCPRuleSubAnimatorConnection>::New(windowMain);
	groupConnections = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.rule_subanimator.groupConnections", "Animator.WPAPanelRuleSubAnimator.Connections",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, connections, connection), true);
	
	igdeMetaPropertyAdapter::OnChanged(target, connections);
	igdeMetaPropertyAdapter::OnChangedUsing<igdeMetaPropertyObject>(controller, connections);
}


void aeMCRuleSubAnimatorProperties::Init(const aeMCAnimatorProperties &properties, aeWindowMain &windowMain){
	pathSubAnimator = deTObjectReference<aeMCPRuleSubAnimatorPathSubAnimator>::New();
	enablePosition = deTObjectReference<aeMCPRuleSubAnimatorEnablePosition>::New();
	enableOrientation = deTObjectReference<aeMCPRuleSubAnimatorEnableOrientation>::New();
	enableSize = deTObjectReference<aeMCPRuleSubAnimatorEnableSize>::New();
	enableVertexPositionSet = deTObjectReference<aeMCPRuleSubAnimatorEnableVertexPositionSet>::New();
	
	connection.Init(properties, windowMain);
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			pathSubAnimator,
			connection.groupConnections,
			enablePosition, enableOrientation, enableSize, enableVertexPositionSet));
}


void aeMCRuleTrackToProperties::Init(const aeMCAnimatorProperties &properties){
	trackBone = deTObjectReference<aeMCPRuleTrackToTrackBone>::New();
	trackAxis = deTObjectReference<aeMCPRuleTrackToTrackAxis>::New();
	upAxis = deTObjectReference<aeMCPRuleTrackToUpAxis>::New();
	upTarget = deTObjectReference<aeMCPRuleTrackToUpTarget>::New();
	lockedAxis = deTObjectReference<aeMCPRuleTrackToLockedAxis>::New();
	targetPosition = deTObjectReference<aeMCPRuleTrackToTargetPosition>::New();
	targetUp = deTObjectReference<aeMCPRuleTrackToTargetUp>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		properties.rule.metaProperties->GetData()
		+ igdeMetaProperty::List(devctag,
			trackBone,
			trackAxis,
			upAxis,
			upTarget, targetUp,
			lockedAxis,
			targetPosition));
	
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, trackBone);
}
