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

#ifndef _DEARRULESTATESNAPSHOT_H_
#define _DEARRULESTATESNAPSHOT_H_

#include "dearRule.h"

class dearAnimationState;
class dearAnimationVPSState;
class deAnimatorRuleStateSnapshot;



/**
 * State snapshot rule.
 */
class dearRuleStateSnapshot : public dearRule{
private:
	//const deAnimatorRuleStateSnapshot &pStateSnapshot;
	
	dearAnimationState *pAnimStates;
	int pAnimStateCount;
	
	dearAnimationVPSState *pAnimVPSStates;
	int pAnimVPSStateCount;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pEnableVPS;
	const bool pUseLastState;
	const int pID;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleStateSnapshot( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleStateSnapshot &rule );
	
	/** Clean up animator. */
	virtual ~dearRuleStateSnapshot();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Capture animator state.
	 * \details The default implementation throws an exception.
	 */
	virtual void CaptureStateInto( int identifier );
	
	/**
	 * Store animation frame.
	 * \details The default implementation throws an exception.
	 */
	virtual void StoreFrameInto( int identifier, const char *moveName, float moveTime );
	
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateStates();
	void pUpdateVPSStates();
};

#endif
