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

#ifndef _AEMCPRULEANIMATIONDIFFERENCE_H_
#define _AEMCPRULEANIMATIONDIFFERENCE_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleAnimationDifference.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleAnimationDifferenceLeadingMoveName : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleAnimationDifferenceLeadingMoveName() : aeTMCPAnimatorRuleType("animationdifference.leadingMoveName",
		"@Animator.WPAPanelRuleAnimationDifference.LeadingMoveName", "@Animator.WPAPanelRuleAnimationDifference.LeadingMoveName.ToolTip"){
		SetDefaultValue("idle");
	};
	
	~aeMCPRuleAnimationDifferenceLeadingMoveName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).leadingMoveName;
	}
};

class aeMCPRuleAnimationDifferenceLeadingMoveTime : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleAnimationDifferenceLeadingMoveTime() : aeTMCPAnimatorRuleType("animationdifference.leadingMoveTime",
		"@Animator.WPAPanelRuleAnimationDifference.LeadingMoveTime", "@Animator.WPAPanelRuleAnimationDifference.LeadingMoveTime.ToolTip"){
	};
	
	~aeMCPRuleAnimationDifferenceLeadingMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).leadingMoveTime;
	}
};

class aeMCPRuleAnimationDifferenceReferenceMoveName : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleAnimationDifferenceReferenceMoveName() : aeTMCPAnimatorRuleType("animationdifference.referenceMoveName",
		"@Animator.WPAPanelRuleAnimationDifference.ReferenceMoveName", "@Animator.WPAPanelRuleAnimationDifference.ReferenceMoveName.ToolTip"){
		SetDefaultValue("idle");
	};
	
	~aeMCPRuleAnimationDifferenceReferenceMoveName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).referenceMoveName;
	}
};

class aeMCPRuleAnimationDifferenceReferenceMoveTime : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleAnimationDifferenceReferenceMoveTime() : aeTMCPAnimatorRuleType("animationdifference.referenceMoveTime",
		"@Animator.WPAPanelRuleAnimationDifference.ReferenceMoveTime", "@Animator.WPAPanelRuleAnimationDifference.ReferenceMoveTime.ToolTip"){
	};
	
	~aeMCPRuleAnimationDifferenceReferenceMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).referenceMoveTime;
	}
};

class aeMCPRuleAnimationDifferenceUseSameMove : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceUseSameMove() : aeTMCPAnimatorRuleType("animationdifference.useSameMove",
		"@Animator.WPAPanelRuleAnimationDifference.SameMove", "@Animator.WPAPanelRuleAnimationDifference.SameMove.ToolTip"){
	};
	
	~aeMCPRuleAnimationDifferenceUseSameMove() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).useSameMove;
	}
};

class aeMCPRuleAnimationDifferenceUseComponentSpace : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceUseComponentSpace() : aeTMCPAnimatorRuleType("animationdifference.useComponentSpace",
		"@Animator.WPAPanelRuleAnimationDifference.ComponentSpace", "@Animator.WPAPanelRuleAnimationDifference.ComponentSpace.ToolTip"){
	};
	
	~aeMCPRuleAnimationDifferenceUseComponentSpace() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).useComponentSpace;
	}
};

class aeMCPRuleAnimationDifferenceEnablePosition : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnablePosition() : aeTMCPAnimatorRuleType("animationdifference.enablePosition",
		"@Animator.WPAPanelRuleAnimationDifference.EnablePosition", "@Animator.WPAPanelRuleAnimationDifference.EnablePosition.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationDifferenceEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleAnimationDifferenceEnableOrientation : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnableOrientation() : aeTMCPAnimatorRuleType("animationdifference.enableOrientation",
		"@Animator.WPAPanelRuleAnimationDifference.EnableRotation", "@Animator.WPAPanelRuleAnimationDifference.EnableRotation.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationDifferenceEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleAnimationDifferenceEnableSize : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnableSize() : aeTMCPAnimatorRuleType("animationdifference.enableSize",
		"@Animator.WPAPanelRuleAnimationDifference.EnableSize", "@Animator.WPAPanelRuleAnimationDifference.EnableSize.ToolTip"){
	};
	
	~aeMCPRuleAnimationDifferenceEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleAnimationDifferenceEnableVertexPositionSet : public aeTMCPAnimatorRuleAnimationDifference<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnableVertexPositionSet() : aeTMCPAnimatorRuleType("animationdifference.enableVertexPositionSet",
		"@Animator.WPAPanelRuleAnimationDifference.EnableVertexPositionSet", "@Animator.WPAPanelRuleAnimationDifference.EnableVertexPositionSet.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleAnimationDifferenceEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};


#endif