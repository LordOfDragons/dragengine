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

#include "aeMCPRule.h"
#include "aeMCPShared.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/common/collection/decTList.h>


// Class aeMCPRuleTree
////////////////////////

namespace {

class cWalkerRuleTree : public igdeMetaPropertyTreeList::WalkerHelper<aeRule::List>{
private:
	const aeAnimator::Ref pAnimator;
	
public:
	cWalkerRuleTree(const aeAnimator::Ref &animator) :
	WalkerHelper<aeRule::List>(&animator->mpRules.GetValue()),
	pAnimator(animator){
	}
	
	void SetCurrent(const deObject::Ref &object) override{
		auto rule = object.DynamicCast<aeRule>();
		DEASSERT_TRUE(pAnimator == rule->GetAnimator())
		
		if(rule->GetParentGroup()){
			SetList(&rule->GetParentGroup()->mpRules.GetValue(), rule->GetIndex());
			
		}else{
			SetList(&pAnimator->mpRules.GetValue(), rule->GetIndex());
		}
	}
	
	void MoveTreeFirst() override{
		SetList(&pAnimator->mpRules.GetValue(), 0);
	}
	
	bool HasChildren() const override{
		const auto group = GetCurrent().DynamicCast<aeRuleGroup>();
		return group && group->mpRules->IsNotEmpty();
	}
	
	bool MoveChildren() override{
		const auto group = GetCurrent().DynamicCast<aeRuleGroup>();
		if(!group || group->mpRules->IsEmpty()){
			return false;
		}
		
		SetList(&group->mpRules.GetValue(), 0);
		return true;
	}
	
	bool HasParent() const override{
		const auto rule = GetCurrent().DynamicCast<aeRule>();
		return rule && rule->GetParentGroup();
	}
	
	bool MoveParent() override{
		const auto rule = GetCurrent().DynamicCast<aeRule>();
		if(!rule){
			return false;
		}
		
		const auto parent = rule->GetParentGroup();
		if(!parent){
			return false;
		}
		
		const auto parentParent = parent->GetParentGroup();
		SetList(&(parentParent ? parentParent->mpRules : pAnimator->mpRules).GetValue(), parent->GetIndex());
		return true;
	}
};

class cActionRuleAdd : public igdeMetaProperty::Action{
	aeMCPRuleTree &pPropertyRules;
	
public:
	cActionRuleAdd(aeMCPRuleTree &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	igdeMetaProperty::Action(owner, context, "@Animator.Action.Rule.Add",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.Action.Rule.Add.ToolTip"),
	pPropertyRules(property){
	}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyRules.IsValid(context)){
			return;
		}
		
		auto &owner = GetOwner();
		auto menu = igdeMenuCascade::Ref::New(owner.GetEnvironment());
		pPropertyRules.AddContextMenuEntriesAdd(menu, context, owner);
		if(menu->GetChildren().IsNotEmpty()){
			menu->PopupBottom(owner);
		}
	}
};

class cActionBaseRule : public igdeMetaProperty::Action{
private:
	aeMCPRuleTree &pPropertyRules;
	
public:
	cActionBaseRule(aeMCPRuleTree &property, igdeWidget &owner, const char *translationTag,
		igdeIcon *icon, const igdeMetaContext::Ref &context = {}) :
	Action(owner, context, decString::Formatted("@{0}", translationTag), icon,
		decString::Formatted("@{0}.ToolTip", translationTag)),
	pPropertyRules(property){
	}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(context && pPropertyRules.IsValid(context)){
			SyncSelection();
			CreateAction(pPropertyRules.GetActionProperty(context), pPropertyRules.GetActionContext(context))->OnAction();
		}
	}
	
	void Update() override{
		const auto &context = GetContext();
		if(context && pPropertyRules.IsValid(context)){
			SyncSelection();
			const auto action = CreateAction(pPropertyRules.GetActionProperty(context), pPropertyRules.GetActionContext(context));
			action->Update();
			SetEnabled(action->GetEnabled());
			
		}else{
			SetEnabled(false);
		}
	}
	
	void SyncSelection(){
		const auto &tree = pPropertyRules.Owner(GetContext()).mpRuleTree;
		auto &storage = pPropertyRules.GetActionStorage(GetContext());
		storage.SetActive(storage->Has(tree.GetActive()) ? tree.GetActive() : aeRule::Ref());
	}
	
