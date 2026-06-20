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

#ifndef _AEMCPRULEANIMATION_H_
#define _AEMCPRULEANIMATION_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleAnimation.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleAnimationMoveName : public aeTMCPAnimatorRuleAnimation<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleAnimationMoveName() : aeTMCPAnimatorRuleType("animation.moveName",
		"@Animator.WPAPanelRuleAnimation.MoveName", "@Animator.WPAPanelRuleAnimation.MoveName.ToolTip"){
			SetEnableAllowed(true);
		};
	
	~aeMCPRuleAnimationMoveName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).moveName;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->hiddenMoveNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleAnimationMoveTime : public aeTMCPAnimatorRuleAnimation<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleAnimationMoveTime() : aeTMCPAnimatorRuleType("animation.moveTime",
		"@Animator.WPAPanelRuleAnimation.MoveTime", "@Animator.WPAPanelRuleAnimation.MoveTime.ToolTip"){
			SetEnableLowerLimit(true);
			SetEnableUpperLimit(true);
		};
	
	~aeMCPRuleAnimationMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).moveTime;
	}
};

class aeMCPRuleAnimationEnablePosition : public aeTMCPAnimatorRuleAnimation<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnablePosition() : aeTMCPAnimatorRuleType("animation.enablePosition",
		"@Animator.WPAPanelRuleAnimation.EnablePosition", "@Animator.WPAPanelRuleAnimation.EnablePosition.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleAnimationEnableOrientation : public aeTMCPAnimatorRuleAnimation<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnableOrientation() : aeTMCPAnimatorRuleType("animation.enableOrientation",
		"@Animator.WPAPanelRuleAnimation.EnableRotation", "@Animator.WPAPanelRuleAnimation.EnableRotation.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleAnimationEnableSize : public aeTMCPAnimatorRuleAnimation<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnableSize() : aeTMCPAnimatorRuleType("animation.enableSize",
		"@Animator.WPAPanelRuleAnimation.EnableSize", "@Animator.WPAPanelRuleAnimation.EnableSize.ToolTip"){
	};
	
	~aeMCPRuleAnimationEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleAnimationEnableVertexPositionSet : public aeTMCPAnimatorRuleAnimation<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnableVertexPositionSet() : aeTMCPAnimatorRuleType("animation.enableVertexPositionSet",
		"@Animator.WPAPanelRuleAnimation.EnableVertexPositionSet", "@Animator.WPAPanelRuleAnimation.EnableVertexPositionSet.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};


class aeMCPRuleAnimationTargetMoveTime : public aeTMCPRuleTarget<aeTMCPAnimatorRuleAnimation<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleAnimationTargetMoveTime() : aeTMCPRuleTarget("animation.targetMoveTime",
		"@Animator.Target.MoveTime", "@Animator.Target.MoveTime.ToolTip"){
	};
	
	~aeMCPRuleAnimationTargetMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetMoveTime;
	}
};


#endif