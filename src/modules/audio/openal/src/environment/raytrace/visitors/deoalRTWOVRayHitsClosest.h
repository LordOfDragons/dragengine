/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	virtual ~deoalRTWOVRayHitsClosest();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Result. */
	inline deoalRayTraceHitElement &GetResult(){ return pResult; }
	inline const deoalRayTraceHitElement &GetResult() const{ return pResult; }
	
	/** \brief Result is set. */
	inline bool GetHasResult() const{ return pHasResult; }
	
	/** \brief Set if result is set. */
	void SetHasResult( bool hasResult );
	
	/** \brief Find front facing hits otherwise back facing hits. */
	inline bool GetFrontFacing() const{ return pFrontFacing; }
	
	/** \brief Set to find front facing hits or back facing hits. */
	void SetFrontFacing( bool frontFacing );
	
	/** \brief Limit distance. */
	float GetLimitDistance() const{ return pLimitDistance; }
	
	/** \brief Set limit distance. */
	void SetLimitDistance( float limitDistance );
	
	
	
	/** \brief Visit optimized ray-trace BVH node. */
	void VisitBVH( const deoalRTWorldBVH &bvh );
	
	/** \brief Visit component. */
	virtual void VisitComponent( const deoalRTWorldBVH::sVisitComponent &rtcomponent );
	/*@}*/
	
	
	
protected:
	void pVisitNode( const deoalRTWorldBVH &bvh, const deoalRTWorldBVH::sVisitNode &node );
};

#endif
