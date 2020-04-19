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

#ifndef _DEOALWOVRAYTRACE_H_
#define _DEOALWOVRAYTRACE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

#include "deoalWorldOctreeVisitor.h"

class deoalAComponent;



/**
 * \brief World octree visitor supporting ray tracing.
 * 
 * Extends basic visitor with support to store ray tracing properties. Visited components are
 * checked for intersection with ray before being send to the respective visit function.
 * 
 * Only components affecting sound are ray-checked and visited. To check also other elements
 * overwrite VisitNode and implement visiting other elements if required.
 */
class deoalWOVRayTrace : public deoalWorldOctreeVisitor{
private:
	decDVector pRayOrigin;
	decDVector pRayDirection;
	decDVector pRayTarget;
	decLayerMask pLayerMask;
	
	decDVector pInvRayDirection;
	bool pCheckAxisX;
	bool pCheckAxisY;
	bool pCheckAxisZ;
	
	double pRayLength;
	decDVector pRayBoxMin;
	decDVector pRayBoxMax;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVRayTrace();
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVRayTrace();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray origin. */
	inline const decDVector &GetRayOrigin() const{ return pRayOrigin; }
	
	/** \brief Ray direction. */
	inline const decDVector &GetRayDirection() const{ return pRayDirection; }
	
	/** \brief Ray direction has X axis component not equal to 0. */
	inline bool GetCheckAxisX() const{ return pCheckAxisX; }
	
	/** \brief Ray direction has Y axis component not equal to 0. */
	inline bool GetCheckAxisY() const{ return pCheckAxisY; }
	
	/** \brief Ray direction has Z axis component not equal to 0. */
	inline bool GetCheckAxisZ() const{ return pCheckAxisZ; }
	
	/**
	 * \brief Inverted ray direction.
	 * 
	 * Returns vector 1/rayDirection for all components.
	 */
	inline const decDVector &GetInvRayDirection() const{ return pInvRayDirection; }
	
	/** \brief Ray length. */
	inline double GetRayLength() const{ return pRayLength; }
	
	/** \brief Ray box minimum extend. */
	inline const decDVector &GetRayBoxMin() const{ return pRayBoxMin; }
	
	/** \brief Ray box maximum extend. */
	inline const decDVector &GetRayBoxMax() const{ return pRayBoxMax; }
	
	/** \brief Set test ray. */
	void SetRay( const decDVector &origin, const decDVector &direction );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
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
	virtual void VisitNode( deoalWorldOctree &node );
	
	/** \brief Visits components affecting sound hit by ray. */
	virtual void VisitNode( deoalDOctree *node, int intersection );
	/*@}*/
	
	
	
protected:
	bool pRayHitsBox( const decDVector &center, const decDVector &halfExtends );
};

#endif
