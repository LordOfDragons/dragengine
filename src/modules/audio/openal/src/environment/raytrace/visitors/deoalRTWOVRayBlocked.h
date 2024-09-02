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

#ifndef _DEOALRTWOVRAYBLOCKED_H_
#define _DEOALRTWOVRAYBLOCKED_H_

#include <dragengine/common/math/decMath.h>

#include "../../../world/octree/deoalRTWorldBVHVisitor.h"



// #define RTWOVRAYBLOCKED_DO_TIMING 1

#ifdef RTWOVRAYBLOCKED_DO_TIMING
#include <dragengine/common/utils/decTimer.h>
#endif



/**
 * \brief Ray tracing world octree visitor checking if ray is blocked.
 * 
 * Similar to deoalWOVRayHitsElement but stops at the first hit and does not store hit parameters.
 */
class deoalRTWOVRayBlocked : public deoalRTWorldBVHVisitor{
private:
	bool pBlocked;
	
#ifdef RTWOVRAYBLOCKED_DO_TIMING
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
	deoalRTWOVRayBlocked();
	
	/** \brief Clean up visitor. */
	virtual ~deoalRTWOVRayBlocked();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray is blocked. */
	inline bool GetBlocked() const{ return pBlocked; }
	
	/** \brief Set if ray is blocked. */
	void SetBlocked( bool blocked );
	
	
	
	/** \brief Visit optimized ray-trace bvh. */
	void VisitBVH( const deoalRTWorldBVH &bvh );
	
	/** \brief Visit optimized ray-trace bvh node. */
	void VisitNode( const deoalRTWorldBVH &bvh, const deoalRTWorldBVH::sVisitNode &node );
	
	/** \brief Visit component. */
	void VisitComponent( const deoalRTWorldBVH::sVisitComponent &rtcomponent );
	/*@}*/
};

#endif