	inline aeMCPRuleTree &GetPropertyRules() const{ return pPropertyRules; }
	virtual Action::Ref CreateAction(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context) const = 0;
};

class cActionRuleRemove : public cActionBaseRule{
public:
	cActionRuleRemove(aeMCPRuleTree &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	cActionBaseRule(property, owner, "Igde.MetaPropertyList.Action.Remove",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), context){
	}
	
	Action::Ref CreateAction(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context) const override{
		return igdeMetaPropertyList::ActionRemove::Ref::New(property, GetOwner(), context);
	}
};

class cActionRuleMoveUp : public cActionBaseRule{
public:
	cActionRuleMoveUp(aeMCPRuleTree &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	cActionBaseRule(property, owner, "Igde.MetaPropertyList.Action.MoveUp",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), context){
	}
	
	Action::Ref CreateAction(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context) const override{
		return igdeMetaPropertyList::ActionMoveUp::Ref::New(property, GetOwner(), context);
	}
};

class cActionRuleMoveDown : public cActionBaseRule{
public:
	cActionRuleMoveDown(aeMCPRuleTree &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	cActionBaseRule(property, owner, "Igde.MetaPropertyList.Action.MoveDown",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown), context){
	}
	
	Action::Ref CreateAction(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context) const override{
		return igdeMetaPropertyList::ActionMoveDown::Ref::New(property, GetOwner(), context);
	}
};

class cActionRuleCopy : public igdeAction{
protected:
	aeMCPRuleTree &pPropertyRules;
	igdeMetaContext::Ref pContext;
	
public:
	cActionRuleCopy(aeMCPRuleTree &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	igdeAction("@Igde.Action.Copy", owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pPropertyRules(property),
	pContext(context){
	}
	
	~cActionRuleCopy() override = default;
	
	void OnAction() override{
		auto active = pPropertyRules.GetActiveObjectType(pContext);
		if(active){
			pContext->GetClipboard().Set(igdeMetaPropertyList::ClipboardData::Ref::New(
				pPropertyRules.GetClipboardDataTypeName(),
					igdeMetaPropertyList::List(devctag,active->CreateCopy())));
		}
	}
	
	void Update() override{
		SetEnabled(pPropertyRules.IsValid(pContext) && pPropertyRules.GetActiveObject(pContext).IsNotNull());
	}
};

class cActionRuleCut : public cActionRuleRemove{
protected:
	deTObjectReference<cActionRuleCopy> pActionCopy;
	
public:
	cActionRuleCut(aeMCPRuleTree &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	cActionRuleRemove(property, owner, context),
	pActionCopy(deTObjectReference<cActionRuleCopy>::New(property, owner, context))
	{
		SetText("@Igde.Action.Cut");
		SetIcon(owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut));
		SetDescription("@Igde.Action.Cut.ToolTip");
	}
	
	void OnAction() override{
		pActionCopy->OnAction();
		cActionRuleRemove::OnAction();
	}
};

class cActionRulePasteIntoGroup : public igdeMetaPropertyList::ActionPaste{
public:
	cActionRulePasteIntoGroup(igdeMetaPropertyList &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	ActionPaste(property, owner, context){
		SetText("@Animator.WPRule.Action.PasteIntoGroup");
		SetDescription("@Animator.WPRule.Action.PasteIntoGroup.ToolTip");
	}
};

}

igdeMetaPropertyTreeList::Walker::Ref aeMCPRuleTree::CreateWalker(const ContextRef &context) const{
	return deTObjectReference<cWalkerRuleTree>::New(aeAnimator::Ref(&Owner(context)));
}

void aeMCPRuleTree::GetObjectItemInfoType(const ContextRef &context,
const ObjectTypeRef &rule, igdeMetaContextItemInfo &info) const{
	info.SetAll(decString::Formatted("{0}: {1}", rule->GetIndex(), rule->mpName.GetValue()),
		Owner(context).GetWindowMain().GetRuleIcon(rule->GetType()));
}

void aeMCPRuleTree::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &environment = menu.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	
	AddContextMenuEntriesAdd(menu, context, owner);
	
	auto &property = GetActionProperty(context);
	const auto rule = GetActiveRule(context);
	auto ctx = GetActionContext(context);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionDuplicate::Ref::New(property, owner, ctx));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionRemove::Ref::New(property, owner, ctx));
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionRemoveAll::Ref::New(property, owner, ctx));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionCopy::Ref::New(property, owner, ctx));
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionCut::Ref::New(property, owner, ctx));
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionPaste::Ref::New(property, owner, ctx));
	
	if(Owner(context).mpRuleTree.GetActive()){
		helper.MenuCommand(menu, igdeMetaPropertyList::ActionPasteBefore::Ref::New(property, owner, ctx));
		helper.MenuCommand(menu, igdeMetaPropertyList::ActionPasteAfter::Ref::New(property, owner, ctx));
	}
	
	auto ruleGroup = rule.DynamicCast<aeRuleGroup>();
	if(ruleGroup){
		helper.MenuCommand(menu, deTObjectReference<cActionRulePasteIntoGroup>::New(property, owner, ruleGroup->GetMetaContext()));
	}
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionMoveUp::Ref::New(property, owner, ctx));
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionMoveDown::Ref::New(property, owner, ctx));
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionMoveTop::Ref::New(property, owner, ctx));
	helper.MenuCommand(menu, igdeMetaPropertyList::ActionMoveBottom::Ref::New(property, owner, ctx));
}

