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

#include "deoalRayCache.h"
#include "deoalRayCacheRay.h"
#include "deoalRayCacheRayHit.h"
#include "deoalRayCacheOctree.h"
#include "../octree/deoalOctreeVisitor.h"
#include "../../audiothread/deoalAudioThread.h"
#include "../../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Find ray octree visitor
////////////////////////////

class deoalRayCacheFindRayVisitor : public deoalOctreeVisitor{
private:
	const decVector &pOrigin;
	const decVector pDirection;
	const float pRangeSquared;
	const float pSpreadDot;
	const decVector pRangeExtend;
	deoalRayCacheRay *pFoundRay;
	
	
	
public:
	deoalRayCacheFindRayVisitor(const decVector &origin, const decVector &direction,
		float range, float spreadDot) :
	pOrigin(origin),
	pDirection(direction.Normalized()),
	pRangeSquared(range * range),
	pSpreadDot(spreadDot),
	pRangeExtend(range, range, range),
	pFoundRay(NULL){
	}
	
	~deoalRayCacheFindRayVisitor() override{
	}
	
	inline deoalRayCacheRay *FoundRay() const{ return pFoundRay; }
	
	void Visit(deoalRayCacheOctree &node){
		if(!((pOrigin - node.GetCenter()).Absolute() - pRangeExtend <= node.GetHalfSize())){
			return;
		}
		
		const int count = node.GetRayCount();
		int i;
		for(i=0; i<count; i++){
			deoalRayCacheRay * const ray = node.GetRayAt(i);
			if((ray->GetOrigin() - pOrigin).LengthSquared() > pRangeSquared){
				continue;
			}
			
			if(pDirection * ray->GetDirection() < pSpreadDot){
				continue;
			}
			
			pFoundRay = ray;
			return;
		}
		
		for(i=0; i<8; i++){
			deoalRayCacheOctree * const child = (deoalRayCacheOctree*)node.GetNodeAt(i);
			if(child){
				Visit(*child);
				if(pFoundRay){
					return;
				}
			}
		}
	}
};



// Stats ray octree visitor
/////////////////////////////

class deoalRayCacheStatsVisitor : public deoalOctreeVisitor{
private:
	int pNodeCount;
	int pMaxNodeRays;
	int pLeafNodeCount;
	int pAvgNodeRays;
	
	
	
public:
	deoalRayCacheStatsVisitor() :
	pNodeCount(0),
	pMaxNodeRays(0),
	pLeafNodeCount(0),
	pAvgNodeRays(0){
	}
	
	inline int GetNodeCount() const{ return pNodeCount; }
	inline int GetMaxNodeRays() const{ return pMaxNodeRays; }
	inline int GetLeafNodeCount() const{ return pLeafNodeCount; }
	inline int GetAvgNodeRays() const{ return pAvgNodeRays; }
	
	void Visit(deoalRayCacheOctree &node){
		pNodeCount++;
		const int count = node.GetRayCount();
		if(count > pMaxNodeRays){
			pMaxNodeRays = count;
		}
		pAvgNodeRays += count;
		
		bool hasChildNodes = false;
		int i;
		for(i=0; i<8; i++){
			deoalRayCacheOctree * const child = (deoalRayCacheOctree*)node.GetNodeAt(i);
			if(child){
				hasChildNodes = true;
				Visit(*child);
			}
		}
		if(!hasChildNodes){
			pLeafNodeCount++;
		}
	}
};



// Class deoalRayCache
////////////////////////

// Constructor, destructor
////////////////////////////

