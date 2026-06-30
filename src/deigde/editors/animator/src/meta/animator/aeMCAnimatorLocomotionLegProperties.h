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

#ifndef _AEMCANIMATORLOCOMOTIONLEGPROPERTIES_H_
#define _AEMCANIMATORLOCOMOTIONLEGPROPERTIES_H_

#include "aeMCPAnimatorLocomotion.h"

class aeWindowMain;


/**
 * Animator meta context properties.
 */
class aeMCAnimatorLocomotionLegProperties{
public:
	deTObjectReference<aeMCPAnimatorLocomotionLegLiftOffTime> liftOffTime = deTObjectReference<aeMCPAnimatorLocomotionLegLiftOffTime>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLegPutDownTime> putDownTime = deTObjectReference<aeMCPAnimatorLocomotionLegPutDownTime>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLegPDPosStand> putDownPosStand = deTObjectReference<aeMCPAnimatorLocomotionLegPDPosStand>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLegPDPosWalk> putDownPosWalk = deTObjectReference<aeMCPAnimatorLocomotionLegPDPosWalk>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLegPDPosRun> putDownPosRun = deTObjectReference<aeMCPAnimatorLocomotionLegPDPosRun>::New();
	
	igdeMetaContext::PropertyList::Ref metaProperties = igdeMetaContext::PropertyList::Ref::New();
	
	deTObjectReference<aeMCPAnimatorLocomotionLegs> legs = deTObjectReference<aeMCPAnimatorLocomotionLegs>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLegIndex> legIndex = deTObjectReference<aeMCPAnimatorLocomotionLegIndex>::New();
	deTObjectReference<aeMCPAnimatorLocomotionLeg> leg;
	
	explicit aeMCAnimatorLocomotionLegProperties(aeWindowMain &windowMain);
	void Init();
};

#endif
