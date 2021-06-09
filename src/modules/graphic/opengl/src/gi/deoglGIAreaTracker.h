/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLGIAREATRACKER_H_
#define _DEOGLGIAREATRACKER_H_

#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/math/decMath.h>

#include "../collidelist/deoglCollideList.h"

class deoglRWorld;
class deoglWorldOctree;
class deoglRComponent;


/**
 * World octree visitor tracking instances entering or leaving GI State area
 * while GI State moves
 */
class deoglGIAreaTracker{
private:
	struct sBox{
		decDVector minExtend, maxExtend;
	};
	
	deoglCollideList pEntering;
	deoglCollideList pLeaving;
	bool pAllLeaving;
	
	deoglRWorld *pWorld;
	decDVector pHalfExtends;
	decLayerMask pLayerMask;
	double pUpdateThreshold;
	
	decDVector pLastUpdatePosition;
	decDVector pPosition;
	bool pValid;
	
	sBox pBoxLastUpdate;
	sBox pBox;
	sBox pBoxKeep;
	sBox pBoxVisit;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglGIAreaTracker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** World or NULL. */
	inline deoglRWorld *GetWorld() const{ return pWorld; }
	
	/** Set world or NULL. */
	void SetWorld( deoglRWorld *world );
	
	/** Half-extends. */
	inline const decDVector &GetHalfExtends() const{ return pHalfExtends; }
	
	/** Set half extends. */
	void SetHalfExtends( const decDVector &halfExtends );
	
	/** Layer mask. */
	const decLayerMask &GetLayerMask(){ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Position distance threshold before updating tracking. */
	inline double GetUpdateThreshold() const{ return pUpdateThreshold; }
	
	/** Set position distance threshold before updating tracking. */
	void SetUpdateThreshold( double threshold );
	
	
	
	/** Position tracking is valid for. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition( const decDVector &position );
	
	/** Tracking is valid. If not becomes valid after next update. */
	inline bool GetValid() const{ return pValid; }
	
	
	
	/** Update tracked instances. */
	void Update();
	
	
	
	/** List of instances entering area. Valid after Update() has been called. */
	inline const deoglCollideList &GetEntering() const{ return pEntering; }
	
	/** List of instances leaving area. Valid after Update() has been called. */
	inline const deoglCollideList &GetLeaving() const{ return pLeaving; }
	
	/** All instances area leaving. Valid after Update() has been called. */
	inline bool GetAllLeaving() const{ return pAllLeaving; }
	
	/** Entering or leaving list is not empty or all leaving flag is set. */
	bool HasChanged() const;
	
	/** Entering and leaving list are empty and all leaving flag is cleared. */
	bool HasNotChanged() const;
	
	/** Clear lists and all leaving flag. */
	void ClearChanges();
	
	/** Reject component. */
	bool RejectComponent( const deoglRComponent &component ) const;
	
	/** Component touches current box. */
	bool ComponentTouches( const deoglRComponent &component ) const;
	/*@}*/
	
	
	
private:
	void pVisitNodeColliding( const deoglWorldOctree &node );
	void pVisitNode( const deoglWorldOctree &node );
	void pVisitComponents( const deoglWorldOctree &node );
	
	void pVisitNodeCollidingNewOnly( const deoglWorldOctree &node );
	void pVisitNodeNewOnly( const deoglWorldOctree &node );
	void pVisitComponentsNewOnly( const deoglWorldOctree &node );
};

#endif
