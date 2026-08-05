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

#include "aeMCControllerProperties.h"
#include "aeMCAnimatorProperties.h"

#include <deigde/meta/property/igdeMetaPropertyAdapters.h>


void aeMCControllerProperties::Init(aeWindowMain &windowMain){
	name = deTObjectReference<aeMCPControllerName>::New();
	minimumValue = deTObjectReference<aeMCPControllerMinimumValue>::New();
	maximumValue = deTObjectReference<aeMCPControllerMaximumValue>::New();
	currentValue = deTObjectReference<aeMCPControllerCurrentValue>::New();
	vector = deTObjectReference<aeMCPControllerVector>::New();
	clamp = deTObjectReference<aeMCPControllerClamp>::New();
	frozen = deTObjectReference<aeMCPControllerFrozen>::New();
	defaultValue = deTObjectReference<aeMCPControllerDefaultValue>::New();
	defaultVector = deTObjectReference<aeMCPControllerDefaultVector>::New();
	
	locomotionAttribute = deTObjectReference<aeMCPControllerLocomotionAttribute>::New();
	locomotionLeg = deTObjectReference<aeMCPControllerLocomotionLeg>::New();
	vectorSimulation = deTObjectReference<aeMCPControllerVectorSimulation>::New();
	groupLocomotionTesting = deTObjectReference<igdeMetaPropertyGroup>::New(
		"controller.groupLocomotionTesting", "Animator.WPController.LocomotionTesting",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			locomotionAttribute,
			locomotionLeg,
			vectorSimulation));
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			name,
			minimumValue, maximumValue,
			currentValue,
			vector,
			clamp,
			frozen,
			defaultValue, defaultVector,
			groupLocomotionTesting));
	
	controllers = deTObjectReference<aeMCPControllers>::New();
	controller = deTObjectReference<aeMCPController>::New(windowMain);
	
	group = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupControllers", "Animator.WPController.Controllers",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, controllers, controller));
	
	igdeMetaPropertyAdapter::OnChanged(name, controllers);
}
