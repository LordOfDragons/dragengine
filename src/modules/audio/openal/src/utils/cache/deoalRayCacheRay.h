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

#ifndef _DEOALRAYCACHERAY_H_
#define _DEOALRAYCACHERAY_H_

#include <dragengine/common/math/decMath.h>


class deoalRayCacheOctree;
class deoalRayCacheRayHit;
class deoalRayTraceHitElementList;
class deoalRayTraceResult;


/**
 * \brief Model ray cache ray.
 */
class deoalRayCacheRay{
	decVector pOrigin;
	decVector pDirection;
	float pLength;
	
	deoalRayCacheRayHit *pHits;
	int pHitCount;
	
	deoalRayCacheOctree *pOctreeNode;
	
	deoalRayCacheRay *pLLPrev;
	deoalRayCacheRay *pLLNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model ray cache ray. */
	deoalRayCacheRay( const decVector &origin, const decVector &direction,
		float length, const deoalRayTraceHitElementList &elements );
	
	/** \brief Create model ray cache ray. */
	deoalRayCacheRay( const decVector &origin, const decVector &direction,
		float length, const deoalRayTraceResult &result );
	
	/** \brief Create model ray cache ray. */
	deoalRayCacheRay( const decVector &origin, const decVector &direction,
		float length, const deoalRayTraceResult &result, int elementCount );
	
	/** \brief Clean up model ray cache ray. */
	~deoalRayCacheRay();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Origin. */
	inline const decVector &GetOrigin() const{ return pOrigin; }
	
	/** \brief Normalized direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Length. */
	inline float GetLength() const{ return pLength; }
	
	/** \brief Number of hits. */
	inline int GetHitCount() const{ return pHitCount; }
	
	/** \brief Hit at index. */
	const deoalRayCacheRayHit &GetHitAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief Octree node. */
	inline deoalRayCacheOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set octree node. */
	void SetOctreeNode( deoalRayCacheOctree *node );
	
	/** \brief Previous ray in cache. */
	inline deoalRayCacheRay *GetLLPrev() const{ return pLLPrev; }
	
	/** \brief Set previous ray in cache. */
	void SetLLPrev( deoalRayCacheRay *ray );
	
	/** \brief Next ray in cache. */
	inline deoalRayCacheRay *GetLLNext() const{ return pLLNext; }
	
	/** \brief Set next ray in cache. */
	void SetLLNext( deoalRayCacheRay *ray );
	/*@}*/
};

#endif
