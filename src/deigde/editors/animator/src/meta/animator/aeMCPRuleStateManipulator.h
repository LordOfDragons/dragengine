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


class aeMCPRuleStateManipulatorMinPosition : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMinPosition() : igdeMetaPropertyMCT(
	"statemanipulator.minPosition", "Animator.WPAPanelRuleStateManipulator.MinPosition"){}
	
	~aeMCPRuleStateManipulatorMinPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinPosition();
	}
};

class aeMCPRuleStateManipulatorMaxPosition : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMaxPosition() : igdeMetaPropertyMCT(
	"statemanipulator.maxPosition", "Animator.WPAPanelRuleStateManipulator.MaxPosition"){}
	
	~aeMCPRuleStateManipulatorMaxPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxPosition();
	}
};

class aeMCPRuleStateManipulatorMinRotation : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleStateManipulatorMinRotation() : igdeMetaPropertyMCT(
	"statemanipulator.minRotation", "Animator.WPAPanelRuleStateManipulator.MinRotation"){}
	
	~aeMCPRuleStateManipulatorMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinRotation();
	}
};

class aeMCPRuleStateManipulatorMaxRotation : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleStateManipulatorMaxRotation() : igdeMetaPropertyMCT(
	"statemanipulator.maxRotation", "Animator.WPAPanelRuleStateManipulator.MaxRotation"){}
	
	~aeMCPRuleStateManipulatorMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxRotation();
	}
};

class aeMCPRuleStateManipulatorMinSize : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMinSize() : igdeMetaPropertyMCT(
	"statemanipulator.minSize", "Animator.WPAPanelRuleStateManipulator.MinScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleStateManipulatorMinSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinSize();
	}
};

class aeMCPRuleStateManipulatorMaxSize : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMaxSize() : igdeMetaPropertyMCT(
	"statemanipulator.maxSize", "Animator.WPAPanelRuleStateManipulator.MaxScaling"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	}
	
	~aeMCPRuleStateManipulatorMaxSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxSize();
	}
};

class aeMCPRuleStateManipulatorMinVertexPositionSet : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleStateManipulatorMinVertexPositionSet() : igdeMetaPropertyMCT(
	"statemanipulator.minVertexPositionSet", "Animator.WPAPanelRuleStateManipulator.MinVertexPositionSet"){}
	
	~aeMCPRuleStateManipulatorMinVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinVertexPositionSet();
	}
};

class aeMCPRuleStateManipulatorMaxVertexPositionSet : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleStateManipulatorMaxVertexPositionSet() : igdeMetaPropertyMCT(
	"statemanipulator.maxVertexPositionSet", "Animator.WPAPanelRuleStateManipulator.MaxVertexPositionSet"){}
	
	~aeMCPRuleStateManipulatorMaxVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaxVertexPositionSet();
	}
};

class aeMCPRuleStateManipulatorEnablePosition : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnablePosition() : igdeMetaPropertyMCT(
	"statemanipulator.enablePosition", "Animator.WPAPanelRuleStateManipulator.EnablePosition"){}
	
	~aeMCPRuleStateManipulatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePosition();
	}
};

class aeMCPRuleStateManipulatorEnableRotation : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableRotation() : igdeMetaPropertyMCT(
	"statemanipulator.enableRotation", "Animator.WPAPanelRuleStateManipulator.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateManipulatorEnableRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableRotation();
	}
};

class aeMCPRuleStateManipulatorEnableSize : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableSize() : igdeMetaPropertyMCT(
	"statemanipulator.enableSize", "Animator.WPAPanelRuleStateManipulator.EnableSize"){}
	
	~aeMCPRuleStateManipulatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableSize();
	}
};

class aeMCPRuleStateManipulatorEnableVertexPositionSet : public aeRuleStateManipulator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableVertexPositionSet() : igdeMetaPropertyMCT(
	"statemanipulator.enableVertexPositionSet", "Animator.WPAPanelRuleStateManipulator.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateManipulatorEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSet();
	}
};


class aeMCPRuleStateManipulatorTargetPosition : public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetPosition() : MetaPropertyTarget("statemanipulator.targetPosition", "Animator.Target.Position"){}
	~aeMCPRuleStateManipulatorTargetPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetPosition();
	}
};

class aeMCPRuleStateManipulatorTargetRotation : public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetRotation() : MetaPropertyTarget("statemanipulator.targetRotation", "Animator.Target.Rotation"){}
	~aeMCPRuleStateManipulatorTargetRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetRotation();
	}
};

class aeMCPRuleStateManipulatorTargetSize : public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetSize() : MetaPropertyTarget("statemanipulator.targetSize", "Animator.Target.Size"){}
	~aeMCPRuleStateManipulatorTargetSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetSize();
	}
};

class aeMCPRuleStateManipulatorTargetVertexPositionSet : public aeRule::MetaPropertyTarget<aeRuleStateManipulator>{
public:
	aeMCPRuleStateManipulatorTargetVertexPositionSet() : MetaPropertyTarget("statemanipulator.targetVertexPositionSet", "Animator.Target.VertexPositionSet"){}
	~aeMCPRuleStateManipulatorTargetVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetVertexPositionSet();
	}
};


#endif