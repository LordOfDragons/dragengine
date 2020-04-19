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

#ifndef _DEOALRTWOVRAYHITSELEMENT_H_
#define _DEOALRTWOVRAYHITSELEMENT_H_

#include <dragengine/common/math/decMath.h>

#include "../deoalRayTraceResult.h"
#include "../../../world/octree/deoalRTWorldBVHVisitor.h"



// #define RTWOVRAYHITSELEMENT_DO_TIMING 1

#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
#include <dragengine/common/utils/decTimer.h>
#endif


/**
 * \brief Ray tracing world octree visitor collecting elements hit by ray sorted by distance.
 * 
 * Visits octree with box enclosing ray. All elements hit by the ray are stored in a list
 * sorted by the hit distance.
 * 
 * \note Thread safe if the assigned deoalRayTraceResult instance is thread safe.
 */
class deoalRTWOVRayHitsElement : public deoalRTWorldBVHVisitor{
private:
	deoalRayTraceResult *pResult;
	deoalRayTraceResult pComponentResult;
	float pThresholdTransmit;
	
#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
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
	void VisitBVH( const deoalRTWorldBVH &bvh );
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalRTWOVRayHitsElement();
	
	/** \brief Clean up visitor. */
	virtual ~deoalRTWOVRayHitsElement();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Result or NULL if not set. */
	inline deoalRayTraceResult *GetResult() const{ return pResult; }
	
	/** \brief Set result or NULL if not set. */
	void SetResult( deoalRayTraceResult *result );
	
	/** \brief Component result. */
	inline deoalRayTraceResult &GetComponentResult(){ return pComponentResult; }
	inline const deoalRayTraceResult &GetComponentResult() const{ return pComponentResult; }
	
	
	
	/** \brief Visit component. */
	virtual void VisitComponent( const deoalRTWorldBVH::sVisitComponent &rtcomponent );
	/*@}*/
	
	
	
protected:
	void pVisitNode( const deoalRTWorldBVH &bvh, const deoalRTWorldBVH::sVisitNode &node );
};

#endif
