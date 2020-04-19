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

#ifndef _MEUOBJECTCLONETEXTURESTOSELECTED_H_
#define _MEUOBJECTCLONETEXTURESTOSELECTED_H_

#include <dragengine/common/collection/decObjectList.h>

#include <deigde/undo/igdeUndo.h>
#include "../../../../world/object/texture/meObjectTextureList.h"

class meObjectList;



/**
 * \brief Undo Action Clone Object Textures to all selected objects.
 */
class meUObjectCloneTexturesToSelected : public igdeUndo{
private:
	decObjectList pList;
	meObjectTextureList pTextureList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectCloneTexturesToSelected( const meObjectList &list, const meObjectTextureList &textureList );
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjectCloneTexturesToSelected();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the list of objects to manipulate. */
	inline decObjectList &GetList(){ return pList; }
	inline const decObjectList &GetList() const{ return pList; }
	/** Retrieves the list of textures to apply. */
	inline meObjectTextureList &GetPropertyList(){ return pTextureList; }
	inline const meObjectTextureList &GetPropertyList() const{ return pTextureList; }
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
