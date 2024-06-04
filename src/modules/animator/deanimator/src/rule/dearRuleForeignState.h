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

#ifndef _DEARRULEFOREIGNSTATE_H_
#define _DEARRULEFOREIGNSTATE_H_

#include "dearRule.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>



/**
 * Foreign state rule.
 */
class dearRuleForeignState : public dearRule{
private:
	const deAnimatorRuleForeignState &pForeignState;
	int pForeignBone;
	int pForeignVPS;
	
	dearControllerTarget pTargetPosition;
	dearControllerTarget pTargetOrientation;
	dearControllerTarget pTargetSize;
	dearControllerTarget pTargetVPS;
	
	const float pScalePosition;
	const float pScaleOrientation;
	const float pScaleSize;
	const float pScaleVPS;
	const deAnimatorRuleForeignState::eCoordinateFrames pSourceCoordFrame;
	const deAnimatorRuleForeignState::eCoordinateFrames pDestCoordFrame;
	const bool pLockX;
	const bool pLockY;
	const bool pLockZ;
	const bool pLockNone;
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pEnableVPS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleForeignState( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleForeignState &rule );
	
	/** Clean up rule. */
	virtual ~dearRuleForeignState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Foreign bone index. */
	inline int GetForeignBone() const{ return pForeignBone; }
	
	
	
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateForeignBone();
};

#endif
