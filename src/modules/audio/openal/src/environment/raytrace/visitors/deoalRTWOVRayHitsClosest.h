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

#ifndef _DEOALRTWOVRAYHITSCLOSEST_H_
#define _DEOALRTWOVRAYHITSCLOSEST_H_

#include <dragengine/common/math/decMath.h>

#include "../deoalRayTraceHitElement.h"
#include "../../../world/octree/deoalRTWorldBVHVisitor.h"



// #define RTWOVRAYHITSCLOSEST_DO_TIMING 1

#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
#include <dragengine/common/utils/decTimer.h>
#endif


/**
 * \brief Ray tracing world octree visitor closest hit elements.
 * 
 * Visits octree with box enclosing ray. Closest element hit by ray is stored.
 * 
 * \note Thread safe.
 */
class deoalRTWOVRayHitsClosest : public deoalRTWorldBVHVisitor{
private:
	deoalRayTraceHitElement pResult;
	bool pHasResult;
	bool pFrontFacing;
	float pLimitDistance;
	
#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
public:
	decTimer timerAll;
	decTimer timerComponent;
	float timingAll;
	int timingAllCount;
	float timingComponent;
	int timingComponentCount;
	float timingComponentOctree;
	int timingComponentOctreeCount;
	float timingFaces;
	int timingFacesCount;
	void StartTiming();
	void EndTiming();
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalRTWOVRayHitsClosest();
	
	/** \brief Clean up visitor. */
	~deoalRTWOVRayHitsClosest() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Result. */
	inline deoalRayTraceHitElement &GetResult(){ return pResult; }
	inline const deoalRayTraceHitElement &GetResult() const{ return pResult; }
	
	/** \brief Result is set. */
	inline bool GetHasResult() const{ return pHasResult; }
	
	/** \brief Set if result is set. */
	void SetHasResult(bool hasResult);
	
	/** \brief Find front facing hits otherwise back facing hits. */
	inline bool GetFrontFacing() const{ return pFrontFacing; }
	
	/** \brief Set to find front facing hits or back facing hits. */
	void SetFrontFacing(bool frontFacing);
	
	/** \brief Limit distance. */
	float GetLimitDistance() const{ return pLimitDistance; }
	
	/** \brief Set limit distance. */
	void SetLimitDistance(float limitDistance);
	
	
	
	/** \brief Visit optimized ray-trace BVH node. */
	void VisitBVH(const deoalRTWorldBVH &bvh);
	
	/** \brief Visit component. */
	void VisitComponent(const deoalRTWorldBVH::sVisitComponent &rtcomponent) override;
	/*@}*/
	
	
	
protected:
	void pVisitNode(const deoalRTWorldBVH &bvh, const deoalRTWorldBVH::sVisitNode &node);
};

#endif
