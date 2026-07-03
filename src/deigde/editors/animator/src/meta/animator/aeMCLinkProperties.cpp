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

#include "aeMCLinkProperties.h"
#include "aeMCAnimatorProperties.h"
#include "aeMCControllerProperties.h"

#include <deigde/meta/property/igdeMetaPropertyAdapters.h>


void aeMCLinkProperties::Init(const aeMCAnimatorProperties &properties, aeWindowMain &windowMain){
	name = deTObjectReference<aeMCPLinkName>::New();
	controller = deTObjectReference<aeMCPLinkController>::New();
	repeat = deTObjectReference<aeMCPLinkRepeat>::New();
	bone = deTObjectReference<aeMCPLinkBone>::New();
	boneParameter = deTObjectReference<aeMCPLinkBoneParameter>::New();
	boneMinimum = deTObjectReference<aeMCPLinkBoneMinimum>::New();
	boneMaximum = deTObjectReference<aeMCPLinkBoneMaximum>::New();
	vertexPositionSet = deTObjectReference<aeMCPLinkVertexPositionSet>::New();
	vertexPositionSetMinimum = deTObjectReference<aeMCPLinkVertexPositionSetMinimum>::New();
	vertexPositionSetMaximum = deTObjectReference<aeMCPLinkVertexPositionSetMaximum>::New();
	wrapY = deTObjectReference<aeMCPLinkWrapY>::New();
	curve = deTObjectReference<aeMCPLinkCurve>::New();
	
	metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			name,
			controller,
			repeat,
			bone,
			boneParameter,
			boneMinimum, boneMaximum,
			vertexPositionSet,
			vertexPositionSetMinimum, vertexPositionSetMaximum,
			wrapY,
			curve));
	
	links = deTObjectReference<aeMCPLinks>::New();
	link = deTObjectReference<aeMCPLink>::New(windowMain);
	group = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLinks", "Animator.WPLink.Links",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, links, link));
	
	igdeMetaPropertyAdapter::OnChanged(name, links);
	igdeMetaPropertyAdapter::OnChanged(properties.controller.controllers, controller);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenBoneNames, bone);
	igdeMetaPropertyAdapter::OnChanged(properties.hiddenVPSNames, vertexPositionSet);
}
