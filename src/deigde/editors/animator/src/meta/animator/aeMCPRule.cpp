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
#include "../../gui/aeWindowMain.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/common/collection/decTList.h>


// Class aeMCPRules
/////////////////////

namespace {

class cActionRuleAdd : public igdeMetaProperty::Action{
	aeMCPRules &pPropertyRule;
	
public:
	cActionRuleAdd(aeMCPRules &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.Action.Rule.Add",
			owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
			"@Animator.Action.Rule.Add.ToolTip"),
		pPropertyRule(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyRule.IsValid(context)){
			return;
		}
		
		auto &owner = GetOwner();
		auto menu = igdeMenuCascade::Ref::New(owner.GetEnvironment());
		pPropertyRule.AddContextMenuEntries(*menu, context, owner);
		if(menu->GetChildren().IsNotEmpty()){
			menu->PopupBottom(owner);
		}
	}
};

}

aeMCPRules::ObjectTypeRef aeMCPRules::CopyObjectType(const ContextRef &context,
const aeRule::List &existingObjects, const ObjectTypeRef &object) const{
	return object->CreateCopy();
}

igdeMetaProperty::Action::Ref aeMCPRules::CreateButtonAction(TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return deTObjectReference<cActionRuleAdd>::New(*this, owner);
		
	default:
		return CreateDefaultButtonAction(target, owner);
	}
}

void aeMCPRules::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &environment = menu.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	
	const aeWindowMain &windowMain = WindowMain(context);
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
	
	AddDefaultContextMenuEntries(menu, context, owner);
}


// Class aeMCPRuleListBones
/////////////////////////////

decStringSet aeMCPRuleAffectedBones::GetAllowedStrings(const igdeMetaContext::Ref &context) const{
	const auto animator = Rule(context).GetAnimator();
	return animator ? animator->hiddenBoneNames.GetValue() : decStringSet();
}

void aeMCPRuleAffectedBones::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, owner, context));
	AddDefaultContextMenuEntries(menu, context, owner);
}


// Class aeMCPRuleListVertexPositionSets
//////////////////////////////////////////

decStringSet aeMCPRuleAffectedVertexPositionSets::GetAllowedStrings(const igdeMetaContext::Ref &context) const{
	const auto animator = Rule(context).GetAnimator();
	return animator ? animator->hiddenVPSNames.GetValue() : decStringSet();
}

void aeMCPRuleAffectedVertexPositionSets::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, igdeMetaPropertyStringSet::ActionAdd::Ref::New(*this, owner, context));
	AddDefaultContextMenuEntries(menu, context, owner);
}
