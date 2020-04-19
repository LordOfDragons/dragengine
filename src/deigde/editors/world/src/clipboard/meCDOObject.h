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

#ifndef _MECDOOBJECT_H_
#define _MECDOOBJECT_H_

#include "../world/object/texture/meObjectTextureList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>

class meObject;



/**
 * \brief Object data for clipboard clips.
 * 
 * Stores informations about an object suitable for clipboard clips.
 */
class meCDOObject{
private:
	decString pClassname;
	decDVector pPosition;
	decVector pSize, pRotation;
	decStringDictionary pProperties;
	meObjectTextureList pTextures;
	int pAttachToIndex;
	decString pAttachToID;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object clip data. */
	meCDOObject( const meObject &object );
	
	/** \brief Clean up object data object. */
	~meCDOObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Classname. */
	inline const decString &GetClassname() const{ return pClassname; }
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Textures. */
	inline const meObjectTextureList &GetTextures() const{ return pTextures; }
	
	/** \brief Attach to index or -1 if not attached to an object. */
	inline int GetAttachToIndex() const{ return pAttachToIndex; }
	
	/** \brief Set attach to index or -1 if not attached to an object. */
	void SetAttachToIndex( int index );
	
	/** \brief Attach to ID in hex string format or empty string  if not attached. */
	inline const decString &GetAttachToID() const{ return pAttachToID; }
	
	/** \brief Set attach to ID in hex string format or empty string  if not attached. */
	void SetAttachToID( const char *id );
	
	/** \brief Update object with stored data. */
	void UpdateObject( meObject &object ) const;
	/*@}*/
};

#endif
