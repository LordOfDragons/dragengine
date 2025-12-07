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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglOptimizer.h"
#include "dragengine/common/exceptions.h"



// Class deoglOptimizer
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglOptimizer::deoglOptimizer(){
	SetPriority(epNormal);
	ResetRemainingTime();
}

deoglOptimizer::~deoglOptimizer(){
}



// Management
///////////////

void deoglOptimizer::DecreaseRemainingTime(int microSeconds){
	pRemainingTime -= microSeconds;
}

void deoglOptimizer::ResetRemainingTime(){
	pRemainingTime = pMaxTime;
}

void deoglOptimizer::SetPriority(int priority){
	if(priority == epLowest){
		pMaxTime = 1000;
		
	}else if(priority == epLower){
		pMaxTime = 5000;
		
	}else if(priority == epNormal){
		pMaxTime = 10000;
		
	}else if(priority == epHigher){
		pMaxTime = 50000;
		
	}else if(priority == epHighest){
		pMaxTime = 100000;
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	pPriority = priority;
}

bool deoglOptimizer::Run(int timeSlice){
	return false;
}
