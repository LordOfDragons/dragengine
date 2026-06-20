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

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleForeignState.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleForeignStateForeignBone : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleForeignStateForeignBone() : aeTMCPAnimatorRuleType("foreignstate.foreignBone",
		"@Animator.WPAPanelRuleForeignState.Bone", "@Animator.WPAPanelRuleForeignState.Bone.ToolTip"){
			SetEnableAllowed(true);
		};
	
	~aeMCPRuleForeignStateForeignBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).foreignBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->hiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleForeignStateForeignVertexPositionSet : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleForeignStateForeignVertexPositionSet() : aeTMCPAnimatorRuleType("foreignstate.foreignVertexPositionSet",
		"@Animator.WPAPanelRuleForeignState.VertexPositionSet", "@Animator.WPAPanelRuleForeignState.VertexPositionSet.ToolTip"){
			SetEnableAllowed(true);
		};
	
	~aeMCPRuleForeignStateForeignVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).foreignVertexPositionSet;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->hiddenVPSNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleForeignStateScalePosition : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScalePosition() : aeTMCPAnimatorRuleType("foreignstate.scalePosition",
		"@Animator.WPAPanelRuleForeignState.ScalePosition", "@Animator.WPAPanelRuleForeignState.ScalePosition.ToolTip"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPRuleForeignStateScalePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).scalePosition;
	}
};

class aeMCPRuleForeignStateScaleOrientation : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleOrientation() : aeTMCPAnimatorRuleType("foreignstate.scaleOrientation",
		"@Animator.WPAPanelRuleForeignState.ScaleRotation", "@Animator.WPAPanelRuleForeignState.ScaleRotation.ToolTip"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPRuleForeignStateScaleOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).scaleOrientation;
	}
};

class aeMCPRuleForeignStateScaleSize : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleSize() : aeTMCPAnimatorRuleType("foreignstate.scaleSize",
		"@Animator.WPAPanelRuleForeignState.ScaleSize", "@Animator.WPAPanelRuleForeignState.ScaleSize.ToolTip"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPRuleForeignStateScaleSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).scaleSize;
	}
};

class aeMCPRuleForeignStateScaleVertexPositionSet : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleForeignStateScaleVertexPositionSet() : aeTMCPAnimatorRuleType("foreignstate.scaleVertexPositionSet",
		"@Animator.WPAPanelRuleForeignState.ScaleVertexPositionSet", "@Animator.WPAPanelRuleForeignState.ScaleVertexPositionSet.ToolTip"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPRuleForeignStateScaleVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).scaleVertexPositionSet;
	}
};

class aeMCPRuleForeignStateEnablePosition : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnablePosition() : aeTMCPAnimatorRuleType("foreignstate.enablePosition",
		"@Animator.WPAPanelRuleForeignState.EnablePosition", "@Animator.WPAPanelRuleForeignState.EnablePosition.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleForeignStateEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleForeignStateEnableOrientation : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableOrientation() : aeTMCPAnimatorRuleType("foreignstate.enableOrientation",
		"@Animator.WPAPanelRuleForeignState.EnableRotation", "@Animator.WPAPanelRuleForeignState.EnableRotation.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleForeignStateEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleForeignStateEnableSize : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableSize() : aeTMCPAnimatorRuleType("foreignstate.enableSize",
		"@Animator.WPAPanelRuleForeignState.EnableSize", "@Animator.WPAPanelRuleForeignState.EnableSize.ToolTip"){
	};
	
	~aeMCPRuleForeignStateEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleForeignStateEnableVertexPositionSet : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleForeignStateEnableVertexPositionSet() : aeTMCPAnimatorRuleType("foreignstate.enableVertexPositionSet",
		"@Animator.WPAPanelRuleForeignState.EnableVertexPositionSet", "@Animator.WPAPanelRuleForeignState.EnableVertexPositionSet.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleForeignStateEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};

class aeMCPRuleForeignStateSourceCoordinateFrame : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>>{
public:
	aeMCPRuleForeignStateSourceCoordinateFrame() : aeTMCPAnimatorRuleType("foreignstate.sourceCoordinateFrame",
		"@Animator.WPAPanelRuleForeignState.SrcCoordFrame", "@Animator.WPAPanelRuleForeignState.SrcCoordFrame.ToolTip"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleForeignState::ecfBoneLocal,
			deAnimatorRuleForeignState::ecfComponent));
		SetDefaultValueEnum(deAnimatorRuleForeignState::ecfBoneLocal);
	};
	
	~aeMCPRuleForeignStateSourceCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).sourceCoordinateFrame;
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

class aeMCPRuleForeignStateDestCoordinateFrame : public aeTMCPAnimatorRuleForeignState<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>>{
public:
	aeMCPRuleForeignStateDestCoordinateFrame() : aeTMCPAnimatorRuleType("foreignstate.destCoordinateFrame",
		"@Animator.WPAPanelRuleForeignState.DestCoordFrame", "@Animator.WPAPanelRuleForeignState.DestCoordFrame.ToolTip"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleForeignState::ecfBoneLocal,
			deAnimatorRuleForeignState::ecfComponent));
		SetDefaultValueEnum(deAnimatorRuleForeignState::ecfBoneLocal);
	};
	
	~aeMCPRuleForeignStateDestCoordinateFrame() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).destCoordinateFrame;
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


class aeMCPRuleForeignStateTargetPosition : public aeTMCPRuleTarget<aeTMCPAnimatorRuleForeignState<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleForeignStateTargetPosition() : aeTMCPRuleTarget("foreignstate.targetPosition",
		"@Animator.Target.Position", "@Animator.Target.Position.ToolTip"){
	};
	
	~aeMCPRuleForeignStateTargetPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetPosition;
	}
};

class aeMCPRuleForeignStateTargetOrientation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleForeignState<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleForeignStateTargetOrientation() : aeTMCPRuleTarget("foreignstate.targetOrientation",
		"@Animator.Target.Orientation", "@Animator.Target.Orientation.ToolTip"){
	};
	
	~aeMCPRuleForeignStateTargetOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetOrientation;
	}
};


class aeMCPRuleForeignStateTargetSize : public aeTMCPRuleTarget<aeTMCPAnimatorRuleForeignState<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleForeignStateTargetSize() : aeTMCPRuleTarget("foreignstate.targetSize",
		"@Animator.Target.Size", "@Animator.Target.Size.ToolTip"){
	};
	
	~aeMCPRuleForeignStateTargetSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetSize;
	}
};

class aeMCPRuleForeignStateTargetVertexPositionSet : public aeTMCPRuleTarget<aeTMCPAnimatorRuleForeignState<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleForeignStateTargetVertexPositionSet() : aeTMCPRuleTarget("foreignstate.targetVertexPositionSet",
		"@Animator.Target.VertexPositionSet", "@Animator.Target.VertexPositionSet.ToolTip"){
	};
	
	~aeMCPRuleForeignStateTargetVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetVertexPositionSet;
	}
};

#endif