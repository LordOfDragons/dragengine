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

#ifndef _DEARRULESTATEMANIPULATOR_H_
#define _DEARRULESTATEMANIPULATOR_H_

#include "dearRule.h"
#include "dragengine/common/math/decMath.h"

class deAnimatorRuleStateManipulator;



/**
 * State manipulator rule.
 */
class dearRuleStateManipulator : public dearRule{
private:
	//const deAnimatorRuleStateManipulator &pStateManipulator;
	
	dearControllerTarget pTargetPosition;
	dearControllerTarget pTargetRotation;
	dearControllerTarget pTargetSize;
	dearControllerTarget pTargetVPS;
	
	const bool pEnablePosition;
	const bool pEnableOrientation;
	const bool pEnableScale;
	const bool pEnableVPS;
	const bool pEnableBones;
	const decVector pMinPosition;
	const decVector pMaxPosition;
	const decVector pMinRotation;
	const decVector pMaxRotation;
	const decVector pMinSize;
	const decVector pMaxSize;
	const float pMinVPS;
	const float pMaxVPS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRuleStateManipulator(dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRuleStateManipulator &rule);
	
	/** Clean up animator. */
	virtual ~dearRuleStateManipulator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Apply to animator. */
	virtual void Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist);
	/*@}*/
};

#endif
