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

#ifndef _DEARRULEGROUP_H_
#define _DEARRULEGROUP_H_

#include "dearRule.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>

class decIntList;
class deAnimator;
class dearBoneStateList;
class dearVPSStateList;



/**
 * Group rule.
 */
class dearRuleGroup : public dearRule{
private:
	const deAnimatorRuleGroup &pGroup;
	
	dearBoneStateList *pStateList;
	dearBoneStateList *pStateList2;
	
	dearVPSStateList *pVPSStateList;
	dearVPSStateList *pVPSStateList2;
	
	dearRule **pRules;
	int pRuleCount;
	
	dearControllerTarget pTargetSelect;
	
	const deAnimatorRuleGroup::eApplicationTypes pApplicationType;
	const bool pUseCurrentState;
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pEnableVPS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleGroup( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleGroup &rule, const decIntList &controllerMapping );
	
	/** Clean up animator. */
	virtual ~dearRuleGroup();
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
	
	/**
	 * Check if a full rebuild of the animator instance is required.
	 */
	virtual bool RebuildInstance() const;
	
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateRules( int firstLink, const decIntList &controllerMapping );
};

#endif
