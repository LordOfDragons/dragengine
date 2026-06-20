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

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleAnimationSelect.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyStringList.h>


class aeMCPRuleAnimationSelectEnablePosition : public aeTMCPAnimatorRuleAnimationSelect<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnablePosition() : aeTMCPAnimatorRuleType("animationselect.enablePosition",
		"@Animator.WPAPanelRuleAnimationSelect.EnablePosition", "@Animator.WPAPanelRuleAnimationSelect.EnablePosition.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationSelectEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleAnimationSelectEnableOrientation : public aeTMCPAnimatorRuleAnimationSelect<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnableOrientation() : aeTMCPAnimatorRuleType("animationselect.enableOrientation",
		"@Animator.WPAPanelRuleAnimationSelect.EnableRotation", "@Animator.WPAPanelRuleAnimationSelect.EnableRotation.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationSelectEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleAnimationSelectEnableSize : public aeTMCPAnimatorRuleAnimationSelect<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnableSize() : aeTMCPAnimatorRuleType("animationselect.enableSize",
		"@Animator.WPAPanelRuleAnimationSelect.EnableSize", "@Animator.WPAPanelRuleAnimationSelect.EnableSize.ToolTip"){
	};
	
	~aeMCPRuleAnimationSelectEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleAnimationSelectEnableVertexPositionSet : public aeTMCPAnimatorRuleAnimationSelect<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationSelectEnableVertexPositionSet() : aeTMCPAnimatorRuleType("animationselect.enableVertexPositionSet",
		"@Animator.WPAPanelRuleAnimationSelect.EnableVertexPositionSet", "@Animator.WPAPanelRuleAnimationSelect.EnableVertexPositionSet.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationSelectEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};

class aeMCPRuleAnimationSelectMoves : public aeTMCPAnimatorRuleAnimationSelect<igdeMetaPropertyStringListStorage>{
public:
	aeMCPRuleAnimationSelectMoves() : aeTMCPAnimatorRuleType("animationselect.moves",
		"@Animator.WPAPanelRuleAnimationSelect.Moves", "@Animator.WPAPanelRuleAnimationSelect.Moves.ToolTip"){
			SetMultiSelection(true);
		};
	
	~aeMCPRuleAnimationSelectMoves() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).moves;
	}
	
	decStringSet GetAllowedStrings(const igdeMetaContext::Ref &context) const override{
		const auto animator = Rule(context).GetAnimator();
		return animator ? animator->hiddenMoveNames.GetValue() : decStringSet();
	}
	
	void AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner) override{
		auto &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, igdeMetaPropertyStringList::ActionAdd::Ref::New(*this, owner, context));
		AddDefaultContextMenuEntries(menu, context, owner);
	}
};


class aeMCPRuleAnimationSelectTargetMoveTime : public aeTMCPRuleTarget<aeTMCPAnimatorRuleAnimationSelect<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleAnimationSelectTargetMoveTime() : aeTMCPRuleTarget("animationselect.targetMoveTime",
		"@Animator.Target.MoveTime", "@Animator.Target.MoveTime.ToolTip"){
	};
	
	~aeMCPRuleAnimationSelectTargetMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetMoveTime;
	}
};

class aeMCPRuleAnimationSelectTargetSelect : public aeTMCPRuleTarget<aeTMCPAnimatorRuleAnimationSelect<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleAnimationSelectTargetSelect() : aeTMCPRuleTarget("animationselect.targetSelect",
		"@Animator.Target.Select", "@Animator.Target.Select.ToolTip"){
	};
	
	~aeMCPRuleAnimationSelectTargetSelect() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetSelect;
	}
};


#endif