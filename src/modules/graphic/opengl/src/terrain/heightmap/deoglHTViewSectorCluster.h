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

#ifndef _DEOGLHTVIEWSECTORCLUSTER_H_
#define _DEOGLHTVIEWSECTORCLUSTER_H_

#include "deoglHTSCluster.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglHTViewSector;
class deoglRenderTaskSharedInstance;


/**
 * Height terrain view sector cluster.
 */
class deoglHTViewSectorCluster{
public:
	enum eBorders{
		ebLeft,
		ebTop,
		ebRight,
		ebBottom
	};
	
	enum eBorderTargets{
		ebtLeft,
		ebtTop,
		ebtRight,
		ebtBottom,
		ebtFixLeft,
		ebtFixTop,
		ebtFixRight,
		ebtFixBottom
	};

	
	
private:
	deoglHTViewSector &pSector;
	
	const decPoint pCoordinate;
	int pLodLevel;
	eBorderTargets pBorderTargets[ 4 ];
	
	decPointerList pRTSInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cluster. */
	deoglHTViewSectorCluster( deoglHTViewSector &sector, const decPoint &coordinate );
	
	/** Clean up cluster. */
	~deoglHTViewSectorCluster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent sector. */
	inline deoglHTViewSector &GetSector() const{ return pSector; }
	
	
	
	/** Coordinate. */
	inline const decPoint &GetCoordinate() const{ return pCoordinate; }
	
	/** Lod level. */
	inline int GetLodLevel() const{ return pLodLevel; }
	
	/** Set lod level. */
	void SetLodLevel( int lodLevel );
	
	/** Border target. */
	eBorderTargets GetBorderTarget( eBorders border ) const;
	
	/** Set border target. */
	void SetBorderTarget( eBorders border, eBorderTargets target );
	
	/** Reset. */
	void Reset();
	
	
	
	/**
	 * Render task shared instance at index for texture. Index 0 is cluster.
	 * Index 1 to 4 borders. Returns NULL if not present.
	 */
	deoglRenderTaskSharedInstance *GetRTSInstanceAt( int texture, int index ) const;
	
	/** Update render task shared instances. */
	void UpdateRTSInstances();
	/*@}*/
	
	
	
private:
	void pRemoveAllRTSInstances();
};

#endif
