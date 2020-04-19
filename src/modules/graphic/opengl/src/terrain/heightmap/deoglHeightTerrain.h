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

#include <dragengine/systems/modules/graphic/deBaseGraphicHeightTerrain.h>
#include <dragengine/common/collection/decPointerList.h>

class deoglRHeightTerrain;
class deoglWorld;
class deoglHTSector;
class deGraphicOpenGl;

class deHeightTerrain;



/**
 * \brief Height terrain peer.
 */
class deoglHeightTerrain : public deBaseGraphicHeightTerrain{
private:
	deGraphicOpenGl &pOgl;
	const deHeightTerrain &pHeightTerrain;
	
	deoglRHeightTerrain *pRHeightTerrain;
	
	decPointerList pSectors;
	bool pDirtySectors;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglHeightTerrain( deGraphicOpenGl &ogl, const deHeightTerrain &heightTerrain );
	
	/** \brief Clean up peer. */
	virtual ~deoglHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Height terrain. */
	inline const deHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Render height terrain. */
	inline deoglRHeightTerrain *GetRHeightTerrain() const{ return pRHeightTerrain; }
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parameter changes. */
	virtual void ParametersChanged();
	
	/** \brief Sector heights changed. */
	virtual void HeightChanged( const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates );
	
	
	
	/** \brief Sector has been added. */
	virtual void SectorAdded( deHeightTerrainSector *sector );
	
	/** \brief Sector has been removed. */
	virtual void SectorRemoved( int index );
	
	/** \brief All sectors have been removed. */
	virtual void AllSectorsRemoved();
	
	/** \brief Sector changed. */
	virtual void SectorChanged( int index );
	
	
	
	/** \brief Decal has been added. */
	virtual void DecalAdded( int sector, deDecal *decal );
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved( int sector, deDecal *decal );
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved( int sector );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
