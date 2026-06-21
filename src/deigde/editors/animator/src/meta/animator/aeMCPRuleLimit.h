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

#ifndef _AEMCPRULELIMIT_H_
#define _AEMCPRULELIMIT_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleLimit.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleLimitMinPosition : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinPosition() : aeTMCPAnimatorRuleType(
	"limit.minPosition", "Animator.WPAPanelRuleLimit.MinPosition"){}
	
	~aeMCPRuleLimitMinPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minPosition;
	}
};

class aeMCPRuleLimitMaxPosition : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxPosition() : aeTMCPAnimatorRuleType(
	"limit.maxPosition", "Animator.WPAPanelRuleLimit.MaxPosition"){}
	
	~aeMCPRuleLimitMaxPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxPosition;
	}
};

class aeMCPRuleLimitMinRotation : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinRotation() : aeTMCPAnimatorRuleType(
	"limit.minRotation", "Animator.WPAPanelRuleLimit.MinRotation"){}
	
	~aeMCPRuleLimitMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minRotation;
	}
};

class aeMCPRuleLimitMaxRotation : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxRotation() : aeTMCPAnimatorRuleType(
	"limit.maxRotation", "Animator.WPAPanelRuleLimit.MaxRotation"){}
	
	~aeMCPRuleLimitMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxRotation;
	}
};

class aeMCPRuleLimitMinScaling : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinScaling() : aeTMCPAnimatorRuleType(
	"limit.minScaling", "Animator.WPAPanelRuleLimit.MinScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleLimitMinScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minScaling;
	}
};

class aeMCPRuleLimitMaxScaling : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxScaling() : aeTMCPAnimatorRuleType(
	"limit.maxScaling", "Animator.WPAPanelRuleLimit.MaxScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleLimitMaxScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxScaling;
	}
};

class aeMCPRuleLimitMinVertexPositionSet : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMinVertexPositionSet() : aeTMCPAnimatorRuleType(
	"limit.minVertexPositionSet", "Animator.WPAPanelRuleLimit.MinVertexPositionSet"){}
	
	~aeMCPRuleLimitMinVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minVertexPositionSet;
	}
};

class aeMCPRuleLimitMaxVertexPositionSet : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMaxVertexPositionSet() : aeTMCPAnimatorRuleType(
	"limit.maxVertexPositionSet", "Animator.WPAPanelRuleLimit.MaxVertexPositionSet"){
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleLimitMaxVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxVertexPositionSet;
	}
};

class aeMCPRuleLimitTargetBone : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleLimitTargetBone() : aeTMCPAnimatorRuleType(
	"limit.targetBone", "Animator.WPAPanelRuleLimit.TargetBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleLimitTargetBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->hiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleLimitCoordinateFrame : public aeTMCPAnimatorRuleLimit<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleLimit::eCoordinateFrames>>{
public:
	aeMCPRuleLimitCoordinateFrame() : aeTMCPAnimatorRuleType(
	"limit.coordinateFrame", "Animator.WPAPanelRuleLimit.CoordFrame"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleLimit::ecfBoneLocal,
			deAnimatorRuleLimit::ecfComponent,
			deAnimatorRuleLimit::ecfTargetBone));
		SetDefaultValueEnum(deAnimatorRuleLimit::ecfComponent);
	}
	
	~aeMCPRuleLimitCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).coordinateFrame;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleLimit::eCoordinateFrames choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleLimit::ecfBoneLocal:
			info.SetAll("@Animator.WPAPanelRuleLimit.CoordFrame.BoneLocal");
			break;
			
		case deAnimatorRuleLimit::ecfComponent:
			info.SetAll("@Animator.WPAPanelRuleLimit.CoordFrame.Component");
			break;
			
		case deAnimatorRuleLimit::ecfTargetBone:
			info.SetAll("@Animator.WPAPanelRuleLimit.CoordFrame.TargetBone");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleLimitEnablePositionXMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionXMin() : aeTMCPAnimatorRuleType(
	"limit.enablePositionXMin", "Animator.WPAPanelRuleLimit.EnablePositionXMin"){}
	
	~aeMCPRuleLimitEnablePositionXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionXMin;
	}
};

class aeMCPRuleLimitEnablePositionXMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionXMax() : aeTMCPAnimatorRuleType(
	"limit.enablePositionXMax", "Animator.WPAPanelRuleLimit.EnablePositionXMax"){}
	
	~aeMCPRuleLimitEnablePositionXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionXMax;
	}
};

class aeMCPRuleLimitEnablePositionYMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMin() : aeTMCPAnimatorRuleType(
	"limit.enablePositionYMin", "Animator.WPAPanelRuleLimit.EnablePositionYMin"){}
	
	~aeMCPRuleLimitEnablePositionYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionYMin;
	}
};

class aeMCPRuleLimitEnablePositionYMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMax() : aeTMCPAnimatorRuleType(
	"limit.enablePositionYMax", "Animator.WPAPanelRuleLimit.EnablePositionYMax"){}
	
	~aeMCPRuleLimitEnablePositionYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionYMax;
	}
};

class aeMCPRuleLimitEnablePositionZMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMin() : aeTMCPAnimatorRuleType(
	"limit.enablePositionZMin", "Animator.WPAPanelRuleLimit.EnablePositionZMin"){}
	
	~aeMCPRuleLimitEnablePositionZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionZMin;
	}
};

class aeMCPRuleLimitEnablePositionZMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMax() : aeTMCPAnimatorRuleType(
	"limit.enablePositionZMax", "Animator.WPAPanelRuleLimit.EnablePositionZMax"){}
	
	~aeMCPRuleLimitEnablePositionZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionZMax;
	}
};

class aeMCPRuleLimitEnableRotationXMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMin() : aeTMCPAnimatorRuleType(
	"limit.enableRotationXMin", "Animator.WPAPanelRuleLimit.EnableRotationXMin"){}
	
	~aeMCPRuleLimitEnableRotationXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationXMin;
	}
};

class aeMCPRuleLimitEnableRotationXMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMax() : aeTMCPAnimatorRuleType(
	"limit.enableRotationXMax", "Animator.WPAPanelRuleLimit.EnableRotationXMax"){}
	
	~aeMCPRuleLimitEnableRotationXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationXMax;
	}
};

class aeMCPRuleLimitEnableRotationYMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMin() : aeTMCPAnimatorRuleType(
	"limit.enableRotationYMin", "Animator.WPAPanelRuleLimit.EnableRotationYMin"){}
	
	~aeMCPRuleLimitEnableRotationYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationYMin;
	}
};

class aeMCPRuleLimitEnableRotationYMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMax() : aeTMCPAnimatorRuleType(
	"limit.enableRotationYMax", "Animator.WPAPanelRuleLimit.EnableRotationYMax"){}
	
	~aeMCPRuleLimitEnableRotationYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationYMax;
	}
};

class aeMCPRuleLimitEnableRotationZMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMin() : aeTMCPAnimatorRuleType(
	"limit.enableRotationZMin", "Animator.WPAPanelRuleLimit.EnableRotationZMin"){}
	
	~aeMCPRuleLimitEnableRotationZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationZMin;
	}
};

class aeMCPRuleLimitEnableRotationZMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMax() : aeTMCPAnimatorRuleType(
	"limit.enableRotationZMax", "Animator.WPAPanelRuleLimit.EnableRotationZMax"){}
	
	~aeMCPRuleLimitEnableRotationZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationZMax;
	}
};

class aeMCPRuleLimitEnableScalingXMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMin() : aeTMCPAnimatorRuleType(
	"limit.enableScalingXMin", "Animator.WPAPanelRuleLimit.EnableScalingXMin"){}
	
	~aeMCPRuleLimitEnableScalingXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingXMin;
	}
};

class aeMCPRuleLimitEnableScalingXMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMax() : aeTMCPAnimatorRuleType(
	"limit.enableScalingXMax", "Animator.WPAPanelRuleLimit.EnableScalingXMax"){}
	
	~aeMCPRuleLimitEnableScalingXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingXMax;
	}
};

class aeMCPRuleLimitEnableScalingYMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMin() : aeTMCPAnimatorRuleType(
	"limit.enableScalingYMin", "Animator.WPAPanelRuleLimit.EnableScalingYMin"){}
	
	~aeMCPRuleLimitEnableScalingYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingYMin;
	}
};

class aeMCPRuleLimitEnableScalingYMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMax() : aeTMCPAnimatorRuleType(
	"limit.enableScalingYMax", "Animator.WPAPanelRuleLimit.EnableScalingYMax"){}
	
	~aeMCPRuleLimitEnableScalingYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingYMax;
	}
};

class aeMCPRuleLimitEnableScalingZMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMin() : aeTMCPAnimatorRuleType(
	"limit.enableScalingZMin", "Animator.WPAPanelRuleLimit.EnableScalingZMin"){}
	
	~aeMCPRuleLimitEnableScalingZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingZMin;
	}
};

class aeMCPRuleLimitEnableScalingZMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMax() : aeTMCPAnimatorRuleType(
	"limit.enableScalingZMax", "Animator.WPAPanelRuleLimit.EnableScalingZMax"){}
	
	~aeMCPRuleLimitEnableScalingZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingZMax;
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMin() : aeTMCPAnimatorRuleType(
	"limit.enableVertexPositionSetMin", "Animator.WPAPanelRuleLimit.EnableVertexPositionSetMin"){}
	
	~aeMCPRuleLimitEnableVertexPositionSetMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSetMin;
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMax() : aeTMCPAnimatorRuleType(
	"limit.enableVertexPositionSetMax", "Animator.WPAPanelRuleLimit.EnableVertexPositionSetMax"){}
	
	~aeMCPRuleLimitEnableVertexPositionSetMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSetMax;
	}
};


#endif