void aeMCPRuleTree::AddContextMenuEntriesAdd(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &environment = menu.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	const auto rule = GetActiveRule(context);
	
	const aeWindowMain &windowMain = Owner(context).GetWindowMain();
	igdeMenuCascade::Ref submenu(igdeMenuCascade::Ref::New(environment, "@Animator.WPRule.Menu.Add"));
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddAnim());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddAnimDiff());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddAnimSelect());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddBoneRot());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddInvKin());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddStateManip());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddStateSnap());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddForeignState());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddGroup());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddSubAnimator());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddTrackTo());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddLimit());
	helper.MenuCommand(submenu, windowMain.GetActionRuleAddMirror());
	menu.AddChild(submenu);
	
	if(rule.DynamicCast<aeRuleGroup>()){
		submenu = igdeMenuCascade::Ref::New(environment, "@Animator.WPRule.Menu.AddIntoGroup");
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupAnim());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupAnimDiff());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupAnimSelect());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupBoneRot());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupInvKin());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupStateManip());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupStateSnap());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupForeignState());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupGroup());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupSubAnimator());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupTrackTo());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupLimit());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupMirror());
		menu.AddChild(submenu);
	}
	
	if(rule){
		submenu = igdeMenuCascade::Ref::New(environment, "@Animator.WPRule.Menu.Insert");
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertAnim());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertAnimDiff());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertAnimSelect());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertBoneRot());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertInvKin());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertStateManip());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertStateSnap());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertForeignState());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertGroup());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertSubAnimator());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertTrackTo());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertLimit());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertMirror());
		menu.AddChild(submenu);
	}
}

igdeMetaProperty::Action::Ref aeMCPRuleTree::CreateButtonAction(TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return deTObjectReference<cActionRuleAdd>::New(*this, owner);
		
	case TargetButton::remove:
		return deTObjectReference<cActionRuleRemove>::New(*this, owner);
		
	case TargetButton::moveUp:
		return deTObjectReference<cActionRuleMoveUp>::New(*this, owner);
		
	case TargetButton::moveDown:
		return deTObjectReference<cActionRuleMoveDown>::New(*this, owner);
		
	default:
		return {};
	}
}

aeRule::Ref aeMCPRuleTree::GetActiveRule(const ContextRef &context) const{
	return IsValid(context) ? Owner(context).mpRuleTree.GetActive() : aeRule::Ref();
}

igdeMetaPropertyListStorage<aeRule, aeRule::List>::Storage &aeMCPRuleTree::GetActionStorage(const ContextRef &context) const{
	const auto rule = GetActiveRule(context);
	const auto parentGroup = rule ? rule->GetParentGroup() : nullptr;
	return parentGroup ? parentGroup->mpRules : Owner(context).mpRules;
}

