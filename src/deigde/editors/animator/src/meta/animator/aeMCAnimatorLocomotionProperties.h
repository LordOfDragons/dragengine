/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _AEMCANIMATORLOCOMOTIONPROPERTIES_H_
#define _AEMCANIMATORLOCOMOTIONPROPERTIES_H_

#include "aeMCPAnimatorLocomotion.h"
#include "aeMCAnimatorLocomotionLegProperties.h"

class windowMain;


/**
 * Animator meta context properties.
 */
class aeMCAnimatorLocomotionProperties{
public:
	deTObjectReference<aeMCPAnimatorLocomotionType> type;
	deTObjectReference<aeMCPAnimatorLocomotionLimitDown> limitLookDown;
	deTObjectReference<aeMCPAnimatorLocomotionLimitUp> limitLookUp;
	deTObjectReference<aeMCPAnimatorLocomotionLimitLeft> limitLookLeft;
	deTObjectReference<aeMCPAnimatorLocomotionLimitRight> limitLookRight;
	deTObjectReference<igdeMetaPropertyGroup> groupTesting;
	
	deTObjectReference<aeMCPAnimatorLocomotionSpeedWalk> speedWalk;
	deTObjectReference<aeMCPAnimatorLocomotionSpeedRun> speedRun;
	deTObjectReference<igdeMetaPropertyGroup> groupMovementSpeeds;
	
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeUD> adjustTimeUpDown;
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeLR> adjustTimeLeftRight;
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeStance> adjustTimeStance;
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeOrientation> adjustTimeOrientation;
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeVelocity> adjustTimeVelocity;
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeTurnIP> adjustTimeTurnIP;
	deTObjectReference<igdeMetaPropertyGroup> groupAdjustmentTimes;
	
	aeMCAnimatorLocomotionLegProperties leg;
	
	deTObjectReference<aeMCPAnimatorLocomotionUseLegPairs> useLegPairs;
	deTObjectReference<aeMCPAnimatorLocomotionLegBlendTime> legBlendTime;
	deTObjectReference<igdeMetaPropertyGroup> groupLegs;
	
	deTObjectReference<aeMCPAnimatorLocomotionShowShapes> showShapes;
	deTObjectReference<aeMCPAnimatorLocomotionUseFoGIK> useFoGIK;
	deTObjectReference<igdeMetaPropertyGroup> groupVisualization;
	
	void Init(aeWindowMain &windowMain);
};

#endif
