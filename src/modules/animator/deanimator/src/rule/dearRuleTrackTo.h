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
