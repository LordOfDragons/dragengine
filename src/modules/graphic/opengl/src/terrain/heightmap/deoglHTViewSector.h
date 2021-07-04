/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLHTVIEWSECTOR_H_
#define _DEOGLHTVIEWSECTOR_H_

#include <dragengine/common/math/decMath.h>

class deoglHTView;
class deoglRHTSector;
class deoglRHeightTerrain;
class deoglHTViewSectorCluster;



/**
 * Height Terrain View Sector. Sector of an instance ( view ) of a height terrain.
 */
class deoglHTViewSector{
private:
	deoglHTView &pView;
	deoglRHTSector &pSector;
	
	deoglHTViewSectorCluster **pClusters;
	int pClusterCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view. */
	deoglHTViewSector( deoglHTView &view, deoglRHTSector &sector );
	
	/** Clean up view. */
	~deoglHTViewSector();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent view. */
	inline deoglHTView &GetView() const{ return pView; }
	
	/** Sector. */
	inline deoglRHTSector &GetSector() const{ return pSector; }
	
	/** Count of clusters. */
	const int GetClusterCount() const{ return pClusterCount; }
	
	/** Cluster at index. */
	deoglHTViewSectorCluster &GetClusterAt( int index ) const;
	
	/** Cluster at coordinate. */
	deoglHTViewSectorCluster &GetClusterAt( const decPoint &coordinates ) const;
	
	/** Reset lod levels to 0 for all clusters and unsets all borders. */
	void ResetClusters();
	
	/**
	 * Update LOD levels on all clusters using the given camera location relative to the
	 * terrain. LOD level neighbor rules are taken into account. Also updates the borders to
	 * render the terrain correctly.
	 */
	void UpdateLODLevels( const decVector &camera );
	
	/**
	 * Height map view is visible. A height map view is visible if at least one contained
	 * clusters is visible.
	 */
	bool IsVisible() const;
	
	/** Update all render task shared instances. */
	void UpdateAllRTSInstances();
	/*@}*/
	
	
	
private:
	void pLimitNeighborLODLevels( int x, int z );
};

#endif
