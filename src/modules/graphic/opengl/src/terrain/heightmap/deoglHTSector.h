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

#ifndef _DEOGLHTSECTOR_H_
#define _DEOGLHTSECTOR_H_

#include <dragengine/common/math/decMath.h>

class deoglHeightTerrain;
class deoglRHTSector;

class deHeightTerrainSector;



/**
 * Height terrain sector.
 */
class deoglHTSector{
private:
	deoglHeightTerrain &pHeightTerrain;
	const deHeightTerrainSector &pSector;
	
	deoglRHTSector *pRSector;
	
	bool pSectorChanged;
	
	decPoint pHeightChangeFrom;
	decPoint pHeightChangeTo;
	bool pDirtyHeights;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create height terrain sector. */
	deoglHTSector( deoglHeightTerrain &heightTerrain, const deHeightTerrainSector &sector );
	
	/** Clean up height terrain sector. */
	~deoglHTSector();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render height terrain sector. */
	inline deoglRHTSector *GetRSector() const{ return pRSector; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Sector changed. */
	void SectorChanged();
	
	/** Heights changed. */
	void HeightChanged( const decPoint &from, const decPoint &to );
	/*@}*/
};

#endif
