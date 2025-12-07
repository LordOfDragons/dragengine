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
	eBorderTargets pBorderTargets[4];
	
	decPointerList pRTSInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cluster. */
	deoglHTViewSectorCluster(deoglHTViewSector &sector, const decPoint &coordinate);
	
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
	void SetLodLevel(int lodLevel);
	
	/** Border target. */
	eBorderTargets GetBorderTarget(eBorders border) const;
	
	/** Set border target. */
	void SetBorderTarget(eBorders border, eBorderTargets target);
	
	/** Reset. */
	void Reset();
	
	
	
	/**
	 * Render task shared instance at index for texture. Index 0 is cluster.
	 * Index 1 to 4 borders. Returns NULL if not present.
	 */
	deoglRenderTaskSharedInstance *GetRTSInstanceAt(int texture, int index) const;
	
	/** Update render task shared instances. */
	void UpdateRTSInstances();
	/*@}*/
	
	
	
private:
	void pRemoveAllRTSInstances();
};

#endif
