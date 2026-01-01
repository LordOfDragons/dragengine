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
	pOptimizers = NULL;
	pOptimizerCount = 0;
	pOptimizerSize = 0;
	pNextOptimizer = 0;
}

deoglOptimizerManager::~deoglOptimizerManager(){
	pCleanUp();
}



// Optimizer Management
/////////////////////////

deoglOptimizer *deoglOptimizerManager::GetOptimizerAt(int index) const{
	if(index < 0 || index >= pOptimizerCount) DETHROW(deeInvalidParam);
	return pOptimizers[index];
}

bool deoglOptimizerManager::HasOptimizer(deoglOptimizer *optimizer) const{
	if(!optimizer) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pOptimizerCount; i++){
		if(optimizer == pOptimizers[i]) return true;
	}
	
	return false;
}

int deoglOptimizerManager::IndexOfOptimizer(deoglOptimizer *optimizer) const{
	if(!optimizer) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pOptimizerCount; i++){
		if(optimizer == pOptimizers[i]) return i;
	}
	
	return -1;
}

void deoglOptimizerManager::AddOptimizer(deoglOptimizer *optimizer){
	if(HasOptimizer(optimizer)) DETHROW(deeInvalidParam);
	
	if(pOptimizerCount == pOptimizerSize){
		int newSize = pOptimizerSize * 3 / 2 + 1;
		deoglOptimizer **newArray = new deoglOptimizer*[newSize];
		if(pOptimizers){
			memcpy(newArray, pOptimizers, sizeof(deoglOptimizer*) * pOptimizerSize);
			delete [] pOptimizers;
		}
		pOptimizers = newArray;
		pOptimizerSize = newSize;
	}
	
	pOptimizers[pOptimizerCount] = optimizer;
	pOptimizerCount++;
	
	optimizer->ResetRemainingTime();
}

void deoglOptimizerManager::RemoveOptimizer(deoglOptimizer *optimizer){
	int i, index = IndexOfOptimizer(optimizer);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pOptimizerCount; i++){
		pOptimizers[i - 1] = pOptimizers[i];
	}
	pOptimizerCount--;
	
	if(pNextOptimizer > index) pNextOptimizer--;
	
	delete optimizer;
}

void deoglOptimizerManager::RemoveAllOptimizers(){
	while(pOptimizerCount > 0){
		pOptimizerCount--;
		delete pOptimizers[pOptimizerCount];
	}
}



// Management
///////////////

void deoglOptimizerManager::Run(int timeSlice){
	int i, grantedTime, runOptimizers = 0;
	decTimer timer;
	
	// run optmizers until the time runs out
	while(timeSlice > 0 && runOptimizers < pOptimizerCount){
		// run the next optimizer
		deoglOptimizer &optimizer = *pOptimizers[pNextOptimizer];
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
			delete pOptimizers[pNextOptimizer];
			
			for(i=pNextOptimizer+1; i<pOptimizerCount; i++){
				pOptimizers[i - 1] = pOptimizers[i];
			}
			pOptimizerCount--;
		}
		
		if(pNextOptimizer == pOptimizerCount) pNextOptimizer = 0;
		
		// adjust the remaining time
		timeSlice -= (int)(timer.GetElapsedTime() * 1000000);
	}
}




// Private Functions
//////////////////////

void deoglOptimizerManager::pCleanUp(){
	RemoveAllOptimizers();
	if(pOptimizers) delete [] pOptimizers;
}
