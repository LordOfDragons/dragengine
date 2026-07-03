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
#include <deigde/meta/property/undo/igdeMetaPropertyCurveBezierUndo.h>
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
		igdeMetaPropertyObjectSetStorage<aeLink>::Storage *target = nullptr;
		aeLink::Ref link;
	};
	
private:
	decTList<sRuleRemoved> pRulesRemoved;
	
public:
	cUndoSetLinks(aeMCPLinks &property, const igdeMetaContext::Ref &context,
	const igdeMetaPropertyList::List &newValue, const char *undoInfo, const char *undoInfoLong) :
	igdeMetaPropertyListUndo(property, context, newValue, undoInfo, undoInfoLong)
	{
		const auto &animator = property.Owner(context);
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
			*t.target = t.target->GetValue() + igdeMetaPropertyObjectSetStorage<aeLink>::SetType(devctag, t.link);
		});
	}
	
	void Redo() override{
		pRulesRemoved.Visit([&](const sRuleRemoved &t){
			*t.target = t.target->GetValue() - igdeMetaPropertyObjectSetStorage<aeLink>::SetType(devctag, t.link);
		});
		igdeMetaPropertyListUndo::Redo();
	}
	
private:
	void pProcessTargets(const aeLink::Ref &link, const aeRule::Ref &rule){
		auto process = [&](igdeMetaPropertyObjectSetStorage<aeLink>::Storage &target){
			if(target->Has(link)){
				pRulesRemoved.Add({rule, &target, link});
			}
		};
		
		process(rule->mpTargetBlendFactor);
		
		switch(rule->GetType()){
		case deAnimatorRuleVisitorIdentify::ertAnimation:{
			const auto r = rule.DynamicCast<aeRuleAnimation>();
			process(r->mpTargetMoveTime);
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertAnimationDifference:{
			const auto r = rule.DynamicCast<aeRuleAnimationDifference>();
			process(r->mpTargetLeadingMoveTime);
			process(r->mpTargetReferenceMoveTime);
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertAnimationSelect:{
			const auto r = rule.DynamicCast<aeRuleAnimationSelect>();
			process(r->mpTargetMoveTime);
			process(r->mpTargetSelect);
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertBoneTransformator:{
			const auto r = rule.DynamicCast<aeRuleBoneTransformator>();
			process(r->mpTargetRotation);
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertForeignState:{
			const auto r = rule.DynamicCast<aeRuleForeignState>();
			process(r->mpTargetPosition);
			process(r->mpTargetOrientation);
			process(r->mpTargetSize);
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertGroup:{
			const auto r = rule.DynamicCast<aeRuleGroup>();
			process(r->mpTargetSelect);
			r->GetRules().Visit([&](const aeRule::Ref &subRule){
				pProcessTargets(link, subRule);
			});
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertInverseKinematic:{
			const auto r = rule.DynamicCast<aeRuleInverseKinematic>();
			process(r->mpTargetGoalPosition);
			process(r->mpTargetGoalOrientation);
			process(r->mpTargetLocalPosition);
			process(r->mpTargetLocalOrientation);
			process(r->mpTargetReachRange);
			process(r->mpTargetReachCenter);
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertStateManipulator:{
			const auto r = rule.DynamicCast<aeRuleStateManipulator>();
			process(r->mpTargetPosition);
			process(r->mpTargetRotation);
			}break;
			
		case deAnimatorRuleVisitorIdentify::ertTrackTo:{
			const auto r = rule.DynamicCast<aeRuleTrackTo>();
			process(r->mpTargetPosition);
			process(r->mpTargetUp);
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
		list.Add(aeLink::Ref::New(pPropertyLink.Owner(context).GetWindowMain(), name));
		pPropertyLink.ChangePropertyValueType(context, list, BuildUndoInfo(pPropertyLink));
	}
};

class cActionLinkRemoveUnused : public igdeMetaProperty::Action{
	aeMCPLinks &pPropertyLink;
	
public:
	cActionLinkRemoveUnused(aeMCPLinks &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.Action.Link.RemoveUnused",
			owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
			"@Animator.Action.Link.RemoveUnused.ToolTip"),
		pPropertyLink(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyLink.IsValid(context)){
			return;
		}
		
		auto &animator = pPropertyLink.Owner(context);
		auto newValue = animator.mpLinks->Collect([&](aeLink *link){
			return animator.CountLinkUsage(link) > 0;
		});
		
		if(newValue == pPropertyLink.GetStorage(context).GetValue()){
			return;
		}
		
		pPropertyLink.ChangePropertyValueType(context, newValue, BuildUndoInfo(pPropertyLink));
	}
};

}

igdeMetaPropertyListUndo::Ref aeMCPLinks::ChangePropertyValue(const ContextRef &context,
const List &newValue, const char *undoInfo, const char *undoInfoLong){
	const auto undo = deTObjectReference<cUndoSetLinks>::New(*this, context, newValue, undoInfo, undoInfoLong);
	
	const auto &rulesRemoved = undo->GetRulesRemoved();
	if(rulesRemoved.IsNotEmpty()){
		decStringSet ruleNames, linkNames;
		rulesRemoved.Visit([&](const cUndoSetLinks::sRuleRemoved &each){
			if(each.rule){
				ruleNames.Add(decString::Formatted("'{}'", each.rule->GetName()));
			}
			if(each.link){
				linkNames.Add(decString::Formatted("'{}'", each.link->GetName()));
			}
		});
		
		auto ruleNamesSorted = decStringList(ruleNames).GetSortedAscending();
		auto linkNamesSorted = decStringList(linkNames).GetSortedAscending();
		auto strRuleNames = ruleNamesSorted.GetCount() > 5
			? DEJoin(ruleNamesSorted.GetHead(5), ", ") + ", ..."
			: DEJoin(ruleNamesSorted, ", ");
		auto strLinkNames = linkNamesSorted.GetCount() > 5
			? DEJoin(linkNamesSorted.GetHead(5), ", ") + ", ..."
			: DEJoin(linkNamesSorted, ", ");
		
		if(igdeCommonDialogs::QuestionFormat(Owner(context).GetWindowMain(), igdeCommonDialogs::ebsYesNo,
		"@Animator.Dialog.RemoveLink.Title", "@Animator.Dialog.RemoveLink.Message",
		strLinkNames.GetString(), ruleNames.GetCount(), strRuleNames.GetString()) != igdeCommonDialogs::ebYes){
			return {};
		}
	}
	
	if(context->GetUndoSystem() && GetCanUndo()){
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		undo->Redo();
		return {};
	}
}

aeMCPLinks::ObjectTypeRef aeMCPLinks::CopyObjectType(const ContextRef &context, const aeLink::List &existingObjects, const ObjectTypeRef &object) const{
	auto copied = aeLink::Ref::New(*object);
	copied->mpName.SetValue(Owner(context).uniqueNameLink.Generate(copied->mpName), false);
	return copied;
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
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, deTObjectReference<cActionLinkAdd>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionLinkRemoveUnused>::New(*this, owner, context));
	AddDefaultContextMenuEntries(menu, context, owner);
}


// Class aeMCPLinkCurve
/////////////////////////

namespace {

class cActionCurveInsertAt : public igdeMetaProperty::Action{
	aeMCPLinkCurve &pPropertyCurve;
	
public:
	cActionCurveInsertAt(aeMCPLinkCurve &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
	igdeMetaProperty::Action(owner, context, "@Animator.WPLink.Action.CurveInsertAt",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.WPLink.Action.CurveInsertAt.ToolTip"),
	pPropertyCurve(property){
	}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyCurve.IsValid(context)){
			return;
		}
		
		const auto &link = pPropertyCurve.Owner(context);
		const auto &controller = link.GetController();
		if(!controller){
			return;
		}
		
		const float x = decMath::linearStep(controller->mpCurrentValue,
			controller->mpMinimumValue, controller->mpMaximumValue);
		float y = 0.0f;
		if(!igdeCommonDialogs::GetFloat(GetOwner(), "@Animator.WPLink.Dialog.InsertCurveValue.Title",
		"@Animator.WPLink.Dialog.InsertCurveValue.YValue", y)){
			return;
		}
		
		auto curve = link.GetCurve();
		curve.AddPoint(decVector2(x, y));
		
		pPropertyCurve.ChangePropertyValue(context, curve, BuildUndoInfo(pPropertyCurve));
	}
};

}

void aeMCPLinkCurve::AddContextMenuEntries(igdeMenuCascade &menu, const ContextRef &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, deTObjectReference<cActionCurveInsertAt>::New(*this, owner, context));
}
