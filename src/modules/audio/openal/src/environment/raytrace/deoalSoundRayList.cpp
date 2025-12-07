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

#include "deoalSoundRay.h"
#include "deoalSoundRaySegment.h"
#include "deoalSoundRayList.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRayList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalSoundRayList::deoalSoundRayList() :
pRays(NULL),
pRayCount(0),
pRaySize(0),

pSegments(NULL),
pSegmentCount(0),
pSegmentSize(0),

pTransmittedRays(NULL),
pTransmittedRayCount(0),
pTransmittedRaySize(0){
}

deoalSoundRayList::~deoalSoundRayList(){
	if(pTransmittedRays){
		delete [] pTransmittedRays;
	}
	if(pSegments){
		delete [] pSegments;
	}
	if(pRays){
		delete [] pRays;
	}
}



// Manegement
///////////////

deoalSoundRay &deoalSoundRayList::GetRayAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pRayCount)
	return pRays[index];
}

const deoalSoundRay &deoalSoundRayList::GetRayAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pRayCount)
	return pRays[index];
}

int deoalSoundRayList::AddRay(){
	if(pRayCount == pRaySize){
		const int newSize = pRaySize + 10;
		deoalSoundRay * const newArray = new deoalSoundRay[newSize];
		if(pRays){
			memcpy(newArray, pRays, sizeof(deoalSoundRay) * pRaySize);
			delete [] pRays;
		}
		pRays = newArray;
		pRaySize = newSize;
	}
	
	deoalSoundRay &ray = pRays[pRayCount];
	ray.SetFirstSegment(pSegmentCount);
	ray.SetSegmentCount(0);
	ray.SetFirstTransmittedRay(pTransmittedRayCount);
	ray.SetTransmittedRayCount(0);
	ray.SetOutside(false);
	return pRayCount++;
}

void deoalSoundRayList::RemoveAllRays(){
	pRayCount = 0;
	pSegmentCount = 0;
	pTransmittedRayCount = 0;
}

deoalSoundRaySegment &deoalSoundRayList::GetSegmentAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pSegmentCount)
	return pSegments[index];
}

const deoalSoundRaySegment &deoalSoundRayList::GetSegmentAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pSegmentCount)
	return pSegments[index];
}

int deoalSoundRayList::AddSegment(){
	if(pSegmentCount == pSegmentSize){
		const int newSize = pSegmentSize + 10;
		deoalSoundRaySegment * const newArray = new deoalSoundRaySegment[newSize];
		if(pSegments){
			memcpy(newArray, pSegments, sizeof(deoalSoundRaySegment) * pSegmentSize);
			delete [] pSegments;
		}
		pSegments = newArray;
		pSegmentSize = newSize;
	}
	
	return pSegmentCount++;
}

deoalSoundRay &deoalSoundRayList::GetTransmittedRayAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pTransmittedRayCount)
	return pTransmittedRays[index];
}

const deoalSoundRay &deoalSoundRayList::GetTransmittedRayAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pTransmittedRayCount)
	return pTransmittedRays[index];
}

int deoalSoundRayList::AddTransmittedRay(){
	if(pTransmittedRayCount == pTransmittedRaySize){
		const int newSize = pTransmittedRaySize + 10;
		deoalSoundRay * const newArray = new deoalSoundRay[newSize];
		if(pTransmittedRays){
			memcpy(newArray, pTransmittedRays, sizeof(deoalSoundRay) * pTransmittedRaySize);
			delete [] pTransmittedRays;
		}
		pTransmittedRays = newArray;
		pTransmittedRaySize = newSize;
	}
	
	deoalSoundRay &ray = pTransmittedRays[pTransmittedRayCount];
	ray.SetFirstSegment(pSegmentCount);
	ray.SetSegmentCount(0);
	ray.SetFirstTransmittedRay(pTransmittedRayCount);
	ray.SetTransmittedRayCount(0);
	ray.SetOutside(false);
	return pTransmittedRayCount++;
}

void deoalSoundRayList::ReserveSize(int rays, int segments, int transmittedRays){
	DEASSERT_TRUE(rays >= 0)
	DEASSERT_TRUE(segments >= 0)
	DEASSERT_TRUE(transmittedRays >= 0)
	
	if(rays > pRaySize){
		deoalSoundRay * const newArray = new deoalSoundRay[rays];
		if(pRays){
			memcpy(newArray, pRays, sizeof(deoalSoundRay) * pRaySize);
			delete [] pRays;
		}
		pRays = newArray;
		pRaySize = rays;
	}
	
	if(segments > pSegmentSize){
		deoalSoundRaySegment * const newArray = new deoalSoundRaySegment[segments];
		if(pSegments){
			memcpy(newArray, pSegments, sizeof(deoalSoundRaySegment) * pSegmentSize);
			delete [] pSegments;
		}
		pSegments = newArray;
		pSegmentSize = segments;
	}
	
	if(transmittedRays > pTransmittedRaySize){
		deoalSoundRay * const newArray = new deoalSoundRay[transmittedRays];
		if(pTransmittedRays){
			memcpy(newArray, pTransmittedRays, sizeof(deoalSoundRay) * pTransmittedRaySize);
			delete [] pTransmittedRays;
		}
		pTransmittedRays = newArray;
		pTransmittedRaySize = transmittedRays;
	}
}



// Operators
//////////////

deoalSoundRayList &deoalSoundRayList::operator=(
const deoalSoundRayList &list){
	pRayCount = 0;
	pSegmentCount = 0;
	pTransmittedRayCount = 0;
	return operator+=(list);
}

deoalSoundRayList &deoalSoundRayList::operator+=(
const deoalSoundRayList &list){
	ReserveSize(pRayCount + list.pRayCount, pSegmentCount + list.pSegmentCount,
		pTransmittedRayCount + list.pTransmittedRayCount);
	const int segmentOffset = pSegmentCount;
	const int rayOffset = pRayCount;
	const int transmittedRayOffset = pTransmittedRayCount;
	int i;
	
	for(i=0; i<list.pTransmittedRayCount; i++){
		const deoalSoundRay &raySrc = list.pTransmittedRays[i];
		deoalSoundRay &ray = pTransmittedRays[transmittedRayOffset + i];
		ray.SetFirstSegment(raySrc.GetFirstSegment() + segmentOffset);
		ray.SetSegmentCount(raySrc.GetSegmentCount());
		ray.SetFirstTransmittedRay(raySrc.GetFirstTransmittedRay() + transmittedRayOffset);
		ray.SetTransmittedRayCount(raySrc.GetTransmittedRayCount());
		ray.SetOutside(raySrc.GetOutside());
	}
	pTransmittedRayCount += list.pTransmittedRayCount;
	
	for(i=0; i<list.pSegmentCount; i++){
		pSegments[segmentOffset + i] = list.pSegments[i];
	}
	pSegmentCount += list.pSegmentCount;
	
	for(i=0; i<list.pRayCount; i++){
		const deoalSoundRay &raySrc = list.pRays[i];
		deoalSoundRay &ray = pRays[rayOffset + i];
		ray.SetFirstSegment(raySrc.GetFirstSegment() + segmentOffset);
		ray.SetSegmentCount(raySrc.GetSegmentCount());
		ray.SetFirstTransmittedRay(raySrc.GetFirstTransmittedRay() + transmittedRayOffset);
		ray.SetTransmittedRayCount(raySrc.GetTransmittedRayCount());
		ray.SetOutside(raySrc.GetOutside());
	}
	pRayCount += list.pRayCount;
	
	return *this;
}
