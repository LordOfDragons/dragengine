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

#ifndef _AEMCPRULE_H_
#define _AEMCPRULE_H_

#include "aeTMCPAnimator.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/link/aeLink.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyList.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


/** Rules */

class aeMCPRules : public aeTMCPAnimator<igdeMetaPropertyListStorage<aeRule, aeRule::List>>{
public:
	aeMCPRules() : aeTMCPAnimator("animator.rules", "Animator.WPRule.Rules"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(8);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Rules");
	}
	
	~aeMCPRules() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).rules;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &rule, igdeMetaContextItemInfo &info) const override{
		info.SetAll(decString::Formatted("{0}: {1}", rule->GetIndex(), rule->GetName()));
	}
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const aeRule::List &existingObjects, const ObjectTypeRef &object) const override;
	
	Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


class aeMCPRule : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPRule(aeWindowMain &windowMain) : aeTMCPAnimator("animator.rule", "", ""){
		SetDefaultValue(aeMCRule::Ref::New(windowMain, nullptr));
	}
	
	~aeMCPRule() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).rule;
	}
};


class aeMCPRuleName : public aeTMCPAnimatorRule<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleName() : aeTMCPAnimatorRule("rule.name", "Animator.WPAPanelRule.Name"){}
	~aeMCPRuleName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).name;
	}
};


class aeMCPRuleBlendMode : public aeTMCPAnimatorRule<igdeMetaPropertySelectionEnumStorage<deAnimatorRule::eBlendModes>>{
public:
	aeMCPRuleBlendMode() : aeTMCPAnimatorRule("rule.blendMode", "Animator.WPAPanelRule.BlendMode"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRule::ebmBlend,
			deAnimatorRule::ebmOverlay));
		SetDefaultValueEnum(deAnimatorRule::ebmBlend);
	}
	
	~aeMCPRuleBlendMode() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).blendMode;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRule::eBlendModes choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRule::ebmBlend:
			info.SetAll("@Animator.WPAPanelRule.BlendMode.Blend");
			break;
			
		case deAnimatorRule::ebmOverlay:
			info.SetAll("@Animator.WPAPanelRule.BlendMode.Overlay");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


class aeMCPRuleBlendFactor : public aeTMCPAnimatorRule<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBlendFactor() : aeTMCPAnimatorRule("rule.blendFactor", "Animator.WPAPanelRule.BlendFactor"){
		SetEnableLowerLimit(true);
		SetEnableUpperLimit(true);
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleBlendFactor() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).blendFactor;
	}
};


class aeMCPRuleInvertBlendFactor : public aeTMCPAnimatorRule<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInvertBlendFactor() : aeTMCPAnimatorRule("rule.invertBlendFactor", "Animator.WPAPanelRule.InvertBlendFactor"){}
	~aeMCPRuleInvertBlendFactor() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).invertBlendFactor;
	}
};


class aeMCPRuleEnabled : public aeTMCPAnimatorRule<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleEnabled() : aeTMCPAnimatorRule("rule.enabled", "Animator.WPAPanelRule.Enabled"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleEnabled() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).enabled;
	}
};


class aeMCPRuleAffectedBones : public aeTMCPAnimatorRule<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPRuleAffectedBones() : aeTMCPAnimatorRule("rule.affectedBones", "Animator.WPAPanelRule.AffectedBones"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPRuleAffectedBones() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).affectedBones;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


class aeMCPRuleAffectedVertexPositionSets : public aeTMCPAnimatorRule<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPRuleAffectedVertexPositionSets() : aeTMCPAnimatorRule("rule.affectedVertexPositionSets", "Animator.WPAPanelRule.AffectedVertexPositionSets"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPRuleAffectedVertexPositionSets() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).affectedVertexPositionSets;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


template <typename T>
class aeTMCPRuleTarget : public T{
public:
	template <typename... A>
	aeTMCPRuleTarget(A&&... args) : T(std::forward<A>(args)...){
		T::SetRows(2);
		T::SetMultiSelection(true);
	}
	
	~aeTMCPRuleTarget() override = default;
	
	void GetObjectItemInfoType(const igdeMetaContext::Ref&, const typename T::ObjectTypeRef &link, igdeMetaContextItemInfo &info) const override{
		info.SetAll(link->GetName());
	}
	
	typename T::SetType GetValidObjectsType(const igdeMetaContext::Ref &context) const override{
		return typename T::SetType(T::Rule(context).GetAnimatorRef().links.GetValue());
	}
	
	void AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner) override{
		auto &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, igdeMetaPropertyObjectSet::ActionAdd::Ref::New(*this, owner, context));
		T::AddDefaultContextMenuEntries(menu, context, owner);
	}
};


class aeMCPRuleTargetBlendFactor : public aeTMCPRuleTarget<aeTMCPAnimatorRule<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleTargetBlendFactor() : aeTMCPRuleTarget("rule.targetBlendFactor", "Animator.Target.BlendFactor"){
		SetRows(3);
	}
	
	~aeMCPRuleTargetBlendFactor() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Rule(context).targetBlendFactor;
	}
};

#endif
