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

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleBoneTransformator.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleBoneTransformatorMinTranslation :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMinTranslation() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.minTranslation",
		"Animator.WPAPanelRuleBoneTransformator.MinTranslation"){}
	
	~aeMCPRuleBoneTransformatorMinTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinTranslation;
	}
};

class aeMCPRuleBoneTransformatorMaxTranslation :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMaxTranslation() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.maxTranslation",
		"Animator.WPAPanelRuleBoneTransformator.MaxTranslation"){}
	
	~aeMCPRuleBoneTransformatorMaxTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxTranslation;
	}
};

class aeMCPRuleBoneTransformatorMinRotation :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleBoneTransformatorMinRotation() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.minRotation",
		"Animator.WPAPanelRuleBoneTransformator.MinRotation"){}
	
	~aeMCPRuleBoneTransformatorMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinRotation;
	}
};

class aeMCPRuleBoneTransformatorMaxRotation :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleBoneTransformatorMaxRotation() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.maxRotation",
		"Animator.WPAPanelRuleBoneTransformator.MaxRotation"){}
	
	~aeMCPRuleBoneTransformatorMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxRotation;
	}
};

class aeMCPRuleBoneTransformatorMinScaling :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMinScaling() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.minScaling",
		"Animator.WPAPanelRuleBoneTransformator.MinScaling")
	{
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleBoneTransformatorMinScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinScaling;
	}
};

class aeMCPRuleBoneTransformatorMaxScaling :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorMaxScaling() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.maxScaling",
		"Animator.WPAPanelRuleBoneTransformator.MaxScaling")
	{
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleBoneTransformatorMaxScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxScaling;
	}
};

class aeMCPRuleBoneTransformatorAxis :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleBoneTransformatorAxis() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.axis",
		"Animator.WPAPanelRuleBoneTransformator.Axis")
	{
		SetDefaultValue(decVector(0.0f, 0.0f, 1.0f));
	}
	
	~aeMCPRuleBoneTransformatorAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpAxis;
	}
};

class aeMCPRuleBoneTransformatorMinAngle :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBoneTransformatorMinAngle() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.minAngle",
		"Animator.WPAPanelRuleBoneTransformator.MinAngle"){}
	
	~aeMCPRuleBoneTransformatorMinAngle() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinAngle;
	}
};

class aeMCPRuleBoneTransformatorMaxAngle :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleBoneTransformatorMaxAngle() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.maxAngle",
		"Animator.WPAPanelRuleBoneTransformator.MaxAngle"){}
	
	~aeMCPRuleBoneTransformatorMaxAngle() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxAngle;
	}
};

class aeMCPRuleBoneTransformatorEnablePosition :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnablePosition() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.enablePosition",
		"Animator.WPAPanelRuleBoneTransformator.EnablePosition"){}
	
	~aeMCPRuleBoneTransformatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePosition;
	}
};

class aeMCPRuleBoneTransformatorEnableOrientation :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnableOrientation() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.enableOrientation",
		"Animator.WPAPanelRuleBoneTransformator.EnableRotation")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleBoneTransformatorEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableOrientation;
	}
};

class aeMCPRuleBoneTransformatorEnableSize :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorEnableSize() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.enableSize",
		"Animator.WPAPanelRuleBoneTransformator.EnableSize"){}
	
	~aeMCPRuleBoneTransformatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableSize;
	}
};

class aeMCPRuleBoneTransformatorUseAxis :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleBoneTransformatorUseAxis() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.useAxis",
		"Animator.WPAPanelRuleBoneTransformator.UseAxis"){}
	
	~aeMCPRuleBoneTransformatorUseAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpUseAxis;
	}
};

class aeMCPRuleBoneTransformatorTargetBone :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleBoneTransformatorTargetBone() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.targetBone",
		"Animator.WPAPanelRuleBoneTransformator.TargetBone")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleBoneTransformatorTargetBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleBoneTransformatorInputBone :
	public aeRuleBoneTransformator::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleBoneTransformatorInputBone() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.inputBone",
		"Animator.WPAPanelRuleBoneTransformator.InputBone")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleBoneTransformatorInputBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpInputBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleBoneTransformatorCoordinateFrame :
	public aeRuleBoneTransformator::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleBoneTransformator::eCoordinateFrames>>{
public:
	aeMCPRuleBoneTransformatorCoordinateFrame() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.coordinateFrame",
		"Animator.WPAPanelRuleBoneTransformator.CoordFrame")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleBoneTransformator::ecfBoneLocal,
			deAnimatorRuleBoneTransformator::ecfComponent,
			deAnimatorRuleBoneTransformator::ecfTargetBone));
		SetDefaultValueEnum(deAnimatorRuleBoneTransformator::ecfComponent);
	}
	
	~aeMCPRuleBoneTransformatorCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpCoordinateFrame;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context,
	deAnimatorRuleBoneTransformator::eCoordinateFrames choice,
	igdeMetaContextItemInfo &info) const override{
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

class aeMCPRuleBoneTransformatorInputSource :
	public aeRuleBoneTransformator::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleBoneTransformator::eInputSources>>{
public:
	aeMCPRuleBoneTransformatorInputSource() : igdeMetaPropertyMCT(
		"animator.rule_bone_transformator.inputSource",
		"Animator.WPAPanelRuleBoneTransformator.InputSource")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleBoneTransformator::eisTargetBlend,
			deAnimatorRuleBoneTransformator::eisTargetDirect,
			deAnimatorRuleBoneTransformator::eisBoneState,
			deAnimatorRuleBoneTransformator::eisBoneStateInverse));
		SetDefaultValueEnum(deAnimatorRuleBoneTransformator::eisTargetBlend);
	}
	
	~aeMCPRuleBoneTransformatorInputSource() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpInputSource;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context,
	deAnimatorRuleBoneTransformator::eInputSources choice,
	igdeMetaContextItemInfo &info) const override{
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


class aeMCPRuleBoneTransformatorTargetTranslation :
	public aeRule::MetaPropertyTarget<aeRuleBoneTransformator>{
public:
	aeMCPRuleBoneTransformatorTargetTranslation() : MetaPropertyTarget(
		"animator.rule_bone_transformator.targetTranslation", "Animator.Target.Translation"){}
	~aeMCPRuleBoneTransformatorTargetTranslation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetTranslation;
	}
};


class aeMCPRuleBoneTransformatorTargetRotation :
	public aeRule::MetaPropertyTarget<aeRuleBoneTransformator>{
public:
	aeMCPRuleBoneTransformatorTargetRotation() : MetaPropertyTarget(
		"animator.rule_bone_transformator.targetRotation", "Animator.Target.Rotation"){}
	~aeMCPRuleBoneTransformatorTargetRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetRotation;
	}
};


class aeMCPRuleBoneTransformatorTargetScaling :
	public aeRule::MetaPropertyTarget<aeRuleBoneTransformator>{
public:
	aeMCPRuleBoneTransformatorTargetScaling() : MetaPropertyTarget(
		"animator.rule_bone_transformator.targetScaling", "Animator.Target.Scaling"){}
	~aeMCPRuleBoneTransformatorTargetScaling() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetScaling;
	}
};


#endif