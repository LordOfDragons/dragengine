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

#ifndef _DEARRULEANIMATIONDIFFERENCE_H_
#define _DEARRULEANIMATIONDIFFERENCE_H_

#include "dearRule.h"

class dearAnimationMove;
class dearAnimationState;
class deAnimatorRuleAnimationDifference;



/**
 * \brief Difference animation rule.
 */
class dearRuleAnimationDifference : public dearRule{
private:
	const deAnimatorRuleAnimationDifference &pAnimationDifference;
	
	dearAnimationMove *pMove1;
	dearAnimationMove *pMove2;
	 //, pDirtyAnimState;
//	dearAnimationState *pAnimStates;
//	int pAnimStateCount;
	
	dearControllerTarget pTargetLeadingMoveTime;
	dearControllerTarget pTargetReferenceMoveTime;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	dearRuleAnimationDifference( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleAnimationDifference &rule );
	
	/** \brief Clean up animator. */
	virtual ~dearRuleAnimationDifference();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist );
	
	/** \brief Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateMove();
//	void pUpdateAnimStates();
};

#endif
