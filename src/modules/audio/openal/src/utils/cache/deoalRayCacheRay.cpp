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

#include "deoalRayCacheRay.h"
#include "deoalRayCacheRayHit.h"
#include "../../environment/raytrace/deoalRayTraceResult.h"
#include "../../environment/raytrace/deoalRayTraceHitElement.h"
#include "../../environment/raytrace/deoalRayTraceHitElementList.h"
#include "../../component/deoalAComponent.h"
#include "../../model/deoalAModel.h"
#include "../../model/deoalModelFace.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayCacheRay
////////////////////////

// Constructor, destructor
////////////////////////////

deoalRayCacheRay::deoalRayCacheRay(const decVector &origin, const decVector &direction,
	float length, const deoalRayTraceHitElementList &elements) :
pOrigin(origin),
pDirection(direction),
pLength(length),
pHits(NULL),
pHitCount(0),
pOctreeNode(NULL),
pLLPrev(NULL),
pLLNext(NULL)
{
	const int elementCount = elements.GetCount();
	if(elementCount == 0){
		return;
	}
	
	pHits = new deoalRayCacheRayHit[elementCount];
	
	for(pHitCount=0; pHitCount<elementCount; pHitCount++){
		const deoalRayTraceHitElement &element = elements.GetAt(pHitCount);
		deoalRayCacheRayHit &hit = pHits[pHitCount];
		
		hit.SetDistance(element.GetDistance());
		hit.SetFaceIndex(element.GetComponentFace());
		hit.SetForwardFacing(element.GetForwardFacing());
		hit.SetPoint(element.GetPoint());
		hit.SetNormal(element.GetNormal());
	}
}

deoalRayCacheRay::deoalRayCacheRay(const decVector &origin, const decVector &direction,
float length, const deoalRayTraceResult &result) :
pOrigin(origin),
pDirection(direction),
pLength(length),
pHits(NULL),
pHitCount(0),
pOctreeNode(NULL),
pLLPrev(NULL),
pLLNext(NULL)
{
	const int count = result.GetElementCount();
	if(count == 0){
		return;
	}
	
	pHits = new deoalRayCacheRayHit[count];
	
	for(pHitCount=0; pHitCount<count; pHitCount++){
		const deoalRayTraceHitElement &element = result.GetElementAt(pHitCount);
		deoalRayCacheRayHit &hit = pHits[pHitCount];
		
		hit.SetDistance(element.GetDistance());
		hit.SetFaceIndex(element.GetComponentFace());
		hit.SetForwardFacing(element.GetForwardFacing());
		hit.SetPoint(element.GetPoint());
		hit.SetNormal(element.GetNormal());
	}
}

deoalRayCacheRay::deoalRayCacheRay(const decVector &origin, const decVector &direction,
	float length, const deoalRayTraceResult &result, int elementCount) :
pOrigin(origin),
pDirection(direction),
pLength(length),
pHits(NULL),
pHitCount(0),
pOctreeNode(NULL),
pLLPrev(NULL),
pLLNext(NULL)
{
	if(elementCount == 0){
		return;
	}
	
	pHits = new deoalRayCacheRayHit[elementCount];
	
	for(pHitCount=0; pHitCount<elementCount; pHitCount++){
		const deoalRayTraceHitElement &element = result.GetElementAt(pHitCount);
		deoalRayCacheRayHit &hit = pHits[pHitCount];
		
		hit.SetDistance(element.GetDistance());
		hit.SetFaceIndex(element.GetComponentFace());
		hit.SetForwardFacing(element.GetForwardFacing());
		hit.SetPoint(element.GetPoint());
		hit.SetNormal(element.GetNormal());
	}
}

deoalRayCacheRay::~deoalRayCacheRay(){
	if(pHits){
		delete [] pHits;
	}
}



// Management
///////////////

const deoalRayCacheRayHit &deoalRayCacheRay::GetHitAt(int index) const{
	if(index < 0 || index >= pHitCount){
		DETHROW(deeInvalidParam);
	}
	return pHits[index];
}



void deoalRayCacheRay::SetOctreeNode(deoalRayCacheOctree *node){
	pOctreeNode = node;
}

void deoalRayCacheRay::SetLLPrev(deoalRayCacheRay *ray){
	pLLPrev = ray;
}

void deoalRayCacheRay::SetLLNext(deoalRayCacheRay *ray) {
	pLLNext = ray;
}
