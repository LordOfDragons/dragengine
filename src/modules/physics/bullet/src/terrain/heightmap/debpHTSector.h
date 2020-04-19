/* 
 * Drag[en]gine Bullet Physics Module
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
#ifndef _DEBPHTSECTOR_H_
#define _DEBPHTSECTOR_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class debpBulletShape;
class debpHeightTerrain;
class debpHeightTerrainShape;
class debpPhysicsBody;

class deHeightTerrainSector;



/** Bullet Height Terrain Sector Cluster. */
struct debpHTSCluster{
	int firstPointX;
	int firstPointZ;
	int pointCountX;
	int pointCountZ;
	
	decVector center;
	decVector halfExtends;
};



/**
 * @brief Height Terrain Sector.
 *
 * Sector of a height terrain peer.
 */
class debpHTSector{
private:
	debpHeightTerrain *pHeightTerrain;
	deHeightTerrainSector *pSector;
	
	int pImageDim;
	
	debpHTSCluster *pClusters;
	int pClusterCount;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	decVector *pPoints;
	bool pDirtyPoints;
	
	bool pMarked;
	
	debpPhysicsBody *pPhyBody;
	debpHeightTerrainShape *pShape;
	debpBulletShape *pBulletShape;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new sector. */
	debpHTSector( debpHeightTerrain *heightTerrain, deHeightTerrainSector *sector );
	/** Cleans up the sector. */
	~debpHTSector();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the bullet height terrain. */
	inline debpHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	/** Retrieves the height terrain sector. */
	inline deHeightTerrainSector *GetSector() const{ return pSector; }
	
	/** Retrieves the image dimension. */
	inline int GetImageDimension() const{ return pImageDim; }
	
	/** Retrieves the number of clusters in the both direction. */
	inline int GetClusterCount() const{ return pClusterCount; }
	/** Retrieves the cluster at the given location. */
	debpHTSCluster &GetClusterAt( int x, int z ) const;
	/** Retrieves the list of clusters. */
	inline debpHTSCluster *GetClusters() const{ return pClusters; }
	
	/** Retrieves the minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Retrieves the marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	/** Sets the marked flag. */
	inline void SetMarked( bool marked ){ pMarked = marked; }
	
	/** Retrieves the points for a height map with or without modifications. */
	inline decVector *GetPoints() const{ return pPoints; }
	
	/** Update heights. */
	void UpdateHeights( int fromX, int fromY, int toX, int toY );
	
	/** Update. */
	void Update();
	/** Parent world changed. */
	void ParentWorldChanged();
	/** Sector changed. */
	void SectorChanged();
	/*@}*/
	
private:
	void pCleanUp();
	void pCreatePoints();
	void pUpdatePoints();
};

// end of include only once
#endif
