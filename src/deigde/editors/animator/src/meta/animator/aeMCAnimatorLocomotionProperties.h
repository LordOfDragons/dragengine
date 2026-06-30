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

class aeMCAnimatorProperties;


/**
 * Animator meta context properties.
 */
class aeMCAnimatorLocomotionProperties{
public:
	deTObjectReference<aeMCPAnimatorLocomotionType> type = deTObjectReference<aeMCPAnimatorLocomotionType>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLimitDown> limitLookDown = deTObjectReference<aeMCPAnimatorLocomotionLimitDown>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLimitUp> limitLookUp = deTObjectReference<aeMCPAnimatorLocomotionLimitUp>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLimitLeft> limitLookLeft = deTObjectReference<aeMCPAnimatorLocomotionLimitLeft>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLimitRight> limitLookRight = deTObjectReference<aeMCPAnimatorLocomotionLimitRight>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupTesting = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionTesting", "Animator.WPPlayground.LocomotionTesting",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			type, limitLookUp, limitLookDown, limitLookLeft, limitLookRight), true);
	
	deTObjectReference<aeMCPAnimatorLocomotionSpeedWalk> speedWalk = deTObjectReference<aeMCPAnimatorLocomotionSpeedWalk>::New();
	deTObjectReference<aeMCPAnimatorLocomotionSpeedRun> speedRun = deTObjectReference<aeMCPAnimatorLocomotionSpeedRun>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupMovementSpeeds = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionMovementSpeeds", "Animator.WPPlayground.MovementSpeeds",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			speedWalk, speedRun), true);
	
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeUD> adjustTimeUpDown = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeUD>::New();
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeLR> adjustTimeLeftRight = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeLR>::New();
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeStance> adjustTimeStance = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeStance>::New();
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeOrientation> adjustTimeOrientation = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeOrientation>::New();
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeVelocity> adjustTimeVelocity = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeVelocity>::New();
	deTObjectReference<aeMCPAnimatorLocomotionAdjTimeTurnIP> adjustTimeTurnIP = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeTurnIP>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupAdjustmentTimes = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionAdjustmentTimes", "Animator.WPPlayground.AdjustmentTimes",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			adjustTimeUpDown, adjustTimeLeftRight, adjustTimeStance, adjustTimeOrientation,
			adjustTimeVelocity, adjustTimeTurnIP), true);
	
	deTObjectReference<aeMCPAnimatorLocomotionUseLegPairs> useLegPairs = deTObjectReference<aeMCPAnimatorLocomotionUseLegPairs>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLegBlendTime> legBlendTime = deTObjectReference<aeMCPAnimatorLocomotionLegBlendTime>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupLegs = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionLegs", "Animator.WPPlayground.Legs", decTObjectOrderedSet<igdeMetaProperty>(), true);
	
	deTObjectReference<aeMCPAnimatorLocomotionShowShapes> showShapes = deTObjectReference<aeMCPAnimatorLocomotionShowShapes>::New();
	deTObjectReference<aeMCPAnimatorLocomotionUseFoGIK> useFoGIK = deTObjectReference<aeMCPAnimatorLocomotionUseFoGIK>::New();
	deTObjectReference<igdeMetaPropertyGroup> groupVisualization = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionVisualization", "Animator.WPPlayground.Visualization",
		decTObjectOrderedSet<igdeMetaProperty>(devctag, showShapes, useFoGIK), true);
	
	explicit aeMCAnimatorLocomotionProperties();
	void Init(const aeMCAnimatorProperties &properties);
};

#endif
