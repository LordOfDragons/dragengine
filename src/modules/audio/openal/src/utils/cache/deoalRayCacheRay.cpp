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

#include "deoalRayCacheRay.h"
#include "deoalRayCacheRayHit.h"
#include "../../environment/raytrace/deoalRayTraceResult.h"
#include "../../environment/raytrace/deoalRayTraceHitElement.h"
#include "../../component/deoalAComponent.h"
#include "../../model/deoalAModel.h"
#include "../../model/deoalModelFace.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayCacheRay
////////////////////////

// Constructor, destructor
////////////////////////////

deoalRayCacheRay::deoalRayCacheRay(const decVector &origin, const decVector &direction,
	float length, const deoalRayTraceHitElement::List &elements) :
pOrigin(origin),
pDirection(direction),
pLength(length),
pOctreeNode(nullptr),
pLLRays(this)
{
	const int elementCount = elements.GetCount();
	if(elementCount == 0){
		return;
	}
	
	pHits.AddRange(elementCount, {});
	
	pHits.VisitIndexed([&](int i, deoalRayCacheRayHit &hit){
		const deoalRayTraceHitElement &element = elements.GetAt(i);
		hit.SetDistance(element.GetDistance());
		hit.SetFaceIndex(element.GetComponentFace());
		hit.SetForwardFacing(element.GetForwardFacing());
		hit.SetPoint(element.GetPoint());
		hit.SetNormal(element.GetNormal());
	});
}

deoalRayCacheRay::deoalRayCacheRay(const decVector &origin, const decVector &direction,
float length, const deoalRayTraceResult &result) :
pOrigin(origin),
pDirection(direction),
pLength(length),
pOctreeNode(nullptr),
pLLRays(this)
{
	const int count = result.GetElements().GetCount();
	if(count == 0){
		return;
	}
	
	pHits.AddRange(count, {});
	pHits.VisitIndexed([&](int i, deoalRayCacheRayHit &hit){
		const deoalRayTraceHitElement &element = result.GetElements()[i];
		hit.SetDistance(element.GetDistance());
		hit.SetFaceIndex(element.GetComponentFace());
		hit.SetForwardFacing(element.GetForwardFacing());
		hit.SetPoint(element.GetPoint());
		hit.SetNormal(element.GetNormal());
	});
}

deoalRayCacheRay::deoalRayCacheRay(const decVector &origin, const decVector &direction,
	float length, const deoalRayTraceResult &result, int elementCount) :
pOrigin(origin),
pDirection(direction),
pLength(length),
pOctreeNode(nullptr),
pLLRays(this)
{
	if(elementCount == 0){
		return;
	}
	
	pHits.AddRange(elementCount, {});
	pHits.VisitIndexed([&](int i, deoalRayCacheRayHit &hit){
		const deoalRayTraceHitElement &element = result.GetElements()[i];
		hit.SetDistance(element.GetDistance());
		hit.SetFaceIndex(element.GetComponentFace());
		hit.SetForwardFacing(element.GetForwardFacing());
		hit.SetPoint(element.GetPoint());
		hit.SetNormal(element.GetNormal());
	});
}

deoalRayCacheRay::~deoalRayCacheRay() = default;



// Management
///////////////

const deoalRayCacheRayHit &deoalRayCacheRay::GetHitAt(int index) const{
	return pHits[index];
}



void deoalRayCacheRay::SetOctreeNode(deoalRayCacheOctree *node){
	pOctreeNode = node;
}
