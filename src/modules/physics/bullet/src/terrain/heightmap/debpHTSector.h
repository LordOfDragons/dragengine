/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
