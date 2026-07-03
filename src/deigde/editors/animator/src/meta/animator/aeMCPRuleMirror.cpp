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

#include "aeMCPRuleMirror.h"
#include "../../animator/aeAnimator.h"


// aeMCPRuleMirrorMirrorBone
//////////////////////////////

decStringSet aeMCPRuleMirrorMirrorBone::GetPropertyAllowedStrings(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetAnimatorRef().mpHiddenBoneNames.GetValue();
}


// aeMCPRuleMirrorMatchNames
//////////////////////////////

namespace{

class cActionMatchNameAdd : public igdeMetaProperty::Action{
	aeMCPRuleMirrorMatchNames &pProperty;
	
public:
	cActionMatchNameAdd(aeMCPRuleMirrorMatchNames &property, igdeWidget &owner,
		const igdeMetaContext::Ref &context) :
	Action(owner, context, "@Igde.MetaPropertyList.Action.Add",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Igde.MetaPropertyList.Action.Add.ToolTip"),
	pProperty(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pProperty.IsValid(context)){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValueType(context);
		auto matchName = aeRuleMirror::MatchName::Ref::New(pProperty.Owner(context).GetWindowMain());
		newValue.Add(matchName);
		pProperty.ChangePropertyValueType(context, newValue, BuildUndoInfo(pProperty));
		pProperty.SetActiveObjectType(context, matchName);
	}
};

class cActionMatchNameAddTemplate : public igdeMetaProperty::Action{
	aeMCPRuleMirrorMatchNames &pProperty;
	const decString pFirst, pSecond;
	const deAnimatorRuleMirror::eMatchNameType pType;
	
public:
	cActionMatchNameAddTemplate(aeMCPRuleMirrorMatchNames &property, igdeWidget &owner,
		const igdeMetaContext::Ref &context, const char *first, const char *second,
		deAnimatorRuleMirror::eMatchNameType type) :
	Action(owner, context, CreateText(owner.GetEnvironment(), first, second, type),
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.WPAPanelRuleMirror.Action.MatchName.AddPreset"),
	pProperty(property), pFirst(first), pSecond(second), pType(type){}
	
	static decString CreateText(igdeEnvironment &environment, const char *first,
	const char *second, deAnimatorRuleMirror::eMatchNameType type){
		auto &tm = environment.GetTranslationManager();
		return decString::Formatted(tm.Translate("Animator.PanelRuleMirror.ListMatchName").ToUTF8(),
			tm.TranslateIf(aeMCPRuleMirrorMatchNameType::GetEnumText(type)).ToUTF8(), first, second);
	}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pProperty.IsValid(context)){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValueType(context);
		auto matchName = aeRuleMirror::MatchName::Ref::New(
			pProperty.Owner(context).GetWindowMain(), pFirst, pSecond, pType);
		newValue.Add(matchName);
		pProperty.ChangePropertyValueType(context, newValue, BuildUndoInfo(pProperty));
		pProperty.SetActiveObjectType(context, matchName);
	}
};

}

void aeMCPRuleMirrorMatchNames::GetObjectItemInfoType(const ContextRef &context,
const ObjectTypeRef &matchName, igdeMetaContextItemInfo &info) const{
	auto &tm = context->GetEnvironment().GetTranslationManager();
	info.SetAll(decString::Formatted(
		tm.Translate("Animator.PanelRuleMirror.ListMatchName").ToUTF8(),
		tm.TranslateIf(aeMCPRuleMirrorMatchNameType::GetEnumText(matchName->mpType)).ToUTF8(),
		matchName->mpFirst.GetValue(), matchName->mpSecond.GetValue()));
}

void aeMCPRuleMirrorMatchNames::AddContextMenuEntries(igdeMenuCascade &menu,
const ContextRef &context, igdeWidget &owner){
	auto &environment = menu.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, deTObjectReference<cActionMatchNameAdd>::New(*this, owner, context));
	
	auto submenu = igdeMenuCascade::Ref::New(environment,
		"@Animator.WPAPanelRuleMirror.Action.MatchName.AddPreset");
	helper.MenuCommand(submenu, deTObjectReference<cActionMatchNameAddTemplate>::New(
		*this, owner, context, ".l", ".r", deAnimatorRuleMirror::emntLast));
	helper.MenuCommand(submenu, deTObjectReference<cActionMatchNameAddTemplate>::New(
		*this, owner, context, ".L", ".R", deAnimatorRuleMirror::emntLast));
	helper.MenuCommand(submenu, deTObjectReference<cActionMatchNameAddTemplate>::New(
		*this, owner, context, "Left", "Right", deAnimatorRuleMirror::emntMiddle));
	helper.MenuCommand(submenu, deTObjectReference<cActionMatchNameAddTemplate>::New(
		*this, owner, context, "LEFT", "RIGHT", deAnimatorRuleMirror::emntMiddle));
	menu.AddChild(submenu);
}
