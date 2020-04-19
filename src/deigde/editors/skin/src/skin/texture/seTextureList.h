/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SETEXTURELIST_H_
#define _SETEXTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seTexture;



/**
 * @brief Texture List.
 */
class seTextureList{
private:
	decObjectOrderedSet pTextures;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new texture list. */
	seTextureList();
	/** Cleans up the texture list. */
	~seTextureList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of textures. */
	int GetCount() const;
	/** Retrieves the texture at the given position. */
	seTexture *GetAt( int index ) const;
	/** Retrieves the texture with the given name or NULL if not found. */
	seTexture *GetNamed( const char *name ) const;
	/** Retrieves the index of the given texture or -1 if not found. */
	int IndexOf( seTexture *texture ) const;
	/** Retrieves the index of the texture with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a texture exists. */
	bool Has( seTexture *texture ) const;
	/** Determines if a texture with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a texture. */
	void Add( seTexture *texture );
	/** Inserts a new texture. */
	void InsertAt( seTexture *texture, int index );
	/** Moves a texture to a new position. */
	void MoveTo( seTexture *texture, int index );
	/** Removes a texture. */
	void Remove( seTexture *texture );
	/** Removes all textures. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	seTextureList &operator=( const seTextureList &list );
	/*@}*/
};

#endif
