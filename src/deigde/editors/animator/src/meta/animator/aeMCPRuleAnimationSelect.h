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

#ifndef _AEMCPRULEANIMATIONSELECT_H_
#define _AEMCPRULEANIMATIONSELECT_H_

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleAnimationSelect.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyStringList.h>


class aeMCPRuleAnimationSelectEnablePosition : public aeRuleAnimationSelect::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnablePosition() : igdeMetaPropertyMCT(
	"animationselect.enablePosition", "Animator.WPAPanelRuleAnimationSelect.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationSelectEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePosition();
	}
};

class aeMCPRuleAnimationSelectEnableOrientation : public aeRuleAnimationSelect::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnableOrientation() : igdeMetaPropertyMCT(
	"animationselect.enableOrientation", "Animator.WPAPanelRuleAnimationSelect.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationSelectEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableOrientation();
	}
};

class aeMCPRuleAnimationSelectEnableSize : public aeRuleAnimationSelect::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnableSize() : igdeMetaPropertyMCT(
	"animationselect.enableSize", "Animator.WPAPanelRuleAnimationSelect.EnableSize"){}
	
	~aeMCPRuleAnimationSelectEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableSize();
	}
};

class aeMCPRuleAnimationSelectEnableVertexPositionSet : public aeRuleAnimationSelect::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnableVertexPositionSet() : igdeMetaPropertyMCT(
	"animationselect.enableVertexPositionSet", "Animator.WPAPanelRuleAnimationSelect.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationSelectEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSet();
	}
};

class aeMCPRuleAnimationSelectMoves : public aeRuleAnimationSelect::MetaProperty<igdeMetaPropertyStringListStorage>{
public:
	aeMCPRuleAnimationSelectMoves() : igdeMetaPropertyMCT("animationselect.moves", "Animator.WPAPanelRuleAnimationSelect.Moves"){
		SetMultiSelection(true);
		SetRows(5);
	}
	
	~aeMCPRuleAnimationSelectMoves() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMoves();
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenMoveNames().GetValue() : decStringSet();
	}
	
	void AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner) override{
		auto &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, igdeMetaPropertyStringList::ActionAdd::Ref::New(*this, owner, context));
		AddDefaultContextMenuEntries(menu, context, owner);
	}
};


class aeMCPRuleAnimationSelectTargetMoveTime : public aeRule::MetaPropertyTarget<aeRuleAnimationSelect>{
public:
	aeMCPRuleAnimationSelectTargetMoveTime() : MetaPropertyTarget("animationselect.targetMoveTime", "Animator.Target.MoveTime"){}
	~aeMCPRuleAnimationSelectTargetMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetLeadMoveTime();
	}
};

class aeMCPRuleAnimationSelectTargetSelect : public aeRule::MetaPropertyTarget<aeRuleAnimationSelect>{
public:
	aeMCPRuleAnimationSelectTargetSelect() : MetaPropertyTarget("animationselect.targetSelect", "Animator.Target.Select"){}
	~aeMCPRuleAnimationSelectTargetSelect() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetSelect();
	}
};


#endif