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
#include "../dearMapAnimationBones.h"
#include "../dearMapAnimationVPS.h"

class dearAnimationMove;
class dearAnimationState;
class deAnimatorRuleAnimationDifference;



/**
 * Difference animation rule.
 */
class dearRuleAnimationDifference : public dearRule{
private:
	const deAnimatorRuleAnimationDifference &pAnimationDifference;
	dearMapAnimationBones pMapAnimationBones;
	dearMapAnimationVPS pMapAnimationVPS;
	
	dearAnimationMove *pMove1;
	dearAnimationMove *pMove2;
	
	dearControllerTarget pTargetLeadingMoveTime;
	dearControllerTarget pTargetReferenceMoveTime;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pEnableVPS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleAnimationDifference( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleAnimationDifference &rule );
	
	/** Clean up animator. */
	virtual ~dearRuleAnimationDifference();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateMove();
};

#endif
