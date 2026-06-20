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

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleStateManipulator.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleStateManipulatorMinPosition : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMinPosition() : aeTMCPAnimatorRuleType("statemanipulator.minPosition",
		"@Animator.WPAPanelRuleStateManipulator.MinPosition", "@Animator.WPAPanelRuleStateManipulator.MinPosition.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorMinPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minPosition;
	}
};

class aeMCPRuleStateManipulatorMaxPosition : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMaxPosition() : aeTMCPAnimatorRuleType("statemanipulator.maxPosition",
		"@Animator.WPAPanelRuleStateManipulator.MaxPosition", "@Animator.WPAPanelRuleStateManipulator.MaxPosition.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorMaxPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxPosition;
	}
};

class aeMCPRuleStateManipulatorMinRotation : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMinRotation() : aeTMCPAnimatorRuleType("statemanipulator.minRotation",
		"@Animator.WPAPanelRuleStateManipulator.MinRotation", "@Animator.WPAPanelRuleStateManipulator.MinRotation.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorMinRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minRotation;
	}
};

class aeMCPRuleStateManipulatorMaxRotation : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMaxRotation() : aeTMCPAnimatorRuleType("statemanipulator.maxRotation",
		"@Animator.WPAPanelRuleStateManipulator.MaxRotation", "@Animator.WPAPanelRuleStateManipulator.MaxRotation.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorMaxRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxRotation;
	}
};

class aeMCPRuleStateManipulatorMinSize : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMinSize() : aeTMCPAnimatorRuleType("statemanipulator.minSize",
		"@Animator.WPAPanelRuleStateManipulator.MinScaling", "@Animator.WPAPanelRuleStateManipulator.MinScaling.ToolTip"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	};
	
	~aeMCPRuleStateManipulatorMinSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minSize;
	}
};

class aeMCPRuleStateManipulatorMaxSize : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleStateManipulatorMaxSize() : aeTMCPAnimatorRuleType("statemanipulator.maxSize",
		"@Animator.WPAPanelRuleStateManipulator.MaxScaling", "@Animator.WPAPanelRuleStateManipulator.MaxScaling.ToolTip"){
		SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
	};
	
	~aeMCPRuleStateManipulatorMaxSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxSize;
	}
};

class aeMCPRuleStateManipulatorMinVertexPositionSet : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleStateManipulatorMinVertexPositionSet() : aeTMCPAnimatorRuleType("statemanipulator.minVertexPositionSet",
		"@Animator.WPAPanelRuleStateManipulator.MinVertexPositionSet", "@Animator.WPAPanelRuleStateManipulator.MinVertexPositionSet.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorMinVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).minVertexPositionSet;
	}
};

class aeMCPRuleStateManipulatorMaxVertexPositionSet : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleStateManipulatorMaxVertexPositionSet() : aeTMCPAnimatorRuleType("statemanipulator.maxVertexPositionSet",
		"@Animator.WPAPanelRuleStateManipulator.MaxVertexPositionSet", "@Animator.WPAPanelRuleStateManipulator.MaxVertexPositionSet.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorMaxVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).maxVertexPositionSet;
	}
};

class aeMCPRuleStateManipulatorEnablePosition : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnablePosition() : aeTMCPAnimatorRuleType("statemanipulator.enablePosition",
		"@Animator.WPAPanelRuleStateManipulator.EnablePosition", "@Animator.WPAPanelRuleStateManipulator.EnablePosition.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleStateManipulatorEnableRotation : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableRotation() : aeTMCPAnimatorRuleType("statemanipulator.enableRotation",
		"@Animator.WPAPanelRuleStateManipulator.EnableRotation", "@Animator.WPAPanelRuleStateManipulator.EnableRotation.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleStateManipulatorEnableRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableRotation;
	}
};

class aeMCPRuleStateManipulatorEnableSize : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableSize() : aeTMCPAnimatorRuleType("statemanipulator.enableSize",
		"@Animator.WPAPanelRuleStateManipulator.EnableSize", "@Animator.WPAPanelRuleStateManipulator.EnableSize.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleStateManipulatorEnableVertexPositionSet : public aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateManipulatorEnableVertexPositionSet() : aeTMCPAnimatorRuleType("statemanipulator.enableVertexPositionSet",
		"@Animator.WPAPanelRuleStateManipulator.EnableVertexPositionSet", "@Animator.WPAPanelRuleStateManipulator.EnableVertexPositionSet.ToolTip"){
		SetDefaultValue(true);
	};
	
	~aeMCPRuleStateManipulatorEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};


class aeMCPRuleStateManipulatorTargetPosition : public aeTMCPRuleTarget<aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleStateManipulatorTargetPosition() : aeTMCPRuleTarget("statemanipulator.targetPosition",
		"@Animator.Target.Position", "@Animator.Target.Position.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorTargetPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetPosition;
	}
};

class aeMCPRuleStateManipulatorTargetRotation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleStateManipulatorTargetRotation() : aeTMCPRuleTarget("statemanipulator.targetRotation",
		"@Animator.Target.Rotation", "@Animator.Target.Rotation.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorTargetRotation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetRotation;
	}
};

class aeMCPRuleStateManipulatorTargetSize : public aeTMCPRuleTarget<aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleStateManipulatorTargetSize() : aeTMCPRuleTarget("statemanipulator.targetSize",
		"@Animator.Target.Size", "@Animator.Target.Size.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorTargetSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetSize;
	}
};

class aeMCPRuleStateManipulatorTargetVertexPositionSet : public aeTMCPRuleTarget<aeTMCPAnimatorRuleStateManipulator<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleStateManipulatorTargetVertexPositionSet() : aeTMCPRuleTarget("statemanipulator.targetVertexPositionSet",
		"@Animator.Target.VertexPositionSet", "@Animator.Target.VertexPositionSet.ToolTip"){
	};
	
	~aeMCPRuleStateManipulatorTargetVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetVertexPositionSet;
	}
};


#endif