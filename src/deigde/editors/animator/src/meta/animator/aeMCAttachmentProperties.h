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

#ifndef _AEMCATTACHMENTPROPERTIES_H_
#define _AEMCATTACHMENTPROPERTIES_H_

#include "aeMCPAttachment.h"

#include <deigde/meta/property/igdeMetaPropertyGroup.h>

class aeWindowMain;
class aeMCAnimatorProperties;


class aeMCAttachmentProperties{
public:
	deTObjectReference<aeMCPAttachmentName> name = deTObjectReference<aeMCPAttachmentName>::New();
	deTObjectReference<aeMCPAttachmentAttachType> attachType = deTObjectReference<aeMCPAttachmentAttachType>::New();
	deTObjectReference<aeMCPAttachmentBoneName> boneName = deTObjectReference<aeMCPAttachmentBoneName>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	deTObjectReference<aeMCPAttachments> attachments = deTObjectReference<aeMCPAttachments>::New();
	deTObjectReference<aeMCPAttachment> attachment;
	deTObjectReference<igdeMetaPropertyGroup> group = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupAttachments", "Animator.WPView.Attachments",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, attachments, attachment));
	
	aeMCAttachmentProperties(aeWindowMain &windowMain);
	void Init(const aeMCAnimatorProperties &properties);
};

#endif
