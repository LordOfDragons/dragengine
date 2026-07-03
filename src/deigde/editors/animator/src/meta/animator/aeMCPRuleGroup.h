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

#ifndef _AEMCPRULEGROUP_H_
#define _AEMCPRULEGROUP_H_

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleGroup.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>


class aeMCPRuleGroupEnablePosition : public aeRuleGroup::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnablePosition() : igdeMetaPropertyMCT(
		"animator.rule_group.enablePosition", "Animator.WPAPanelRuleGroup.EnablePosition")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleGroupEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePosition;
	}
};

class aeMCPRuleGroupEnableOrientation : public aeRuleGroup::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnableOrientation() : igdeMetaPropertyMCT(
		"animator.rule_group.enableOrientation", "Animator.WPAPanelRuleGroup.EnableRotation")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleGroupEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableOrientation;
	}
};

class aeMCPRuleGroupEnableSize : public aeRuleGroup::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnableSize() : igdeMetaPropertyMCT(
		"animator.rule_group.enableSize", "Animator.WPAPanelRuleGroup.EnableSize"){}
	
	~aeMCPRuleGroupEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableSize;
	}
};

class aeMCPRuleGroupEnableVertexPositionSet :
	public aeRuleGroup::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnableVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_group.enableVertexPositionSet",
		"Animator.WPAPanelRuleGroup.EnableVertexPositionSet")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleGroupEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableVertexPositionSet;
	}
};

class aeMCPRuleGroupUseCurrentState : public aeRuleGroup::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupUseCurrentState() : igdeMetaPropertyMCT(
		"animator.rule_group.useCurrentState", "Animator.WPAPanelRuleGroup.UseCurrentState"){}
	
	~aeMCPRuleGroupUseCurrentState() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpUseCurrentState;
	}
};

class aeMCPRuleGroupApplicationType :
	public aeRuleGroup::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleGroup::eApplicationTypes>>{
public:
	aeMCPRuleGroupApplicationType() : igdeMetaPropertyMCT(
		"animator.rule_group.applicationType", "Animator.WPAPanelRuleGroup.ApplicationType")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleGroup::eatAll,
			deAnimatorRuleGroup::eatSelect));
		SetDefaultValueEnum(deAnimatorRuleGroup::eatAll);
	}
	
	~aeMCPRuleGroupApplicationType() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpApplicationType;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context,
	deAnimatorRuleGroup::eApplicationTypes choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleGroup::eatAll:
			info.SetAll("@Animator.WPAPanelRuleGroup.ApplicationType.All");
			break;
			
		case deAnimatorRuleGroup::eatSelect:
			info.SetAll("@Animator.WPAPanelRuleGroup.ApplicationType.Select");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


class aeMCPRuleGroupTargetSelect : public aeRule::MetaPropertyTarget<aeRuleGroup>{
public:
	aeMCPRuleGroupTargetSelect() : MetaPropertyTarget(
		"animator.rule_group.targetSelect", "Animator.Target.Select"){}
	~aeMCPRuleGroupTargetSelect() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetSelect;
	}
};


class aeMCPRuleGroupRules :
	public aeRuleGroup::MetaProperty<igdeMetaPropertyListStorage<aeRule, aeRule::List>>{
public:
	aeMCPRuleGroupRules() : igdeMetaPropertyMCT(
		"animator.rule_group.groupRules", "Animator.WPRule.Rules"){
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Rules");
	}
	
	~aeMCPRuleGroupRules() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpRules;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &rule,
	igdeMetaContextItemInfo &info) const override{
		info.SetAll(decString::Formatted("{0}: {1}", rule->GetIndex(), rule->GetName()));
	}
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const aeRule::List &existingObjects,
	const ObjectTypeRef &object) const override{
		auto &rule = Owner(context);
		auto copied = object->CreateCopy();
		copied->mpName.SetValue(rule.uniqueNameRule.Generate(copied->mpName), false);
		return copied;
	}
};

#endif