deoalRayCache::deoalRayCache(const decVector &minExtend, const decVector &maxExtend) :
pReadCount(0),
pRange(0.1f),
pSpread(0.5f),
pSpreadDot(cosf(DEG2RAD * pSpread)),
pOctreeDepth(4),
pRootRay(NULL),
pTailRay(NULL),
pRayCount(0),
pOctree(NULL)
{
	// NOTE
	// how to define the spread? possible solution how much deviation distance at a certain
	// distance. for example 0.1m difference on 10m is atan(0.1/10.0) ~ 0.57 degrees.
	// using right now 0.5 degrees as spread
	
	pOctree = new deoalRayCacheOctree((minExtend + maxExtend) * 0.5f,
		(maxExtend - minExtend) * 0.5f);
	
/*
// no ray cache
minExt=(7.967,-0.000,3.900) maxExt=(42.000,4.500,22.800) time=73.85ms     [[microphone]]
minExt=(-13.000,-0.000,-8.000) maxExt=(44.103,2.500,2.900) time=26.95ms   [simple]
minExt=(2.086,-0.000,-8.000) maxExt=(37.914,2.500,1.900) time=26.62ms     [openings]
minExt=(39.100,-0.000,-4.700) maxExt=(40.800,2.500,-3.050) time=45.83ms   [thickness]
minExt=(19.730,-0.000,0.100) maxExt=(52.800,2.500,10.800) time=23.72ms    [small reverberant]
minExt=(13.000,-0.000,3.900) maxExt=(27.000,4.500,22.800) time=44.46ms    [large reverberant]

WOVRHE: all=72.550 comp=0.966(7467) compOctree=55.584(7467) face=0.000(0) rayAdd=0 rayFound=0
WOVRHE: all=26.161 comp=0.567(4595) compOctree=11.716(4595) face=0.000(0) rayAdd=0 rayFound=0
WOVRHE: all=25.854 comp=0.631(5122) compOctree=13.386(5122) face=0.000(0) rayAdd=0 rayFound=0
WOVRHE: all=44.390 comp=1.628(12398) compOctree=26.545(12398) face=0.000(0) rayAdd=0 rayFound=0
WOVRHE: all=23.294 comp=0.436(3504) compOctree=9.565(3504) face=0.000(0) rayAdd=0 rayFound=0
WOVRHE: all=43.977 comp=0.583(4363) compOctree=35.045(4363) face=0.000(0) rayAdd=0 rayFound=0

// ray cache with locking
WOVRHE: all=92.000 comp=10.864(7436) compOctree=53.249(7340) face=0.000(0) rayAdd=7340 rayFound=96
WOVRHE: all=41.218 comp=7.613(4638) compOctree=11.159(4509) face=0.000(0) rayAdd=4509 rayFound=129
WOVRHE: all=39.690 comp=7.648(5186) compOctree=9.176(4221) face=0.000(0) rayAdd=4221 rayFound=965
WOVRHE: all=82.880 comp=20.033(12397) compOctree=17.725(8143) face=0.000(0) rayAdd=8143 rayFound=4254
WOVRHE: all=37.722 comp=7.781(3528) compOctree=8.305(3320) face=0.000(0) rayAdd=3320 rayFound=208
WOVRHE: all=57.504 comp=8.487(4364) compOctree=31.176(4085) face=0.000(0) rayAdd=4085 rayFound=279

minExt=(7.967,-0.000,3.900) maxExt=(42.000,4.500,22.800) time=91.96ms
minExt=(-13.000,-0.000,-8.000) maxExt=(44.103,2.500,2.900) time=41.19ms
minExt=(2.086,-0.000,-8.000) maxExt=(37.914,2.500,1.900) time=39.64ms
minExt=(39.100,-0.000,-4.700) maxExt=(40.800,2.500,-3.050) time=82.84ms
minExt=(19.730,-0.000,0.100) maxExt=(52.800,2.500,10.800) time=37.69ms
minExt=(13.000,-0.000,3.900) maxExt=(27.000,4.500,22.800) time=57.46ms

// without locking => single thread only
WOVRHE: all=89.919 comp=9.604(7436) compOctree=52.070(7340) face=0.000(0) rayAdd=7340 rayFound=96
WOVRHE: all=40.688 comp=6.898(4638) compOctree=11.106(4509) face=0.000(0) rayAdd=4509 rayFound=129
WOVRHE: all=38.759 comp=6.962(5186) compOctree=9.006(4221) face=0.000(0) rayAdd=4221 rayFound=965
WOVRHE: all=80.370 comp=18.378(12397) compOctree=17.642(8143) face=0.000(0) rayAdd=8143 rayFound=4254
WOVRHE: all=37.170 comp=7.090(3528) compOctree=8.244(3320) face=0.000(0) rayAdd=3320 rayFound=208
WOVRHE: all=56.228 comp=7.808(4364) compOctree=30.586(4085) face=0.000(0) rayAdd=4085 rayFound=279

minExt=(7.967,-0.000,3.900) maxExt=(42.000,4.500,22.800) time=89.88ms
minExt=(-13.000,-0.000,-8.000) maxExt=(44.103,2.500,2.900) time=40.65ms
minExt=(2.086,-0.000,-8.000) maxExt=(37.914,2.500,1.900) time=38.72ms
minExt=(39.100,-0.000,-4.700) maxExt=(40.800,2.500,-3.050) time=80.33ms
minExt=(19.730,-0.000,0.100) maxExt=(52.800,2.500,10.800) time=37.13ms
minExt=(13.000,-0.000,3.900) maxExt=(27.000,4.500,22.800) time=56.19ms

/model/room/room1.demodel       (rays=33,   nodes=77,   maxRays=2,   avgRays=1)
/model/prop/speaker1.demodel    (rays=18,   nodes=56,   maxRays=2,   avgRays=1)
/model/prop/door1.demodel       (rays=24,   nodes=71,   maxRays=1,   avgRays=1)
/model/hallway/straight.demodel (rays=352,  nodes=455,  maxRays=7,   avgRays=1)
/model/prop/roomLabel.demodel   (rays=2,    nodes=9,    maxRays=1,   avgRays=1)
/model/hallway/end.demodel      (rays=4,    nodes=13,   maxRays=1,   avgRays=1)
/model/hallway/corner.demodel   (rays=4,    nodes=16,   maxRays=1,   avgRays=1)
/model/prop/blinds1.demodel     (rays=2,    nodes=9,    maxRays=1,   avgRays=1)
/model/prop/blinds2.demodel     (rays=3,    nodes=10,   maxRays=1,   avgRays=1)
/model/room/room2.demodel       (rays=15,   nodes=41,   maxRays=2,   avgRays=1)
/model/room/room3.demodel       (rays=2436, nodes=1576, maxRays=162, avgRays=2)
/model/prop/pillar1.demodel     (rays=3360, nodes=1865, maxRays=11,  avgRays=2)
/model/prop/entrance.demodel    (rays=317,  nodes=525,  maxRays=4,   avgRays=1)
/model/prop/railing2.demodel    (rays=86,   nodes=194,  maxRays=2,   avgRays=1)
/model/prop/railing1.demodel    (rays=635,  nodes=812,  maxRays=11,  avgRays=1)
/model/prop/pole.demodel        (rays=7,    nodes=25,   maxRays=1,   avgRays=1)
/model/prop/desk.demodel        (rays=0,    nodes=1,    maxRays=0,   avgRays=0)
/model/prop/chair.demodel       (rays=0,    nodes=1,    maxRays=0,   avgRays=0)
*/
}

