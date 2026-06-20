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
	aeMCPRuleBoneTransformatorMinTranslation() : aeTMCPAnimatorRuleType("bonetransformator.minTranslation",
		"@Animator.WPAPanelRuleBoneTransformator.MinTranslation", "@Animator.WPAPanelRuleBoneTransformator.MinTranslation.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorMinTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minTranslation;
	}
};

class aeMCPRuleBoneTransformatorMaxTranslation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMaxTranslation() : aeTMCPAnimatorRuleType("bonetransformator.maxTranslation",
		"@Animator.WPAPanelRuleBoneTransformator.MaxTranslation", "@Animator.WPAPanelRuleBoneTransformator.MaxTranslation.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorMaxTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxTranslation;
	}
};

class aeMCPRuleBoneTransformatorMinRotation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMinRotation() : aeTMCPAnimatorRuleType("bonetransformator.minRotation",
		"@Animator.WPAPanelRuleBoneTransformator.MinRotation", "@Animator.WPAPanelRuleBoneTransformator.MinRotation.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minRotation;
	}
};

class aeMCPRuleBoneTransformatorMaxRotation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMaxRotation() : aeTMCPAnimatorRuleType("bonetransformator.maxRotation",
		"@Animator.WPAPanelRuleBoneTransformator.MaxRotation", "@Animator.WPAPanelRuleBoneTransformator.MaxRotation.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxRotation;
	}
};

class aeMCPRuleBoneTransformatorMinScaling : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMinScaling() : aeTMCPAnimatorRuleType("bonetransformator.minScaling",
		"@Animator.WPAPanelRuleBoneTransformator.MinScaling", "@Animator.WPAPanelRuleBoneTransformator.MinScaling.ToolTip"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	};
	
	~aeMCPRuleBoneTransformatorMinScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minScaling;
	}
};

class aeMCPRuleBoneTransformatorMaxScaling : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMaxScaling() : aeTMCPAnimatorRuleType("bonetransformator.maxScaling",
		"@Animator.WPAPanelRuleBoneTransformator.MaxScaling", "@Animator.WPAPanelRuleBoneTransformator.MaxScaling.ToolTip"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	};
	
	~aeMCPRuleBoneTransformatorMaxScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxScaling;
	}
};

class aeMCPRuleBoneTransformatorAxis : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorAxis() : aeTMCPAnimatorRuleType("bonetransformator.axis",
		"@Animator.WPAPanelRuleBoneTransformator.Axis", "@Animator.WPAPanelRuleBoneTransformator.Axis.ToolTip"){
		SetDefaultValue(decVector(0.0f, 0.0f, 1.0f));
	};
	
	~aeMCPRuleBoneTransformatorAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).axis;
	}
};

class aeMCPRuleBoneTransformatorMinAngle : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBoneTransformatorMinAngle() : aeTMCPAnimatorRuleType("bonetransformator.minAngle",
		"@Animator.WPAPanelRuleBoneTransformator.MinAngle", "@Animator.WPAPanelRuleBoneTransformator.MinAngle.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorMinAngle() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minAngle;
	}
};

class aeMCPRuleBoneTransformatorMaxAngle : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBoneTransformatorMaxAngle() : aeTMCPAnimatorRuleType("bonetransformator.maxAngle",
		"@Animator.WPAPanelRuleBoneTransformator.MaxAngle", "@Animator.WPAPanelRuleBoneTransformator.MaxAngle.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorMaxAngle() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxAngle;
	}
};

