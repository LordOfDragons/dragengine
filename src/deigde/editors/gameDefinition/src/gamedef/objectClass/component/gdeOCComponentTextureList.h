/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCCOMPONENTTEXTURELIST_H_
#define _GDEOCCOMPONENTTEXTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCComponentTexture;



/**
 * \brief Object class component texture list.
 */
class gdeOCComponentTextureList{
private:
	decObjectOrderedSet pTextures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class component texture list. */
	gdeOCComponentTextureList();
	
	/** \brief Create copy of object class component texture list. */
	gdeOCComponentTextureList( const gdeOCComponentTextureList &list );
	
	/** \brief Clean up object class component texture list. */
	~gdeOCComponentTextureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class component texturees. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeOCComponentTexture *GetAt( int index ) const;
	
	/** \brief Named object class component texture or \em NULL if absent. */
	gdeOCComponentTexture *GetNamed( const char *name ) const;
	
	/** \brief Index of named object class component texture or -1 if absent. */
	int IndexOf( gdeOCComponentTexture *texture ) const;
	
	/** \brief Index of named object class component texture or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeOCComponentTexture *texture ) const;
	
	/** \brief Named object class component texture is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add object class component texture. */
	void Add( gdeOCComponentTexture *texture );
	
	/** \brief Remove object class component texture. */
	void Remove( gdeOCComponentTexture *texture );
	
	/** \brief Remove all object class component texturees. */
	void RemoveAll();
	
	/** \brief Set from another object class component texture list. */
	gdeOCComponentTextureList &operator=( const gdeOCComponentTextureList &list );
	/*@}*/
};

#endif
