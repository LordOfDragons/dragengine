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

#include "aeMCAnimatorLocomotionProperties.h"
#include "aeMCAnimatorProperties.h"


void aeMCAnimatorLocomotionProperties::Init(aeWindowMain &windowMain){
	type = deTObjectReference<aeMCPAnimatorLocomotionType>::New();
	limitLookDown = deTObjectReference<aeMCPAnimatorLocomotionLimitDown>::New();
	limitLookUp = deTObjectReference<aeMCPAnimatorLocomotionLimitUp>::New();
	limitLookLeft = deTObjectReference<aeMCPAnimatorLocomotionLimitLeft>::New();
	limitLookRight = deTObjectReference<aeMCPAnimatorLocomotionLimitRight>::New();
	groupTesting = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionTesting", "Animator.WPPlayground.LocomotionTesting",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			type, limitLookUp, limitLookDown, limitLookLeft, limitLookRight),
		true);
	
	speedWalk = deTObjectReference<aeMCPAnimatorLocomotionSpeedWalk>::New();
	speedRun = deTObjectReference<aeMCPAnimatorLocomotionSpeedRun>::New();
	groupMovementSpeeds = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionMovementSpeeds", "Animator.WPPlayground.MovementSpeeds",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			speedWalk, speedRun),
		true);
	
	adjustTimeUpDown = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeUD>::New();
	adjustTimeLeftRight = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeLR>::New();
	adjustTimeStance = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeStance>::New();
	adjustTimeOrientation = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeOrientation>::New();
	adjustTimeVelocity = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeVelocity>::New();
	adjustTimeTurnIP = deTObjectReference<aeMCPAnimatorLocomotionAdjTimeTurnIP>::New();
	groupAdjustmentTimes = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionAdjustmentTimes", "Animator.WPPlayground.AdjustmentTimes",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			adjustTimeUpDown, adjustTimeLeftRight, adjustTimeStance, adjustTimeOrientation,
			adjustTimeVelocity, adjustTimeTurnIP),
		true);
	
	leg.Init(windowMain);
	
	useLegPairs = deTObjectReference<aeMCPAnimatorLocomotionUseLegPairs>::New();
	legBlendTime = deTObjectReference<aeMCPAnimatorLocomotionLegBlendTime>::New();
	groupLegs = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionLegs", "Animator.WPPlayground.Legs",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			legBlendTime,
			useLegPairs,
			leg.legIndex,
			leg.leg),
		true);
	
	showShapes = deTObjectReference<aeMCPAnimatorLocomotionShowShapes>::New();
	useFoGIK = deTObjectReference<aeMCPAnimatorLocomotionUseFoGIK>::New();
	groupVisualization = deTObjectReference<igdeMetaPropertyGroup>::New(
		"animator.groupLocomotionVisualization", "Animator.WPPlayground.Visualization",
		decTObjectOrderedSet<igdeMetaProperty>(devctag,
			showShapes, useFoGIK),
		true);
}
