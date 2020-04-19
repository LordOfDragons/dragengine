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

#ifndef _DEARRULESUBANIMATOR_H_
#define _DEARRULESUBANIMATOR_H_

#include "dearRule.h"
#include <dragengine/common/collection/decIntList.h>

class dearAnimator;
class deAnimatorRuleSubAnimator;
class dearBoneStateList;



/**
 * \brief Sub-Animator rule.
 */
class dearRuleSubAnimator : public dearRule{
private:
	const deAnimatorRuleSubAnimator &pSubAnimator;
	
	dearAnimator *pArSubAnimator;
	unsigned int pSubAnimatorUpdateTracker;
	
	dearRule **pRules;
	int pRuleCount;
	
	dearBoneStateList *pStateList;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pDirectUseStates;
	const bool pHasSubAnimator;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	dearRuleSubAnimator( dearAnimatorInstance &instance, int firstLink,
		const deAnimatorRuleSubAnimator &rule, const decIntList &controllerMapping );
	
	/** \brief Clean up animator. */
	virtual ~dearRuleSubAnimator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Capture animator state.
	 * \details The default implementation throws an exception.
	 */
	virtual void CaptureStateInto( int identifier );
	
	/**
	 * \brief Store animation frame.
	 * \details The default implementation throws an exception.
	 */
	virtual void StoreFrameInto( int identifier, const char *moveName, float moveTime );
	
	/**
	 * \brief Check if a full rebuild of the animator instance is required.
	 */
	virtual bool RebuildInstance();
	
	/** \brief Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist );
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateRules( const decIntList &controllerMapping );
};

#endif
