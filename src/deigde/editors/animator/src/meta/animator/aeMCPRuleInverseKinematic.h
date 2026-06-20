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

#ifndef _AEMCPRULEINVERSEKINEMATIC_H_
#define _AEMCPRULEINVERSEKINEMATIC_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleInverseKinematic.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleInverseKinematicGoalPosition : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicGoalPosition() : aeTMCPAnimatorRuleType("inversekinematic.goalPosition",
		"@Animator.WPAPanelRuleInverseKinematic.GoalPosition", "@Animator.WPAPanelRuleInverseKinematic.GoalPosition.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).goalPosition;
	}
};

class aeMCPRuleInverseKinematicGoalOrientation : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicGoalOrientation() : aeTMCPAnimatorRuleType("inversekinematic.goalOrientation",
		"@Animator.WPAPanelRuleInverseKinematic.GoalOrientation", "@Animator.WPAPanelRuleInverseKinematic.GoalOrientation.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).goalOrientation;
	}
};

class aeMCPRuleInverseKinematicLocalPosition : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicLocalPosition() : aeTMCPAnimatorRuleType("inversekinematic.localPosition",
		"@Animator.WPAPanelRuleInverseKinematic.LocalPosition", "@Animator.WPAPanelRuleInverseKinematic.LocalPosition.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).localPosition;
	}
};

class aeMCPRuleInverseKinematicLocalOrientation : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicLocalOrientation() : aeTMCPAnimatorRuleType("inversekinematic.localOrientation",
		"@Animator.WPAPanelRuleInverseKinematic.LocalOrientation", "@Animator.WPAPanelRuleInverseKinematic.LocalOrientation.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).localOrientation;
	}
};

class aeMCPRuleInverseKinematicAdjustOrientation : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicAdjustOrientation() : aeTMCPAnimatorRuleType("inversekinematic.adjustOrientation",
		"@Animator.WPAPanelRuleInverseKinematic.AdjustOrientation", "@Animator.WPAPanelRuleInverseKinematic.AdjustOrientation.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicAdjustOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).adjustOrientation;
	}
};

class aeMCPRuleInverseKinematicUseSolverBone : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicUseSolverBone() : aeTMCPAnimatorRuleType("inversekinematic.useSolverBone",
		"@Animator.WPAPanelRuleInverseKinematic.UseSolverBone", "@Animator.WPAPanelRuleInverseKinematic.UseSolverBone.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicUseSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).useSolverBone;
	}
};

class aeMCPRuleInverseKinematicSolverBone : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicSolverBone() : aeTMCPAnimatorRuleType("inversekinematic.solverBone",
		"@Animator.WPAPanelRuleInverseKinematic.SolverBone", "@Animator.WPAPanelRuleInverseKinematic.SolverBone.ToolTip"){
			SetEnableAllowed(true);
		};
	
	~aeMCPRuleInverseKinematicSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).solverBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->hiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachRange : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleInverseKinematicReachRange() : aeTMCPAnimatorRuleType("inversekinematic.reachRange",
		"@Animator.WPAPanelRuleInverseKinematic.Range", "@Animator.WPAPanelRuleInverseKinematic.Range.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).reachRange;
	}
};

class aeMCPRuleInverseKinematicReachBone : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicReachBone() : aeTMCPAnimatorRuleType("inversekinematic.reachBone",
		"@Animator.WPAPanelRuleInverseKinematic.ReachBone", "@Animator.WPAPanelRuleInverseKinematic.ReachBone.ToolTip"){
			SetEnableAllowed(true);
		};
	
	~aeMCPRuleInverseKinematicReachBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).reachBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->hiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachCenter : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicReachCenter() : aeTMCPAnimatorRuleType("inversekinematic.reachCenter",
		"@Animator.WPAPanelRuleInverseKinematic.ReachCenter", "@Animator.WPAPanelRuleInverseKinematic.ReachCenter.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).reachCenter;
	}
};


class aeMCPRuleInverseKinematicTargetGoalPosition : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetGoalPosition() : aeTMCPRuleTarget("inversekinematic.targetGoalPosition",
		"@Animator.WPAPanelRuleInverseKinematic.GoalPosition", "@Animator.WPAPanelRuleInverseKinematic.GoalPosition.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicTargetGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetGoalPosition;
	}
};

class aeMCPRuleInverseKinematicTargetGoalOrientation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetGoalOrientation() : aeTMCPRuleTarget("inversekinematic.targetGoalOrientation",
		"@Animator.WPAPanelRuleInverseKinematic.GoalOrientation", "@Animator.WPAPanelRuleInverseKinematic.GoalOrientation.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicTargetGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetGoalOrientation;
	}
};

class aeMCPRuleInverseKinematicTargetLocalPosition : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetLocalPosition() : aeTMCPRuleTarget("inversekinematic.targetLocalPosition",
		"@Animator.WPAPanelRuleInverseKinematic.LocalPosition", "@Animator.WPAPanelRuleInverseKinematic.LocalPosition.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicTargetLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetLocalPosition;
	}
};

class aeMCPRuleInverseKinematicTargetLocalOrientation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetLocalOrientation() : aeTMCPRuleTarget("inversekinematic.targetLocalOrientation",
		"@Animator.WPAPanelRuleInverseKinematic.LocalOrientation", "@Animator.WPAPanelRuleInverseKinematic.LocalOrientation.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicTargetLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetLocalOrientation;
	}
};

class aeMCPRuleInverseKinematicTargetReachRange : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetReachRange() : aeTMCPRuleTarget("inversekinematic.targetReachRange",
		"@Animator.WPAPanelRuleInverseKinematic.Range", "@Animator.WPAPanelRuleInverseKinematic.Range.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicTargetReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetReachRange;
	}
};

class aeMCPRuleInverseKinematicTargetReachCenter : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetReachCenter() : aeTMCPRuleTarget("inversekinematic.targetReachCenter",
		"@Animator.WPAPanelRuleInverseKinematic.ReachCenter", "@Animator.WPAPanelRuleInverseKinematic.ReachCenter.ToolTip"){
	};
	
	~aeMCPRuleInverseKinematicTargetReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetReachCenter;
	}
};


#endif