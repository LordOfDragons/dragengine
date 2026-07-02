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
#include <deigde/meta/property/igdeMetaPropertyTreeList.h>


/** Rules */

class aeMCPRuleTree : public aeAnimator::MetaProperty<igdeMetaPropertyTreeListStorage<aeRule>>{
public:
	aeMCPRuleTree() : igdeMetaPropertyMCT("animator.ruleTree", "Animator.WPRule.Rules"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(8);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Rules");
	}
	
	~aeMCPRuleTree() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPRuleTree();
	}
	
	Walker::Ref CreateWalker(const ContextRef &context) const override;
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &rule, igdeMetaContextItemInfo &info) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
	void AddContextMenuEntriesAdd(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner);
	Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner) override;
	
	aeRule::Ref GetActiveRule(const ContextRef &context) const;
	igdeMetaPropertyListStorage<aeRule, aeRule::List>::Storage &GetActionStorage(const ContextRef &context) const;
	igdeMetaPropertyList &GetActionProperty(const ContextRef &context) const;
	ContextRef GetActionContext(const ContextRef &context) const;
};



class aeMCPRules : public aeAnimator::MetaProperty<igdeMetaPropertyListStorage<aeRule, aeRule::List>>{
public:
	aeMCPRules() : igdeMetaPropertyMCT("animator.rules", "Animator.WPRule.Rules"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(8);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Rules");
	}
	
	~aeMCPRules() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPRules();
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &rule, igdeMetaContextItemInfo &info) const override;
	ObjectTypeRef CopyObjectType(const ContextRef &context, const aeRule::List &existingObjects, const ObjectTypeRef &object) const override;
};


class aeMCPRule : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPRule(aeWindowMain &windowMain) : igdeMetaPropertyMCT("animator.rule", "", ""){
		SetDefaultValue(aeRule::CreateMetaContext(windowMain, nullptr));
	}
	
	~aeMCPRule() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPRule();
	}
};


class aeMCPRuleName : public aeRule::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleName() : igdeMetaPropertyMCT("rule.name", "Animator.WPAPanelRule.Name"){}
	~aeMCPRuleName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPName();
	}
};


class aeMCPRuleBlendMode : public aeRule::MetaProperty<igdeMetaPropertySelectionEnumStorage<deAnimatorRule::eBlendModes>>{
public:
	aeMCPRuleBlendMode() : igdeMetaPropertyMCT("rule.blendMode", "Animator.WPAPanelRule.BlendMode"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRule::ebmBlend,
			deAnimatorRule::ebmOverlay));
		SetDefaultValueEnum(deAnimatorRule::ebmBlend);
	}
	
	~aeMCPRuleBlendMode() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPBlendMode();
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


class aeMCPRuleBlendFactor : public aeRule::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBlendFactor() : igdeMetaPropertyMCT("rule.blendFactor", "Animator.WPAPanelRule.BlendFactor"){
		SetEnableLowerLimit(true);
		SetEnableUpperLimit(true);
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleBlendFactor() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPBlendFactor();
	}
};


class aeMCPRuleInvertBlendFactor : public aeRule::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInvertBlendFactor() : igdeMetaPropertyMCT("rule.invertBlendFactor", "Animator.WPAPanelRule.InvertBlendFactor"){}
	~aeMCPRuleInvertBlendFactor() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPInvertBlendFactor();
	}
};


class aeMCPRuleEnabled : public aeRule::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleEnabled() : igdeMetaPropertyMCT("rule.enabled", "Animator.WPAPanelRule.Enabled"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleEnabled() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnabled();
	}
};


class aeMCPRuleAffectedBones : public aeRule::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPRuleAffectedBones() : igdeMetaPropertyMCT("rule.affectedBones", "Animator.WPAPanelRule.AffectedBones"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPRuleAffectedBones() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPAffectedBones();
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


class aeMCPRuleAffectedVertexPositionSets : public aeRule::MetaProperty<igdeMetaPropertyStringSetStorage>{
public:
	aeMCPRuleAffectedVertexPositionSets() : igdeMetaPropertyMCT("rule.affectedVertexPositionSets", "Animator.WPAPanelRule.AffectedVertexPositionSets"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPRuleAffectedVertexPositionSets() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPAffectedVps();
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const igdeMetaContext::Ref &context, igdeWidget &owner) override;
};


class aeMCPRuleTargetBlendFactor : public aeRule::MetaPropertyTarget<aeRule>{
public:
	aeMCPRuleTargetBlendFactor() : MetaPropertyTarget("rule.targetBlendFactor", "Animator.Target.BlendFactor"){}
	~aeMCPRuleTargetBlendFactor() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetBlendFactor();
	}
};

#endif
