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

#ifndef _DEOALMOVRAYBLOCKED_H_
#define _DEOALMOVRAYBLOCKED_H_

#include <dragengine/common/math/decMath.h>

#include "../../../model/octree/deoalModelOctreeVisitor.h"
#include "../../../model/octree/deoalModelRTOctree.h"
#include "../../../model/octree/deoalModelRTBVH.h"

class deoalAComponent;
class deoalAModel;
class deoalModelFace;


/**
 * \brief Model octree visitor detecting blocked ray.
 * 
 * Similar to deoalMOVRayHitsFaces but stops visiting at the first found hit and does not
 * store hit parameters.
 */
class deoalMOVRayBlocked : public deoalModelOctreeVisitor{
private:
	deoalAComponent &pComponent;
	decVector pRayOrigin;
	decVector pRayDirection;
	decVector pRayTarget;
	
	decDVector pInvRayDirection;
	bool pCheckAxisX;
	bool pCheckAxisY;
	bool pCheckAxisZ;
	
	float pRayLength;
	decVector pRayBoxMin;
	decVector pRayBoxMax;
	
	bool pBlocked;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalMOVRayBlocked( deoalAComponent &component );
	
	/** \brief Clean up visitor. */
	virtual ~deoalMOVRayBlocked();
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
	
	
	
	/** \brief Ray is blocked. */
	inline bool GetBlocked() const{ return pBlocked; }
	
	/** \brief Set if ray is blocked. */
	void SetBlocked( bool blocked );
	
	
	
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
