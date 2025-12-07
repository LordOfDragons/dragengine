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

#include <stdio.h>
#include <string.h>

#include "dearAnimator.h"
#include "deDEAnimator.h"
#include "animation/dearAnimation.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animation/deAnimation.h>



// Class dearAnimator
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimator::dearAnimator(deDEAnimator &module, deAnimator &animator) :
pModule(module),
pAnimator(animator),
pAnimation(nullptr),
pUpdateTracker(0)
{
	if(animator.GetAnimation()){
		pAnimation = (dearAnimation*)animator.GetAnimation()->GetPeerAnimator();
	}
}

dearAnimator::~dearAnimator(){
	pCleanUp();
}



// Management
///////////////



// Notifications
//////////////////

void dearAnimator::RigChanged(){
	pUpdateTracker++;
}

void dearAnimator::AnimationChanged(){
	pUpdateTracker++;
	
	pAnimation = nullptr;
	if(pAnimator.GetAnimation()){
		pAnimation = (dearAnimation*)pAnimator.GetAnimation()->GetPeerAnimator();
	}
}

void dearAnimator::BonesChanged(){
	pUpdateTracker++;
}

void dearAnimator::VertexPositionSetsChanged(){
	pUpdateTracker++;
}

void dearAnimator::ControllerCountChanged(){
	pUpdateTracker++;
}

void dearAnimator::LinksChanged(){
	pUpdateTracker++;
}

void dearAnimator::RulesChanged(){
	pUpdateTracker++;
}



// Private functions
//////////////////////

void dearAnimator::pCleanUp(){
}
