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

#include "aeMCPAttachment.h"
#include "../../gui/aeWindowMain.h"
#include "../../loadsave/aeLoadSaveSystem.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/meta/property/undo/igdeMetaPropertySetUndo.h>


// Class aeMCPAttachments
///////////////////////////

namespace {

class cActionAttachmentAdd : public igdeMetaProperty::Action{
	aeMCPAttachments &pPropertyAttachment;
	
public:
	cActionAttachmentAdd(aeMCPAttachments &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	igdeMetaProperty::Action(owner, context, "@Igde.MetaPropertyList.Action.Add",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Igde.MetaPropertyList.Action.Add.ToolTip"),
	pPropertyAttachment(property){
	}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyAttachment.IsValid(context)){
			return;
		}
		
		auto list = pPropertyAttachment.GetPropertyValueType(context);
		list.Add(aeAttachment::Ref::New(pPropertyAttachment.Owner(context).GetWindowMain(), Translate("Animator.DefaultName.Attachment").ToUTF8()));
		pPropertyAttachment.ChangePropertyValueType(context, list, BuildUndoInfo(pPropertyAttachment));
	}
};

class cActionAttachmentLoadConfig : public igdeMetaProperty::Action{
	aeMCPAttachments &pPropertyAttachments;
	
public:
	cActionAttachmentLoadConfig(aeMCPAttachments &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	Action(owner, context, "@Animator.WPView.Action.AttachmentLoadConfig",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"@Animator.WPView.Action.AttachmentLoadConfig.ToolTip"),
	pPropertyAttachments(property){
	}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyAttachments.IsValid(context)){
			return;
		}
		
		auto &animator = pPropertyAttachments.Owner(context);
		auto &windowMain = animator.GetWindowMain();
		auto &lssys = windowMain.GetLoadSaveSystem();
		if(!igdeCommonDialogs::GetFileOpen(windowMain, "@Animator.WPView.Dialog.OpenAttachmentConfig.Title",
		*windowMain.GetEnvironment().GetFileSystemGame(), lssys.GetAttConfigFilePatterns(), animator.pathAttachmentConfig)){
			return;
		}
		
		lssys.LoadAttConfig(animator.pathAttachmentConfig, animator);
	}
};

class cActionAttachmentSaveConfig : public igdeMetaProperty::Action{
	aeMCPAttachments &pPropertyAttachments;
	
public:
	cActionAttachmentSaveConfig(aeMCPAttachments &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	igdeMetaProperty::Action(owner, context, "@Animator.WPView.Action.AttachmentSaveConfig",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"@Animator.WPView.Action.AttachmentSaveConfig.ToolTip"),
	pPropertyAttachments(property){
	}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyAttachments.IsValid(context)){
			return;
		}
		
		auto &animator = pPropertyAttachments.Owner(context);
		auto &windowMain = animator.GetWindowMain();
		auto &lssys = windowMain.GetLoadSaveSystem();
		if(!igdeCommonDialogs::GetFileSave(windowMain, "@Animator.WPView.Dialog.SaveAttachmentConfig.Title",
		*windowMain.GetEnvironment().GetFileSystemGame(), lssys.GetAttConfigFilePatterns(), animator.pathAttachmentConfig)){
			return;
		}
		
		lssys.SaveAttConfig(animator.pathAttachmentConfig, animator);
	}
};

}

aeAttachment::Ref aeMCPAttachments::CopyObjectType(const ContextRef &context,
const SetType &existingObjects, const ObjectTypeRef &object) const{
	auto copied = aeAttachment::Ref::New(Owner(context).GetWindowMain(), *object);
	copied->GetMPName().SetValue(Owner(context).uniqueNameAttachment.Generate(copied->GetMPName()), false);
	return copied;
}

igdeMetaProperty::Action::Ref aeMCPAttachments::CreateButtonAction(TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return deTObjectReference<cActionAttachmentAdd>::New(*this, owner);
		
	default:
		return CreateDefaultButtonAction(target, owner);
	}
}

void aeMCPAttachments::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, deTObjectReference<cActionAttachmentAdd>::New(*this, owner, context));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, deTObjectReference<cActionAttachmentLoadConfig>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionAttachmentSaveConfig>::New(*this, owner, context));
	
	AddDefaultContextMenuEntries(menu, context, owner);
}


// Class aeMCPAttachmentWObject
/////////////////////////////////

aeMCPAttachmentWObject::aeMCPAttachmentWObject() : igdeMetaPropertyMCT("animator.attachment.wobject", "", ""){
	SetProperties(igdeWObject::MetaProperties::global.properties);
}
