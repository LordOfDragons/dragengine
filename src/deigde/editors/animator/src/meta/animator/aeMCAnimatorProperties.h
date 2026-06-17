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

#include "aeMCPAnimator.h"
#include "aeMCPController.h"

class aeWindowMain;


/**
 * Controller meta context properties.
 */
class aeMCControllerProperties{
public:
	deTObjectReference<aeMCPControllerName> name = deTObjectReference<aeMCPControllerName>::New();
	deTObjectReference<aeMCPControllerMinimumValue> minimumValue = deTObjectReference<aeMCPControllerMinimumValue>::New();
	deTObjectReference<aeMCPControllerMaximumValue> maximumValue = deTObjectReference<aeMCPControllerMaximumValue>::New();
	deTObjectReference<aeMCPControllerCurrentValue> currentValue = deTObjectReference<aeMCPControllerCurrentValue>::New();
	deTObjectReference<aeMCPControllerVector> vector = deTObjectReference<aeMCPControllerVector>::New();
	deTObjectReference<aeMCPControllerClamp> clamp = deTObjectReference<aeMCPControllerClamp>::New();
	deTObjectReference<aeMCPControllerFrozen> frozen = deTObjectReference<aeMCPControllerFrozen>::New();
	deTObjectReference<aeMCPControllerDefaultValue> defaultValue = deTObjectReference<aeMCPControllerDefaultValue>::New();
	deTObjectReference<aeMCPControllerDefaultVector> defaultVector = deTObjectReference<aeMCPControllerDefaultVector>::New();
	
	deTObjectReference<aeMCPControllerLocomotionAttribute> locomotionAttribute = deTObjectReference<aeMCPControllerLocomotionAttribute>::New();
	deTObjectReference<aeMCPControllerLocomotionLeg> locomotionLeg = deTObjectReference<aeMCPControllerLocomotionLeg>::New();
	deTObjectReference<aeMCPControllerVectorSimulation> vectorSimulation = deTObjectReference<aeMCPControllerVectorSimulation>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupLocomotionTesting = deTObjectReference<igdeMetaPropertyGroup>::New(
		"controller.groupLocomotionTesting", "@Animator.WPController.LocomotionTesting", "@Animator.WPController.LocomotionTesting.ToolTip",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			locomotionAttribute,
			locomotionLeg,
			vectorSimulation));
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			name,
			minimumValue,
			maximumValue,
			currentValue,
			vector,
			clamp,
			frozen,
			defaultValue,
			defaultVector,
			groupLocomotionTesting)
	);
	
	deTObjectReference<aeMCPControllers> controllers = deTObjectReference<aeMCPControllers>::New();
	deTObjectReference<aeMCPController> controller;
	deTObjectReference<igdeMetaPropertyGroup> group = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupControllers", "@Animator.WPController.Controllers", "@Animator.WPController.Controllers.ToolTip",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, controllers, controller));
	
	aeMCControllerProperties(aeWindowMain &windowMain) :
		controller(deTObjectReference<aeMCPController>::New(windowMain)){}
};


/**
 * Animator meta context properties.
 */
class aeMCAnimatorProperties{
public:
	deTObjectReference<aeMCPAnimatorRig> rig = deTObjectReference<aeMCPAnimatorRig>::New();
	deTObjectReference<aeMCPAnimatorAnimation> animation = deTObjectReference<aeMCPAnimatorAnimation>::New();
	deTObjectReference<aeMCPAnimatorAffectedBones> affectedBones = deTObjectReference<aeMCPAnimatorAffectedBones>::New();
	deTObjectReference<aeMCPAnimatorAffectedVertexPositionSets> affectedVertexPositionSets = deTObjectReference<aeMCPAnimatorAffectedVertexPositionSets>::New();
	
	aeMCControllerProperties controller;
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			rig,
			animation,
			affectedBones,
			affectedVertexPositionSets,
			controller.group));
	
	aeMCAnimatorProperties(aeWindowMain &windowMain) :
		controller(windowMain){}
};

#endif
