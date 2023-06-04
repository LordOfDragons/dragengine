/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDETEXTUREPROPERTYLIST_H_
#define _IGDETEXTUREPROPERTYLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class igdeTextureProperty;



/**
 * \brief Texture Property List.
 */
class DE_DLL_EXPORT igdeTexturePropertyList{
private:
	decPointerList pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create texture property list. */
	igdeTexturePropertyList();
	
	/** \brief Clean up texture property list. */
	~igdeTexturePropertyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of texture properties. */
	int GetCount() const;
	
	/** \brief Texture property at the given position. */
	const igdeTextureProperty *GetAt( int index ) const;
	
	/** \brief Texture property with the given name or NULL if not found. */
	const igdeTextureProperty *GetNamed( const char *name ) const;
	
	/** \brief Index of the given texture property or -1 if not found. */
	int IndexOf( const igdeTextureProperty *texture ) const;
	
	/** \brief Index of the texture property with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Determines if a texture property exists. */
	bool Has( const igdeTextureProperty *texture ) const;
	
	/** \brief Determines if a texture property with the given name exists. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Adds a texture property. */
	void Add( igdeTextureProperty *texture );
	
	/** \brief Removes a texture property. */
	void Remove( const igdeTextureProperty *texture );
	
	/** \brief Removes all texture properties. */
	void RemoveAll();
	/*@}*/
};

#endif
