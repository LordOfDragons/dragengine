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
#include "aeMCAnimatorLocomotionLegProperties.h"

#include "aeMCPAnimator.h"

class aeWindowMain;


/**
 * Animator meta context properties.
 */
class aeMCAnimatorProperties{
public:
	deTObjectReference<aeMCPHiddenBoneNames> hiddenBoneNames = deTObjectReference<aeMCPHiddenBoneNames>::New();
	deTObjectReference<aeMCPHiddenVPSNames> hiddenVPSNames = deTObjectReference<aeMCPHiddenVPSNames>::New();
	deTObjectReference<aeMCPHiddenMoveNames> hiddenMoveNames = deTObjectReference<aeMCPHiddenMoveNames>::New();
	
	deTObjectReference<aeMCPAnimatorRig> rig = deTObjectReference<aeMCPAnimatorRig>::New();
	deTObjectReference<aeMCPAnimatorAnimation> animation = deTObjectReference<aeMCPAnimatorAnimation>::New();
	deTObjectReference<aeMCPAnimatorAffectedBones> affectedBones = deTObjectReference<aeMCPAnimatorAffectedBones>::New();
	deTObjectReference<aeMCPAnimatorAffectedVertexPositionSets> affectedVertexPositionSets = deTObjectReference<aeMCPAnimatorAffectedVertexPositionSets>::New();
	
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
	aeMCAnimatorLocomotionLegProperties locomotionLeg;
	
	deTObjectReference<aeMCPAnimatorPlaygroundControllers> playgroundControllers;
	
	deTObjectReference<aeMCPAnimatorDisplayModelPath> displayModelPath = deTObjectReference<aeMCPAnimatorDisplayModelPath>::New();
	deTObjectReference<aeMCPAnimatorDisplaySkinPath> displaySkinPath = deTObjectReference<aeMCPAnimatorDisplaySkinPath>::New();
	deTObjectReference<aeMCPAnimatorDisplayRigPath> displayRigPath = deTObjectReference<aeMCPAnimatorDisplayRigPath>::New();
	
	deTObjectReference<aeMCPAnimatorResetState> resetState = deTObjectReference<aeMCPAnimatorResetState>::New();
	deTObjectReference<aeMCPAnimatorBaseAnimatorPath> baseAnimatorPath = deTObjectReference<aeMCPAnimatorBaseAnimatorPath>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupBaseAnimator = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupBaseAnimator", "Animator.WPView.TestingBaseAnimator",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, baseAnimatorPath, resetState), true);
	
	deTObjectReference<aeMCPAnimatorPlaySpeed> playSpeed = deTObjectReference<aeMCPAnimatorPlaySpeed>::New();
	deTObjectReference<aeMCPAnimatorTimeStep> timeStep = deTObjectReference<aeMCPAnimatorTimeStep>::New();
	deTObjectReference<aeMCPAnimatorPaused> paused = deTObjectReference<aeMCPAnimatorPaused>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupPlayback = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupPlayback", "Animator.WPView.Playback",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, playSpeed, paused));
	
	deTObjectReference<aeMCPAnimatorSky> sky = deTObjectReference<aeMCPAnimatorSky>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupSky = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupSky", "Animator.WPView.Sky",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, sky), true);
	
	deTObjectReference<aeMCPAnimatorEnvironmentObject> environmentObject = deTObjectReference<aeMCPAnimatorEnvironmentObject>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupEnvironmentObject = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupEnvironmentObject", "Animator.WPView.EnvironmentObject",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, environmentObject), true);
	
	deTObjectReference<aeMCPAnimatorCamera> camera = deTObjectReference<aeMCPAnimatorCamera>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupCamera = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupCamera", "Animator.WPView.Camera",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, camera), true);
	
	deTObjectReference<aeMCPAnimatorCameraAttachToBone> cameraAttachToBone = deTObjectReference<aeMCPAnimatorCameraAttachToBone>::New();
	deTObjectReference<aeMCPAnimatorCameraAttachBone> cameraAttachBone = deTObjectReference<aeMCPAnimatorCameraAttachBone>::New();
	deTObjectReference<aeMCPAnimatorCameraAttachRelativePosition> cameraAttachRelativePosition = deTObjectReference<aeMCPAnimatorCameraAttachRelativePosition>::New();
	deTObjectReference<aeMCPAnimatorCameraAttachRelativeRotation> cameraAttachRelativeRotation = deTObjectReference<aeMCPAnimatorCameraAttachRelativeRotation>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupCameraAttach = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupCameraAttach", "Animator.WPView.CameraAttaching",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, cameraAttachToBone, cameraAttachBone,
			cameraAttachRelativePosition, cameraAttachRelativeRotation), true);
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	igdeMetaContext::PropertyList::Ref metaPropertiesController = igdeMetaContext::PropertyList::Ref::New();
	igdeMetaContext::PropertyList::Ref metaPropertiesLink = igdeMetaContext::PropertyList::Ref::New();
	igdeMetaContext::PropertyList::Ref metaPropertiesRule = igdeMetaContext::PropertyList::Ref::New();
	igdeMetaContext::PropertyList::Ref metaPropertiesPlayground = igdeMetaContext::PropertyList::Ref::New();
	igdeMetaContext::PropertyList::Ref metaPropertiesView = igdeMetaContext::PropertyList::Ref::New();
	
	aeMCAnimatorProperties(aeWindowMain &windowMain);
	void Init();
};

#endif
