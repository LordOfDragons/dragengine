/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	dearCreateRuleVisitor( dearAnimatorInstance &instance, const dearAnimator &animator,
		const decIntList &controllerMapping, int firstLink );
	
	/** Clean up object. */
	virtual ~dearCreateRuleVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset visitor. */
	void Reset();
	
	/** Created rule. */
	inline dearRule *GetCreatedRule() const{ return pCreatedRule; }
	
	/** Rule has been created. */
	bool HasCreatedRule() const;
	
	/** Create rule. */
	dearRule *CreateRuleFrom( deAnimatorRule &engRule );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit animator rule. */
	virtual void VisitRule( deAnimatorRule &rule );
	
	/** Visit animation rule. */
	virtual void VisitAnimation( deAnimatorRuleAnimation &rule );
	
	/** Visit animation difference rule. */
	virtual void VisitAnimationDifference( deAnimatorRuleAnimationDifference &rule );
	
	/** Visit animation select rule. */
	virtual void VisitAnimationSelect( deAnimatorRuleAnimationSelect &rule );
	
	/** Visit bone rotator animator rule. */
	virtual void VisitBoneTransformator( deAnimatorRuleBoneTransformator &rule );
	
	/** Visit inverse kinematic animator rule. */
	virtual void VisitInverseKinematic( deAnimatorRuleInverseKinematic &rule );
	
	/** Visit state manipulator rule. */
	virtual void VisitStateManipulator( deAnimatorRuleStateManipulator &rule );
	
	/** Visit state snapshot rule. */
	virtual void VisitStateSnapshot( deAnimatorRuleStateSnapshot &rule );
	
	/** Visit foreign state rule. */
	virtual void VisitForeignState( deAnimatorRuleForeignState &rule );
	
	/** Visit group rule. */
	virtual void VisitGroup( deAnimatorRuleGroup &rule );
	
	/** Visit sub animator rule. */
	virtual void VisitSubAnimator( deAnimatorRuleSubAnimator &rule );
	
	/** Visit track to rule. */
	virtual void VisitTrackTo( deAnimatorRuleTrackTo &rule );
	
	/** Visit limit rule. */
	virtual void VisitLimit( deAnimatorRuleLimit &rule );
	
	/**
	 * Visit mirror rule.
	 * \version 1.9
	 */
	virtual void VisitMirror( deAnimatorRuleMirror &rule );
	/*@}*/
};

#endif
