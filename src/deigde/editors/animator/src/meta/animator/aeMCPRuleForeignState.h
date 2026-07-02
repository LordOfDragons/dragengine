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


class aeMCPRuleForeignStateForeignBone : public aeRuleForeignState::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleForeignStateForeignBone() : igdeMetaPropertyMCT("foreignstate.foreignBone", "Animator.WPAPanelRuleForeignState.Bone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleForeignStateForeignBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPForeignBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleForeignStateForeignVertexPositionSet : public aeRuleForeignState::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleForeignStateForeignVertexPositionSet() : igdeMetaPropertyMCT(
	"foreignstate.foreignVertexPositionSet", "Animator.WPAPanelRuleForeignState.VertexPositionSet"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleForeignStateForeignVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPForeignVertexPositionSet();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenVPSNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleForeignStateScalePosition : public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScalePosition() : igdeMetaPropertyMCT(
	"foreignstate.scalePosition", "Animator.WPAPanelRuleForeignState.ScalePosition"){
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScalePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPScalePosition();
	}
};

class aeMCPRuleForeignStateScaleOrientation : public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleOrientation() : igdeMetaPropertyMCT(
	"foreignstate.scaleOrientation", "Animator.WPAPanelRuleForeignState.ScaleRotation"){
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScaleOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPScaleOrientation();
	}
};

class aeMCPRuleForeignStateScaleSize : public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleSize() : igdeMetaPropertyMCT(
	"foreignstate.scaleSize", "Animator.WPAPanelRuleForeignState.ScaleSize"){
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScaleSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPScaleSize();
	}
};

class aeMCPRuleForeignStateScaleVertexPositionSet : public aeRuleForeignState::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleVertexPositionSet() : igdeMetaPropertyMCT(
	"foreignstate.scaleVertexPositionSet", "Animator.WPAPanelRuleForeignState.ScaleVertexPositionSet"){
		SetDefaultValue(1.0f);
	}
	
	~aeMCPRuleForeignStateScaleVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPScaleVertexPositionSet();
	}
};

class aeMCPRuleForeignStateEnablePosition : public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnablePosition() : igdeMetaPropertyMCT(
	"foreignstate.enablePosition", "Animator.WPAPanelRuleForeignState.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleForeignStateEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePosition();
	}
};

class aeMCPRuleForeignStateEnableOrientation : public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableOrientation() : igdeMetaPropertyMCT(
	"foreignstate.enableOrientation", "Animator.WPAPanelRuleForeignState.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleForeignStateEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableOrientation();
	}
};

class aeMCPRuleForeignStateEnableSize : public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableSize() : igdeMetaPropertyMCT(
	"foreignstate.enableSize", "Animator.WPAPanelRuleForeignState.EnableSize"){}
	
	~aeMCPRuleForeignStateEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableSize();
	}
};

class aeMCPRuleForeignStateEnableVertexPositionSet : public aeRuleForeignState::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableVertexPositionSet() : igdeMetaPropertyMCT(
	"foreignstate.enableVertexPositionSet", "Animator.WPAPanelRuleForeignState.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleForeignStateEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSet();
	}
};

class aeMCPRuleForeignStateSourceCoordinateFrame : public aeRuleForeignState::MetaProperty<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>>{
public:
	aeMCPRuleForeignStateSourceCoordinateFrame() : igdeMetaPropertyMCT(
	"foreignstate.sourceCoordinateFrame", "Animator.WPAPanelRuleForeignState.SrcCoordFrame"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleForeignState::ecfBoneLocal,
			deAnimatorRuleForeignState::ecfComponent));
		SetDefaultValueEnum(deAnimatorRuleForeignState::ecfBoneLocal);
	}
	
	~aeMCPRuleForeignStateSourceCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPSourceCoordinateFrame();
	}

	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleForeignState::eCoordinateFrames choice, igdeMetaContextItemInfo &info) const override{
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

class aeMCPRuleForeignStateDestCoordinateFrame : public aeRuleForeignState::MetaProperty<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>>{
public:
	aeMCPRuleForeignStateDestCoordinateFrame() : igdeMetaPropertyMCT(
	"foreignstate.destCoordinateFrame", "Animator.WPAPanelRuleForeignState.DestCoordFrame"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleForeignState::ecfBoneLocal,
			deAnimatorRuleForeignState::ecfComponent));
		SetDefaultValueEnum(deAnimatorRuleForeignState::ecfBoneLocal);
	}
	
	~aeMCPRuleForeignStateDestCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPDestCoordinateFrame();
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleForeignState::eCoordinateFrames choice, igdeMetaContextItemInfo &info) const override{
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
	aeMCPRuleForeignStateTargetPosition() : MetaPropertyTarget("foreignstate.targetPosition", "Animator.Target.Position"){}
	~aeMCPRuleForeignStateTargetPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetPosition();
	}
};

class aeMCPRuleForeignStateTargetOrientation : public aeRule::MetaPropertyTarget<aeRuleForeignState>{
public:
	aeMCPRuleForeignStateTargetOrientation() : MetaPropertyTarget("foreignstate.targetOrientation", "Animator.Target.Orientation"){}
	~aeMCPRuleForeignStateTargetOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetOrientation();
	}
};


class aeMCPRuleForeignStateTargetSize : public aeRule::MetaPropertyTarget<aeRuleForeignState>{
public:
	aeMCPRuleForeignStateTargetSize() : MetaPropertyTarget("foreignstate.targetSize", "Animator.Target.Size"){}
	~aeMCPRuleForeignStateTargetSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetSize();
	}
};

class aeMCPRuleForeignStateTargetVertexPositionSet : public aeRule::MetaPropertyTarget<aeRuleForeignState>{
public:
	aeMCPRuleForeignStateTargetVertexPositionSet() : MetaPropertyTarget("foreignstate.targetVertexPositionSet", "Animator.Target.VertexPositionSet"){}
	~aeMCPRuleForeignStateTargetVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetVertexPositionSet();
	}
};

#endif