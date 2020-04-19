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

#ifndef _GDESKYLIST_H_
#define _GDESKYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeSky;



/**
 * \brief Sky list.
 */
class gdeSkyList{
private:
	decObjectOrderedSet pSkys;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky list. */
	gdeSkyList();
	
	/** \brief Create copy of sky list. */
	gdeSkyList( const gdeSkyList &list );
	
	/** \brief Clean up sky list. */
	~gdeSkyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of skys. */
	int GetCount() const;
	
	/** \brief Sky at position. */
	gdeSky *GetAt( int index ) const;
	
	/** \brief Sky with path or \em NULL if absent. */
	gdeSky *GetWithPath( const char *path ) const;
	
	/** \brief Index of sky or -1 if absent. */
	int IndexOf( gdeSky *sky ) const;
	
	/** \brief Index of  sky with path or -1 if absent. */
	int IndexOfWithPath( const char *path ) const;
	
	/** \brief Sky is present. */
	bool Has( gdeSky *sky ) const;
	
	/** \brief Sky with path is present. */
	bool HasWithPath( const char *path ) const;
	
	/** \brief Add sky. */
	void Add( gdeSky *sky );
	
	/** \brief Remove sky. */
	void Remove( gdeSky *sky );
	
	/** \brief Remove all skys. */
	void RemoveAll();
	
	/** \brief Set from another sky list. */
	gdeSkyList &operator=( const gdeSkyList &list );
	/*@}*/
};

#endif
