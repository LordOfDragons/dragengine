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

#include "dearAnimation.h"
#include "dearAnimationMove.h"
#include "../deDEAnimator.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Class dearAnimation
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimation::dearAnimation(deDEAnimator *module, deAnimation *animation){
	if(!module || !animation){
		DETHROW(deeInvalidParam);
	}
	
	pModule = module;
	
	pAnimation = animation;
	
	pCreateMoves();
}

dearAnimation::~dearAnimation(){
}



// Management
///////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)			pModule->LogInfoFormat("Animation Timer: %s = %iys", what, (int)(timer.GetElapsedTime() * 1000000.0)); timer.Reset()
	#define DEBUG_PRINT_TIMER_TOTAL(what)	pModule->LogInfoFormat("Animation Timer-Total: %s = %iys", what, (int)(timerTotal.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif



int dearAnimation::GetMoveCount() const{
	return pMoves.GetCount();
}

dearAnimationMove *dearAnimation::GetMoveAt(int index) const{
	return (dearAnimationMove*)pMoves.GetAt(index);
}

dearAnimationMove *dearAnimation::GetMoveNamed(const char *name) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pMoves.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dearAnimationMove * const move = (dearAnimationMove*)pMoves.GetAt(i);
		if(move->GetName() == name){
			return move;
		}
	}
	
	return NULL;
}



// Private functions
//////////////////////

void dearAnimation::pCreateMoves(){
	const int count = pAnimation->GetMoveCount();
	if(count == 0){
		return;
	}
	
	pMoves.RemoveAll();
	
	int i;
	for(i=0; i<count; i++){
		pMoves.Add(dearAnimationMove::Ref::New(*pAnimation->GetMove(i)));
	}
}
