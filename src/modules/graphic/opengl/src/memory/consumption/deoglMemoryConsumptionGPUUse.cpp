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

#include "deoglMemoryConsumptionGPUUse.h"

#include "deoglMemoryConsumptionGPU.h"


// Class deoglMemoryConsumptionGPUUse
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionGPUUse::deoglMemoryConsumptionGPUUse(deoglMemoryConsumptionGPU &tracker) :
pTracker(tracker),
pConsumption(0){
}

deoglMemoryConsumptionGPUUse::~deoglMemoryConsumptionGPUUse(){
	SetConsumption(0);
}



// Management
///////////////

void deoglMemoryConsumptionGPUUse::SetConsumption(unsigned long long consumption){
	if(consumption > pConsumption){
		IncrementConsumption(consumption - pConsumption);
		
	}else if(consumption < pConsumption){
		DecrementConsumption(pConsumption - consumption);
	}
}

void deoglMemoryConsumptionGPUUse::IncrementConsumption(unsigned long long consumption){
	if(consumption == 0){
		return;
	}
	
	pTracker.IncrementConsumption(consumption);
	
	if(pConsumption == 0){
		pTracker.IncrementCount();
	}
	
	pConsumption += consumption;
}

void deoglMemoryConsumptionGPUUse::DecrementConsumption(unsigned long long consumption){
	if(consumption == 0){
		return;
	}
	
	if(consumption < pConsumption){
		pTracker.DecrementConsumption(consumption);
		pConsumption -= consumption;
		
	}else{
		pTracker.DecrementConsumption(pConsumption);
		pTracker.DecrementCount();
		pConsumption = 0;
	}
}



// Operators
//////////////

deoglMemoryConsumptionGPUUse &deoglMemoryConsumptionGPUUse::operator=(unsigned long long consumption){
	SetConsumption(consumption);
	return *this;
}

deoglMemoryConsumptionGPUUse &deoglMemoryConsumptionGPUUse::operator+=(unsigned long long consumption){
	IncrementConsumption(consumption);
	return *this;
}

deoglMemoryConsumptionGPUUse &deoglMemoryConsumptionGPUUse::operator-=(unsigned long long consumption){
	DecrementConsumption(consumption);
	return *this;
}
