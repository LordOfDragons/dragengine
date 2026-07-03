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

#ifndef _AEMCANIMATORPROPERTIES_H_
#define _AEMCANIMATORPROPERTIES_H_

#include "aeMCLinkProperties.h"
#include "aeMCControllerProperties.h"
#include "aeMCRuleProperties.h"
#include "aeMCAttachmentProperties.h"
#include "aeMCAnimatorLocomotionProperties.h"

#include "aeMCPAnimator.h"

class aeWindowMain;


/**
 * Animator meta context properties.
 */
class aeMCAnimatorProperties{
public:
	deTObjectReference<aeMCPHiddenBoneNames> hiddenBoneNames;
	deTObjectReference<aeMCPHiddenVPSNames> hiddenVPSNames;
	deTObjectReference<aeMCPHiddenMoveNames> hiddenMoveNames;
	
	deTObjectReference<aeMCPAnimatorRig> rig;
	deTObjectReference<aeMCPAnimatorAnimation> animation;
	deTObjectReference<aeMCPAnimatorAffectedBones> affectedBones;
	deTObjectReference<aeMCPAnimatorAffectedVertexPositionSets> affectedVertexPositionSets;
	
	aeMCControllerProperties controller;
	aeMCLinkProperties link;
	aeMCRuleProperties rule;
	aeMCRuleAnimationProperties ruleAnimation;
	aeMCRuleAnimationDifferenceProperties ruleAnimationDifference;
	aeMCRuleAnimationSelectProperties ruleAnimationSelect;
	aeMCRuleBoneTransformatorProperties ruleBoneTransformator;
	aeMCRuleForeignStateProperties ruleForeignState;
	aeMCRuleGroupProperties ruleGroup;
	aeMCRuleInverseKinematicProperties ruleInverseKinematic;
	aeMCRuleLimitProperties ruleLimit;
	aeMCRuleMirrorProperties ruleMirror;
	aeMCRuleStateManipulatorProperties ruleStateManipulator;
	aeMCRuleStateSnapshotProperties ruleStateSnapshot;
	aeMCRuleSubAnimatorProperties ruleSubAnimator;
	aeMCRuleTrackToProperties ruleTrackTo;
	aeMCAttachmentProperties attachment;
	aeMCAnimatorLocomotionProperties locomotion;
	
	deTObjectReference<aeMCPAnimatorPlaygroundControllers> playgroundControllers;
	
	deTObjectReference<aeMCPAnimatorDisplayModelPath> displayModelPath;
	deTObjectReference<aeMCPAnimatorDisplaySkinPath> displaySkinPath;
	deTObjectReference<aeMCPAnimatorDisplayRigPath> displayRigPath;
	
	deTObjectReference<aeMCPAnimatorResetState> resetState;
	deTObjectReference<aeMCPAnimatorBaseAnimatorPath> baseAnimatorPath;
	deTObjectReference<igdeMetaPropertyGroup> groupBaseAnimator;
	
	deTObjectReference<aeMCPAnimatorPlaySpeed> playSpeed;
	deTObjectReference<aeMCPAnimatorTimeStep> timeStep;
	deTObjectReference<aeMCPAnimatorPaused> paused;
	deTObjectReference<igdeMetaPropertyGroup> groupPlayback;
	
	deTObjectReference<aeMCPAnimatorSky> sky;
	deTObjectReference<igdeMetaPropertyGroup> groupSky;
	
	deTObjectReference<aeMCPAnimatorEnvironmentObject> environmentObject;
	deTObjectReference<igdeMetaPropertyGroup> groupEnvironmentObject;
	
	deTObjectReference<aeMCPAnimatorCamera> camera;
	deTObjectReference<igdeMetaPropertyGroup> groupCamera;
	
	deTObjectReference<aeMCPAnimatorCameraAttachToBone> cameraAttachToBone;
	deTObjectReference<aeMCPAnimatorCameraAttachBone> cameraAttachBone;
	deTObjectReference<aeMCPAnimatorCameraAttachRelativePosition> cameraAttachRelativePosition;
	deTObjectReference<aeMCPAnimatorCameraAttachRelativeRotation> cameraAttachRelativeRotation;
	deTObjectReference<igdeMetaPropertyGroup> groupCameraAttach;
	
	igdeMetaPropertyUndoHistory::Ref undoHistory;
	deTObjectReference<igdeMetaPropertyGroup> groupUndoHistory;
	
	igdeMetaContext::PropertyList::Ref metaProperties;
	igdeMetaContext::PropertyList::Ref metaPropertiesController;
	igdeMetaContext::PropertyList::Ref metaPropertiesLink;
	igdeMetaContext::PropertyList::Ref metaPropertiesRule;
	igdeMetaContext::PropertyList::Ref metaPropertiesPlayground;
	igdeMetaContext::PropertyList::Ref metaPropertiesView;
	
	void Init(aeWindowMain &windowMain);
};

#endif
