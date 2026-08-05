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

#ifndef _AEMCPRULESTATEMANIPULATOR_H_
#define _AEMCPRULESTATEMANIPULATOR_H_

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleStateManipulator.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleStateManipulatorMinPosition :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMinPosition() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.minPosition",
		"Animator.WPAPanelRuleStateManipulator.MinPosition"){}
	
	~aeMCPRuleStateManipulatorMinPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinPosition;
	}
};

class aeMCPRuleStateManipulatorMaxPosition :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMaxPosition() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.maxPosition",
		"Animator.WPAPanelRuleStateManipulator.MaxPosition"){}
		
	~aeMCPRuleStateManipulatorMaxPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxPosition;
	}
};

class aeMCPRuleStateManipulatorMinRotation :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleStateManipulatorMinRotation() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.minRotation",
		"Animator.WPAPanelRuleStateManipulator.MinRotation"){}
	
	~aeMCPRuleStateManipulatorMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinRotation;
	}
};

class aeMCPRuleStateManipulatorMaxRotation :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleStateManipulatorMaxRotation() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.maxRotation",
		"Animator.WPAPanelRuleStateManipulator.MaxRotation"){}
	
	~aeMCPRuleStateManipulatorMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxRotation;
	}
};

class aeMCPRuleStateManipulatorMinSize :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMinSize() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.minSize",
		"Animator.WPAPanelRuleStateManipulator.MinScaling")
	{
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleStateManipulatorMinSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinSize;
	}
};

class aeMCPRuleStateManipulatorMaxSize :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMaxSize() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.maxSize",
		"Animator.WPAPanelRuleStateManipulator.MaxScaling")
	{
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleStateManipulatorMaxSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxSize;
	}
};

class aeMCPRuleStateManipulatorMinVertexPositionSet :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleStateManipulatorMinVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.minVertexPositionSet",
		"Animator.WPAPanelRuleStateManipulator.MinVertexPositionSet"){}
	
	~aeMCPRuleStateManipulatorMinVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMinVertexPositionSet;
	}
};

class aeMCPRuleStateManipulatorMaxVertexPositionSet :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleStateManipulatorMaxVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.maxVertexPositionSet",
		"Animator.WPAPanelRuleStateManipulator.MaxVertexPositionSet"){}
	
	~aeMCPRuleStateManipulatorMaxVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMaxVertexPositionSet;
	}
};

class aeMCPRuleStateManipulatorEnablePosition :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnablePosition() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.enablePosition",
		"Animator.WPAPanelRuleStateManipulator.EnablePosition"){}
	
	~aeMCPRuleStateManipulatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePosition;
	}
};

class aeMCPRuleStateManipulatorEnableRotation :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableRotation() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.enableRotation",
		"Animator.WPAPanelRuleStateManipulator.EnableRotation")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateManipulatorEnableRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableRotation;
	}
};

class aeMCPRuleStateManipulatorEnableSize :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableSize() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.enableSize",
		"Animator.WPAPanelRuleStateManipulator.EnableSize"){}
	
	~aeMCPRuleStateManipulatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableSize;
	}
};

class aeMCPRuleStateManipulatorEnableVertexPositionSet :
	public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_state_manipulator.enableVertexPositionSet",
		"Animator.WPAPanelRuleStateManipulator.EnableVertexPositionSet")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateManipulatorEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableVertexPositionSet;
	}
};


class aeMCPRuleStateManipulatorTargetPosition :
	public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetPosition() : MetaPropertyTarget(
		"animator.rule_state_manipulator.targetPosition", "Animator.Target.Position"){}
	~aeMCPRuleStateManipulatorTargetPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetPosition;
	}
};

class aeMCPRuleStateManipulatorTargetRotation :
	public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetRotation() : MetaPropertyTarget(
		"animator.rule_state_manipulator.targetRotation", "Animator.Target.Rotation"){}
	~aeMCPRuleStateManipulatorTargetRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetRotation;
	}
};

class aeMCPRuleStateManipulatorTargetSize :
	public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetSize() : MetaPropertyTarget(
		"animator.rule_state_manipulator.targetSize", "Animator.Target.Size"){}
	~aeMCPRuleStateManipulatorTargetSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetSize;
	}
};

class aeMCPRuleStateManipulatorTargetVertexPositionSet :
	public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetVertexPositionSet() : MetaPropertyTarget(
		"animator.rule_state_manipulator.targetVertexPositionSet",
		"Animator.Target.VertexPositionSet"){}
	~aeMCPRuleStateManipulatorTargetVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetVertexPositionSet;
	}
};


#endif