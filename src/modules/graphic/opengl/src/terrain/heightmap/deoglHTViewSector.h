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
	deoglHTViewSector(deoglHTView &view, deoglRHTSector &sector);
	
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
	deoglHTViewSectorCluster &GetClusterAt(int index) const;
	
	/** Cluster at coordinate. */
	deoglHTViewSectorCluster &GetClusterAt(const decPoint &coordinates) const;
	
	/** Reset lod levels to 0 for all clusters and unsets all borders. */
	void ResetClusters();
	
	/**
	 * Update LOD levels on all clusters using the given camera location relative to the
	 * terrain. LOD level neighbor rules are taken into account. Also updates the borders to
	 * render the terrain correctly.
	 */
	void UpdateLODLevels(const decVector &camera);
	
	/**
	 * Height map view is visible. A height map view is visible if at least one contained
	 * clusters is visible.
	 */
	bool IsVisible() const;
	
	/** Update all render task shared instances. */
	void UpdateAllRTSInstances();
	/*@}*/
	
	
	
private:
	void pLimitNeighborLODLevels(int x, int z);
};

#endif
