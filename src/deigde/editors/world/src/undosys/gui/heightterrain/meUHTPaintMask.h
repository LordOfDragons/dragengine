/* 
 * Drag[en]gine IGDE World Editor
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

// include only once
#ifndef _MEUHTPAINTMASK_H_
#define _MEUHTPAINTMASK_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;
class meHeightTerrainSector;
class meHeightTerrainTexture;



/**
 * \brief Undo Mask Terrain Mask Painting.
 *
 * Undo action to undo and redo mask painting on a height terrain.
 */
class meUHTPaintMask : public igdeUndo{
private:
	meWorld *pWorld;
	meHeightTerrainSector *pSector;
	meHeightTerrainTexture *pTexture;
	
	decBoundary pGrid;
	decPoint pSize;
	
	unsigned char *pOldValues;
	unsigned char *pNewValues;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTPaintMask( int drawMode, meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture,
		const decPoint &grid, const decPoint &size, unsigned char *oldValues );
	/** \brief Clean up object. */
	virtual ~meUHTPaintMask();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
	void pSaveValues();
	void pRestoreValues( unsigned char *values );
};

// end of include only once
#endif
