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

#ifndef _IGDEGDSKYLIST_H_
#define _IGDEGDSKYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDSky;



/**
 * \brief Game definition sky list.
 */
class igdeGDSkyList{
private:
	decObjectOrderedSet pSkies;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDSkyList();
	
	/** \brief Clean up list. */
	~igdeGDSkyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of skies. */
	int GetCount() const;
	
	/** \brief Sky by index. */
	igdeGDSky *GetAt( int index ) const;
	
	/** \brief Sky with path or NULL if absent. */
	igdeGDSky *GetWithPath( const char *path ) const;
	
	/** \brief Sky with name or NULL if absent. */
	igdeGDSky *GetWithName( const char *name ) const;
	
	/** \brief Index of sky or -1 if absent. */
	int IndexOf( igdeGDSky *sky ) const;
	
	/** \brief Index of sky with path or -1 if absent. */
	int IndexOfWithPath( const char *path ) const;
	
	/** \brief Index of sky with name or -1 if absent. */
	int IndexOfWithName( const char *name ) const;
	
	/** \brief Index of sky with name or -1 if absent. */
	int IndexOfWithPathOrName( const char *path, const char *name ) const;
	
	/** \brief Sky is present. */
	bool Has( igdeGDSky *sky ) const;
	
	/** \brief Sky with path is present. */
	bool HasWithPath( const char *path ) const;
	
	/** \brief Sky with name is present. */
	bool HasWithName( const char *name ) const;
	
	/** \brief Sky with name or path is present. */
	bool HasWithPathOrName( const char *path, const char *name ) const;
	
	/** \brief Add sky. */
	void Add( igdeGDSky *sky );
	
	/** \brief Insert sky. */
	void InsertAt( igdeGDSky *sky, int index );
	
	/** \brief Move sky to new position. */
	void MoveTo( igdeGDSky *sky, int index );
	
	/** \brief Remove sky. */
	void Remove( igdeGDSky *sky );
	
	/** \brief Remove all skies. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	igdeGDSkyList &operator=( const igdeGDSkyList &list );
	/*@}*/
};

#endif
