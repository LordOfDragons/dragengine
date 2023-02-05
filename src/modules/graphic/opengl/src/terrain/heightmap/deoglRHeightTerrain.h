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

#ifndef _DEOGLRHEIGHTTERRAIN_H_
#define _DEOGLRHEIGHTTERRAIN_H_

#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/deObject.h>

class deoglRWorld;
class deoglRHTSector;
class deoglRenderThread;
class deoglHeightTerrainListener;

class deHeightTerrain;


/**
 * Render height terrain.
 */
class deoglRHeightTerrain : public deObject{
public:
	typedef deTObjectReference<deoglRHeightTerrain> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	
	int pSectorResolution;
	float pSectorSize;
	
	decObjectList pSectors;
	bool pSectorsRequirePrepareForRender;
	
	decObjectOrderedSet pListeners;
	int pListenerIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render height terrain. */
	deoglRHeightTerrain( deoglRenderThread &renderThread, const deHeightTerrain &heightTerrain );
	
protected:
	/** Clean up render height terrain. */
	virtual ~deoglRHeightTerrain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Parent world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** Height image size. */
	inline int GetSectorResolution() const{ return pSectorResolution; }
	
	/** Sector dimension. */
	inline float GetSectorSize() const{ return pSectorSize; }
	
	/** Prepare for render. */
	void PrepareForRender();
	
	
	
	/** Number of sectors. */
	int GetSectorCount() const;
	
	/** Sector at index. */
	deoglRHTSector &GetSectorAt( int index ) const;
	
	/** Add sector. */
	void AddSector( deoglRHTSector *htsector );
	
	/** Remove all sectors. */
	void RemoveAllSectors();
	
	/** Sector requires prepare for render. */
	void SectorRequirePrepareForRender();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener( deoglHeightTerrainListener *listener );
	
	/** Remove listener if existing. */
	void RemoveListener( deoglHeightTerrainListener *listener );
	
	/** Notify all that this height terrain has been destroyed. */
	void NotifyHeightTerrainDestroyed();
	
	/** Notify all sectors changed. */
	void NotifySectorsChanged();
	/*@}*/
};

#endif
