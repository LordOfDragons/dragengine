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

#ifndef _DEOALWOVPREPARERAYTRACE_H_
#define _DEOALWOVPREPARERAYTRACE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/collection/decPointerList.h>

#include "deoalWorldOctreeVisitor.h"

class deoalAWorld;
class deoalRTWorldBVH;


/**
 * \brief World octree visitor prepare for ray-tracing.
 * 
 * This is a two stage visitor due to deoalAComponent potentially altering the extends and thus
 * the octree node. This potential altering of the extends is allowed to avoid transforming
 * components outside the audible range which improves performance. Allowing this forces us
 * though to first collect all affected components and then to prepare them after visiting.
 * This way changes to the octree doe not affect the ongoing visiting. This also means the
 * visitor should be allocated once and then reused to avoid allocating the array each time.
 */
class deoalWOVPrepareRayTrace : public deoalWorldOctreeVisitor{
private:
	decDVector pCenter;
	double pRadius;
	double pRadiusSquared;
	decLayerMask pLayerMask;
	deoalRTWorldBVH *pRTWorldBVH;
	
	decPointerList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVPrepareRayTrace();
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVPrepareRayTrace();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Center. */
	inline const decDVector &GetCenter() const{ return pCenter; }
	
	/** \brief Set center. */
	void SetCenter( const decDVector &center );
	
	/** \brief Radius. */
	inline double GetRadius() const{ return pRadius; }
	
	/** \brief Set radius. */
	void SetRadius( double radius );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Ray trace world BVH or NULL. */
	inline deoalRTWorldBVH *GetRTWorldBVH() const{ return pRTWorldBVH; }
	
	/** \brief Set ray trace world bch or NULL. */
	void SetRTWorldBVH( deoalRTWorldBVH *bvh );
	
	
	
	/**
	 * \brief Visit world with visitor.
	 * 
	 * Always use this method instead of doing visiting on your own. Clears first the array
	 * of components, then visits the world and then prepare the collected components.
	 */
	void Visit( deoalAWorld &world );
	
	
	
	/** \brief Visits components affecting sound hit by ray. */
	virtual void VisitNode( deoalDOctree *node, int intersection );
	/*@}*/
	
	
	
private:
	void pPrepareRTWorldBVH( deoalRTWorldBVH &bvh );
};

#endif
