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
#ifndef _MEUHTPAINTVISIBILITY_H_
#define _MEUHTPAINTVISIBILITY_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;
class meBitArray;



/**
 * \brief Undo Height Terrain Visibility Painting.
 *
 * Undo action to undo and redo visibility painting on a height terrain.
 */
class meUHTPaintVisibility : public igdeUndo{
private:
	meWorld *pWorld;
	
	decBoundary pSector;
	decBoundary pGrid;
	decPoint pSize;
	
	meBitArray *pOldVis;
	meBitArray *pNewVis;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTPaintVisibility( int drawMode, meWorld *world, const decPoint &sector, const decPoint &grid, const decPoint &size, meBitArray *oldVis );
	/** \brief Clean up object. */
	virtual ~meUHTPaintVisibility();
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
	void pSaveVisibility();
	void pRestoreVisibility( meBitArray *vis );
};

// end of include only once
#endif
