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
	aeMCPRuleLimitMinPosition() : aeTMCPAnimatorRuleType("limit.minPosition",
		"@Animator.WPAPanelRuleLimit.MinPosition", "@Animator.WPAPanelRuleLimit.MinPosition.ToolTip"){
	};
	
	~aeMCPRuleLimitMinPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minPosition;
	}
};

class aeMCPRuleLimitMaxPosition : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxPosition() : aeTMCPAnimatorRuleType("limit.maxPosition",
		"@Animator.WPAPanelRuleLimit.MaxPosition", "@Animator.WPAPanelRuleLimit.MaxPosition.ToolTip"){
	};
	
	~aeMCPRuleLimitMaxPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxPosition;
	}
};

class aeMCPRuleLimitMinRotation : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinRotation() : aeTMCPAnimatorRuleType("limit.minRotation",
		"@Animator.WPAPanelRuleLimit.MinRotation", "@Animator.WPAPanelRuleLimit.MinRotation.ToolTip"){
	};
	
	~aeMCPRuleLimitMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minRotation;
	}
};

class aeMCPRuleLimitMaxRotation : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxRotation() : aeTMCPAnimatorRuleType("limit.maxRotation",
		"@Animator.WPAPanelRuleLimit.MaxRotation", "@Animator.WPAPanelRuleLimit.MaxRotation.ToolTip"){
	};
	
	~aeMCPRuleLimitMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxRotation;
	}
};

class aeMCPRuleLimitMinScaling : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMinScaling() : aeTMCPAnimatorRuleType("limit.minScaling",
		"@Animator.WPAPanelRuleLimit.MinScaling", "@Animator.WPAPanelRuleLimit.MinScaling.ToolTip"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	};
	
	~aeMCPRuleLimitMinScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minScaling;
	}
};

class aeMCPRuleLimitMaxScaling : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleLimitMaxScaling() : aeTMCPAnimatorRuleType("limit.maxScaling",
		"@Animator.WPAPanelRuleLimit.MaxScaling", "@Animator.WPAPanelRuleLimit.MaxScaling.ToolTip"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	};
	
	~aeMCPRuleLimitMaxScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxScaling;
	}
};

class aeMCPRuleLimitMinVertexPositionSet : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMinVertexPositionSet() : aeTMCPAnimatorRuleType("limit.minVertexPositionSet",
		"@Animator.WPAPanelRuleLimit.MinVertexPositionSet", "@Animator.WPAPanelRuleLimit.MinVertexPositionSet.ToolTip"){
	};
	
	~aeMCPRuleLimitMinVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minVertexPositionSet;
	}
};

class aeMCPRuleLimitMaxVertexPositionSet : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleLimitMaxVertexPositionSet() : aeTMCPAnimatorRuleType("limit.maxVertexPositionSet",
		"@Animator.WPAPanelRuleLimit.MaxVertexPositionSet", "@Animator.WPAPanelRuleLimit.MaxVertexPositionSet.ToolTip"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPRuleLimitMaxVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxVertexPositionSet;
	}
};

class aeMCPRuleLimitTargetBone : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleLimitTargetBone() : aeTMCPAnimatorRuleType("limit.targetBone",
		"@Animator.WPAPanelRuleLimit.TargetBone", "@Animator.WPAPanelRuleLimit.TargetBone.ToolTip"){
			SetEnableAllowed(true);
		};
	
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
	aeMCPRuleLimitCoordinateFrame() : aeTMCPAnimatorRuleType("limit.coordinateFrame",
		"@Animator.WPAPanelRuleLimit.CoordFrame", "@Animator.WPAPanelRuleLimit.CoordFrame.ToolTip"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleLimit::ecfBoneLocal,
			deAnimatorRuleLimit::ecfComponent,
			deAnimatorRuleLimit::ecfTargetBone));
		SetDefaultValueEnum(deAnimatorRuleLimit::ecfComponent);
	};
	
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
	aeMCPRuleLimitEnablePositionXMin() : aeTMCPAnimatorRuleType("limit.enablePositionXMin",
		"@Animator.WPAPanelRuleLimit.EnablePositionXMin", "@Animator.WPAPanelRuleLimit.EnablePositionXMin.ToolTip"){};
	
	~aeMCPRuleLimitEnablePositionXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionXMin;
	}
};

class aeMCPRuleLimitEnablePositionXMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionXMax() : aeTMCPAnimatorRuleType("limit.enablePositionXMax",
		"@Animator.WPAPanelRuleLimit.EnablePositionXMax", "@Animator.WPAPanelRuleLimit.EnablePositionXMax.ToolTip"){};
	
	~aeMCPRuleLimitEnablePositionXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionXMax;
	}
};

class aeMCPRuleLimitEnablePositionYMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMin() : aeTMCPAnimatorRuleType("limit.enablePositionYMin",
		"@Animator.WPAPanelRuleLimit.EnablePositionYMin", "@Animator.WPAPanelRuleLimit.EnablePositionYMin.ToolTip"){};
	
	~aeMCPRuleLimitEnablePositionYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionYMin;
	}
};

class aeMCPRuleLimitEnablePositionYMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionYMax() : aeTMCPAnimatorRuleType("limit.enablePositionYMax",
		"@Animator.WPAPanelRuleLimit.EnablePositionYMax", "@Animator.WPAPanelRuleLimit.EnablePositionYMax.ToolTip"){};
	
	~aeMCPRuleLimitEnablePositionYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionYMax;
	}
};

class aeMCPRuleLimitEnablePositionZMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMin() : aeTMCPAnimatorRuleType("limit.enablePositionZMin",
		"@Animator.WPAPanelRuleLimit.EnablePositionZMin", "@Animator.WPAPanelRuleLimit.EnablePositionZMin.ToolTip"){};
	
	~aeMCPRuleLimitEnablePositionZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionZMin;
	}
};

class aeMCPRuleLimitEnablePositionZMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnablePositionZMax() : aeTMCPAnimatorRuleType("limit.enablePositionZMax",
		"@Animator.WPAPanelRuleLimit.EnablePositionZMax", "@Animator.WPAPanelRuleLimit.EnablePositionZMax.ToolTip"){};
	
	~aeMCPRuleLimitEnablePositionZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePositionZMax;
	}
};

class aeMCPRuleLimitEnableRotationXMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMin() : aeTMCPAnimatorRuleType("limit.enableRotationXMin",
		"@Animator.WPAPanelRuleLimit.EnableRotationXMin", "@Animator.WPAPanelRuleLimit.EnableRotationXMin.ToolTip"){};
	
	~aeMCPRuleLimitEnableRotationXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationXMin;
	}
};

class aeMCPRuleLimitEnableRotationXMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationXMax() : aeTMCPAnimatorRuleType("limit.enableRotationXMax",
		"@Animator.WPAPanelRuleLimit.EnableRotationXMax", "@Animator.WPAPanelRuleLimit.EnableRotationXMax.ToolTip"){};
	
	~aeMCPRuleLimitEnableRotationXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationXMax;
	}
};

class aeMCPRuleLimitEnableRotationYMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMin() : aeTMCPAnimatorRuleType("limit.enableRotationYMin",
		"@Animator.WPAPanelRuleLimit.EnableRotationYMin", "@Animator.WPAPanelRuleLimit.EnableRotationYMin.ToolTip"){};
	
	~aeMCPRuleLimitEnableRotationYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationYMin;
	}
};

class aeMCPRuleLimitEnableRotationYMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationYMax() : aeTMCPAnimatorRuleType("limit.enableRotationYMax",
		"@Animator.WPAPanelRuleLimit.EnableRotationYMax", "@Animator.WPAPanelRuleLimit.EnableRotationYMax.ToolTip"){};
	
	~aeMCPRuleLimitEnableRotationYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationYMax;
	}
};

class aeMCPRuleLimitEnableRotationZMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMin() : aeTMCPAnimatorRuleType("limit.enableRotationZMin",
		"@Animator.WPAPanelRuleLimit.EnableRotationZMin", "@Animator.WPAPanelRuleLimit.EnableRotationZMin.ToolTip"){};
	
	~aeMCPRuleLimitEnableRotationZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationZMin;
	}
};

class aeMCPRuleLimitEnableRotationZMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableRotationZMax() : aeTMCPAnimatorRuleType("limit.enableRotationZMax",
		"@Animator.WPAPanelRuleLimit.EnableRotationZMax", "@Animator.WPAPanelRuleLimit.EnableRotationZMax.ToolTip"){};
	
	~aeMCPRuleLimitEnableRotationZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotationZMax;
	}
};

class aeMCPRuleLimitEnableScalingXMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMin() : aeTMCPAnimatorRuleType("limit.enableScalingXMin",
		"@Animator.WPAPanelRuleLimit.EnableScalingXMin", "@Animator.WPAPanelRuleLimit.EnableScalingXMin.ToolTip"){};
	
	~aeMCPRuleLimitEnableScalingXMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingXMin;
	}
};

class aeMCPRuleLimitEnableScalingXMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingXMax() : aeTMCPAnimatorRuleType("limit.enableScalingXMax",
		"@Animator.WPAPanelRuleLimit.EnableScalingXMax", "@Animator.WPAPanelRuleLimit.EnableScalingXMax.ToolTip"){};
	
	~aeMCPRuleLimitEnableScalingXMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingXMax;
	}
};

class aeMCPRuleLimitEnableScalingYMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMin() : aeTMCPAnimatorRuleType("limit.enableScalingYMin",
		"@Animator.WPAPanelRuleLimit.EnableScalingYMin", "@Animator.WPAPanelRuleLimit.EnableScalingYMin.ToolTip"){};
	
	~aeMCPRuleLimitEnableScalingYMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingYMin;
	}
};

class aeMCPRuleLimitEnableScalingYMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingYMax() : aeTMCPAnimatorRuleType("limit.enableScalingYMax",
		"@Animator.WPAPanelRuleLimit.EnableScalingYMax", "@Animator.WPAPanelRuleLimit.EnableScalingYMax.ToolTip"){};
	
	~aeMCPRuleLimitEnableScalingYMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingYMax;
	}
};

class aeMCPRuleLimitEnableScalingZMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMin() : aeTMCPAnimatorRuleType("limit.enableScalingZMin",
		"@Animator.WPAPanelRuleLimit.EnableScalingZMin", "@Animator.WPAPanelRuleLimit.EnableScalingZMin.ToolTip"){};
	
	~aeMCPRuleLimitEnableScalingZMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingZMin;
	}
};

class aeMCPRuleLimitEnableScalingZMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableScalingZMax() : aeTMCPAnimatorRuleType("limit.enableScalingZMax",
		"@Animator.WPAPanelRuleLimit.EnableScalingZMax", "@Animator.WPAPanelRuleLimit.EnableScalingZMax.ToolTip"){};
	
	~aeMCPRuleLimitEnableScalingZMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableScalingZMax;
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMin : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMin() : aeTMCPAnimatorRuleType("limit.enableVertexPositionSetMin",
		"@Animator.WPAPanelRuleLimit.EnableVertexPositionSetMin", "@Animator.WPAPanelRuleLimit.EnableVertexPositionSetMin.ToolTip"){};
	
	~aeMCPRuleLimitEnableVertexPositionSetMin() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSetMin;
	}
};

class aeMCPRuleLimitEnableVertexPositionSetMax : public aeTMCPAnimatorRuleLimit<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleLimitEnableVertexPositionSetMax() : aeTMCPAnimatorRuleType("limit.enableVertexPositionSetMax",
		"@Animator.WPAPanelRuleLimit.EnableVertexPositionSetMax", "@Animator.WPAPanelRuleLimit.EnableVertexPositionSetMax.ToolTip"){};
	
	~aeMCPRuleLimitEnableVertexPositionSetMax() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSetMax;
	}
};


#endif