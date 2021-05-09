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
#ifndef _DEOGLHTVIEW_H_
#define _DEOGLHTVIEW_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class deoglRHeightTerrain;
class deoglHTViewSector;
class deoglCollisionVolume;



/**
 * @brief Height Terrain View.
 *
 * Manages an instance ( view ) of a height terrain.
 */
class deoglHTView{
private:
	deoglRHeightTerrain *pHeightTerrain;
	
	deoglHTViewSector **pSectors;
	int pSectorCount;
	int pSectorSize;
	
	unsigned int pHTUpdateTrackerValue;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new view. */
	deoglHTView( deoglRHeightTerrain *heightTerrain );
	/** Cleans up the view. */
	~deoglHTView();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Height terrain. */
	inline deoglRHeightTerrain &GetHeightTerrain() const{ return *pHeightTerrain; }
	
	/** Retrieves the number of sectors. */
	inline int GetSectorCount() const{ return pSectorCount; }
	/** Retrieves the sector at the given index. */
	deoglHTViewSector *GetSectorAt( int index ) const;
	/** Adds a sector. */
	void AddSector( deoglHTViewSector *sector );
	/** Removes all sectors. */
	void RemoveAllSectors();
	
	/** Rebuilds the sectors from the height terrain. */
	void RebuildSectors();
	/** Resets the lod levels to 0 for all sector clusters and unsets all borders. */
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
	
	/** \brief Prepate for rendering. */
	void PrepareForRendering();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
