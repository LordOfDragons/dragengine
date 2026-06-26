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

#ifndef _AEMCPATTACHMENT_H_
#define _AEMCPATTACHMENT_H_

#include "aeTMCPAnimator.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/attachment/aeAttachment.h"

#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>


/** Attachments. */
class aeMCPAttachments : public aeTMCPAnimator<igdeMetaPropertySetStorage<aeAttachment, aeAnimator::AttachmentSet>>{
public:
	aeMCPAttachments() : aeTMCPAnimator("animator.attachments", "Animator.WPView.Attachments"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(3);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Attachments");
	}
	
	~aeMCPAttachments() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).attachments;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &attachment, igdeMetaContextItemInfo &info) const override{
		info.SetAll(decString(attachment->GetName()));
	}
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const SetType &existingObjects, const ObjectTypeRef &object) const override;
	
	Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Attachment. */
class aeMCPAttachment : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPAttachment(aeWindowMain &windowMain) : aeTMCPAnimator("animator.attachment", "", ""){
		SetDefaultValue(aeMCAttachment::Ref::New(windowMain, nullptr));
	}
	
	~aeMCPAttachment() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).attachment;
	}
};

/** Attachment name. */
class aeMCPAttachmentName : public aeTMCPAnimatorAttachment<igdeMetaPropertyStringStorage>{
public:
	aeMCPAttachmentName() : aeTMCPAnimatorAttachment("animator.attachment.name", "Animator.WPView.AttachmentName"){}
	~aeMCPAttachmentName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Attachment(context).name;
	}
};

/** Attachment attach type. */
class aeMCPAttachmentAttachType : public aeTMCPAnimatorAttachment<igdeMetaPropertySelectionEnumStorage<aeAttachment::eAttachTypes>>{
public:
	aeMCPAttachmentAttachType() : aeTMCPAnimatorAttachment("animator.attachment.attachType", "Animator.WPView.Attach"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			aeAttachment::eatNone,
			aeAttachment::eatBone,
			aeAttachment::eatRig));
		SetDefaultValueEnum(aeAttachment::eatNone);
	}
	~aeMCPAttachmentAttachType() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Attachment(context).attachType;
	}
	
	void GetChoiceItemInfoEnum(const igdeMetaContext::Ref &context, aeAttachment::eAttachTypes choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case aeAttachment::eatNone:
			info.SetAll("None", "@Animator.WPView.AttachType.None");
			break;
			
		case aeAttachment::eatBone:
			info.SetAll("Bone", "@Animator.WPView.AttachType.Bone");
			break;
			
		case aeAttachment::eatRig:
			info.SetAll("Rig", "@Animator.WPView.AttachType.Rig");
			break;
		}
	}
};

/** Attachment bone name. */
class aeMCPAttachmentBoneName : public aeTMCPAnimatorAttachment<igdeMetaPropertyStringStorage>{
public:
	aeMCPAttachmentBoneName() : aeTMCPAnimatorAttachment("animator.attachment.boneName", "Animator.WPView.AttachBone"){
		SetEnableAllowed(true);
	}
	~aeMCPAttachmentBoneName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Attachment(context).boneName;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Attachment(context).GetAnimator();
		return animator ? animator->hiddenBoneNames.GetValue() : decStringSet();
	}
};

/** Attachment WObject. */
class aeMCPAttachmentWObject : public aeTMCPAnimatorAttachment<igdeMetaPropertyContextStorage>{
public:
	aeMCPAttachmentWObject();
	~aeMCPAttachmentWObject() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Attachment(context).wobject;
	}
};

#endif