class aeMCPRuleBoneTransformatorEnablePosition : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnablePosition() : aeTMCPAnimatorRuleType("bonetransformator.enablePosition",
		"@Animator.WPAPanelRuleBoneTransformator.EnablePosition", "@Animator.WPAPanelRuleBoneTransformator.EnablePosition.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleBoneTransformatorEnableOrientation : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnableOrientation() : aeTMCPAnimatorRuleType("bonetransformator.enableOrientation",
		"@Animator.WPAPanelRuleBoneTransformator.EnableRotation", "@Animator.WPAPanelRuleBoneTransformator.EnableRotation.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleBoneTransformatorEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleBoneTransformatorEnableSize : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnableSize() : aeTMCPAnimatorRuleType("bonetransformator.enableSize",
		"@Animator.WPAPanelRuleBoneTransformator.EnableSize", "@Animator.WPAPanelRuleBoneTransformator.EnableSize.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleBoneTransformatorUseAxis : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorUseAxis() : aeTMCPAnimatorRuleType("bonetransformator.useAxis",
		"@Animator.WPAPanelRuleBoneTransformator.UseAxis", "@Animator.WPAPanelRuleBoneTransformator.UseAxis.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorUseAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).useAxis;
	}
};

class aeMCPRuleBoneTransformatorTargetBone : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleBoneTransformatorTargetBone() : aeTMCPAnimatorRuleType("bonetransformator.targetBone",
		"@Animator.WPAPanelRuleBoneTransformator.TargetBone", "@Animator.WPAPanelRuleBoneTransformator.TargetBone.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorTargetBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetBone;
	}
};

class aeMCPRuleBoneTransformatorInputBone : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleBoneTransformatorInputBone() : aeTMCPAnimatorRuleType("bonetransformator.inputBone",
		"@Animator.WPAPanelRuleBoneTransformator.InputBone", "@Animator.WPAPanelRuleBoneTransformator.InputBone.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorInputBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).inputBone;
	}
};

class aeMCPRuleBoneTransformatorCoordinateFrame : public aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleBoneTransformator::eCoordinateFrames>>{
public:
	aeMCPRuleBoneTransformatorCoordinateFrame() : aeTMCPAnimatorRuleType("bonetransformator.coordinateFrame",
		"@Animator.WPAPanelRuleBoneTransformator.CoordFrame", "@Animator.WPAPanelRuleBoneTransformator.CoordFrame.ToolTip"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleBoneTransformator::ecfBoneLocal,
			deAnimatorRuleBoneTransformator::ecfComponent,
			deAnimatorRuleBoneTransformator::ecfTargetBone));
		SetDefaultValueEnum(deAnimatorRuleBoneTransformator::ecfComponent);
	};
	
	~aeMCPRuleBoneTransformatorCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).coordinateFrame;
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
	aeMCPRuleBoneTransformatorInputSource() : aeTMCPAnimatorRuleType("bonetransformator.inputSource",
		"@Animator.WPAPanelRuleBoneTransformator.InputSource", "@Animator.WPAPanelRuleBoneTransformator.InputSource.ToolTip"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleBoneTransformator::eisTargetBlend,
			deAnimatorRuleBoneTransformator::eisTargetDirect,
			deAnimatorRuleBoneTransformator::eisBoneState,
			deAnimatorRuleBoneTransformator::eisBoneStateInverse));
		SetDefaultValueEnum(deAnimatorRuleBoneTransformator::eisTargetBlend);
	};
	
	~aeMCPRuleBoneTransformatorInputSource() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).inputSource;
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
	aeMCPRuleBoneTransformatorTargetTranslation() : aeTMCPRuleTarget("bonetransformator.targetTranslation",
		"@Animator.Target.Translation", "@Animator.Target.Translation.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorTargetTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetTranslation;
	}
};

class aeMCPRuleBoneTransformatorTargetRotation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleBoneTransformatorTargetRotation() : aeTMCPRuleTarget("bonetransformator.targetRotation",
		"@Animator.Target.Rotation", "@Animator.Target.Rotation.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorTargetRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetRotation;
	}
};


class aeMCPRuleBoneTransformatorTargetScaling : public aeTMCPRuleTarget<aeTMCPAnimatorRuleBoneTransformator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleBoneTransformatorTargetScaling() : aeTMCPRuleTarget("bonetransformator.targetScaling",
		"@Animator.Target.Scaling", "@Animator.Target.Scaling.ToolTip"){
	};
	
	~aeMCPRuleBoneTransformatorTargetScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetScaling;
	}
};


#endif