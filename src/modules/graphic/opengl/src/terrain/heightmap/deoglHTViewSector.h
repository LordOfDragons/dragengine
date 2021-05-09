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

// include only once
#ifndef _DEOGLHTVIEWSECTOR_H_
#define _DEOGLHTVIEWSECTOR_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class deoglHTView;
class deoglRHTSector;
class deoglRHeightTerrain;

// definitions
enum eHTVSBorders{
	ehtvsbLeft,
	ehtvsbTop,
	ehtvsbRight,
	ehtvsbBottom
};

/** Height Terrain View Cluster. */
struct sHTVSCluster{
	char lodLevel;
	char borders[ 4 ];
};



/**
 * @brief Height Terrain View Sector.
 *
 * Sector of an instance ( view ) of a height terrain.
 */
class deoglHTViewSector{
private:
	deoglHTView &pView;
	deoglRHTSector &pSector;
	sHTVSCluster *pClusters;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new view. */
	deoglHTViewSector( deoglHTView &view, deoglRHTSector &sector );
	/** Cleans up the view. */
	~deoglHTViewSector();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent view. */
	inline deoglHTView &GetView() const{ return pView; }
	/** Retrieves the sector. */
	inline deoglRHTSector &GetSector() const{ return pSector; }
	/** Retrieves the clusters. */
	inline sHTVSCluster *GetClusters() const{ return pClusters; }
	/**
	 * Retrieves the given cluster. A LOD level below 0 indicates an invisible cluster that
	 * should not be rendered at all. borders set to true have to use the alternate border
	 * to patch up cracks.
	 */
	sHTVSCluster &GetClusterAt( int x, int z ) const;
	sHTVSCluster &GetClusterAt( const decPoint &coordinates ) const;
	
	/** Resets the lod levels to 0 for all clusters and unsets all borders. */
	void ResetClusters();
	/**
	 * Determines the visibility of clusters using a collision volume. LOD levels in contact
	 * with the collision volume receive a LOD level of 0 whereas clusters not in contact with
	 * the collision volume receive a LOD level of -1. The collision volume has to be relative
	 * to the parent terrain mesh local coordinate system.
	 */
	//void DetermineVisibilityUsing( deoglCollisionVolume *collisionVolume );
	/**
	 * Updates the LOD levels on all clusters using the given camera location relative to the
	 * terrain. LOD level neighbor rules are taken into account. Also updates the borders to
	 * render the terrain correctly.
	 */
	void UpdateLODLevels( const decVector &camera );
	/**
	 * Determines if the height map view is visible. A height map view is visible if
	 * at least one contained clusters is visible.
	 */
	bool IsVisible() const;
	/*@}*/
	
private:
	void pLimitNeighborLODLevels( int x, int z );
};

// end of include only once
#endif
