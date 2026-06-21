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

#include "aeMCPLink.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/rule/aeRuleAnimation.h"
#include "../../animator/rule/aeRuleAnimationDifference.h"
#include "../../animator/rule/aeRuleAnimationSelect.h"
#include "../../animator/rule/aeRuleBoneTransformator.h"
#include "../../animator/rule/aeRuleForeignState.h"
#include "../../animator/rule/aeRuleGroup.h"
#include "../../animator/rule/aeRuleInverseKinematic.h"
#include "../../animator/rule/aeRuleStateManipulator.h"
#include "../../animator/rule/aeRuleTrackTo.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/meta/property/undo/igdeMetaPropertyListUndo.h>

#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>


// Class aeMCPLinks
/////////////////////

namespace {

class cUndoSetLinks : public igdeMetaPropertyListUndo{
public:
	struct sRuleRemoved{
		aeRule::Ref rule;
		aeControllerTarget::Ref target;
		aeLink::Ref link;
	};
	
private:
	decTList<sRuleRemoved> pRulesRemoved;
	
public:
	cUndoSetLinks(aeMCPLinks &property, const igdeMetaContext::Ref &context,
	const igdeMetaPropertyList::List &newValue, const char *undoInfo, const char *undoInfoLong) :
	igdeMetaPropertyListUndo(property, context, newValue, undoInfo, undoInfoLong)
	{
		const auto &animator = property.Animator(context);
		const auto oldLinks = property.ConvertList(GetOldValue());
		const auto newLinks = property.ConvertList(GetNewValue());
		const auto &rules = animator.GetRules();
		
		oldLinks.Visit([&](const aeLink::Ref &link){
			if(newLinks.Has(link)){
				return;
			}
			
			rules.Visit([&](const aeRule::Ref &rule){
				pProcessTargets(link, rule);
			});
		});
	}
	
	inline const decTList<sRuleRemoved> &GetRulesRemoved() const{ return pRulesRemoved; }
	
	void Undo() override{
		igdeMetaPropertyListUndo::Undo();
		pRulesRemoved.Visit([&](const sRuleRemoved &t){
			t.target->AddLink(t.link);
			t.rule->NotifyRuleChanged();
		});
	}
	
