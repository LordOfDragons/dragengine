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

#ifndef _MEUOBJECTADDUSEDTEXTURES_H_
#define _MEUOBJECTADDUSEDTEXTURES_H_

#include <deigde/undo/igdeUndo.h>
#include "../../../../world/object/texture/meObjectTextureList.h"

#include <dragengine/common/string/decStringList.h>

class meObject;



/**
 * \brief Undo Action Object Add Used Textures.
 */
class meUObjectAddUsedTextures : public igdeUndo{
private:
	meObject *pObject;
	decStringList pTextureNameList;
	meObjectTextureList pTextureList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectAddUsedTextures( meObject *object );
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjectAddUsedTextures();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the list of textures names. */
	inline decStringList &GetTextureNameList(){ return pTextureNameList; }
	inline const decStringList &GetTextureNameList() const{ return pTextureNameList; }
	/** Retrieves the list of added textures if existing already. */
	inline meObjectTextureList &GetTextureList(){ return pTextureList; }
	inline const meObjectTextureList &GetTextureList() const{ return pTextureList; }
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
