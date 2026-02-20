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

#include "deoalSoundRaySegment.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRaySegment
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalSoundRaySegment::deoalSoundRaySegment() :
pDirection(0.0f, 0.0f, 1.0f),
pLength(0.0f),
pDistance(0.0f),
pGainLow(0.0f),
pGainMedium(0.0f),
pGainHigh(0.0f),
pAbsorptionSumLow(0.0f),
pAbsorptionSumMedium(0.0f),
pAbsorptionSumHigh(0.0f),
pBounceCount(0),
pTransmittedCount(0){
}

deoalSoundRaySegment::deoalSoundRaySegment(const deoalSoundRaySegment &segment) :
pPosition(segment.pPosition),
pDirection(segment.pDirection),
pNormal(segment.pNormal),
pLength(segment.pLength),
pDistance(segment.pDistance),
pGainLow(segment.pGainLow),
pGainMedium(segment.pGainMedium),
pGainHigh(segment.pGainHigh),
pAbsorptionSumLow(segment.pAbsorptionSumLow),
pAbsorptionSumMedium(segment.pAbsorptionSumMedium),
pAbsorptionSumHigh(segment.pAbsorptionSumHigh),
pBounceCount(segment.pBounceCount),
pTransmittedCount(segment.pTransmittedCount){
}



// Manegement
/////////////

void deoalSoundRaySegment::SetPosition(const decVector &position){
	pPosition = position;
}

void deoalSoundRaySegment::SetDirection(const decVector &direction){
	pDirection = direction;
}

void deoalSoundRaySegment::SetNormal(const decVector &normal){
	pNormal = normal;
}

void deoalSoundRaySegment::SetLength(float length){
	pLength = length;
}

void deoalSoundRaySegment::SetDistance(float distance){
	pDistance = distance;
}

void deoalSoundRaySegment::SetGainLow(float gain){
	pGainLow = gain;
}

void deoalSoundRaySegment::SetGainMedium(float gain){
	pGainMedium = gain;
}

void deoalSoundRaySegment::SetGainHigh(float gain){
	pGainHigh = gain;
}

void deoalSoundRaySegment::SetAbsorptionSumLow(float absorption){
	   pAbsorptionSumLow = absorption;
}

void deoalSoundRaySegment::SetAbsorptionSumMedium(float absorption){
	   pAbsorptionSumMedium = absorption;
}

void deoalSoundRaySegment::SetAbsorptionSumHigh(float absorption){
	   pAbsorptionSumHigh = absorption;
}

void deoalSoundRaySegment::SetBounceCount(int count){
	pBounceCount = count;
}

void deoalSoundRaySegment::SetTransmittedCount(int count){
	pTransmittedCount = count;
}



deoalSoundRaySegment &deoalSoundRaySegment::operator=(const deoalSoundRaySegment &segment){
	pPosition = segment.pPosition;
	pDirection = segment.pDirection;
	pNormal = segment.pNormal;
	pLength = segment.pLength;
	pDistance = segment.pDistance;
	pGainLow = segment.pGainLow;
	pGainMedium = segment.pGainMedium;
	pGainHigh = segment.pGainHigh;
	pAbsorptionSumLow = segment.pAbsorptionSumLow;
	pAbsorptionSumMedium = segment.pAbsorptionSumMedium;
	pAbsorptionSumHigh = segment.pAbsorptionSumHigh;
	pBounceCount = segment.pBounceCount;
	pTransmittedCount = segment.pTransmittedCount;
	return *this;
}