	void Redo() override{
		pRulesRemoved.Visit([&](const sRuleRemoved &t){
			t.target->RemoveLink(t.link);
			t.rule->NotifyRuleChanged();
		});
		igdeMetaPropertyListUndo::Redo();
	}
	
private:
	void pProcessTargets(const aeLink::Ref &link, const aeRule::Ref &rule){
		auto process = [&](const aeControllerTarget::Ref &target){
			if(target->GetLinks().Has(link)){
				pRulesRemoved.Add({rule, target, link});
			}
		};
		
		process(rule->GetTargetBlendFactor());
		
		switch(rule->GetType()){
		case deAnimatorRuleVisitorIdentify::ertAnimation:{
			const auto r = rule.DynamicCast<aeRuleAnimation>();
			process(r->GetTargetMoveTime());
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertAnimationDifference:{
			const auto r = rule.DynamicCast<aeRuleAnimationDifference>();
			process(r->GetTargetLeadingMoveTime());
			process(r->GetTargetReferenceMoveTime());

			}break;
			
		case deAnimatorRuleVisitorIdentify::ertAnimationSelect:{
			const auto r = rule.DynamicCast<aeRuleAnimationSelect>();
			process(r->GetTargetMoveTime());
			process(r->GetTargetSelect());
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertBoneTransformator:{
			const auto r = rule.DynamicCast<aeRuleBoneTransformator>();
			process(r->GetTargetRotation());
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertForeignState:{
			const auto r = rule.DynamicCast<aeRuleForeignState>();
			process(r->GetTargetPosition());
			process(r->GetTargetOrientation());
			process(r->GetTargetSize());
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertGroup:{
			const auto r = rule.DynamicCast<aeRuleGroup>();
			process(r->GetTargetSelect());
			r->GetRules().Visit([&](const aeRule::Ref &subRule){
				pProcessTargets(link, subRule);
			});
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertInverseKinematic:{
			const auto r = rule.DynamicCast<aeRuleInverseKinematic>();
			process(r->GetTargetGoalPosition());
			process(r->GetTargetGoalOrientation());
			process(r->GetTargetLocalPosition());
			process(r->GetTargetLocalOrientation());
			process(r->GetTargetReachRange());
			process(r->GetTargetReachCenter());
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertStateManipulator:{
			const auto r = rule.DynamicCast<aeRuleStateManipulator>();
			process(r->GetTargetPosition());
			process(r->GetTargetRotation());
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertTrackTo:{
			const auto r = rule.DynamicCast<aeRuleTrackTo>();
			process(r->GetTargetPosition());
			process(r->GetTargetUp());
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertStateSnapshot:
		case deAnimatorRuleVisitorIdentify::ertSubAnimator:
		case deAnimatorRuleVisitorIdentify::ertLimit:
		case deAnimatorRuleVisitorIdentify::ertMirror:
		case deAnimatorRuleVisitorIdentify::ertUnknown:
			break;
		}
	}
};

class cActionLinkAdd : public igdeMetaProperty::Action{
	aeMCPLinks &pPropertyLink;
	
public:
	cActionLinkAdd(aeMCPLinks &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.Action.Link.Add",
			owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
			"@Animator.Action.Link.Add.ToolTip"),
		pPropertyLink(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyLink.IsValid(context)){
			return;
		}
		
		decString name(Translate("Animator.DefaultName.Link").ToUTF8());
		if(!igdeCommonDialogs::GetString(GetOwner(), "@Animator.Dialog.AddLink.Title",
		"@Animator.Dialog.AddLink.Name", name)){
			return;
		}
		
		auto list = pPropertyLink.GetStorage(context).GetValue();
		list.Add(aeLink::Ref::New(pPropertyLink.WindowMain(context), name));
		pPropertyLink.ChangePropertyValueType(context, list, BuildUndoInfo(pPropertyLink));
	}
};

}

igdeMetaPropertyListUndo::Ref aeMCPLinks::ChangePropertyValue(const ContextRef &context,
const List &newValue, const char *undoInfo, const char *undoInfoLong){
	const auto undo = deTObjectReference<cUndoSetLinks>::New(*this, context, newValue, undoInfo, undoInfoLong);
	
	const auto &rulesRemoved = undo->GetRulesRemoved();
	if(rulesRemoved.IsNotEmpty()){
		decStringList names;
		rulesRemoved.Visit([&](const cUndoSetLinks::sRuleRemoved &each){
			if(each.rule){
				names.Add(each.rule->GetName());
			}
		});
		names.SortAscending();
		auto strNames = names.GetCount() > 5 ? DEJoin(names.GetHead(5), ", ") + ", ..." : DEJoin(names, ", ");
		
		if(igdeCommonDialogs::QuestionFormat(WindowMain(context), igdeCommonDialogs::ebsYesNo,
		"@Animator.Dialog.RemoveLink.Title", "@Animator.Dialog.RemoveLink.Message",
		names.GetCount(), strNames.GetString()) != igdeCommonDialogs::ebYes){
			return {};
		}
	}
	
	if(context->GetUndoSystem()){
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		undo->Redo();
		return {};
	}
}

aeMCPLinks::ObjectTypeRef aeMCPLinks::CopyObjectType(const ContextRef &context, const aeLink::List &existingObjects, const ObjectTypeRef &object) const{
	return aeLink::Ref::New(WindowMain(context), *object);
}

igdeMetaProperty::Action::Ref aeMCPLinks::CreateButtonAction(TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return deTObjectReference<cActionLinkAdd>::New(*this, owner);
		
	default:
		return CreateDefaultButtonAction(target, owner);
	}
}

void aeMCPLinks::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	const auto &windowMain = Animator(context).GetWindowMain();
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, windowMain.GetActionLinkAdd());
	helper.MenuCommand(menu, windowMain.GetActionLinkDuplicate());
	AddDefaultContextMenuEntries(menu, context, owner);
}
