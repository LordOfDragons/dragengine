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

#ifndef _DEARRULESUBANIMATOR_H_
#define _DEARRULESUBANIMATOR_H_

#include "dearRule.h"
#include <dragengine/common/collection/decIntList.h>

class dearAnimator;
class deAnimatorRuleSubAnimator;



/**
 * Sub-Animator rule.
 */
class dearRuleSubAnimator : public dearRule{
private:
	const deAnimatorRuleSubAnimator &pSubAnimator;
	
	dearAnimator *pArSubAnimator;
	unsigned int pSubAnimatorUpdateTracker;
	
	dearRule **pRules;
	int pRuleCount;
	
	dearBoneStateList *pStateList;
	dearVPSStateList *pVPSStateList;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pEnableVPS;
	const bool pDirectUseStates;
	const bool pHasSubAnimator;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleSubAnimator( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleSubAnimator &rule, const decIntList &controllerMapping );
	
	/** Clean up animator. */
	virtual ~dearRuleSubAnimator();
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
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateRules( const decIntList &controllerMapping );
};

#endif
