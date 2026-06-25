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

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleGroup.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>


class aeMCPRuleGroupEnablePosition : public aeTMCPAnimatorRuleGroup<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnablePosition() : aeTMCPAnimatorRuleType(
	"group.enablePosition", "Animator.WPAPanelRuleGroup.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleGroupEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleGroupEnableOrientation : public aeTMCPAnimatorRuleGroup<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnableOrientation() : aeTMCPAnimatorRuleType(
	"group.enableOrientation", "Animator.WPAPanelRuleGroup.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleGroupEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleGroupEnableSize : public aeTMCPAnimatorRuleGroup<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnableSize() : aeTMCPAnimatorRuleType(
	"group.enableSize", "Animator.WPAPanelRuleGroup.EnableSize"){}
	
	~aeMCPRuleGroupEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleGroupEnableVertexPositionSet : public aeTMCPAnimatorRuleGroup<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupEnableVertexPositionSet() : aeTMCPAnimatorRuleType(
	"group.enableVertexPositionSet", "Animator.WPAPanelRuleGroup.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleGroupEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};

class aeMCPRuleGroupUseCurrentState : public aeTMCPAnimatorRuleGroup<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleGroupUseCurrentState() : aeTMCPAnimatorRuleType(
	"group.useCurrentState", "Animator.WPAPanelRuleGroup.UseCurrentState"){}
	
	~aeMCPRuleGroupUseCurrentState() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).useCurrentState;
	}
};

class aeMCPRuleGroupApplicationType : public aeTMCPAnimatorRuleGroup<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleGroup::eApplicationTypes>>{
public:
	aeMCPRuleGroupApplicationType() : aeTMCPAnimatorRuleType(
	"group.applicationType", "Animator.WPAPanelRuleGroup.ApplicationType"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleGroup::eatAll,
			deAnimatorRuleGroup::eatSelect));
		SetDefaultValueEnum(deAnimatorRuleGroup::eatAll);
	}
	
	~aeMCPRuleGroupApplicationType() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).applicationType;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleGroup::eApplicationTypes choice, igdeMetaContextItemInfo &info) const override{
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


class aeMCPRuleGroupTargetSelect : public aeTMCPRuleTarget<aeTMCPAnimatorRuleGroup<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleGroupTargetSelect() : aeTMCPRuleTarget("group.targetSelect", "Animator.Target.Select"){
		SetRows(3);
	}
	
	~aeMCPRuleGroupTargetSelect() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetSelect;
	}
};


class aeMCPRuleGroupRules : public aeTMCPAnimatorRuleGroup<igdeMetaPropertyListStorage<aeRule, aeRule::List>>{
public:
	aeMCPRuleGroupRules() : aeTMCPAnimatorRuleType("ruleGroup.rules", "Animator.WPRule.Rules"){
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Controllers");
	}
	
	~aeMCPRuleGroupRules() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).rules;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &rule, igdeMetaContextItemInfo &info) const override{
		info.SetAll(decString::Formatted("{0}: {1}", rule->GetIndex(), rule->GetName()));
	}
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const aeRule::List &existingObjects, const ObjectTypeRef &object) const override{
		auto copied = object->CreateCopy(WindowMain(context));
		copied->name.SetValue(RuleType(context).uniqueNameRule.Generate(copied->name), false);
		return copied;
	}
};

#endif