igdeMetaPropertyList &aeMCPRuleTree::GetActionProperty(const ContextRef &context) const{
	return GetActionStorage(context).Property();
}

igdeMetaContext::Ref aeMCPRuleTree::GetActionContext(const ContextRef &context) const{
	const auto rule = GetActiveRule(context);
	const auto parentGroup = rule ? rule->GetParentGroup() : nullptr;
	return parentGroup ? parentGroup->GetMetaContext() : Owner(context).GetMetaContext().StaticCast<igdeMetaContext>();
}


// Class aeMCPRules
/////////////////////

void aeMCPRules::GetObjectItemInfoType(const ContextRef &context,
const ObjectTypeRef &rule, igdeMetaContextItemInfo &info) const{
	info.SetAll(decString::Formatted("{0}: {1}", rule->GetIndex(), rule->mpName.GetValue()),
		Owner(context).GetWindowMain().GetRuleIcon(rule->GetType()));
}

aeMCPRules::ObjectTypeRef aeMCPRules::CopyObjectType(const ContextRef &context,
const aeRule::List &existingObjects, const ObjectTypeRef &object) const{
	auto copied = object->CreateCopy();
	copied->mpName.SetValue(Owner(context).uniqueNameRule.Generate(copied->mpName), false);
	return copied;
}


// Class cActionAffectedBonesMirror
/////////////////////////////////////

namespace {

class cActionAffectedBonesMirror : public aeActionMirrorStringSet{
	aeMCPRuleAffectedBones &pProperty;
	
public:
	cActionAffectedBonesMirror(aeMCPRuleAffectedBones &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	aeActionMirrorStringSet(property, owner, context),
	pProperty(property){
		SetText("@Animator.WPAnimator.Action.BoneMirror");
		SetDescription("@Animator.WPAnimator.Action.BoneMirror.ToolTip");
	}
};

class cActionAffectedBonesMirrorSelected : public cActionAffectedBonesMirror{
public:
	cActionAffectedBonesMirrorSelected(aeMCPRuleAffectedBones &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	cActionAffectedBonesMirror(property, owner, context){
		pSelected = true;
		SetText("@Animator.WPAnimator.Action.BoneMirrorSelected");
		SetDescription("@Animator.WPAnimator.Action.BoneMirrorSelected.ToolTip");
	}
};

}

decStringSet aeMCPRuleAffectedBones::GetAllowedStrings(const igdeMetaContext::Ref &context) const{
	const auto animator = Owner(context).GetAnimator();
	return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
}

void aeMCPRuleAffectedBones::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, owner, context));
	AddDefaultContextMenuEntries(menu, context, owner);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedBonesMirror>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedBonesMirrorSelected>::New(*this, owner, context));
}


// Class aeMCPRuleListVertexPositionSets
//////////////////////////////////////////

namespace {

class cActionAffectedVpsMirror : public aeActionMirrorStringSet{
	aeMCPRuleAffectedVertexPositionSets &pProperty;
	
public:
	cActionAffectedVpsMirror(aeMCPRuleAffectedVertexPositionSets &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	aeActionMirrorStringSet(property, owner, context),
	pProperty(property){
		SetText("@Animator.WPAnimator.Action.VPSMirror");
		SetDescription("@Animator.WPAnimator.Action.VPSMirror.ToolTip");
	}
};

class cActionAffectedVpsMirrorSelected : public cActionAffectedVpsMirror{
public:
	cActionAffectedVpsMirrorSelected(aeMCPRuleAffectedVertexPositionSets &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
	cActionAffectedVpsMirror(property, owner, context){
		pSelected = true;
		SetText("@Animator.WPAnimator.Action.VPSMirrorSelected");
		SetDescription("@Animator.WPAnimator.Action.VPSMirrorSelected.ToolTip");
	}
};

}

decStringSet aeMCPRuleAffectedVertexPositionSets::GetAllowedStrings(const igdeMetaContext::Ref &context) const{
	const auto animator = Owner(context).GetAnimator();
	return animator ? animator->mpHiddenVpsNames.GetValue() : decStringSet();
}

void aeMCPRuleAffectedVertexPositionSets::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, owner, context));
	AddDefaultContextMenuEntries(menu, context, owner);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedVpsMirror>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionAffectedVpsMirrorSelected>::New(*this, owner, context));
}