deoalRayCache::~deoalRayCache(){
	if(pOctree){
		delete pOctree;
	}
	
	while(pTailRay){
		deoalRayCacheRay * const next = pTailRay;
		pTailRay = pTailRay->GetLLPrev();
		delete next;
	}
}



// Management
///////////////

deoalRayCacheRay *deoalRayCache::FindRay(const decVector &origin, const decVector &direction){
	pLockRead();
	deoalRayCacheRay *ray = NULL;
	
	try{
		ray = pFindRay(origin, direction);
		
	}catch(const deException &){
		pUnlockRead();
		throw;
	}
	
	pUnlockRead();
	return ray;
}

void deoalRayCache::CacheRay(const decVector &origin, const decVector &direction,
float length, const deoalRayTraceHitElementList &elements){
	deMutexGuard lock(pMutexWrite);
	pCacheRay(new deoalRayCacheRay(origin, direction.Normalized(), length, elements));
}

void deoalRayCache::CacheRay(const decVector &origin, const decVector &direction,
float length, const deoalRayTraceResult &result){
	deMutexGuard lock(pMutexWrite);
	pCacheRay(new deoalRayCacheRay(origin, direction.Normalized(), length, result));
}

void deoalRayCache::CacheRay(const decVector &origin, const decVector &direction,
float length, const deoalRayTraceResult &result, int elementCount){
	deMutexGuard lock(pMutexWrite);
	pCacheRay(new deoalRayCacheRay(origin, direction.Normalized(), length, result, elementCount));
}



