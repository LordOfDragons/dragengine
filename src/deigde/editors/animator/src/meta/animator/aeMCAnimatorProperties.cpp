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

#include "aeMCAnimatorProperties.h"

#include <deigde/meta/property/igdeMetaPropertyAdapters.h>


void aeMCAnimatorProperties::Init(aeWindowMain &windowMain){
	// animator
	hiddenBoneNames = deTObjectReference<aeMCPHiddenBoneNames>::New();
	hiddenVPSNames = deTObjectReference<aeMCPHiddenVPSNames>::New();
	hiddenMoveNames = deTObjectReference<aeMCPHiddenMoveNames>::New();
	
	rig = deTObjectReference<aeMCPAnimatorRig>::New();
	animation = deTObjectReference<aeMCPAnimatorAnimation>::New();
	affectedBones = deTObjectReference<aeMCPAnimatorAffectedBones>::New();
	affectedVertexPositionSets = deTObjectReference<aeMCPAnimatorAffectedVertexPositionSets>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			rig, animation,
			affectedBones, affectedVertexPositionSets));
	
	// controller
	controller.Init(windowMain);
	metaPropertiesController = igdeMetaContext::PropertyList::Ref::New(controller.group->GetProperties());
	
	// link
	link.Init(*this, windowMain);
	metaPropertiesLink = igdeMetaContext::PropertyList::Ref::New(link.group->GetProperties());
	
	// rule
	rule.Init(windowMain);
	metaPropertiesRule = igdeMetaContext::PropertyList::Ref::New(rule.group->GetProperties());
	
	ruleAnimation.Init(*this);
	ruleAnimationDifference.Init(*this);
	ruleAnimationSelect.Init(*this);
	ruleBoneTransformator.Init(*this);
	ruleForeignState.Init(*this);
	ruleGroup.Init(*this);
	ruleInverseKinematic.Init(*this);
	ruleLimit.Init(*this);
	ruleMirror.Init(*this, windowMain);
	ruleStateManipulator.Init(*this);
	ruleStateSnapshot.Init(*this);
	ruleSubAnimator.Init(*this);
	ruleTrackTo.Init(*this);
	
	// locomotion
	locomotion.Init(windowMain);
	
	// playground
	playgroundControllers = deTObjectReference<aeMCPAnimatorPlaygroundControllers>::New(
		controller.name, controller.currentValue);
	
	metaPropertiesPlayground = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			playgroundControllers,
			locomotion.groupTesting,
			locomotion.groupMovementSpeeds,
			locomotion.groupAdjustmentTimes,
			locomotion.groupLegs,
			locomotion.groupVisualization));
	
	// view
	displayModelPath = deTObjectReference<aeMCPAnimatorDisplayModelPath>::New();
	displaySkinPath = deTObjectReference<aeMCPAnimatorDisplaySkinPath>::New();
	displayRigPath = deTObjectReference<aeMCPAnimatorDisplayRigPath>::New();
	
	resetState = deTObjectReference<aeMCPAnimatorResetState>::New();
	baseAnimatorPath = deTObjectReference<aeMCPAnimatorBaseAnimatorPath>::New();
	groupBaseAnimator = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupBaseAnimator", "Animator.WPView.TestingBaseAnimator",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, baseAnimatorPath, resetState), true);
	
	sky = deTObjectReference<aeMCPAnimatorSky>::New();
	groupSky = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupSky", "Animator.WPView.Sky",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, sky), true);
	
	environmentObject = deTObjectReference<aeMCPAnimatorEnvironmentObject>::New();
	groupEnvironmentObject = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupEnvironmentObject", "Animator.WPView.EnvironmentObject",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, environmentObject), true);
	
	camera = deTObjectReference<aeMCPAnimatorCamera>::New();
	groupCamera = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupCamera", "Animator.WPView.Camera",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, camera), true);
	
	cameraAttachToBone = deTObjectReference<aeMCPAnimatorCameraAttachToBone>::New();
	cameraAttachBone = deTObjectReference<aeMCPAnimatorCameraAttachBone>::New();
	cameraAttachRelativePosition = deTObjectReference<aeMCPAnimatorCameraAttachRelativePosition>::New();
	cameraAttachRelativeRotation = deTObjectReference<aeMCPAnimatorCameraAttachRelativeRotation>::New();
	groupCameraAttach = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupCameraAttach", "Animator.WPView.CameraAttaching",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, cameraAttachToBone, cameraAttachBone,
			cameraAttachRelativePosition, cameraAttachRelativeRotation), true);
	
	igdeMetaPropertyAdapter::OnChanged(hiddenBoneNames, cameraAttachBone);
	
	playSpeed = deTObjectReference<aeMCPAnimatorPlaySpeed>::New();
	timeStep = deTObjectReference<aeMCPAnimatorTimeStep>::New();
	paused = deTObjectReference<aeMCPAnimatorPaused>::New();
	groupPlayback = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupPlayback", "Animator.WPView.Playback",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, playSpeed, paused));
	
	attachment.Init(*this, windowMain);
	
	undoHistory = igdeMetaPropertyUndoHistory::CreateForGroup("animator.undoHistory");
	groupUndoHistory = igdeMetaPropertyUndoHistory::CreateGroup("animator.groupUndoHistory", undoHistory);
	
	metaPropertiesView = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			displayModelPath, displaySkinPath, displayRigPath,
			groupBaseAnimator,
			groupSky,
			groupEnvironmentObject,
			groupCamera,
			groupCameraAttach,
			groupPlayback,
			attachment.group,
			groupUndoHistory));
}
