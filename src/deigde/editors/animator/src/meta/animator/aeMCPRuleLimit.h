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

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleLimit.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleLimitMinPosition : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinPosition() : igdeMetaPropertyMCT(
		"animator.rule_limit.minPosition", "Animator.WPAPanelRuleLimit.MinPosition"){}
	
	~aeMCPRuleLimitMinPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinPosition;
	}
};

class aeMCPRuleLimitMaxPosition : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxPosition() : igdeMetaPropertyMCT(
		"animator.rule_limit.maxPosition", "Animator.WPAPanelRuleLimit.MaxPosition"){}
	
	~aeMCPRuleLimitMaxPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxPosition;
	}
};

class aeMCPRuleLimitMinRotation :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleLimitMinRotation() : igdeMetaPropertyMCT(
		"animator.rule_limit.minRotation", "Animator.WPAPanelRuleLimit.MinRotation"){}
	
	~aeMCPRuleLimitMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinRotation;
	}
};

class aeMCPRuleLimitMaxRotation :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleLimitMaxRotation() : igdeMetaPropertyMCT(
		"animator.rule_limit.maxRotation", "Animator.WPAPanelRuleLimit.MaxRotation"){}
	
	~aeMCPRuleLimitMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxRotation;
	}
};

class aeMCPRuleLimitMinScaling : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinScaling() : igdeMetaPropertyMCT(
		"animator.rule_limit.minScaling", "Animator.WPAPanelRuleLimit.MinScaling")
	{
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleLimitMinScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinScaling;
	}
};

class aeMCPRuleLimitMaxScaling : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxScaling() : igdeMetaPropertyMCT(
		"animator.rule_limit.maxScaling", "Animator.WPAPanelRuleLimit.MaxScaling")
	{
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleLimitMaxScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxScaling;
	}
};

class aeMCPRuleLimitMinVertexPositionSet :
public aeRuleLimit::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMinVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_limit.minVertexPositionSet",
		"Animator.WPAPanelRuleLimit.MinVertexPositionSet"){}
	
	~aeMCPRuleLimitMinVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinVertexPositionSet;
	}
};

class aeMCPRuleLimitMaxVertexPositionSet :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMaxVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_limit.maxVertexPositionSet",
		"Animator.WPAPanelRuleLimit.MaxVertexPositionSet")
	{
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleLimitMaxVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxVertexPositionSet;
	}
};

class aeMCPRuleLimitTargetBone : public aeRuleLimit::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleLimitTargetBone() : igdeMetaPropertyMCT(
		"animator.rule_limit.targetBone", "Animator.WPAPanelRuleLimit.TargetBone")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleLimitTargetBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleLimitCoordinateFrame :
	public aeRuleLimit::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleLimit::eCoordinateFrames>>{
public:
	aeMCPRuleLimitCoordinateFrame() : igdeMetaPropertyMCT(
		"animator.rule_limit.coordinateFrame", "Animator.WPAPanelRuleLimit.CoordFrame")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleLimit::ecfBoneLocal,
			deAnimatorRuleLimit::ecfComponent,
			deAnimatorRuleLimit::ecfTargetBone));
		SetDefaultValueEnum(deAnimatorRuleLimit::ecfComponent);
	}
	
	~aeMCPRuleLimitCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpCoordinateFrame;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context,
	deAnimatorRuleLimit::eCoordinateFrames choice, igdeMetaContextItemInfo &info) const override{
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

class aeMCPRuleLimitEnablePositionXMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionXMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enablePositionXMin", "Animator.WPAPanelRuleLimit.EnablePositionXMin"){}
	
	~aeMCPRuleLimitEnablePositionXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePositionXMin;
	}
};

class aeMCPRuleLimitEnablePositionXMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionXMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enablePositionXMax", "Animator.WPAPanelRuleLimit.EnablePositionXMax"){}
	
	~aeMCPRuleLimitEnablePositionXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePositionXMax;
	}
};

class aeMCPRuleLimitEnablePositionYMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enablePositionYMin", "Animator.WPAPanelRuleLimit.EnablePositionYMin"){}
	
	~aeMCPRuleLimitEnablePositionYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePositionYMin;
	}
};

class aeMCPRuleLimitEnablePositionYMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enablePositionYMax", "Animator.WPAPanelRuleLimit.EnablePositionYMax"){}
	
	~aeMCPRuleLimitEnablePositionYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePositionYMax;
	}
};

class aeMCPRuleLimitEnablePositionZMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enablePositionZMin", "Animator.WPAPanelRuleLimit.EnablePositionZMin"){}
	
	~aeMCPRuleLimitEnablePositionZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePositionZMin;
	}
};

class aeMCPRuleLimitEnablePositionZMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enablePositionZMax", "Animator.WPAPanelRuleLimit.EnablePositionZMax"){}
	
	~aeMCPRuleLimitEnablePositionZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePositionZMax;
	}
};

class aeMCPRuleLimitEnableRotationXMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableRotationXMin", "Animator.WPAPanelRuleLimit.EnableRotationXMin"){}
	
	~aeMCPRuleLimitEnableRotationXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableRotationXMin;
	}
};

class aeMCPRuleLimitEnableRotationXMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableRotationXMax", "Animator.WPAPanelRuleLimit.EnableRotationXMax"){}
	
	~aeMCPRuleLimitEnableRotationXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableRotationXMax;
	}
};

class aeMCPRuleLimitEnableRotationYMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableRotationYMin", "Animator.WPAPanelRuleLimit.EnableRotationYMin"){}
	
	~aeMCPRuleLimitEnableRotationYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableRotationYMin;
	}
};

class aeMCPRuleLimitEnableRotationYMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableRotationYMax", "Animator.WPAPanelRuleLimit.EnableRotationYMax"){}
	
	~aeMCPRuleLimitEnableRotationYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableRotationYMax;
	}
};

class aeMCPRuleLimitEnableRotationZMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableRotationZMin", "Animator.WPAPanelRuleLimit.EnableRotationZMin"){}
	
	~aeMCPRuleLimitEnableRotationZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableRotationZMin;
	}
};

class aeMCPRuleLimitEnableRotationZMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableRotationZMax", "Animator.WPAPanelRuleLimit.EnableRotationZMax"){}
	
	~aeMCPRuleLimitEnableRotationZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableRotationZMax;
	}
};

class aeMCPRuleLimitEnableScalingXMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableScalingXMin", "Animator.WPAPanelRuleLimit.EnableScalingXMin"){}
	
	~aeMCPRuleLimitEnableScalingXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableScalingXMin;
	}
};

class aeMCPRuleLimitEnableScalingXMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableScalingXMax", "Animator.WPAPanelRuleLimit.EnableScalingXMax"){}
	
	~aeMCPRuleLimitEnableScalingXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableScalingXMax;
	}
};

class aeMCPRuleLimitEnableScalingYMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableScalingYMin", "Animator.WPAPanelRuleLimit.EnableScalingYMin"){}
	
	~aeMCPRuleLimitEnableScalingYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableScalingYMin;
	}
};

class aeMCPRuleLimitEnableScalingYMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableScalingYMax", "Animator.WPAPanelRuleLimit.EnableScalingYMax"){}
	
	~aeMCPRuleLimitEnableScalingYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableScalingYMax;
	}
};

class aeMCPRuleLimitEnableScalingZMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableScalingZMin", "Animator.WPAPanelRuleLimit.EnableScalingZMin"){}
	
	~aeMCPRuleLimitEnableScalingZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableScalingZMin;
	}
};

class aeMCPRuleLimitEnableScalingZMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableScalingZMax", "Animator.WPAPanelRuleLimit.EnableScalingZMax"){}
	
	~aeMCPRuleLimitEnableScalingZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableScalingZMax;
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMin :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMin() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableVertexPositionSetMin",
		"Animator.WPAPanelRuleLimit.EnableVertexPositionSetMin"){}
	
	~aeMCPRuleLimitEnableVertexPositionSetMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableVertexPositionSetMin;
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMax :
	public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMax() : igdeMetaPropertyMCT(
		"animator.rule_limit.enableVertexPositionSetMax",
		"Animator.WPAPanelRuleLimit.EnableVertexPositionSetMax"){}
	
	~aeMCPRuleLimitEnableVertexPositionSetMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableVertexPositionSetMax;
	}
};


#endif