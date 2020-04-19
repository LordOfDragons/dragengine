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

#ifndef _GDESKINLIST_H_
#define _GDESKINLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeSkin;



/**
 * \brief Skin list.
 */
class gdeSkinList{
private:
	decObjectOrderedSet pSkins;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin list. */
	gdeSkinList();
	
	/** \brief Create copy of skin list. */
	gdeSkinList( const gdeSkinList &list );
	
	/** \brief Clean up skin list. */
	~gdeSkinList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of skins. */
	int GetCount() const;
	
	/** \brief Skin at position. */
	gdeSkin *GetAt( int index ) const;
	
	/** \brief Skin with path or \em NULL if absent. */
	gdeSkin *GetWithPath( const char *path ) const;
	
	/** \brief Index of skin or -1 if absent. */
	int IndexOf( gdeSkin *skin ) const;
	
	/** \brief Index of skin with path or -1 if absent. */
	int IndexOfWithPath( const char *path ) const;
	
	/** \brief Skin is present. */
	bool Has( gdeSkin *skin ) const;
	
	/** \brief Skin with path is present. */
	bool HasWithPath( const char *path ) const;
	
	/** \brief Add skin. */
	void Add( gdeSkin *skin );
	
	/** \brief Remove skin. */
	void Remove( gdeSkin *skin );
	
	/** \brief Remove all skins. */
	void RemoveAll();
	
	/** \brief Set from another skin list. */
	gdeSkinList &operator=( const gdeSkinList &list );
	/*@}*/
};

#endif
