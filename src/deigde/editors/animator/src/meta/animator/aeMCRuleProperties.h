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
class aeMCAnimatorProperties;


class aeMCRuleProperties{
public:
	deTObjectReference<aeMCPRuleName> name;
	deTObjectReference<aeMCPRuleBlendMode> blendMode;
	deTObjectReference<aeMCPRuleBlendFactor> blendFactor;
	deTObjectReference<aeMCPRuleInvertBlendFactor> invertBlendFactor;
	deTObjectReference<aeMCPRuleEnabled> enabled;
	deTObjectReference<aeMCPRuleAffectedBones> affectedBones;
	deTObjectReference<aeMCPRuleAffectedVertexPositionSets> affectedVertexPositionSets;
	deTObjectReference<aeMCPRuleTargetBlendFactor> targetBlendFactor;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	deTObjectReference<aeMCPRuleTree> ruleTree;
	deTObjectReference<aeMCPRules> rules;
	deTObjectReference<aeMCPRule> rule;
	deTObjectReference<igdeMetaPropertyGroup> group;
	
	void Init(aeWindowMain &windowMain);
};


class aeMCRuleAnimationProperties{
public:
	deTObjectReference<aeMCPRuleAnimationMoveName> moveName;
	deTObjectReference<aeMCPRuleAnimationMoveTime> moveTime;
	deTObjectReference<aeMCPRuleAnimationEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleAnimationEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleAnimationEnableSize> enableSize;
	deTObjectReference<aeMCPRuleAnimationEnableVertexPositionSet> enableVertexPositionSet;
	deTObjectReference<aeMCPRuleAnimationTargetMoveTime> targetMoveTime;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleAnimationDifferenceProperties{
public:
	deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveName> leadingMoveName;
	deTObjectReference<aeMCPRuleAnimationDifferenceLeadingMoveTime> leadingMoveTime;
	deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveName> referenceMoveName;
	deTObjectReference<aeMCPRuleAnimationDifferenceReferenceMoveTime> referenceMoveTime;
	deTObjectReference<aeMCPRuleAnimationDifferenceUseComponentSpace> useComponentSpace;
	deTObjectReference<aeMCPRuleAnimationDifferenceEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleAnimationDifferenceEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleAnimationDifferenceEnableSize> enableSize;
	deTObjectReference<aeMCPRuleAnimationDifferenceEnableVertexPositionSet> enableVertexPositionSet;
	deTObjectReference<aeMCPRuleAnimationDifferenceTargetLeadMoveTime> targetLeadMoveTime;
	deTObjectReference<aeMCPRuleAnimationDifferenceTargetRefMoveTime> targetRefMoveTime;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleAnimationSelectProperties{
public:
	deTObjectReference<aeMCPRuleAnimationSelectMoves> moves;
	deTObjectReference<aeMCPRuleAnimationSelectEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleAnimationSelectEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleAnimationSelectEnableSize> enableSize;
	deTObjectReference<aeMCPRuleAnimationSelectEnableVertexPositionSet> enableVertexPositionSet;
	deTObjectReference<aeMCPRuleAnimationSelectTargetMoveTime> targetMoveTime;
	deTObjectReference<aeMCPRuleAnimationSelectTargetSelect> targetSelect;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleBoneTransformatorProperties{
public:
	deTObjectReference<aeMCPRuleBoneTransformatorMinTranslation> minTranslation;
	deTObjectReference<aeMCPRuleBoneTransformatorMaxTranslation> maxTranslation;
	deTObjectReference<aeMCPRuleBoneTransformatorMinRotation> minRotation;
	deTObjectReference<aeMCPRuleBoneTransformatorMaxRotation> maxRotation;
	deTObjectReference<aeMCPRuleBoneTransformatorMinScaling> minScaling;
	deTObjectReference<aeMCPRuleBoneTransformatorMaxScaling> maxScaling;
	deTObjectReference<aeMCPRuleBoneTransformatorAxis> axis;
	deTObjectReference<aeMCPRuleBoneTransformatorMinAngle> minAngle;
	deTObjectReference<aeMCPRuleBoneTransformatorMaxAngle> maxAngle;
	deTObjectReference<aeMCPRuleBoneTransformatorEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleBoneTransformatorEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleBoneTransformatorEnableSize> enableSize;
	deTObjectReference<aeMCPRuleBoneTransformatorUseAxis> useAxis;
	deTObjectReference<aeMCPRuleBoneTransformatorTargetBone> targetBone;
	deTObjectReference<aeMCPRuleBoneTransformatorInputBone> inputBone;
	deTObjectReference<aeMCPRuleBoneTransformatorCoordinateFrame> coordinateFrame;
	deTObjectReference<aeMCPRuleBoneTransformatorInputSource> inputSource;
	deTObjectReference<aeMCPRuleBoneTransformatorTargetTranslation> targetTranslation;
	deTObjectReference<aeMCPRuleBoneTransformatorTargetRotation> targetRotation;
	deTObjectReference<aeMCPRuleBoneTransformatorTargetScaling> targetScaling;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleForeignStateProperties{
public:
	deTObjectReference<aeMCPRuleForeignStateForeignBone> foreignBone;
	deTObjectReference<aeMCPRuleForeignStateForeignVertexPositionSet> foreignVertexPositionSet;
	deTObjectReference<aeMCPRuleForeignStateScalePosition> scalePosition;
	deTObjectReference<aeMCPRuleForeignStateScaleOrientation> scaleOrientation;
	deTObjectReference<aeMCPRuleForeignStateScaleSize> scaleSize;
	deTObjectReference<aeMCPRuleForeignStateScaleVertexPositionSet> scaleVertexPositionSet;
	deTObjectReference<aeMCPRuleForeignStateEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleForeignStateEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleForeignStateEnableSize> enableSize;
	deTObjectReference<aeMCPRuleForeignStateEnableVertexPositionSet> enableVertexPositionSet;
	deTObjectReference<aeMCPRuleForeignStateSourceCoordinateFrame> sourceCoordinateFrame;
	deTObjectReference<aeMCPRuleForeignStateDestCoordinateFrame> destCoordinateFrame;
	deTObjectReference<aeMCPRuleForeignStateTargetPosition> targetPosition;
	deTObjectReference<aeMCPRuleForeignStateTargetOrientation> targetOrientation;
	deTObjectReference<aeMCPRuleForeignStateTargetSize> targetSize;
	deTObjectReference<aeMCPRuleForeignStateTargetVertexPositionSet> targetVertexPositionSet;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleGroupProperties{
public:
	deTObjectReference<aeMCPRuleGroupRules> rules;
	deTObjectReference<aeMCPRuleGroupEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleGroupEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleGroupEnableSize> enableSize;
	deTObjectReference<aeMCPRuleGroupEnableVertexPositionSet> enableVertexPositionSet;
	deTObjectReference<aeMCPRuleGroupUseCurrentState> useCurrentState;
	deTObjectReference<aeMCPRuleGroupApplicationType> applicationType;
	deTObjectReference<aeMCPRuleGroupTargetSelect> targetSelect;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleInverseKinematicProperties{
public:
	deTObjectReference<aeMCPRuleInverseKinematicGoalPosition> goalPosition;
	deTObjectReference<aeMCPRuleInverseKinematicGoalOrientation> goalOrientation;
	deTObjectReference<aeMCPRuleInverseKinematicLocalPosition> localPosition;
	deTObjectReference<aeMCPRuleInverseKinematicLocalOrientation> localOrientation;
	deTObjectReference<aeMCPRuleInverseKinematicAdjustOrientation> adjustOrientation;
	deTObjectReference<aeMCPRuleInverseKinematicUseSolverBone> useSolverBone;
	deTObjectReference<aeMCPRuleInverseKinematicSolverBone> solverBone;
	deTObjectReference<aeMCPRuleInverseKinematicReachRange> reachRange;
	deTObjectReference<aeMCPRuleInverseKinematicReachBone> reachBone;
	deTObjectReference<aeMCPRuleInverseKinematicReachCenter> reachCenter;
	deTObjectReference<aeMCPRuleInverseKinematicTargetGoalPosition> targetGoalPosition;
	deTObjectReference<aeMCPRuleInverseKinematicTargetGoalOrientation> targetGoalOrientation;
	deTObjectReference<aeMCPRuleInverseKinematicTargetLocalPosition> targetLocalPosition;
	deTObjectReference<aeMCPRuleInverseKinematicTargetLocalOrientation> targetLocalOrientation;
	deTObjectReference<aeMCPRuleInverseKinematicTargetReachRange> targetReachRange;
	deTObjectReference<aeMCPRuleInverseKinematicTargetReachCenter> targetReachCenter;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleLimitProperties{
public:
	deTObjectReference<aeMCPRuleLimitMinPosition> minPosition;
	deTObjectReference<aeMCPRuleLimitMaxPosition> maxPosition;
	deTObjectReference<aeMCPRuleLimitMinRotation> minRotation;
	deTObjectReference<aeMCPRuleLimitMaxRotation> maxRotation;
	deTObjectReference<aeMCPRuleLimitMinScaling> minScaling;
	deTObjectReference<aeMCPRuleLimitMaxScaling> maxScaling;
	deTObjectReference<aeMCPRuleLimitMinVertexPositionSet> minVertexPositionSet;
	deTObjectReference<aeMCPRuleLimitMaxVertexPositionSet> maxVertexPositionSet;
	deTObjectReference<aeMCPRuleLimitTargetBone> targetBone;
	deTObjectReference<aeMCPRuleLimitCoordinateFrame> coordinateFrame;
	deTObjectReference<aeMCPRuleLimitEnablePositionXMin> enablePositionXMin;
	deTObjectReference<aeMCPRuleLimitEnablePositionXMax> enablePositionXMax;
	deTObjectReference<aeMCPRuleLimitEnablePositionYMin> enablePositionYMin;
	deTObjectReference<aeMCPRuleLimitEnablePositionYMax> enablePositionYMax;
	deTObjectReference<aeMCPRuleLimitEnablePositionZMin> enablePositionZMin;
	deTObjectReference<aeMCPRuleLimitEnablePositionZMax> enablePositionZMax;
	deTObjectReference<aeMCPRuleLimitEnableRotationXMin> enableRotationXMin;
	deTObjectReference<aeMCPRuleLimitEnableRotationXMax> enableRotationXMax;
	deTObjectReference<aeMCPRuleLimitEnableRotationYMin> enableRotationYMin;
	deTObjectReference<aeMCPRuleLimitEnableRotationYMax> enableRotationYMax;
	deTObjectReference<aeMCPRuleLimitEnableRotationZMin> enableRotationZMin;
	deTObjectReference<aeMCPRuleLimitEnableRotationZMax> enableRotationZMax;
	deTObjectReference<aeMCPRuleLimitEnableScalingXMin> enableScalingXMin;
	deTObjectReference<aeMCPRuleLimitEnableScalingXMax> enableScalingXMax;
	deTObjectReference<aeMCPRuleLimitEnableScalingYMin> enableScalingYMin;
	deTObjectReference<aeMCPRuleLimitEnableScalingYMax> enableScalingYMax;
	deTObjectReference<aeMCPRuleLimitEnableScalingZMin> enableScalingZMin;
	deTObjectReference<aeMCPRuleLimitEnableScalingZMax> enableScalingZMax;
	deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMin> enableVertexPositionSetMin;
	deTObjectReference<aeMCPRuleLimitEnableVertexPositionSetMax> enableVertexPositionSetMax;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleMirrorMatchNameProperties{
public:
	deTObjectReference<aeMCPRuleMirrorMatchNameFirst> first;
	deTObjectReference<aeMCPRuleMirrorMatchNameSecond> second;
	deTObjectReference<aeMCPRuleMirrorMatchNameType> type;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	deTObjectReference<aeMCPRuleMirrorMatchNames> matchNames;
	deTObjectReference<aeMCPRuleMirrorMatchName> matchName;
	deTObjectReference<igdeMetaPropertyGroup> groupMatchNames;
	
	void Init(const aeMCAnimatorProperties &properties, aeWindowMain &windowMain);
};


class aeMCRuleMirrorProperties{
public:
	deTObjectReference<aeMCPRuleMirrorMirrorAxis> mirrorAxis;
	deTObjectReference<aeMCPRuleMirrorMirrorBone> mirrorBone;
	deTObjectReference<aeMCPRuleMirrorEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleMirrorEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleMirrorEnableSize> enableSize;
	deTObjectReference<aeMCPRuleMirrorEnableVertexPositionSet> enableVertexPositionSet;
	
	aeMCRuleMirrorMatchNameProperties matchName;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties, aeWindowMain &windowMain);
};


class aeMCRuleStateManipulatorProperties{
public:
	deTObjectReference<aeMCPRuleStateManipulatorMinPosition> minPosition;
	deTObjectReference<aeMCPRuleStateManipulatorMaxPosition> maxPosition;
	deTObjectReference<aeMCPRuleStateManipulatorMinRotation> minRotation;
	deTObjectReference<aeMCPRuleStateManipulatorMaxRotation> maxRotation;
	deTObjectReference<aeMCPRuleStateManipulatorMinSize> minSize;
	deTObjectReference<aeMCPRuleStateManipulatorMaxSize> maxSize;
	deTObjectReference<aeMCPRuleStateManipulatorMinVertexPositionSet> minVertexPositionSet;
	deTObjectReference<aeMCPRuleStateManipulatorMaxVertexPositionSet> maxVertexPositionSet;
	deTObjectReference<aeMCPRuleStateManipulatorEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleStateManipulatorEnableRotation> enableRotation;
	deTObjectReference<aeMCPRuleStateManipulatorEnableSize> enableSize;
	deTObjectReference<aeMCPRuleStateManipulatorEnableVertexPositionSet> enableVertexPositionSet;
	deTObjectReference<aeMCPRuleStateManipulatorTargetPosition> targetPosition;
	deTObjectReference<aeMCPRuleStateManipulatorTargetRotation> targetRotation;
	deTObjectReference<aeMCPRuleStateManipulatorTargetSize> targetSize;
	deTObjectReference<aeMCPRuleStateManipulatorTargetVertexPositionSet> targetVertexPositionSet;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleStateSnapshotProperties{
public:
	deTObjectReference<aeMCPRuleStateSnapshotUseLastState> useLastState;
	deTObjectReference<aeMCPRuleStateSnapshotID> id;
	deTObjectReference<aeMCPRuleStateSnapshotEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleStateSnapshotEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleStateSnapshotEnableSize> enableSize;
	deTObjectReference<aeMCPRuleStateSnapshotEnableVertexPositionSet> enableVertexPositionSet;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleSubAnimatorProperties{
public:
	deTObjectReference<aeMCPRuleSubAnimatorPathSubAnimator> pathSubAnimator;
	deTObjectReference<aeMCPRuleSubAnimatorEnablePosition> enablePosition;
	deTObjectReference<aeMCPRuleSubAnimatorEnableOrientation> enableOrientation;
	deTObjectReference<aeMCPRuleSubAnimatorEnableSize> enableSize;
	deTObjectReference<aeMCPRuleSubAnimatorEnableVertexPositionSet> enableVertexPositionSet;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};


class aeMCRuleTrackToProperties{
public:
	deTObjectReference<aeMCPRuleTrackToTrackBone> trackBone;
	deTObjectReference<aeMCPRuleTrackToTrackAxis> trackAxis;
	deTObjectReference<aeMCPRuleTrackToUpAxis> upAxis;
	deTObjectReference<aeMCPRuleTrackToUpTarget> upTarget;
	deTObjectReference<aeMCPRuleTrackToLockedAxis> lockedAxis;
	deTObjectReference<aeMCPRuleTrackToTargetPosition> targetPosition;
	deTObjectReference<aeMCPRuleTrackToTargetUp> targetUp;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	
	void Init(const aeMCAnimatorProperties &properties);
};

#endif
