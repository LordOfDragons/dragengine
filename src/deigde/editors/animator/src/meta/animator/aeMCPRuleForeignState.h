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

#ifndef _AEMCPRULEFOREIGNSTATE_H_
#define _AEMCPRULEFOREIGNSTATE_H_

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleForeignState.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleForeignStateForeignBone :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleForeignStateForeignBone() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.foreignBone", "Animator.WPAPanelRuleForeignState.Bone")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleForeignStateForeignBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpForeignBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleForeignStateForeignVertexPositionSet :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleForeignStateForeignVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.foreignVertexPositionSet",
		"Animator.WPAPanelRuleForeignState.VertexPositionSet")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleForeignStateForeignVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpForeignVertexPositionSet;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenVpsNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleForeignStateScalePosition :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScalePosition() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.scalePosition",
		"Animator.WPAPanelRuleForeignState.ScalePosition")
	{
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScalePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpScalePosition;
	}
};

class aeMCPRuleForeignStateScaleOrientation :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleOrientation() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.scaleOrientation",
		"Animator.WPAPanelRuleForeignState.ScaleRotation")
	{
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScaleOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpScaleOrientation;
	}
};

class aeMCPRuleForeignStateScaleSize :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleSize() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.scaleSize",
		"Animator.WPAPanelRuleForeignState.ScaleSize")
	{
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScaleSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpScaleSize;
	}
};

class aeMCPRuleForeignStateScaleVertexPositionSet :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.scaleVertexPositionSet",
		"Animator.WPAPanelRuleForeignState.ScaleVertexPositionSet")
	{
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScaleVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpScaleVertexPositionSet;
	}
};

class aeMCPRuleForeignStateEnablePosition :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnablePosition() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.enablePosition",
		"Animator.WPAPanelRuleForeignState.EnablePosition")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleForeignStateEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePosition;
	}
};

class aeMCPRuleForeignStateEnableOrientation :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableOrientation() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.enableOrientation",
		"Animator.WPAPanelRuleForeignState.EnableRotation")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleForeignStateEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableOrientation;
	}
};

class aeMCPRuleForeignStateEnableSize :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableSize() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.enableSize", "Animator.WPAPanelRuleForeignState.EnableSize"){}
	
	~aeMCPRuleForeignStateEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableSize;
	}
};

class aeMCPRuleForeignStateEnableVertexPositionSet :
	public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.enableVertexPositionSet",
		"Animator.WPAPanelRuleForeignState.EnableVertexPositionSet")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleForeignStateEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableVertexPositionSet;
	}
};

class aeMCPRuleForeignStateSourceCoordinateFrame :
	public aeRuleForeignState::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>>{
public:
	aeMCPRuleForeignStateSourceCoordinateFrame() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.sourceCoordinateFrame",
		"Animator.WPAPanelRuleForeignState.SrcCoordFrame")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleForeignState::ecfBoneLocal,
			deAnimatorRuleForeignState::ecfComponent));
		SetDefaultValueEnum(deAnimatorRuleForeignState::ecfBoneLocal);
	}
	
	~aeMCPRuleForeignStateSourceCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpSourceCoordinateFrame;
	}

	void GetChoiceItemInfoEnum(const ContextRef &context,
	deAnimatorRuleForeignState::eCoordinateFrames choice,
	igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleForeignState::ecfBoneLocal:
			info.SetAll("@Animator.WPAPanelRuleForeignState.CoordFrame.BoneLocal");
			break;
			
		case deAnimatorRuleForeignState::ecfComponent:
			info.SetAll("@Animator.WPAPanelRuleForeignState.CoordFrame.Component");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleForeignStateDestCoordinateFrame :
	public aeRuleForeignState::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>>{
public:
	aeMCPRuleForeignStateDestCoordinateFrame() : igdeMetaPropertyMCT(
		"animator.rule_foreign_state.destCoordinateFrame",
		"Animator.WPAPanelRuleForeignState.DestCoordFrame")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleForeignState::ecfBoneLocal,
			deAnimatorRuleForeignState::ecfComponent));
		SetDefaultValueEnum(deAnimatorRuleForeignState::ecfBoneLocal);
	}
	
	~aeMCPRuleForeignStateDestCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpDestCoordinateFrame;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context,
	deAnimatorRuleForeignState::eCoordinateFrames choice,
	igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleForeignState::ecfBoneLocal:
			info.SetAll("@Animator.WPAPanelRuleForeignState.CoordFrame.BoneLocal");
			break;
			
		case deAnimatorRuleForeignState::ecfComponent:
			info.SetAll("@Animator.WPAPanelRuleForeignState.CoordFrame.Component");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


class aeMCPRuleForeignStateTargetPosition : public aeRule::MetaPropertyTarget<aeRuleForeignState>{
public:
	aeMCPRuleForeignStateTargetPosition() : MetaPropertyTarget(
		"animator.rule_foreign_state.targetPosition", "Animator.Target.Position"){}
	~aeMCPRuleForeignStateTargetPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetPosition;
	}
};

class aeMCPRuleForeignStateTargetOrientation : public aeRule::MetaPropertyTarget<aeRuleForeignState>{
public:
	aeMCPRuleForeignStateTargetOrientation() : MetaPropertyTarget(
		"animator.rule_foreign_state.targetOrientation", "Animator.Target.Orientation"){}
	~aeMCPRuleForeignStateTargetOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetOrientation;
	}
};


class aeMCPRuleForeignStateTargetSize : public aeRule::MetaPropertyTarget<aeRuleForeignState>{
public:
	aeMCPRuleForeignStateTargetSize() : MetaPropertyTarget(
		"animator.rule_foreign_state.targetSize", "Animator.Target.Size"){}
	~aeMCPRuleForeignStateTargetSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetSize;
	}
};

class aeMCPRuleForeignStateTargetVertexPositionSet :
	public aeRule::MetaPropertyTarget<aeRuleForeignState>{
public:
	aeMCPRuleForeignStateTargetVertexPositionSet() : MetaPropertyTarget(
		"animator.rule_foreign_state.targetVertexPositionSet", "Animator.Target.VertexPositionSet"){}
	~aeMCPRuleForeignStateTargetVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetVertexPositionSet;
	}
};

#endif