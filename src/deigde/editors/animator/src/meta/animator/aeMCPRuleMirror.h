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

#ifndef _AEMCPRULEMIRROR_H_
#define _AEMCPRULEMIRROR_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleMirror.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleMirrorMirrorAxis : public aeTMCPAnimatorRuleMirror<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleMirror::eMirrorAxis>>{
public:
	aeMCPRuleMirrorMirrorAxis() : aeTMCPAnimatorRuleType(
	"mirror.mirrorAxis", "Animator.WPAPanelRuleMirror.Axis"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleMirror::emaX,
			deAnimatorRuleMirror::emaY,
			deAnimatorRuleMirror::emaZ
		));
		SetDefaultValueEnum(deAnimatorRuleMirror::emaX);
	}
	
	~aeMCPRuleMirrorMirrorAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMirrorAxis();
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleMirror::eMirrorAxis choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleMirror::emaX:
			info.SetAll("@Animator.WPAPanelRuleMirror.Axis.X");
			break;
			
		case deAnimatorRuleMirror::emaY:
			info.SetAll("@Animator.WPAPanelRuleMirror.Axis.Y");
			break;
			
		case deAnimatorRuleMirror::emaZ:
			info.SetAll("@Animator.WPAPanelRuleMirror.Axis.Z");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleMirrorMirrorBone : public aeTMCPAnimatorRuleMirror<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleMirrorMirrorBone() : aeTMCPAnimatorRuleType(
	"mirror.mirrorBone", "Animator.WPAPanelRuleMirror.Bone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleMirrorMirrorBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMirrorBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleMirrorEnablePosition : public aeTMCPAnimatorRuleMirror<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnablePosition() : aeTMCPAnimatorRuleType(
	"mirror.enablePosition", "Animator.WPAPanelRuleMirror.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleMirrorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPEnablePosition();
	}
};

class aeMCPRuleMirrorEnableOrientation : public aeTMCPAnimatorRuleMirror<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnableOrientation() : aeTMCPAnimatorRuleType(
	"mirror.enableOrientation", "Animator.WPAPanelRuleMirror.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleMirrorEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPEnableOrientation();
	}
};

class aeMCPRuleMirrorEnableSize : public aeTMCPAnimatorRuleMirror<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnableSize() : aeTMCPAnimatorRuleType(
	"mirror.enableSize", "Animator.WPAPanelRuleMirror.EnableSize"){}
	
	~aeMCPRuleMirrorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPEnableSize();
	}
};

class aeMCPRuleMirrorEnableVertexPositionSet : public aeTMCPAnimatorRuleMirror<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnableVertexPositionSet() : aeTMCPAnimatorRuleType(
	"mirror.enableVertexPositionSet", "Animator.WPAPanelRuleMirror.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleMirrorEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPEnableVertexPositionSet();
	}
};

#endif