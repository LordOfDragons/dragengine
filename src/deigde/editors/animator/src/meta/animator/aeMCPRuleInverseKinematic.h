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


class aeMCPRuleInverseKinematicGoalPosition : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicGoalPosition() : igdeMetaPropertyMCT(
	"inversekinematic.goalPosition", "Animator.WPAPanelRuleInverseKinematic.GoalPosition"){}
	
	~aeMCPRuleInverseKinematicGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPGoalPosition();
	}
};

class aeMCPRuleInverseKinematicGoalOrientation : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleInverseKinematicGoalOrientation() : igdeMetaPropertyMCT(
	"inversekinematic.goalOrientation", "Animator.WPAPanelRuleInverseKinematic.GoalOrientation"){}
	
	~aeMCPRuleInverseKinematicGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPGoalOrientation();
	}
};

class aeMCPRuleInverseKinematicLocalPosition : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicLocalPosition() : igdeMetaPropertyMCT(
	"inversekinematic.localPosition", "Animator.WPAPanelRuleInverseKinematic.LocalPosition"){}
	
	~aeMCPRuleInverseKinematicLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPLocalPosition();
	}
};

class aeMCPRuleInverseKinematicLocalOrientation : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorageQuaternion>{
public:
	aeMCPRuleInverseKinematicLocalOrientation() : igdeMetaPropertyMCT(
	"inversekinematic.localOrientation", "Animator.WPAPanelRuleInverseKinematic.LocalOrientation"){}
	
	~aeMCPRuleInverseKinematicLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPLocalOrientation();
	}
};

class aeMCPRuleInverseKinematicAdjustOrientation : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicAdjustOrientation() : igdeMetaPropertyMCT(
	"inversekinematic.adjustOrientation", "Animator.WPAPanelRuleInverseKinematic.AdjustOrientation"){}
	
	~aeMCPRuleInverseKinematicAdjustOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPAdjustOrientation();
	}
};

class aeMCPRuleInverseKinematicUseSolverBone : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleInverseKinematicUseSolverBone() : igdeMetaPropertyMCT(
	"inversekinematic.useSolverBone", "Animator.WPAPanelRuleInverseKinematic.UseSolverBone"){}
	
	~aeMCPRuleInverseKinematicUseSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPUseSolverBone();
	}
};

class aeMCPRuleInverseKinematicSolverBone : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicSolverBone() : igdeMetaPropertyMCT(
	"inversekinematic.solverBone", "Animator.WPAPanelRuleInverseKinematic.SolverBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleInverseKinematicSolverBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPSolverBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachRange : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPRuleInverseKinematicReachRange() : igdeMetaPropertyMCT(
	"inversekinematic.reachRange", "Animator.WPAPanelRuleInverseKinematic.Range"){}
	
	~aeMCPRuleInverseKinematicReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPReachRange();
	}
};

class aeMCPRuleInverseKinematicReachBone : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleInverseKinematicReachBone() : igdeMetaPropertyMCT(
	"inversekinematic.reachBone", "Animator.WPAPanelRuleInverseKinematic.ReachBone"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleInverseKinematicReachBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPReachBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};

class aeMCPRuleInverseKinematicReachCenter : public aeRuleInverseKinematic::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPRuleInverseKinematicReachCenter() : igdeMetaPropertyMCT(
	"inversekinematic.reachCenter", "Animator.WPAPanelRuleInverseKinematic.ReachCenter"){}
	
	~aeMCPRuleInverseKinematicReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPReachCenter();
	}
};


class aeMCPRuleInverseKinematicTargetGoalPosition : public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetGoalPosition() : MetaPropertyTarget(
	"inversekinematic.targetGoalPosition", "Animator.WPAPanelRuleInverseKinematic.GoalPosition"){}
	~aeMCPRuleInverseKinematicTargetGoalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetGoalPosition();
	}
};

class aeMCPRuleInverseKinematicTargetGoalOrientation : public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetGoalOrientation() : MetaPropertyTarget(
	"inversekinematic.targetGoalOrientation", "Animator.WPAPanelRuleInverseKinematic.GoalOrientation"){}
	~aeMCPRuleInverseKinematicTargetGoalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetGoalOrientation();
	}
};

class aeMCPRuleInverseKinematicTargetLocalPosition : public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetLocalPosition() : MetaPropertyTarget(
	"inversekinematic.targetLocalPosition", "Animator.WPAPanelRuleInverseKinematic.LocalPosition"){}
	~aeMCPRuleInverseKinematicTargetLocalPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetLocalPosition();
	}
};

class aeMCPRuleInverseKinematicTargetLocalOrientation : public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetLocalOrientation() : MetaPropertyTarget(
	"inversekinematic.targetLocalOrientation", "Animator.WPAPanelRuleInverseKinematic.LocalOrientation"){}
	~aeMCPRuleInverseKinematicTargetLocalOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetLocalOrientation();
	}
};

class aeMCPRuleInverseKinematicTargetReachRange : public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetReachRange() : MetaPropertyTarget(
	"inversekinematic.targetReachRange", "Animator.WPAPanelRuleInverseKinematic.Range"){}
	~aeMCPRuleInverseKinematicTargetReachRange() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetReachRange();
	}
};

class aeMCPRuleInverseKinematicTargetReachCenter : public aeRule::MetaPropertyTarget<aeRuleInverseKinematic>{
public:
	aeMCPRuleInverseKinematicTargetReachCenter() : MetaPropertyTarget(
	"inversekinematic.targetReachCenter", "Animator.WPAPanelRuleInverseKinematic.ReachCenter"){}
	~aeMCPRuleInverseKinematicTargetReachCenter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPTargetReachCenter();
	}
};


#endif