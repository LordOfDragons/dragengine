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

#ifndef _DEANIMATORRULEVISITOR_H_
#define _DEANIMATORRULEVISITOR_H_

#include "../../../dragengine_export.h"

class deAnimatorRule;
class deAnimatorRuleAnimation;
class deAnimatorRuleAnimationDifference;
class deAnimatorRuleAnimationSelect;
class deAnimatorRuleBoneTransformator;
class deAnimatorRuleInverseKinematic;
class deAnimatorRuleStateManipulator;
class deAnimatorRuleStateSnapshot;
class deAnimatorRuleForeignState;
class deAnimatorRuleGroup;
class deAnimatorRuleSubAnimator;
class deAnimatorRuleTrackTo;
class deAnimatorRuleLimit;
class deAnimatorRuleMirror;


/**
 * \brief Animator rule visitor.
 */
class DE_DLL_EXPORT deAnimatorRuleVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator rule visitor . */
	deAnimatorRuleVisitor();
	
	/** \brief Clean up animator rule visitor. */
	virtual ~deAnimatorRuleVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit animator rule. */
	virtual void VisitRule(deAnimatorRule &rule);
	
	/** \brief Visit animation rule. */
	virtual void VisitAnimation(deAnimatorRuleAnimation &rule);
	
	/** \brief Visit animation difference rule. */
	virtual void VisitAnimationDifference(deAnimatorRuleAnimationDifference &rule);
	
	/** \brief Visit animation select rule. */
	virtual void VisitAnimationSelect(deAnimatorRuleAnimationSelect &rule);
	
	/** \brief Visit bone rotator animator rule. */
	virtual void VisitBoneTransformator(deAnimatorRuleBoneTransformator &rule);
	
	/** \brief Visit inverse kinematic animator rule. */
	virtual void VisitInverseKinematic(deAnimatorRuleInverseKinematic &rule);
	
	/** \brief Visit state manipulator rule. */
	virtual void VisitStateManipulator(deAnimatorRuleStateManipulator &rule);
	
	/** \brief Visit state snapshot rule. */
	virtual void VisitStateSnapshot(deAnimatorRuleStateSnapshot &rule);
	
	/** \brief Visit foreign state rule. */
	virtual void VisitForeignState(deAnimatorRuleForeignState &rule);
	
	/** \brief Visit group rule. */
	virtual void VisitGroup(deAnimatorRuleGroup &rule);
	
	/** \brief Visit sub animator rule. */
	virtual void VisitSubAnimator(deAnimatorRuleSubAnimator &rule);
	
	/** \brief Visit track to rule. */
	virtual void VisitTrackTo(deAnimatorRuleTrackTo &rule);
	
	/** \brief Visit limit rule. */
	virtual void VisitLimit(deAnimatorRuleLimit &rule);
	
	/**
	 * \brief Visit mirror rule.
	 * \version 1.9
	 */
	virtual void VisitMirror(deAnimatorRuleMirror &rule);
	/*@}*/
};

#endif
