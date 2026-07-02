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

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleAnimation.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleAnimationMoveName : public aeRuleAnimation::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleAnimationMoveName() : igdeMetaPropertyMCT("animation.moveName", "Animator.WPAPanelRuleAnimation.MoveName"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleAnimationMoveName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMoveName();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenMoveNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleAnimationMoveTime : public aeRuleAnimation::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleAnimationMoveTime() : igdeMetaPropertyMCT("animation.moveTime", "Animator.WPAPanelRuleAnimation.MoveTime"){
		SetEnableLowerLimit(true);
		SetEnableUpperLimit(true);
	}
	
	~aeMCPRuleAnimationMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMoveTime();
	}
};

class aeMCPRuleAnimationEnablePosition : public aeRuleAnimation::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnablePosition() : igdeMetaPropertyMCT("animation.enablePosition", "Animator.WPAPanelRuleAnimation.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePosition();
	}
};

class aeMCPRuleAnimationEnableOrientation : public aeRuleAnimation::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnableOrientation() : igdeMetaPropertyMCT("animation.enableOrientation", "Animator.WPAPanelRuleAnimation.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableOrientation();
	}
};

class aeMCPRuleAnimationEnableSize : public aeRuleAnimation::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnableSize() : igdeMetaPropertyMCT("animation.enableSize", "Animator.WPAPanelRuleAnimation.EnableSize"){}
	~aeMCPRuleAnimationEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableSize();
	}
};

class aeMCPRuleAnimationEnableVertexPositionSet : public aeRuleAnimation::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationEnableVertexPositionSet() : igdeMetaPropertyMCT("animation.enableVertexPositionSet", "Animator.WPAPanelRuleAnimation.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSet();
	}
};


class aeMCPRuleAnimationTargetMoveTime : public aeRule::MetaPropertyTarget<aeRuleAnimation>{
public:
	aeMCPRuleAnimationTargetMoveTime() : MetaPropertyTarget("animation.targetMoveTime", "Animator.Target.MoveTime"){}
	~aeMCPRuleAnimationTargetMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetMoveTime();
	}
};


#endif