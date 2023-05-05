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

#ifndef _DEARRULEANIMATIONSELECT_H_
#define _DEARRULEANIMATIONSELECT_H_

#include "dearRule.h"
#include "../dearMapAnimationBones.h"
#include "../dearMapAnimationVPS.h"

#include <dragengine/common/collection/decObjectList.h>

class dearAnimationState;
class deAnimatorRuleAnimationSelect;



/**
 * Select animation rule.
 */
class dearRuleAnimationSelect : public dearRule{
private:
	const deAnimatorRuleAnimationSelect &pAnimationSelect;
	dearMapAnimationBones pMapAnimationBones;
	dearMapAnimationVPS pMapAnimationVPS;
	
	decObjectList pMoves;
	
	dearControllerTarget pTargetMoveTime;
	dearControllerTarget pTargetSelect;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pEnableVPS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleAnimationSelect( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleAnimationSelect &rule );
	
	/** Clean up animator. */
	virtual ~dearRuleAnimationSelect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateMoves();
};

#endif
