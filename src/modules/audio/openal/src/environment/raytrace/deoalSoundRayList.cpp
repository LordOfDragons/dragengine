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

#include "deoalSoundRayList.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRayList
////////////////////////////

// Manegement
///////////////

deoalSoundRay &deoalSoundRayList::GetRayAt(int index){
	return pRays.GetAt(index);
}

const deoalSoundRay &deoalSoundRayList::GetRayAt(int index) const{
	return pRays.GetAt(index);
}

int deoalSoundRayList::AddRay(){
	pRays.Add(deoalSoundRay(pSegments.GetCount(), pTransmittedRays.GetCount()));
	return pRays.GetCount() - 1;
}

void deoalSoundRayList::RemoveAllRays(){
	pRays.RemoveAll();
	pSegments.RemoveAll();
	pTransmittedRays.RemoveAll();
}

deoalSoundRaySegment &deoalSoundRayList::GetSegmentAt(int index){
	return pSegments.GetAt(index);
}

const deoalSoundRaySegment &deoalSoundRayList::GetSegmentAt(int index) const{
	return pSegments.GetAt(index);
}

int deoalSoundRayList::AddSegment(){
	pSegments.Add({});
	return pSegments.GetCount() - 1;
}

deoalSoundRay &deoalSoundRayList::GetTransmittedRayAt(int index){
	return pTransmittedRays.GetAt(index);
}

const deoalSoundRay &deoalSoundRayList::GetTransmittedRayAt(int index) const{
	return pTransmittedRays.GetAt(index);
}

int deoalSoundRayList::AddTransmittedRay(){
	pTransmittedRays.Add(deoalSoundRay(pSegments.GetCount(), pTransmittedRays.GetCount()));
	return pTransmittedRays.GetCount() - 1;
}

void deoalSoundRayList::ReserveSize(int rays, int segments, int transmittedRays){
	pRays.EnlargeCapacity(rays);
	pSegments.EnlargeCapacity(segments);
	pTransmittedRays.EnlargeCapacity(transmittedRays);
}



// Operators
//////////////

deoalSoundRayList &deoalSoundRayList::operator=(const deoalSoundRayList &list){
	RemoveAllRays();
	return operator+=(list);
}

deoalSoundRayList &deoalSoundRayList::operator+=(const deoalSoundRayList &list){
	ReserveSize(pRays.GetCount() + list.pRays.GetCount(),
		pSegments.GetCount() + list.pSegments.GetCount(),
		pTransmittedRays.GetCount() + list.pTransmittedRays.GetCount());
	
	const int segmentOffset = pSegments.GetCount();
	const int transmittedRayOffset = pTransmittedRays.GetCount();
	
	list.pTransmittedRays.Visit([&](const deoalSoundRay &raySrc){
		pTransmittedRays.Add(deoalSoundRay(
			raySrc.GetFirstSegment() + segmentOffset,
			raySrc.GetSegmentCount(),
			raySrc.GetFirstTransmittedRay() + transmittedRayOffset,
			raySrc.GetTransmittedRayCount(),
			raySrc.GetOutside()));
	});
	
	pSegments += list.pSegments;
	
	list.pRays.Visit([&](const deoalSoundRay &raySrc){
		pRays.Add(deoalSoundRay(
			raySrc.GetFirstSegment() + segmentOffset,
			raySrc.GetSegmentCount(),
			raySrc.GetFirstTransmittedRay() + transmittedRayOffset,
			raySrc.GetTransmittedRayCount(),
			raySrc.GetOutside()));
	});
	
	return *this;
}
