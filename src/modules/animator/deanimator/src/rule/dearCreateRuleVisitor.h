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

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitor.h>

class decIntList;
class dearAnimator;
class dearAnimatorInstance;
class dearRule;
class deAnimator;



/**
 * Visitor creating peer animator rule from an engine animator rule.
 */
class dearCreateRuleVisitor : public deAnimatorRuleVisitor{
private:
	dearAnimatorInstance &pInstance;
	const dearAnimator &pAnimator;
	const decIntList &pControllerMapping;
	const int pFirstLink;
	
	dearRule *pCreatedRule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new visitor. */
	dearCreateRuleVisitor(dearAnimatorInstance &instance, const dearAnimator &animator,
		const decIntList &controllerMapping, int firstLink);
	
	/** Clean up object. */
	virtual ~dearCreateRuleVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset visitor. */
	void Reset();
	
	/** Created rule. */
	inline dearRule *GetCreatedRule() const{return pCreatedRule;}
	
	/** Rule has been created. */
	bool HasCreatedRule() const;
	
	/** Create rule. */
	dearRule *CreateRuleFrom(deAnimatorRule &engRule);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit animator rule. */
	virtual void VisitRule(deAnimatorRule &rule);
	
	/** Visit animation rule. */
	virtual void VisitAnimation(deAnimatorRuleAnimation &rule);
	
	/** Visit animation difference rule. */
	virtual void VisitAnimationDifference(deAnimatorRuleAnimationDifference &rule);
	
	/** Visit animation select rule. */
	virtual void VisitAnimationSelect(deAnimatorRuleAnimationSelect &rule);
	
	/** Visit bone rotator animator rule. */
	virtual void VisitBoneTransformator(deAnimatorRuleBoneTransformator &rule);
	
	/** Visit inverse kinematic animator rule. */
	virtual void VisitInverseKinematic(deAnimatorRuleInverseKinematic &rule);
	
	/** Visit state manipulator rule. */
	virtual void VisitStateManipulator(deAnimatorRuleStateManipulator &rule);
	
	/** Visit state snapshot rule. */
	virtual void VisitStateSnapshot(deAnimatorRuleStateSnapshot &rule);
	
	/** Visit foreign state rule. */
	virtual void VisitForeignState(deAnimatorRuleForeignState &rule);
	
	/** Visit group rule. */
	virtual void VisitGroup(deAnimatorRuleGroup &rule);
	
	/** Visit sub animator rule. */
	virtual void VisitSubAnimator(deAnimatorRuleSubAnimator &rule);
	
	/** Visit track to rule. */
	virtual void VisitTrackTo(deAnimatorRuleTrackTo &rule);
	
	/** Visit limit rule. */
	virtual void VisitLimit(deAnimatorRuleLimit &rule);
	
	/**
	 * Visit mirror rule.
	 * \version 1.9
	 */
	virtual void VisitMirror(deAnimatorRuleMirror &rule);
	/*@}*/
};

#endif
