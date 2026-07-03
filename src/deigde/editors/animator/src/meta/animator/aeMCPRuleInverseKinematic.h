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

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleInverseKinematic.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


class aeMCPRuleInverseKinematicGoalPosition :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicGoalPosition() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.goalPosition",
		"Animator.WPAPanelRuleInverseKinematic.GoalPosition"){}
	
	~aeMCPRuleInverseKinematicGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpGoalPosition;
	}
};

class aeMCPRuleInverseKinematicGoalOrientation :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleInverseKinematicGoalOrientation() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.goalOrientation",
		"Animator.WPAPanelRuleInverseKinematic.GoalOrientation"){}
	
	~aeMCPRuleInverseKinematicGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpGoalOrientation;
	}
};

class aeMCPRuleInverseKinematicLocalPosition :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicLocalPosition() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.localPosition",
		"Animator.WPAPanelRuleInverseKinematic.LocalPosition"){}
	
	~aeMCPRuleInverseKinematicLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpLocalPosition;
	}
};

class aeMCPRuleInverseKinematicLocalOrientation :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleInverseKinematicLocalOrientation() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.localOrientation",
		"Animator.WPAPanelRuleInverseKinematic.LocalOrientation"){}
	
	~aeMCPRuleInverseKinematicLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpLocalOrientation;
	}
};

class aeMCPRuleInverseKinematicAdjustOrientation :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicAdjustOrientation() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.adjustOrientation",
		"Animator.WPAPanelRuleInverseKinematic.AdjustOrientation"){}
	
	~aeMCPRuleInverseKinematicAdjustOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpAdjustOrientation;
	}
};

class aeMCPRuleInverseKinematicUseSolverBone :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicUseSolverBone() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.useSolverBone",
		"Animator.WPAPanelRuleInverseKinematic.UseSolverBone"){}
	
	~aeMCPRuleInverseKinematicUseSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpUseSolverBone;
	}
};

class aeMCPRuleInverseKinematicSolverBone :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicSolverBone() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.solverBone",
		"Animator.WPAPanelRuleInverseKinematic.SolverBone")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleInverseKinematicSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpSolverBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachRange :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleInverseKinematicReachRange() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.reachRange",
		"Animator.WPAPanelRuleInverseKinematic.Range"){}
	
	~aeMCPRuleInverseKinematicReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpReachRange;
	}
};

class aeMCPRuleInverseKinematicReachBone :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicReachBone() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.reachBone",
		"Animator.WPAPanelRuleInverseKinematic.ReachBone")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleInverseKinematicReachBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpReachBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachCenter :
	public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicReachCenter() : igdeMetaPropertyMCT(
		"animator.rule_inverse_kinematics.reachCenter",
		"Animator.WPAPanelRuleInverseKinematic.ReachCenter"){}
	
	~aeMCPRuleInverseKinematicReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpReachCenter;
	}
};


class aeMCPRuleInverseKinematicTargetGoalPosition :
	public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetGoalPosition() : MetaPropertyTarget(
		"animator.rule_inverse_kinematics.targetGoalPosition",
		"Animator.WPAPanelRuleInverseKinematic.GoalPosition"){}
	~aeMCPRuleInverseKinematicTargetGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetGoalPosition;
	}
};

class aeMCPRuleInverseKinematicTargetGoalOrientation :
	public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetGoalOrientation() : MetaPropertyTarget(
		"animator.rule_inverse_kinematics.targetGoalOrientation",
		"Animator.WPAPanelRuleInverseKinematic.GoalOrientation"){}
	~aeMCPRuleInverseKinematicTargetGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetGoalOrientation;
	}
};

class aeMCPRuleInverseKinematicTargetLocalPosition :
	public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetLocalPosition() : MetaPropertyTarget(
		"animator.rule_inverse_kinematics.targetLocalPosition",
		"Animator.WPAPanelRuleInverseKinematic.LocalPosition"){}
	~aeMCPRuleInverseKinematicTargetLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetLocalPosition;
	}
};

class aeMCPRuleInverseKinematicTargetLocalOrientation :
	public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetLocalOrientation() : MetaPropertyTarget(
		"animator.rule_inverse_kinematics.targetLocalOrientation",
		"Animator.WPAPanelRuleInverseKinematic.LocalOrientation"){}
	~aeMCPRuleInverseKinematicTargetLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetLocalOrientation;
	}
};

class aeMCPRuleInverseKinematicTargetReachRange :
	public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetReachRange() : MetaPropertyTarget(
		"animator.rule_inverse_kinematics.targetReachRange",
		"Animator.WPAPanelRuleInverseKinematic.Range"){}
	~aeMCPRuleInverseKinematicTargetReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetReachRange;
	}
};

class aeMCPRuleInverseKinematicTargetReachCenter :
	public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetReachCenter() : MetaPropertyTarget(
		"animator.rule_inverse_kinematics.targetReachCenter",
		"Animator.WPAPanelRuleInverseKinematic.ReachCenter"){}
	~aeMCPRuleInverseKinematicTargetReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTargetReachCenter;
	}
};


#endif