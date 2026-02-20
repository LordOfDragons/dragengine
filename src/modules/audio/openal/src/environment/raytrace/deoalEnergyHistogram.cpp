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
#include <stdlib.h>
#include <string.h>

#include "deoalEnergyHistogram.h"
#include "deoalImpulseResponse.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnergyHistogram
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnergyHistogram::deoalEnergyHistogram() :
pSlotCount(0),
pBandCount(1),
pTime(0.0f),
pSlotTime(0.0f),
pInvSlotTime(0.0f),
pHalfSlotSize(0.0f){
}

deoalEnergyHistogram::deoalEnergyHistogram(int slotCount, int bandCount, float time) :
pSlotCount(slotCount),
pBandCount(bandCount),
pTime(decMath::max(time, 0.0f)),
pInvSlotTime(0.0f),
pHalfSlotSize(0.0f)
{
	if(slotCount < 0 || bandCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(slotCount == 0){
		return;
	}
	
	pEntries.AddRange(bandCount * slotCount, {});
	pSlotTime = time / (float)slotCount;
	pInvSlotTime = 1.0f / pSlotTime;
	pHalfSlotSize = pSlotTime * 0.5f;
}

deoalEnergyHistogram::deoalEnergyHistogram(const deoalEnergyHistogram &histogram) :
pSlotCount(histogram.pSlotCount),
pBandCount(histogram.pBandCount),
pTime(histogram.pTime),
pSlotTime(histogram.pSlotTime),
pEntries(histogram.pEntries),
pInvSlotTime(histogram.pInvSlotTime),
pHalfSlotSize(histogram.pHalfSlotSize){
}



// Manegement
/////////////

void deoalEnergyHistogram::SetParameters(int slotCount, int bandCount, float time){
	time = decMath::max(time, 0.0f);
	
	if(slotCount == pSlotCount && bandCount == pBandCount
	&& fabsf(time - pTime) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	if(slotCount < 0 || bandCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	pEntries.SetAll(bandCount * slotCount, 0.0f);
	
	pSlotCount = slotCount;
	pBandCount = bandCount;
	pTime = time;
	
	if(slotCount > 0){
		pSlotTime = time / (float)slotCount;
		pInvSlotTime = 1.0f / pSlotTime;
		
	}else{
		pSlotTime = 0.0f;
		pInvSlotTime = 0.0f;
	}
	
	pHalfSlotSize = pSlotTime * 0.5f;
}

void deoalEnergyHistogram::Clear(){
	pEntries.SetRangeAt(0, pEntries.GetCount(), 0.0f);
}

void deoalEnergyHistogram::Add(float time, const float *energies){
	const int index = decMath::max((int)((time - pHalfSlotSize) * pInvSlotTime), 0);
	if(index > pSlotCount - 1){
		return;
	}
	
	float * const slot = pEntries.GetArrayPointer() + index;
	int i;
	for(i=0; i<pBandCount; i++){
		slot[pSlotCount * i] += energies[i];
	}
}
void deoalEnergyHistogram::Add(const deoalEnergyHistogram &histogram){
	if(histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount){
		DETHROW(deeInvalidParam);
	}
	
	pEntries.VisitIndexed([&](int i, float &value){
		value += histogram.pEntries[i];
	});
}

void deoalEnergyHistogram::Add(const deoalImpulseResponse &impulseResponse){
	if(pBandCount != 3){
		DETHROW(deeInvalidParam);
	}
	
	impulseResponse.GetImpulses().Visit([&](const deoalImpulseResponse::sImpulse &impulse){
		const float energies[3] = {impulse.low, impulse.medium, impulse.high};
		Add(impulse.time, energies);
	});
}

void deoalEnergyHistogram::AddMax(float time, const float *energies){
	const int index = decMath::max((int)((time - pHalfSlotSize) * pInvSlotTime), 0);
	if(index > pSlotCount - 1){
		return;
	}
	
	float *slot = pEntries.GetArrayPointer() + index;
	int i;
	
	for(i=0; i<pBandCount; i++){
		if(energies[i] > slot[pSlotCount * i]){
			slot[pSlotCount * i] = energies[i];
		}
	}
}

void deoalEnergyHistogram::AddMax(const deoalEnergyHistogram &histogram){
	if(histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount){
		DETHROW(deeInvalidParam);
	}
	
	pEntries.VisitIndexed([&](int i, float &value){
		const float otherValue = histogram.pEntries[i];
		if(otherValue > value){
			value = otherValue;
		}
	});
}

void deoalEnergyHistogram::BlendAdd(const deoalEnergyHistogram &histogram){
	if(histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount){
		DETHROW(deeInvalidParam);
	}
	
	pEntries.VisitIndexed([&](int i, float &value){
		value += histogram.pEntries[i];
	});
}

void deoalEnergyHistogram::BlendAddScaled(const deoalEnergyHistogram &histogram, float factor){
	if(histogram.pSlotCount != pSlotCount || histogram.pBandCount != pBandCount){
		DETHROW(deeInvalidParam);
	}
	
	pEntries.VisitIndexed([&](int i, float &value){
		value += histogram.pEntries[i] * factor;
	});
}

void deoalEnergyHistogram::Integrate(float *integrations) const{
	if(!integrations){
		DETHROW(deeInvalidParam);
	}
	
	int i, j;
	for(i=0; i<pBandCount; i++){
		const float * const entries = pEntries.GetArrayPointer() + pSlotCount * i;
		float &integration = integrations[i];
		integration = 0.0f;
		
		for(j=0; j<pSlotCount; j++){
			integration += entries[j] * pSlotTime;
		}
	}
}



// Operators
//////////////

deoalEnergyHistogram &deoalEnergyHistogram::operator=(const deoalEnergyHistogram &histogram){
	SetParameters(histogram.pSlotCount, histogram.pBandCount, histogram.pTime);
	memcpy(pEntries.GetArrayPointer(), histogram.pEntries.GetArrayPointer(), sizeof(float) * pEntries.GetCount());
	return *this;
}

deoalEnergyHistogram &deoalEnergyHistogram::operator*=(float factor){
	pEntries.Visit([&](float &value){
		value *= factor;
	});
	return *this;
}
