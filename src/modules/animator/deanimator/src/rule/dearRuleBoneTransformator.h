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

#ifndef _DEARRULEBONETRANSFORMATOR_H_
#define _DEARRULEBONETRANSFORMATOR_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>

#include "dearRule.h"



/**
 * Base animator rule.
 */
class dearRuleBoneTransformator : public dearRule{
private:
	const deAnimatorRuleBoneTransformator &pBoneTransformator;
	
	int pTargetBone;
	
	dearControllerTarget pTargetTranslation;
	dearControllerTarget pTargetRotation;
	dearControllerTarget pTargetScaling;
	
	const deAnimatorRuleBoneTransformator::eCoordinateFrames pCoordinateFrame;
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableSize;
	const bool pUseAxis;
	const decVector pMinTranslation;
	const decVector pMaxTranslation;
	const decVector pMinRotation;
	const decVector pMaxRotation;
	const decVector pMinScaling;
	const decVector pMaxScaling;
	const decVector pAxis;
	const float pMinAngle;
	const float pMaxAngle;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleBoneTransformator( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleBoneTransformator &rule );
	
	/** Clean up rule. */
	virtual ~dearRuleBoneTransformator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Target bone index. */
	inline int GetTargetBone() const{ return pTargetBone; }
	
	
	
	/** Apply to animator. */
	virtual void Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist );
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateTargetBone();
};

#endif
