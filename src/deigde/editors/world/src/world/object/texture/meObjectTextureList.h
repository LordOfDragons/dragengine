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
#ifndef _MEOBJECTTEXTURELIST_H_
#define _MEOBJECTTEXTURELIST_H_

// includes
#include <dragengine/common/collection/decObjectOrderedSet.h>

// predefinitions
class meObjectTexture;



/**
 * @brief Object Texture List.
 * List of object textures.
 */
class meObjectTextureList{
private:
	decObjectOrderedSet pTextures;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectTextureList();
	/** Cleans up the list. */
	~meObjectTextureList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of textures. */
	int GetTextureCount() const;
	/** Retrieves the texture at the given position. */
	meObjectTexture *GetTextureAt( int position ) const;
	/** Retrieves the index of the given texture or -1 if not found. */
	int IndexOfTexture( meObjectTexture *texture ) const;
	/** Determines if the given texture exists. */
	bool HasTexture( meObjectTexture *texture );
	/** Adds an texture throwing an exception if already existing. */
	void AddTexture( meObjectTexture *texture );
	/** Adds an texture if not existing already. */
	void AddTextureIfAbsent( meObjectTexture *texture );
	/** Removes an texture throwing an exception if not existing. */
	void RemoveTexture( meObjectTexture *texture );
	/** Removes an texture if existing. */
	void RemoveTextureIfPresent( meObjectTexture *texture );
	/** Removes all textures. */
	void RemoveAllTextures();
	/*@}*/
};

// end of include only once
#endif
