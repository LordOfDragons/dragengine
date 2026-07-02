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


aeMCAnimatorProperties::aeMCAnimatorProperties(aeWindowMain &windowMain) :
controller(windowMain),
link(windowMain),
rule(windowMain),
attachment(windowMain),
locomotionLeg(windowMain)
{
	Init();
}

void aeMCAnimatorProperties::Init(){
	metaProperties->GetData() += decTObjectOrderedSet<igdeMetaProperty>(devctag,
		rig,
		animation,
		affectedBones,
		affectedVertexPositionSets);
	
	controller.Init(*this);
	metaPropertiesController->GetData() += controller.group->GetProperties();
	
	link.Init(*this);
	metaPropertiesLink->GetData() += link.group->GetProperties();
	
	rule.Init(*this);
	metaPropertiesRule->GetData() += rule.group->GetProperties();
	
	ruleAnimation.Init(*this);
	ruleAnimationDifference.Init(*this);
	ruleAnimationSelect.Init(*this);
	ruleBoneTransformator.Init(*this);
	ruleForeignState.Init(*this);
	ruleGroup.Init(*this);
	ruleInverseKinematic.Init(*this);
	ruleLimit.Init(*this);
	ruleMirror.Init(*this);
	ruleStateManipulator.Init(*this);
	ruleStateSnapshot.Init(*this);
	ruleSubAnimator.Init(*this);
	ruleTrackTo.Init(*this);
	
	locomotion.Init(*this);
	locomotionLeg.Init();
	
	playgroundControllers = deTObjectReference<aeMCPAnimatorPlaygroundControllers>::New(controller.name, controller.currentValue);
	metaPropertiesPlayground->GetData() += decTObjectOrderedSet<igdeMetaProperty>(devctag,
		playgroundControllers,
		locomotion.groupTesting,
		locomotion.groupMovementSpeeds,
		locomotion.groupAdjustmentTimes,
		locomotion.groupLegs,
		locomotion.groupVisualization);
	
	igdeMetaPropertyAdapter::OnChanged(hiddenBoneNames, cameraAttachBone);
	
	attachment.Init(*this);
	
	metaPropertiesView->GetData() += decTObjectOrderedSet<igdeMetaProperty>(devctag,
		displayModelPath,
		displaySkinPath,
		displayRigPath,
		groupBaseAnimator,
		groupSky,
		groupEnvironmentObject,
		groupCamera,
		groupCameraAttach,
		groupPlayback,
		attachment.group,
		groupUndoHistory);
}
