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
#ifndef _MEUHTADDTEXTURE_H_
#define _MEUHTADDTEXTURE_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class meWorld;
class meHeightTerrainSector;
class meHeightTerrainTexture;



/**
 * \brief Undo Height Terrain Add Texture.
 *
 * Undo action to add a texture to a height terrain.
 */
class meUHTAddTexture : public igdeUndo{
private:
	meWorld *pWorld;
	meHeightTerrainSector *pSector;
	meHeightTerrainTexture *pTexture;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTAddTexture( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture );
	
protected:
	/** \brief Clean up object. */
	virtual ~meUHTAddTexture();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif
