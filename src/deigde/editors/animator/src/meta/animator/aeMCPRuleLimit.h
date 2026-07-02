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
	"limit.minPosition", "Animator.WPAPanelRuleLimit.MinPosition"){}
	
	~aeMCPRuleLimitMinPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinPosition();
	}
};

class aeMCPRuleLimitMaxPosition : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxPosition() : igdeMetaPropertyMCT(
	"limit.maxPosition", "Animator.WPAPanelRuleLimit.MaxPosition"){}
	
	~aeMCPRuleLimitMaxPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxPosition();
	}
};

class aeMCPRuleLimitMinRotation : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleLimitMinRotation() : igdeMetaPropertyMCT(
	"limit.minRotation", "Animator.WPAPanelRuleLimit.MinRotation"){}
	
	~aeMCPRuleLimitMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinRotation();
	}
};

class aeMCPRuleLimitMaxRotation : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleLimitMaxRotation() : igdeMetaPropertyMCT(
	"limit.maxRotation", "Animator.WPAPanelRuleLimit.MaxRotation"){}
	
	~aeMCPRuleLimitMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxRotation();
	}
};

class aeMCPRuleLimitMinScaling : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinScaling() : igdeMetaPropertyMCT(
	"limit.minScaling", "Animator.WPAPanelRuleLimit.MinScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleLimitMinScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinScaling();
	}
};

class aeMCPRuleLimitMaxScaling : public aeRuleLimit::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxScaling() : igdeMetaPropertyMCT(
	"limit.maxScaling", "Animator.WPAPanelRuleLimit.MaxScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleLimitMaxScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxScaling();
	}
};

class aeMCPRuleLimitMinVertexPositionSet : public aeRuleLimit::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMinVertexPositionSet() : igdeMetaPropertyMCT(
	"limit.minVertexPositionSet", "Animator.WPAPanelRuleLimit.MinVertexPositionSet"){}
	
	~aeMCPRuleLimitMinVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinVertexPositionSet();
	}
};

class aeMCPRuleLimitMaxVertexPositionSet : public aeRuleLimit::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMaxVertexPositionSet() : igdeMetaPropertyMCT(
	"limit.maxVertexPositionSet", "Animator.WPAPanelRuleLimit.MaxVertexPositionSet"){
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleLimitMaxVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxVertexPositionSet();
	}
};

class aeMCPRuleLimitTargetBone : public aeRuleLimit::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleLimitTargetBone() : igdeMetaPropertyMCT(
	"limit.targetBone", "Animator.WPAPanelRuleLimit.TargetBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleLimitTargetBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleLimitCoordinateFrame : public aeRuleLimit::MetaProperty<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleLimit::eCoordinateFrames>>{
public:
	aeMCPRuleLimitCoordinateFrame() : igdeMetaPropertyMCT(
	"limit.coordinateFrame", "Animator.WPAPanelRuleLimit.CoordFrame"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleLimit::ecfBoneLocal,
			deAnimatorRuleLimit::ecfComponent,
			deAnimatorRuleLimit::ecfTargetBone));
		SetDefaultValueEnum(deAnimatorRuleLimit::ecfComponent);
	}
	
	~aeMCPRuleLimitCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPCoordinateFrame();
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

class aeMCPRuleLimitEnablePositionXMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionXMin() : igdeMetaPropertyMCT(
	"limit.enablePositionXMin", "Animator.WPAPanelRuleLimit.EnablePositionXMin"){}
	
	~aeMCPRuleLimitEnablePositionXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePositionXMin();
	}
};

class aeMCPRuleLimitEnablePositionXMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionXMax() : igdeMetaPropertyMCT(
	"limit.enablePositionXMax", "Animator.WPAPanelRuleLimit.EnablePositionXMax"){}
	
	~aeMCPRuleLimitEnablePositionXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePositionXMax();
	}
};

class aeMCPRuleLimitEnablePositionYMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMin() : igdeMetaPropertyMCT(
	"limit.enablePositionYMin", "Animator.WPAPanelRuleLimit.EnablePositionYMin"){}
	
	~aeMCPRuleLimitEnablePositionYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePositionYMin();
	}
};

class aeMCPRuleLimitEnablePositionYMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMax() : igdeMetaPropertyMCT(
	"limit.enablePositionYMax", "Animator.WPAPanelRuleLimit.EnablePositionYMax"){}
	
	~aeMCPRuleLimitEnablePositionYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePositionYMax();
	}
};

class aeMCPRuleLimitEnablePositionZMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMin() : igdeMetaPropertyMCT(
	"limit.enablePositionZMin", "Animator.WPAPanelRuleLimit.EnablePositionZMin"){}
	
	~aeMCPRuleLimitEnablePositionZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePositionZMin();
	}
};

class aeMCPRuleLimitEnablePositionZMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMax() : igdeMetaPropertyMCT(
	"limit.enablePositionZMax", "Animator.WPAPanelRuleLimit.EnablePositionZMax"){}
	
	~aeMCPRuleLimitEnablePositionZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePositionZMax();
	}
};

class aeMCPRuleLimitEnableRotationXMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMin() : igdeMetaPropertyMCT(
	"limit.enableRotationXMin", "Animator.WPAPanelRuleLimit.EnableRotationXMin"){}
	
	~aeMCPRuleLimitEnableRotationXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableRotationXMin();
	}
};

class aeMCPRuleLimitEnableRotationXMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMax() : igdeMetaPropertyMCT(
	"limit.enableRotationXMax", "Animator.WPAPanelRuleLimit.EnableRotationXMax"){}
	
	~aeMCPRuleLimitEnableRotationXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableRotationXMax();
	}
};

class aeMCPRuleLimitEnableRotationYMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMin() : igdeMetaPropertyMCT(
	"limit.enableRotationYMin", "Animator.WPAPanelRuleLimit.EnableRotationYMin"){}
	
	~aeMCPRuleLimitEnableRotationYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableRotationYMin();
	}
};

class aeMCPRuleLimitEnableRotationYMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMax() : igdeMetaPropertyMCT(
	"limit.enableRotationYMax", "Animator.WPAPanelRuleLimit.EnableRotationYMax"){}
	
	~aeMCPRuleLimitEnableRotationYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableRotationYMax();
	}
};

class aeMCPRuleLimitEnableRotationZMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMin() : igdeMetaPropertyMCT(
	"limit.enableRotationZMin", "Animator.WPAPanelRuleLimit.EnableRotationZMin"){}
	
	~aeMCPRuleLimitEnableRotationZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableRotationZMin();
	}
};

class aeMCPRuleLimitEnableRotationZMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMax() : igdeMetaPropertyMCT(
	"limit.enableRotationZMax", "Animator.WPAPanelRuleLimit.EnableRotationZMax"){}
	
	~aeMCPRuleLimitEnableRotationZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableRotationZMax();
	}
};

class aeMCPRuleLimitEnableScalingXMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMin() : igdeMetaPropertyMCT(
	"limit.enableScalingXMin", "Animator.WPAPanelRuleLimit.EnableScalingXMin"){}
	
	~aeMCPRuleLimitEnableScalingXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableScalingXMin();
	}
};

class aeMCPRuleLimitEnableScalingXMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMax() : igdeMetaPropertyMCT(
	"limit.enableScalingXMax", "Animator.WPAPanelRuleLimit.EnableScalingXMax"){}
	
	~aeMCPRuleLimitEnableScalingXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableScalingXMax();
	}
};

class aeMCPRuleLimitEnableScalingYMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMin() : igdeMetaPropertyMCT(
	"limit.enableScalingYMin", "Animator.WPAPanelRuleLimit.EnableScalingYMin"){}
	
	~aeMCPRuleLimitEnableScalingYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableScalingYMin();
	}
};

class aeMCPRuleLimitEnableScalingYMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMax() : igdeMetaPropertyMCT(
	"limit.enableScalingYMax", "Animator.WPAPanelRuleLimit.EnableScalingYMax"){}
	
	~aeMCPRuleLimitEnableScalingYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableScalingYMax();
	}
};

class aeMCPRuleLimitEnableScalingZMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMin() : igdeMetaPropertyMCT(
	"limit.enableScalingZMin", "Animator.WPAPanelRuleLimit.EnableScalingZMin"){}
	
	~aeMCPRuleLimitEnableScalingZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableScalingZMin();
	}
};

class aeMCPRuleLimitEnableScalingZMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMax() : igdeMetaPropertyMCT(
	"limit.enableScalingZMax", "Animator.WPAPanelRuleLimit.EnableScalingZMax"){}
	
	~aeMCPRuleLimitEnableScalingZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableScalingZMax();
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMin : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMin() : igdeMetaPropertyMCT(
	"limit.enableVertexPositionSetMin", "Animator.WPAPanelRuleLimit.EnableVertexPositionSetMin"){}
	
	~aeMCPRuleLimitEnableVertexPositionSetMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSetMin();
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMax : public aeRuleLimit::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMax() : igdeMetaPropertyMCT(
	"limit.enableVertexPositionSetMax", "Animator.WPAPanelRuleLimit.EnableVertexPositionSetMax"){}
	
	~aeMCPRuleLimitEnableVertexPositionSetMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSetMax();
	}
};


#endif