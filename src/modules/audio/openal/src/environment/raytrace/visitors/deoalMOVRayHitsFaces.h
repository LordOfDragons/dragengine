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

#ifndef _DEOALMOVRAYHITSFACES_H_
#define _DEOALMOVRAYHITSFACES_H_

#include <dragengine/common/math/decMath.h>

#include "../../../model/octree/deoalModelOctreeVisitor.h"
#include "../../../model/octree/deoalModelRTOctree.h"
#include "../../../model/octree/deoalModelRTBVH.h"

class deoalAComponent;
class deoalAModel;
class deoalModelFace;
class deoalRayTraceResult;


// #define MOVRAYHITSFACES_DO_TIMIING 1

#ifdef MOVRAYHITSFACES_DO_TIMIING
#include <dragengine/common/utils/decTimer.h>
#endif


/**
 * \brief Model octree visitor collecting faces hit by ray sorted by distance.
 * 
 * Visits octree with box enclosing ray. All faces hit by the ray are stored in a list
 * sorted by the hit distance.
 */
class deoalMOVRayHitsFaces : public deoalModelOctreeVisitor{
private:
	deoalAComponent &pComponent;
	const deoalAModel &pModel;
	decVector pRayOrigin;
	decVector pRayDirection;
	decVector pRayTarget;
	deoalRayTraceResult *pResult;
	bool pResultInWorldSpace;
	
	decVector pInvRayDirection;
	bool pCheckAxisX;
	bool pCheckAxisY;
	bool pCheckAxisZ;
	
	float pRayLength;
	decVector pRayBoxMin;
	decVector pRayBoxMax;
	
#ifdef MOVRAYHITSFACES_DO_TIMIING
public:
	decTimer timer;
	float timing;
	int timingCount;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalMOVRayHitsFaces( deoalAComponent &component, const deoalAModel &model );
	
	/** \brief Clean up visitor. */
	virtual ~deoalMOVRayHitsFaces();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray origin. */
	inline const decVector &GetRayOrigin() const{ return pRayOrigin; }
	
	/** \brief Ray direction. */
	inline const decVector &GetRayDirection() const{ return pRayDirection; }
	
	/** \brief Ray length. */
	inline float GetRayLength() const{ return pRayLength; }
	
	/** \brief Ray box minimum extend. */
	inline const decVector &GetRayBoxMin() const{ return pRayBoxMin; }
	
	/** \brief Ray box maximum extend. */
	inline const decVector &GetRayBoxMax() const{ return pRayBoxMax; }
	
	/** \brief Set test ray. */
	void SetRay( const decVector &origin, const decVector &direction );
	
	
	
	/** \brief Result or NULL if not set. */
	inline deoalRayTraceResult *GetResult() const{ return pResult; }
	
	/** \brief Set result or NULL if not set. */
	void SetResult( deoalRayTraceResult *result );
	
	/** \brief Transform result into world space. */
	inline bool GetResultInWorldSpace() const{ return pResultInWorldSpace; }
	
	/** \brief Set to transform result into world space. */
	void SetResultInWorldSpace( bool resultInWorldSpace );
	
	
	
	/**
	 * \brief Visit octree node.
	 * 
	 * Visitor is responsible to check if the node and children thereof have to be
	 * visited. This allows to apply special checks not handled by deoalDOctree.
	 * 
	 * To visit world call VisitNode(*world.GetOctree()).
	 * 
	 * The default implementation calls VisitNode(deoalDOctree*,int) then visits child nodes.
	 */
	virtual void VisitNode( deoalModelOctree &node );
	
	/** \brief Visit octree node. */
	virtual void VisitNode( deoalOctree *node, int intersection );
	
	/** \brief Visit optimized ray-trace octree node. */
	void VisitOctree( const deoalModelRTOctree &octree );
	
	/** \brief Visit optimized ray-trace BVH node. */
	void VisitBVH( const deoalModelRTBVH &bvh );
	/*@}*/
	
	
	
protected:
	void pVisitNode( const deoalModelRTOctree &octree, const deoalModelRTOctree::sNode &node );
	void pVisitNode( const deoalModelRTBVH &bvh, const deoalModelRTBVH::sNode &node );
	bool pRayHitsBox( const decVector &center, const decVector &halfExtends );
};

#endif
