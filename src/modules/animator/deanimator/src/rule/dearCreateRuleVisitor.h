/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEARCREATERULEVISITOR_H_
#define _DEARCREATERULEVISITOR_H_

#include "dearRule.h"

#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitor.h>

class dearAnimator;
class dearAnimatorInstance;
class deAnimator;



/**
 * Visitor creating peer animator rule from an engine animator rule.
 */
class dearCreateRuleVisitor : public deAnimatorRuleVisitor{
private:
	dearAnimatorInstance &pInstance;
	const dearAnimator &pAnimator;
	const decTList<int> &pControllerMapping;
	const int pFirstLink;
	
	dearRule::Ref pCreatedRule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	dearCreateRuleVisitor(dearAnimatorInstance &instance, const dearAnimator &animator,
		const decTList<int> &controllerMapping, int firstLink);
	
	/** Clean up object. */
	~dearCreateRuleVisitor() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset visitor. */
	void Reset();
	
	/** Created rule. */
	inline dearRule::Ref &GetCreatedRule(){ return pCreatedRule; }
	
	/** Rule has been created. */
	bool HasCreatedRule() const;
	
	/** Create rule. */
	dearRule::Ref &CreateRuleFrom(deAnimatorRule &engRule);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit animator rule. */
	void VisitRule(deAnimatorRule &rule) override;
	
	/** Visit animation rule. */
	void VisitAnimation(deAnimatorRuleAnimation &rule) override;
	
	/** Visit animation difference rule. */
	void VisitAnimationDifference(deAnimatorRuleAnimationDifference &rule) override;
	
	/** Visit animation select rule. */
	void VisitAnimationSelect(deAnimatorRuleAnimationSelect &rule) override;
	
	/** Visit bone rotator animator rule. */
	void VisitBoneTransformator(deAnimatorRuleBoneTransformator &rule) override;
	
	/** Visit inverse kinematic animator rule. */
	void VisitInverseKinematic(deAnimatorRuleInverseKinematic &rule) override;
	
	/** Visit state manipulator rule. */
	void VisitStateManipulator(deAnimatorRuleStateManipulator &rule) override;
	
	/** Visit state snapshot rule. */
	void VisitStateSnapshot(deAnimatorRuleStateSnapshot &rule) override;
	
	/** Visit foreign state rule. */
	void VisitForeignState(deAnimatorRuleForeignState &rule) override;
	
	/** Visit group rule. */
	void VisitGroup(deAnimatorRuleGroup &rule) override;
	
	/** Visit sub animator rule. */
	void VisitSubAnimator(deAnimatorRuleSubAnimator &rule) override;
	
	/** Visit track to rule. */
	void VisitTrackTo(deAnimatorRuleTrackTo &rule) override;
	
	/** Visit limit rule. */
	void VisitLimit(deAnimatorRuleLimit &rule) override;
	
	/**
	 * Visit mirror rule.
	 * \version 1.9
	 */
	void VisitMirror(deAnimatorRuleMirror &rule) override;
	/*@}*/
};

#endif
