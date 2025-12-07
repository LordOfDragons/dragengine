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

#ifndef _DEOGLHTVIEW_H_
#define _DEOGLHTVIEW_H_

#include "deoglHeightTerrainListener.h"
#include "deoglRHeightTerrain.h"

#include <dragengine/common/math/decMath.h>

class deoglHTViewSector;
class deoglCollisionVolume;



/**
 * Height Terrain View.
 */
class deoglHTView : public deoglHeightTerrainListener{
public:
	typedef deTObjectReference<deoglHTView> Ref;
	
	
private:
	class HTListener : public deoglHeightTerrainListener{
		deoglHTView &pHTView;
		
	public:
		HTListener(deoglHTView &htview);
		virtual void SectorsChanged(deoglRHeightTerrain &heightTerrain);
	};
	
	
	
	const deoglRHeightTerrain::Ref pHeightTerrain;
	deoglHeightTerrainListener::Ref pHTListener;
	
	deoglHTViewSector **pSectors;
	int pSectorCount;
	int pSectorSize;
	
	bool pDirtySectors;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view. */
	deoglHTView(deoglRHeightTerrain *heightTerrain);
	
protected:
	/** Clean up view. */
	virtual ~deoglHTView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Height terrain. */
	inline deoglRHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Count of sectors. */
	inline int GetSectorCount() const{ return pSectorCount; }
	
	/** Sector at index. */
	deoglHTViewSector *GetSectorAt(int index) const;
	
	/** Add sector. */
	void AddSector(deoglHTViewSector *sector);
	
	/** Remove all sectors. */
	void RemoveAllSectors();
	
	/** Reset lod levels to 0 for all sector clusters and unsets all borders. */
	void ResetClusters();
	
	/**
	 * Update LOD levels on all clusters using the given camera location relative to the
	 * terrain. LOD level neighbor rules are taken into account. Also updates the borders to
	 * render the terrain correctly.
	 */
	void UpdateLODLevels(const decVector &camera);
	
	/** Prepate. */
	void Prepare();
	
	/** Prepate for rendering. */
	void PrepareForRendering();
	
	/** Update all render task shared instances. */
	void UpdateAllRTSInstances();
	
	
	
	/** Sectors changed. */
	virtual void SectorsChanged(deoglRHeightTerrain &heightTerrain);
	/*@}*/
	
	
	
private:
	void pRebuildSectors();
};

#endif
