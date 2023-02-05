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

#ifndef _DEOGLHEIGHTTERRAIN_H_
#define _DEOGLHEIGHTTERRAIN_H_

#include "deoglRHeightTerrain.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicHeightTerrain.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglWorld;
class deoglHTSector;
class deGraphicOpenGl;

class deHeightTerrain;



/**
 * Height terrain peer.
 */
class deoglHeightTerrain : public deBaseGraphicHeightTerrain{
private:
	deGraphicOpenGl &pOgl;
	const deHeightTerrain &pHeightTerrain;
	
	deoglRHeightTerrain::Ref pRHeightTerrain;
	
	decObjectList pSectors;
	bool pDirtySectors;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglHeightTerrain( deGraphicOpenGl &ogl, const deHeightTerrain &heightTerrain );
	
	/** Clean up peer. */
	virtual ~deoglHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Height terrain. */
	inline const deHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Render height terrain. */
	inline const deoglRHeightTerrain::Ref &GetRHeightTerrain() const{ return pRHeightTerrain; }
	
	/** Update. */
	void Update( float elapsed );
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Parameter changes. */
	virtual void ParametersChanged();
	
	/** Sector heights changed. */
	virtual void HeightChanged( const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates );
	
	
	
	/** Sector has been added. */
	virtual void SectorAdded( deHeightTerrainSector *sector );
	
	/** Sector has been removed. */
	virtual void SectorRemoved( int index );
	
	/** All sectors have been removed. */
	virtual void AllSectorsRemoved();
	
	/** Sector changed. */
	virtual void SectorChanged( int index );
	
	
	
	/** Decal has been added. */
	virtual void DecalAdded( int sector, deDecal *decal );
	
	/** Decal has been removed. */
	virtual void DecalRemoved( int sector, deDecal *decal );
	
	/** All decals have been removed. */
	virtual void AllDecalsRemoved( int sector );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
