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

#ifndef _AEMCPRULEBONETRANSFORMATOR_H_
#define _AEMCPRULEBONETRANSFORMATOR_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleBoneTransformator.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleBoneTransformatorMinTranslation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMinTranslation() : aeTMCPAnimatorRuleType(
	"bonetransformator.minTranslation", "Animator.WPAPanelRuleBoneTransformator.MinTranslation"){}
	
	~aeMCPRuleBoneTransformatorMinTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMinTranslation();
	}
};

class aeMCPRuleBoneTransformatorMaxTranslation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMaxTranslation() : aeTMCPAnimatorRuleType(
	"bonetransformator.maxTranslation", "Animator.WPAPanelRuleBoneTransformator.MaxTranslation"){}
	
	~aeMCPRuleBoneTransformatorMaxTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMaxTranslation();
	}
};

class aeMCPRuleBoneTransformatorMinRotation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleBoneTransformatorMinRotation() : aeTMCPAnimatorRuleType(
	"bonetransformator.minRotation", "Animator.WPAPanelRuleBoneTransformator.MinRotation"){}
	
	~aeMCPRuleBoneTransformatorMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMinRotation();
	}
};

class aeMCPRuleBoneTransformatorMaxRotation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleBoneTransformatorMaxRotation() : aeTMCPAnimatorRuleType(
	"bonetransformator.maxRotation", "Animator.WPAPanelRuleBoneTransformator.MaxRotation"){}
	
	~aeMCPRuleBoneTransformatorMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMaxRotation();
	}
};

class aeMCPRuleBoneTransformatorMinScaling : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMinScaling() : aeTMCPAnimatorRuleType(
	"bonetransformator.minScaling", "Animator.WPAPanelRuleBoneTransformator.MinScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleBoneTransformatorMinScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMinScaling();
	}
};

class aeMCPRuleBoneTransformatorMaxScaling : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMaxScaling() : aeTMCPAnimatorRuleType(
	"bonetransformator.maxScaling", "Animator.WPAPanelRuleBoneTransformator.MaxScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleBoneTransformatorMaxScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMaxScaling();
	}
};

class aeMCPRuleBoneTransformatorAxis : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorAxis() : aeTMCPAnimatorRuleType(
	"bonetransformator.axis", "Animator.WPAPanelRuleBoneTransformator.Axis"){
		SetDefaultValue(decVector(0.0f, 0.0f, 1.0f));
	}
	
	~aeMCPRuleBoneTransformatorAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPAxis();
	}
};

class aeMCPRuleBoneTransformatorMinAngle : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBoneTransformatorMinAngle() : aeTMCPAnimatorRuleType(
	"bonetransformator.minAngle", "Animator.WPAPanelRuleBoneTransformator.MinAngle"){}
	
	~aeMCPRuleBoneTransformatorMinAngle() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMinAngle();
	}
};

class aeMCPRuleBoneTransformatorMaxAngle : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBoneTransformatorMaxAngle() : aeTMCPAnimatorRuleType(
	"bonetransformator.maxAngle", "Animator.WPAPanelRuleBoneTransformator.MaxAngle"){}
	
	~aeMCPRuleBoneTransformatorMaxAngle() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPMaxAngle();
	}
};

class aeMCPRuleBoneTransformatorEnablePosition : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnablePosition() : aeTMCPAnimatorRuleType(
	"bonetransformator.enablePosition", "Animator.WPAPanelRuleBoneTransformator.EnablePosition"){}
	
	~aeMCPRuleBoneTransformatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPEnablePosition();
	}
};

class aeMCPRuleBoneTransformatorEnableOrientation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnableOrientation() : aeTMCPAnimatorRuleType(
	"bonetransformator.enableOrientation", "Animator.WPAPanelRuleBoneTransformator.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleBoneTransformatorEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPEnableOrientation();
	}
};

class aeMCPRuleBoneTransformatorEnableSize : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnableSize() : aeTMCPAnimatorRuleType(
	"bonetransformator.enableSize", "Animator.WPAPanelRuleBoneTransformator.EnableSize"){}
	
	~aeMCPRuleBoneTransformatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPEnableSize();
	}
};

class aeMCPRuleBoneTransformatorUseAxis : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorUseAxis() : aeTMCPAnimatorRuleType(
	"bonetransformator.useAxis", "Animator.WPAPanelRuleBoneTransformator.UseAxis"){}
	
	~aeMCPRuleBoneTransformatorUseAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPUseAxis();
	}
};

