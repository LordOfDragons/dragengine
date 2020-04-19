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
