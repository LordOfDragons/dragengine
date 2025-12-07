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

#ifndef _DEOALWOVRAYHITSELEMENT_H_
#define _DEOALWOVRAYHITSELEMENT_H_

#include <dragengine/common/math/decMath.h>

#include "../deoalRayTraceResult.h"
#include "../../../world/octree/deoalWOVRayTrace.h"



// #define WOVRAYHITSELEMENT_DO_TIMING 1

#ifdef WOVRAYHITSELEMENT_DO_TIMING
#include <dragengine/common/utils/decTimer.h>
#endif


/**
 * \brief World octree visitor collecting elements hit by ray sorted by distance.
 * 
 * Visits octree with box enclosing ray. All elements hit by the ray are stored in a list
 * sorted by the hit distance.
 * 
 * \note Thread safe if the assigned deoalRayTraceResult instance is thread safe.
 */
class deoalWOVRayHitsElement : public deoalWOVRayTrace{
private:
	deoalRayTraceResult *pResult;
	deoalRayTraceResult pComponentResult;
	float pThresholdTransmit;
	
#ifdef WOVRAYHITSELEMENT_DO_TIMING
public:
	decTimer timerAll;
	decTimer timerComponent;
	float timingAll;
	float timingComponent;
	int timingComponentCount;
	float timingComponentOctree;
	int timingComponentOctreeCount;
	float timingFaces;
	int timingFacesCount;
	int rayAddCount;
	int rayFoundCount;
	void StartTiming();
	void EndTiming();
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVRayHitsElement();
	
	/** \brief Clean up visitor. */
	~deoalWOVRayHitsElement() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Result or NULL if not set. */
	inline deoalRayTraceResult *GetResult() const{ return pResult; }
	
	/** \brief Set result or NULL if not set. */
	void SetResult(deoalRayTraceResult *result);
	
	/** \brief Component result. */
	inline deoalRayTraceResult &GetComponentResult(){ return pComponentResult; }
	inline const deoalRayTraceResult &GetComponentResult() const{ return pComponentResult; }
	
	
	
	/** \brief Visit component. */
	void VisitComponent(deoalAComponent *component) override;
	/*@}*/
	
	
	
protected:
	decVector pClampToBox(const decVector &minExtend, const decVector &maxExtend,
		const decVector &rayOrigin, const decVector &rayDirection);
};

#endif