class aeMCPRuleBoneTransformatorTargetBone : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleBoneTransformatorTargetBone() : aeTMCPAnimatorRuleType(
	"bonetransformator.targetBone", "Animator.WPAPanelRuleBoneTransformator.TargetBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleBoneTransformatorTargetBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleBoneTransformatorInputBone : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleBoneTransformatorInputBone() : aeTMCPAnimatorRuleType(
	"bonetransformator.inputBone", "Animator.WPAPanelRuleBoneTransformator.InputBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleBoneTransformatorInputBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPInputBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleBoneTransformatorCoordinateFrame : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleBoneTransformator::eCoordinateFrames>>{
public:
	aeMCPRuleBoneTransformatorCoordinateFrame() : aeTMCPAnimatorRuleType(
	"bonetransformator.coordinateFrame", "Animator.WPAPanelRuleBoneTransformator.CoordFrame"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleBoneTransformator::ecfBoneLocal,
			deAnimatorRuleBoneTransformator::ecfComponent,
			deAnimatorRuleBoneTransformator::ecfTargetBone));
		SetDefaultValueEnum(deAnimatorRuleBoneTransformator::ecfComponent);
	}
	
	~aeMCPRuleBoneTransformatorCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPCoordinateFrame();
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleBoneTransformator::eCoordinateFrames choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleBoneTransformator::ecfBoneLocal:
			info.SetAll("@Animator.WPAPanelRuleBoneTransformator.CoordFrame.BoneLocal");
			break;
			
		case deAnimatorRuleBoneTransformator::ecfComponent:
			info.SetAll("@Animator.WPAPanelRuleBoneTransformator.CoordFrame.Component");
			break;
			
		case deAnimatorRuleBoneTransformator::ecfTargetBone:
			info.SetAll("@Animator.WPAPanelRuleBoneTransformator.CoordFrame.TargetBone");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleBoneTransformatorInputSource : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleBoneTransformator::eInputSources>>{
public:
	aeMCPRuleBoneTransformatorInputSource() : aeTMCPAnimatorRuleType(
	"bonetransformator.inputSource", "Animator.WPAPanelRuleBoneTransformator.InputSource"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleBoneTransformator::eisTargetBlend,
			deAnimatorRuleBoneTransformator::eisTargetDirect,
			deAnimatorRuleBoneTransformator::eisBoneState,
			deAnimatorRuleBoneTransformator::eisBoneStateInverse));
		SetDefaultValueEnum(deAnimatorRuleBoneTransformator::eisTargetBlend);
	}
	
	~aeMCPRuleBoneTransformatorInputSource() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPInputSource();
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleBoneTransformator::eInputSources choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleBoneTransformator::eisTargetBlend:
			info.SetAll("@Animator.WPAPanelRuleBoneTransformator.InputSource.TargetBlend");
			break;
			
		case deAnimatorRuleBoneTransformator::eisTargetDirect:
			info.SetAll("@Animator.WPAPanelRuleBoneTransformator.InputSource.TargetDirect");
			break;
			
		case deAnimatorRuleBoneTransformator::eisBoneState:
			info.SetAll("@Animator.WPAPanelRuleBoneTransformator.InputSource.BoneState");
			break;
			
		case deAnimatorRuleBoneTransformator::eisBoneStateInverse:
			info.SetAll("@Animator.WPAPanelRuleBoneTransformator.InputSource.BoneStateInverse");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


class aeMCPRuleBoneTransformatorTargetTranslation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleBoneTransformatorTargetTranslation() : aeTMCPRuleTarget("bonetransformator.targetTranslation", "Animator.Target.Translation"){
		SetRows(3);
	}
	
	~aeMCPRuleBoneTransformatorTargetTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetTranslation();
	}
};

class aeMCPRuleBoneTransformatorTargetRotation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleBoneTransformatorTargetRotation() : aeTMCPRuleTarget("bonetransformator.targetRotation", "Animator.Target.Rotation"){
		SetRows(3);
	}
	
	~aeMCPRuleBoneTransformatorTargetRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetRotation();
	}
};


class aeMCPRuleBoneTransformatorTargetScaling : public aeTMCPRuleTarget<aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleBoneTransformatorTargetScaling() : aeTMCPRuleTarget("bonetransformator.targetScaling", "Animator.Target.Scaling"){
		SetRows(3);
	}
	
	~aeMCPRuleBoneTransformatorTargetScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetScaling();
	}
};


#endif