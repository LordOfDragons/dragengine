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

#ifndef _IGDEGDCCTEXTURELIST_H_
#define _IGDEGDCCTEXTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCCTexture;



/**
 * \brief Game Definition Class Component Texture List.
 */
class DE_DLL_EXPORT igdeGDCCTextureList{
private:
	decObjectOrderedSet pTextures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCCTextureList();
	
	/** \brief Create copy of a game definition class component texture list. */
	igdeGDCCTextureList( const igdeGDCCTextureList &list );
	
	/** \brief Clean up list. */
	~igdeGDCCTextureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of textures. */
	int GetCount() const;
	
	/** \brief Texture by index. */
	igdeGDCCTexture *GetAt( int index ) const;
	
	/** \brief Texture by name or NULL if not found. */
	igdeGDCCTexture *GetNamed( const char *name ) const;
	
	/** \brief Index of an texture or -1 if not found. */
	int IndexOf( igdeGDCCTexture *texture ) const;
	
	/** \brief Index of a named texture or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Determines if an texture exists. */
	bool Has( igdeGDCCTexture *texture ) const;
	
	/** \brief Determines if an named texture exists. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Adds an texture. */
	void Add( igdeGDCCTexture *texture );
	
	/** \brief Inserts a new texture. */
	void InsertAt( igdeGDCCTexture *texture, int index );
	
	/** \brief Moves an texture to a new position. */
	void MoveTo( igdeGDCCTexture *texture, int index );
	
	/** \brief Removes an texture. */
	void Remove( igdeGDCCTexture *texture );
	
	/** \brief Removes all textures. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCCTextureList &list );
	
	/** \brief Set list from another list. */
	igdeGDCCTextureList &operator=( const igdeGDCCTextureList &list );
	/*@}*/
};

#endif
