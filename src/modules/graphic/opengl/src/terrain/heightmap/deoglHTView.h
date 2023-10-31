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
		HTListener( deoglHTView &htview );
		virtual void SectorsChanged( deoglRHeightTerrain &heightTerrain );
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
	deoglHTView( deoglRHeightTerrain *heightTerrain );
	
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
	deoglHTViewSector *GetSectorAt( int index ) const;
	
	/** Add sector. */
	void AddSector( deoglHTViewSector *sector );
	
	/** Remove all sectors. */
	void RemoveAllSectors();
	
	/** Reset lod levels to 0 for all sector clusters and unsets all borders. */
	void ResetClusters();
	
	/**
	 * Update LOD levels on all clusters using the given camera location relative to the
	 * terrain. LOD level neighbor rules are taken into account. Also updates the borders to
	 * render the terrain correctly.
	 */
	void UpdateLODLevels( const decVector &camera );
	
	/** Prepate. */
	void Prepare();
	
	/** Prepate for rendering. */
	void PrepareForRendering();
	
	/** Update all render task shared instances. */
	void UpdateAllRTSInstances();
	
	
	
	/** Sectors changed. */
	virtual void SectorsChanged( deoglRHeightTerrain &heightTerrain );
	/*@}*/
	
	
	
private:
	void pRebuildSectors();
};

#endif
