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
	aeMCPRuleInverseKinematicGoalPosition() : aeTMCPAnimatorRuleType(
	"inversekinematic.goalPosition", "Animator.WPAPanelRuleInverseKinematic.GoalPosition"){}
	
	~aeMCPRuleInverseKinematicGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPGoalPosition();
	}
};

class aeMCPRuleInverseKinematicGoalOrientation : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleInverseKinematicGoalOrientation() : aeTMCPAnimatorRuleType(
	"inversekinematic.goalOrientation", "Animator.WPAPanelRuleInverseKinematic.GoalOrientation"){}
	
	~aeMCPRuleInverseKinematicGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPGoalOrientation();
	}
};

class aeMCPRuleInverseKinematicLocalPosition : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicLocalPosition() : aeTMCPAnimatorRuleType(
	"inversekinematic.localPosition", "Animator.WPAPanelRuleInverseKinematic.LocalPosition"){}
	
	~aeMCPRuleInverseKinematicLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPLocalPosition();
	}
};

class aeMCPRuleInverseKinematicLocalOrientation : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleInverseKinematicLocalOrientation() : aeTMCPAnimatorRuleType(
	"inversekinematic.localOrientation", "Animator.WPAPanelRuleInverseKinematic.LocalOrientation"){}
	
	~aeMCPRuleInverseKinematicLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPLocalOrientation();
	}
};

class aeMCPRuleInverseKinematicAdjustOrientation : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicAdjustOrientation() : aeTMCPAnimatorRuleType(
	"inversekinematic.adjustOrientation", "Animator.WPAPanelRuleInverseKinematic.AdjustOrientation"){}
	
	~aeMCPRuleInverseKinematicAdjustOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPAdjustOrientation();
	}
};

class aeMCPRuleInverseKinematicUseSolverBone : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicUseSolverBone() : aeTMCPAnimatorRuleType(
	"inversekinematic.useSolverBone", "Animator.WPAPanelRuleInverseKinematic.UseSolverBone"){}
	
	~aeMCPRuleInverseKinematicUseSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPUseSolverBone();
	}
};

class aeMCPRuleInverseKinematicSolverBone : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicSolverBone() : aeTMCPAnimatorRuleType(
	"inversekinematic.solverBone", "Animator.WPAPanelRuleInverseKinematic.SolverBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleInverseKinematicSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPSolverBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachRange : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleInverseKinematicReachRange() : aeTMCPAnimatorRuleType(
	"inversekinematic.reachRange", "Animator.WPAPanelRuleInverseKinematic.Range"){}
	
	~aeMCPRuleInverseKinematicReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPReachRange();
	}
};

class aeMCPRuleInverseKinematicReachBone : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicReachBone() : aeTMCPAnimatorRuleType(
	"inversekinematic.reachBone", "Animator.WPAPanelRuleInverseKinematic.ReachBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleInverseKinematicReachBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPReachBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachCenter : public aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicReachCenter() : aeTMCPAnimatorRuleType(
	"inversekinematic.reachCenter", "Animator.WPAPanelRuleInverseKinematic.ReachCenter"){}
	
	~aeMCPRuleInverseKinematicReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPReachCenter();
	}
};


class aeMCPRuleInverseKinematicTargetGoalPosition : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetGoalPosition() : aeTMCPRuleTarget(
	"inversekinematic.targetGoalPosition", "Animator.WPAPanelRuleInverseKinematic.GoalPosition"){
		SetRows(3);
	}
	
	~aeMCPRuleInverseKinematicTargetGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetGoalPosition();
	}
};

class aeMCPRuleInverseKinematicTargetGoalOrientation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetGoalOrientation() : aeTMCPRuleTarget(
	"inversekinematic.targetGoalOrientation", "Animator.WPAPanelRuleInverseKinematic.GoalOrientation"){
		SetRows(3);
	}
	
	~aeMCPRuleInverseKinematicTargetGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetGoalOrientation();
	}
};

class aeMCPRuleInverseKinematicTargetLocalPosition : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetLocalPosition() : aeTMCPRuleTarget(
	"inversekinematic.targetLocalPosition", "Animator.WPAPanelRuleInverseKinematic.LocalPosition"){
		SetRows(3);
	}
	
	~aeMCPRuleInverseKinematicTargetLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetLocalPosition();
	}
};

class aeMCPRuleInverseKinematicTargetLocalOrientation : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetLocalOrientation() : aeTMCPRuleTarget(
	"inversekinematic.targetLocalOrientation", "Animator.WPAPanelRuleInverseKinematic.LocalOrientation"){
		SetRows(3);
	}
	
	~aeMCPRuleInverseKinematicTargetLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetLocalOrientation();
	}
};

class aeMCPRuleInverseKinematicTargetReachRange : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetReachRange() : aeTMCPRuleTarget(
	"inversekinematic.targetReachRange", "Animator.WPAPanelRuleInverseKinematic.Range"){
		SetRows(3);
	}
	
	~aeMCPRuleInverseKinematicTargetReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetReachRange();
	}
};

class aeMCPRuleInverseKinematicTargetReachCenter : public aeTMCPRuleTarget<aeTMCPAnimatorRuleInverseKinematic<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleInverseKinematicTargetReachCenter() : aeTMCPRuleTarget(
	"inversekinematic.targetReachCenter", "Animator.WPAPanelRuleInverseKinematic.ReachCenter"){
		SetRows(3);
	}
	
	~aeMCPRuleInverseKinematicTargetReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).GetMPTargetReachCenter();
	}
};


#endif