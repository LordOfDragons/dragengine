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
#include <dragengine/deObject.h>

class deoglRWorld;
class deoglRHTSector;
class deoglRenderThread;

class deHeightTerrain;



/**
 * \brief Render height terrain.
 */
class deoglRHeightTerrain : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	
	decObjectList pSectors;
	
	int pSectorResolution;
	float pSectorSize;
	
	unsigned int pUpdateTracker;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render height terrain. */
	deoglRHeightTerrain( deoglRenderThread &renderThread, const deHeightTerrain &heightTerrain );
	
	/** \brief Clean up render height terrain. */
	virtual ~deoglRHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Parent world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** \brief Height image size. */
	inline int GetSectorResolution() const{ return pSectorResolution; }
	
	/** \brief Sector dimension. */
	inline float GetSectorSize() const{ return pSectorSize; }
	
	
	
	/** \brief Number of sectors. */
	int GetSectorCount() const;
	
	/** \brief Sector at index. */
	deoglRHTSector &GetSectorAt( int index ) const;
	
	/** \brief Add sector. */
	void AddSector( deoglRHTSector *htsector );
	
	/** \brief Remove all sectors. */
	void RemoveAllSectors();
	
	
	
	/** \brief Update tracker value. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	
	
	
	/** \brief Update VBOs. */
	void UpdateVBOs();
	/*@}*/
};

#endif
