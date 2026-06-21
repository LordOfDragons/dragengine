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

#ifndef _AEMCPRULESUBANIMATOR_H_
#define _AEMCPRULESUBANIMATOR_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleSubAnimator.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>


class aeMCPRuleSubAnimatorPathSubAnimator : public aeTMCPAnimatorRuleSubAnimator<igdeMetaPropertyPathStorage>{
public:
	aeMCPRuleSubAnimatorPathSubAnimator() : aeTMCPAnimatorRuleType("subanimator.pathSubAnimator",
		"Animator.WPAPanelRuleSubAnimator.Path",
		igdeEnvironment::eFilePatternListTypes::efpltAnimator){
	}
	
	~aeMCPRuleSubAnimatorPathSubAnimator() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).pathSubAnimator;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->GetDirectoryPath() : decString();
	}
};

class aeMCPRuleSubAnimatorEnablePosition : public aeTMCPAnimatorRuleSubAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnablePosition() : aeTMCPAnimatorRuleType("subanimator.enablePosition",
		"Animator.WPAPanelRuleSubAnimator.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleSubAnimatorEnableOrientation : public aeTMCPAnimatorRuleSubAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnableOrientation() : aeTMCPAnimatorRuleType("subanimator.enableOrientation",
		"Animator.WPAPanelRuleSubAnimator.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleSubAnimatorEnableSize : public aeTMCPAnimatorRuleSubAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnableSize() : aeTMCPAnimatorRuleType("subanimator.enableSize",
		"Animator.WPAPanelRuleSubAnimator.EnableSize"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleSubAnimatorEnableVertexPositionSet : public aeTMCPAnimatorRuleSubAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnableVertexPositionSet() : aeTMCPAnimatorRuleType("subanimator.enableVertexPositionSet",
		"Animator.WPAPanelRuleSubAnimator.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};


#endif