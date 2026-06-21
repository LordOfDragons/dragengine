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

#ifndef _AEMCLINKPROPERTIES_H_
#define _AEMCLINKPROPERTIES_H_

#include "aeMCPLink.h"
#include "aeMCListeners.h"

#include <deigde/meta/property/igdeMetaPropertyGroup.h>

class aeWindowMain;
class aeMCAnimatorProperties;


class aeMCLinkProperties{
public:
	deTObjectReference<aeMCPLinkName> name = deTObjectReference<aeMCPLinkName>::New();
	deTObjectReference<aeMCPLinkController> controller = deTObjectReference<aeMCPLinkController>::New();
	deTObjectReference<aeMCPLinkRepeat> repeat = deTObjectReference<aeMCPLinkRepeat>::New();
	deTObjectReference<aeMCPLinkBone> bone = deTObjectReference<aeMCPLinkBone>::New();
	deTObjectReference<aeMCPLinkBoneParameter> boneParameter = deTObjectReference<aeMCPLinkBoneParameter>::New();
	deTObjectReference<aeMCPLinkBoneMinimum> boneMinimum = deTObjectReference<aeMCPLinkBoneMinimum>::New();
	deTObjectReference<aeMCPLinkBoneMaximum> boneMaximum = deTObjectReference<aeMCPLinkBoneMaximum>::New();
	deTObjectReference<aeMCPLinkVertexPositionSet> vertexPositionSet = deTObjectReference<aeMCPLinkVertexPositionSet>::New();
	deTObjectReference<aeMCPLinkVertexPositionSetMinimum> vertexPositionSetMinimum = deTObjectReference<aeMCPLinkVertexPositionSetMinimum>::New();
	deTObjectReference<aeMCPLinkVertexPositionSetMaximum> vertexPositionSetMaximum = deTObjectReference<aeMCPLinkVertexPositionSetMaximum>::New();
	deTObjectReference<aeMCPLinkWrapY> wrapY = deTObjectReference<aeMCPLinkWrapY>::New();
	deTObjectReference<aeMCPLinkCurve> curve = deTObjectReference<aeMCPLinkCurve>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New(
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			name,
			controller,
			repeat,
			bone,
			boneParameter,
			boneMinimum,
			boneMaximum,
			vertexPositionSet,
			vertexPositionSetMinimum,
			vertexPositionSetMaximum,
			wrapY,
			curve));
	
	deTObjectReference<aeMCPLinks> links = deTObjectReference<aeMCPLinks>::New();
	deTObjectReference<aeMCPLink> link;
	deTObjectReference<igdeMetaPropertyGroup> group = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLinks", "Animator.WPLink.Links",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, links, link));
	
	aeMCLinkProperties(aeWindowMain &windowMain);
	void Init(const aeMCAnimatorProperties &properties);
};

#endif
