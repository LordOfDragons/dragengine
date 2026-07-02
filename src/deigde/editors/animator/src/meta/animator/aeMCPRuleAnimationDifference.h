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

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleAnimationDifference.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleAnimationDifferenceLeadingMoveName : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleAnimationDifferenceLeadingMoveName() : igdeMetaPropertyMCT(
	"animationdifference.leadingMoveName", "Animator.WPAPanelRuleAnimationDifference.LeadingMoveName"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleAnimationDifferenceLeadingMoveName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPLeadingMoveName();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenMoveNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleAnimationDifferenceLeadingMoveTime : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleAnimationDifferenceLeadingMoveTime() : igdeMetaPropertyMCT(
	"animationdifference.leadingMoveTime", "Animator.WPAPanelRuleAnimationDifference.LeadingMoveTime"){
		SetEnableLowerLimit(true);
		SetEnableUpperLimit(true);
	}
	
	~aeMCPRuleAnimationDifferenceLeadingMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPLeadingMoveTime();
	}
};

class aeMCPRuleAnimationDifferenceReferenceMoveName : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleAnimationDifferenceReferenceMoveName() : igdeMetaPropertyMCT(
	"animationdifference.referenceMoveName", "Animator.WPAPanelRuleAnimationDifference.ReferenceMoveName"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleAnimationDifferenceReferenceMoveName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPReferenceMoveName();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenMoveNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleAnimationDifferenceReferenceMoveTime : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleAnimationDifferenceReferenceMoveTime() : igdeMetaPropertyMCT(
	"animationdifference.referenceMoveTime", "Animator.WPAPanelRuleAnimationDifference.ReferenceMoveTime"){
		SetEnableLowerLimit(true);
		SetEnableUpperLimit(true);
	}
	
	~aeMCPRuleAnimationDifferenceReferenceMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPReferenceMoveTime();
	}
};

class aeMCPRuleAnimationDifferenceUseComponentSpace : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceUseComponentSpace() : igdeMetaPropertyMCT(
	"animationdifference.useComponentSpace", "Animator.WPAPanelRuleAnimationDifference.ComponentSpace"){}
	
	~aeMCPRuleAnimationDifferenceUseComponentSpace() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPUseComponentSpace();
	}
};

class aeMCPRuleAnimationDifferenceEnablePosition : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnablePosition() : igdeMetaPropertyMCT(
	"animationdifference.enablePosition", "Animator.WPAPanelRuleAnimationDifference.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationDifferenceEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePosition();
	}
};

class aeMCPRuleAnimationDifferenceEnableOrientation : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnableOrientation() : igdeMetaPropertyMCT(
	"animationdifference.enableOrientation", "Animator.WPAPanelRuleAnimationDifference.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationDifferenceEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableOrientation();
	}
};

class aeMCPRuleAnimationDifferenceEnableSize : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnableSize() : igdeMetaPropertyMCT(
	"animationdifference.enableSize", "Animator.WPAPanelRuleAnimationDifference.EnableSize"){}
	
	~aeMCPRuleAnimationDifferenceEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableSize();
	}
};

class aeMCPRuleAnimationDifferenceEnableVertexPositionSet : public aeRuleAnimationDifference::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleAnimationDifferenceEnableVertexPositionSet() : igdeMetaPropertyMCT(
	"animationdifference.enableVertexPositionSet", "Animator.WPAPanelRuleAnimationDifference.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleAnimationDifferenceEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSet();
	}
};


class aeMCPRuleAnimationDifferenceTargetLeadMoveTime : public aeRule::MetaPropertyTarget<aeRuleAnimationDifference>{
public:
	aeMCPRuleAnimationDifferenceTargetLeadMoveTime() : MetaPropertyTarget(
	"animationdifference.targetLeadMoveTime", "Animator.Target.LeadingMoveTime"){}
	~aeMCPRuleAnimationDifferenceTargetLeadMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetLeadMoveTime();
	}
};


class aeMCPRuleAnimationDifferenceTargetRefMoveTime : public aeRule::MetaPropertyTarget<aeRuleAnimationDifference>{
public:
	aeMCPRuleAnimationDifferenceTargetRefMoveTime() : MetaPropertyTarget(
	"animationdifference.targetRefMoveTime", "Animator.Target.ReferenceMoveTime"){}
	~aeMCPRuleAnimationDifferenceTargetRefMoveTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetRefMoveTime();
	}
};


#endif