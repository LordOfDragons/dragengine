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
