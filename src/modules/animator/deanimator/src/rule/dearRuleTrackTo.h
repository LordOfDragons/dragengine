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

#ifndef _DEARRULETRACKTO_H_
#define _DEARRULETRACKTO_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>

#include "dearRule.h"



/**
 * Track to rule.
 */
class dearRuleTrackTo : public dearRule{
private:
	const deAnimatorRuleTrackTo &pTrackTo;
	
	int pTrackBone;
	
	dearControllerTarget pTargetPosition;
	dearControllerTarget pTargetUp;
	
	const deAnimatorRuleTrackTo::eLockedAxis pLockedAxis;
	const deAnimatorRuleTrackTo::eUpTarget pUpTarget;
	const deAnimatorRuleTrackTo::eTrackAxis pTrackAxis;
	const deAnimatorRuleTrackTo::eTrackAxis pUpAxis;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleTrackTo( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleTrackTo &rule );
	
	/** Clean up rule. */
	virtual ~dearRuleTrackTo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Track bone index. */
	inline int GetTrackBone() const{ return pTrackBone; }
	
	
	
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateTrackBone();
};

#endif
