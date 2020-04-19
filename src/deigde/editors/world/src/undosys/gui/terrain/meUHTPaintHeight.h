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
#ifndef _MEUHTPAINTHEIGHT_H_
#define _MEUHTPAINTHEIGHT_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;



/**
 * \brief Undo Height Terrain Height Painting.
 *
 * Undo action to undo and redo height painting on a height terrain.
 */
class meUHTPaintHeight : public igdeUndo{
private:
	meWorld *pWorld;
	
	decBoundary pSector;
	decBoundary pGrid;
	decPoint pSize;
	
	float *pOldHeights;
	float *pNewHeights;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTPaintHeight( int drawMode, meWorld *world, const decPoint &sector, const decPoint &grid, const decPoint &size, float *oldHeights );
	/** \brief Clean up object. */
	virtual ~meUHTPaintHeight();
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
	void pSaveHeights();
	void pRestoreHeights( float *heights );
};

// end of include only once
#endif
