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

#ifndef _MEUHTNAVSPACEFACEREMOVE_H_
#define _MEUHTNAVSPACEFACEREMOVE_H_

#include <deigde/undo/igdeUndo.h>

class meHeightTerrainNavSpaceType;
class meHeightTerrainNavSpaceFace;

class decObjectOrderedSet;



/**
 * \brief Undo action height terrain navigation space remove face.
 */
class meUHTNavSpaceFaceRemove : public igdeUndo{
private:
	struct sFace{
		meHeightTerrainNavSpaceType *type;
		meHeightTerrainNavSpaceFace *face;
	};
	
	sFace *pFaces;
	int pFaceCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	meUHTNavSpaceFaceRemove( const decObjectOrderedSet &faces );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~meUHTNavSpaceFaceRemove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