void deoalRayCache::DebugPrint(deoalAudioThread &audioThread, const char *source){
	deoalATLogger &logger = audioThread.GetLogger();
	
	deoalRayCacheStatsVisitor stats;
	stats.Visit(*pOctree);
	
	//logger.LogInfoFormat( "RayCache: %s (%d rays)", source, pRayCount );
	logger.LogInfoFormat("RayCache: %s (rays=%d, nodes=%d, leaves=%d maxRays=%d, avgRays=%d)",
		source, pRayCount, stats.GetNodeCount(), stats.GetLeafNodeCount(), stats.GetMaxNodeRays(),
		stats.GetAvgNodeRays() / decMath::max(stats.GetLeafNodeCount(), 1));
	
#if 0
	deoalRayCacheRay *ray = pRootRay;
	int index = 0;
	
	while(ray){
		const int count2 = ray->GetHitCount();
		int j;
		logger.LogInfoFormat("- Ray %d: origin=(%.3f,%.3f,%.3f) direction=(%.3f,%.3f,%.3f) length=%.3f hits=%d",
			index, ray->GetOrigin().x, ray->GetOrigin().y, ray->GetOrigin().z, ray->GetDirection().x,
			ray->GetDirection().y, ray->GetDirection().z, ray->GetLength(), count2);
		for(j=0; j<count2; j++){
			const deoalRayCacheRayHit &hit = ray->GetHitAt(j);
			logger.LogInfoFormat("  - Hit %d: position=(%.3f,%.3f,%.3f) normal=(%.3f,%.3f,%.3f)"
			" distance=%.3f face=%d forward=%d", j, hit.GetPoint().x, hit.GetPoint().y,
			hit.GetPoint().z, hit.GetNormal().x, hit.GetNormal().y, hit.GetNormal().z,
			hit.GetDistance(), hit.GetFaceIndex(), hit.GetForwardFacing());
		}
		index++;
		ray = ray->GetLLNext();
	}
#endif
}



// Private Functions
//////////////////////

void deoalRayCache::pLockRead(){
	deMutexGuard lock(pMutexRead);
	pReadCount++;
	
	if(pReadCount == 1){
		// first call read-using the deVirtualFileSystem. lock the mutex to prevent all
		// calls modifying the deVirtualFileSystem to modify while in use
		pMutexWrite.Lock();
	}
}

void deoalRayCache::pUnlockRead(){
	deMutexGuard lock(pMutexRead);
	pReadCount--;
	
	if(pReadCount == 0){
		// last call read-using the deVirtualFileSystem. unlock the mutex allowing all
		// calls modifying the deVirtualFileSystem to continue
		pMutexWrite.Unlock();
	}
}

deoalRayCacheRay *deoalRayCache::pFindRay(const decVector &origin, const decVector &direction) const{
	deoalRayCacheFindRayVisitor visitor(origin, direction, pRange, pSpreadDot);
	visitor.Visit(*pOctree);
	return visitor.FoundRay();
}

void deoalRayCache::pCacheRay(deoalRayCacheRay *ray){
	// this method has to do multiple tasks to deal with potentially multiple callers trying
	// to add/replace the same cached ray. we want to keep the longest ray even if another
	// shorter ray happens to come by right after the longer one
	deoalRayCacheRay * const cachedRay = pFindRay(ray->GetOrigin(), ray->GetDirection());
	
	if(cachedRay){
		// if the cached ray is longer delete the new ray
		if(cachedRay->GetLength() > ray->GetLength()){
			delete ray;
			return;
		}
		
		// remove node
		cachedRay->GetOctreeNode()->RemoveRay(cachedRay);
		
		if(cachedRay->GetLLPrev()){
			cachedRay->GetLLPrev()->SetLLNext(cachedRay->GetLLNext());
		}
		if(cachedRay->GetLLNext()){
			cachedRay->GetLLNext()->SetLLPrev(cachedRay->GetLLPrev());
		}
		if(pRootRay == cachedRay){
			pRootRay = cachedRay->GetLLNext();
		}
		if(pTailRay == cachedRay){
			pTailRay = cachedRay->GetLLPrev();
		}
		pRayCount--;
		delete cachedRay;
	}
	
	// insert into octree
	try{
		pOctree->InsertRayIntoTree(ray, pOctreeDepth);
		
	}catch(const deException &){
		delete ray;
		throw;
	}
	
	// add ray
	if(pTailRay){
		pTailRay->SetLLNext(ray);
		ray->SetLLPrev(pTailRay);
		
	}else if(!pRootRay){
		pRootRay = ray;
	}
	pTailRay = ray;
	pRayCount++;
}
