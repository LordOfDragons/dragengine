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

#ifndef _DEARRULELIMIT_H_
#define _DEARRULELIMIT_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>

#include "dearRule.h"



/**
 * Animator rule limit.
 */
class dearRuleLimit : public dearRule{
private:
	const deAnimatorRuleLimit &pLimit;
	
	int pTargetBone;
	
	const bool pEnablePositionXMin;
	const bool pEnablePositionXMax;
	const bool pEnablePositionYMin;
	const bool pEnablePositionYMax;
	const bool pEnablePositionZMin;
	const bool pEnablePositionZMax;
	const bool pEnablePositionAny;
	
	const bool pEnableRotationXMin;
	const bool pEnableRotationXMax;
	const bool pEnableRotationYMin;
	const bool pEnableRotationYMax;
	const bool pEnableRotationZMin;
	const bool pEnableRotationZMax;
	const bool pEnableRotationAny;
	
	const bool pEnableScalingXMin;
	const bool pEnableScalingXMax;
	const bool pEnableScalingYMin;
	const bool pEnableScalingYMax;
	const bool pEnableScalingZMin;
	const bool pEnableScalingZMax;
	const bool pEnableScalingAny;
	
	const bool pEnableVPSMin;
	const bool pEnableVPSMax;
	const bool pEnableVPSAny;
	
	const bool pEnabledAny;
	
	const deAnimatorRuleLimit::eCoordinateFrames pCoordinateFrame;
	const decVector pMinPosition;
	const decVector pMaxPosition;
	const decVector pMinRotation;
	const decVector pMaxRotation;
	const decVector pMinScaling;
	const decVector pMaxScaling;
	const float pMinVPS;
	const float pMaxVPS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleLimit( dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleLimit &rule );
	
	/** Clean up animator. */
	virtual ~dearRuleLimit();
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
