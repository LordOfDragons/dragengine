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
#include "deoglOptimizerManager.h"
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/exceptions.h>



// Class deoglOptimizerManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOptimizerManager::deoglOptimizerManager(){
	pNextOptimizer = 0;
}

deoglOptimizerManager::~deoglOptimizerManager(){
	pCleanUp();
}



// Optimizer Management
/////////////////////////

void deoglOptimizerManager::AddOptimizer(deoglOptimizer *optimizer){
	DEASSERT_NOTNULL(optimizer)
	optimizer->ResetRemainingTime();
	pOptimizers.Add(optimizer);
}

void deoglOptimizerManager::RemoveOptimizer(deoglOptimizer *optimizer){
	const int index = pOptimizers.IndexOf(optimizer);
	DEASSERT_TRUE(index != -1)
	
	pOptimizers.RemoveFrom(index);
	
	if(pNextOptimizer > index) pNextOptimizer--;
}

void deoglOptimizerManager::RemoveAllOptimizers(){
	pOptimizers.RemoveAll();
}



// Management
///////////////

void deoglOptimizerManager::Run(int timeSlice){
	int grantedTime, runOptimizers = 0;
	decTimer timer;
	
	// run optmizers until the time runs out
	while(timeSlice > 0 && runOptimizers < pOptimizers.GetCount()){
		// run the next optimizer
		deoglOptimizer &optimizer = *pOptimizers.GetAt(pNextOptimizer);
		grantedTime = optimizer.GetRemainingTime();
		if(grantedTime > timeSlice){
			grantedTime = timeSlice;
		}
		
		if(optimizer.Run(grantedTime)){
			optimizer.DecreaseRemainingTime(grantedTime);
			if(optimizer.GetRemainingTime() <= 0){
				optimizer.ResetRemainingTime();
				pNextOptimizer++;
				runOptimizers++;
			}
			
		}else{
			pOptimizers.RemoveFrom(pNextOptimizer);
		}
		
		if(pNextOptimizer == pOptimizers.GetCount()) pNextOptimizer = 0;
		
		// adjust the remaining time
		timeSlice -= (int)(timer.GetElapsedTime() * 1000000);
	}
}




// Private Functions
//////////////////////

void deoglOptimizerManager::pCleanUp(){
	RemoveAllOptimizers();
}
