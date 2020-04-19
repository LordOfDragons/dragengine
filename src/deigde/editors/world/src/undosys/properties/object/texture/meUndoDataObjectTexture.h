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

#ifndef _MEUNDODATAOBJECTTEXTURE_H_
#define _MEUNDODATAOBJECTTEXTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../../../../world/object/texture/meObjectTextureList.h"

class meObject;



/**
 * \brief Object Property Undo Data.
 */
class meUndoDataObjectTexture : public deObject{
private:
	meObject *pObject;
	meObjectTexture *pOldTexture;
	meObjectTexture *pNewTexture;
	meObjectTextureList pOldTextureList;
	meObjectTextureList pNewTextureList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo data object. */
	meUndoDataObjectTexture( meObject *object );
	
protected:
	/** \brief Clean up undo data object. */
	virtual ~meUndoDataObjectTexture();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the object pointer. */
	inline meObject *GetObject() const{ return pObject; }
	/** Retrieves the old texture object or NULL if not existing. */
	inline meObjectTexture *GetOldTexture() const{ return pOldTexture; }
	/** Sets the old texture object or NULL if not existing. */
	void SetOldTexture( meObjectTexture *texture );
	/** Retrieves the new texture object or NULL if not existing. */
	inline meObjectTexture *GetNewTexture() const{ return pNewTexture; }
	/** Sets the new texture object or NULL if not existing. */
	void SetNewTexture( meObjectTexture *texture );
	/** Retrieves the old texture list if used. */
	inline meObjectTextureList &GetOldTextureList(){ return pOldTextureList; }
	inline const meObjectTextureList &GetOldTextureList() const{ return pOldTextureList; }
	/** Retrieves the new texture list if used. */
	inline meObjectTextureList &GetNewTextureList(){ return pNewTextureList; }
	inline const meObjectTextureList &GetNewTextureList() const{ return pNewTextureList; }
	/*@}*/
};

#endif
