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

#ifndef _DEOALRTWORLDOCTREEVISITOR_H_
#define _DEOALRTWORLDOCTREEVISITOR_H_

#include "deoalRTWorldOctree.h"



/**
 * \brief Ray trace world octree visitor.
 */
class deoalRTWorldOctreeVisitor{
private:
	decVector pRayOrigin;
	decVector pRayDirection;
	decVector pRayTarget;
	
	decVector pInvRayDirection;
	bool pCheckAxisX;
	bool pCheckAxisY;
	bool pCheckAxisZ;
	
	float pRayLength;
	decVector pRayBoxMin;
	decVector pRayBoxMax;
	
// 	decVector pRayNormDir;
// 	float pSphereDot;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalRTWorldOctreeVisitor();
	
	/** \brief Clean up visitor. */
	virtual ~deoalRTWorldOctreeVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray origin. */
	inline const decVector &GetRayOrigin() const{ return pRayOrigin; }
	
	/** \brief Ray direction. */
	inline const decVector &GetRayDirection() const{ return pRayDirection; }
	
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
	inline const decVector &GetInvRayDirection() const{ return pInvRayDirection; }
	
	/** \brief Ray length. */
	inline float GetRayLength() const{ return pRayLength; }
	
	/** \brief Ray box minimum extend. */
	inline const decVector &GetRayBoxMin() const{ return pRayBoxMin; }
	
	/** \brief Ray box maximum extend. */
	inline const decVector &GetRayBoxMax() const{ return pRayBoxMax; }
	
	/** \brief Set test ray. */
	void SetRay( const decVector &origin, const decVector &direction );
	
	/** \brief Set test ray. */
	void SetRay( const deoalRTWorldOctree &octree, const decDVector &origin, const decDVector &direction );
	
	
	
	/** \brief Visit optimized ray-trace octree node. */
	void VisitOctree( const deoalRTWorldOctree &octree );
	
	/** \brief Visit component. */
	virtual void VisitComponent( const deoalRTWorldOctree::sVisitComponent &rtcomponent );
	/*@}*/
	
	
	
protected:
	void pVisitNode( const deoalRTWorldOctree &octree, const deoalRTWorldOctree::sVisitNode &node );
	bool pRayHitsBox( const decVector &center, const decVector &halfExtends ) const;
// 	bool pRayHitsSphere( const decVector &center, float radiusSquared ) const;
	bool pRayHitsBox( const decVector &center, const decVector &halfExtends, float &closestDistance ) const;
};

#endif
