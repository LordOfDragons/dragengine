/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CECOORDSYSTEMLIST_H_
#define _CECOORDSYSTEMLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceCoordSystem;



/**
 * \brief Conversation coordinate system list.
 */
class ceCoordSystemList{
private:
	decObjectOrderedSet pCoordSystems;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new coordinate system list. */
	ceCoordSystemList();
	/** \brief Cleans up the coordinate system list. */
	~ceCoordSystemList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of coordinate systems. */
	int GetCount() const;
	/** \brief Retrieves the coordinate system at the given position. */
	ceCoordSystem *GetAt( int index ) const;
	/** \brief Retrieves the coordinate system with the given id or NULL if not found. */
	ceCoordSystem *GetWithID( const char *id ) const;
	/** \brief Retrieves the coordinate system with the given alias id or NULL if not found. */
	ceCoordSystem *GetWithAliasID( const char *id ) const;
	/** \brief Retrieves the coordinate system with the given id or alias id or NULL if not found. */
	ceCoordSystem *GetWithIDOrAliasID( const char *id ) const;
	/** \brief Retrieves the index of the given coordinate system or -1 if not found. */
	int IndexOf( ceCoordSystem *coordSystem ) const;
	/** \brief Retrieves the index of the coordinate system with the given id or -1 if not found. */
	int IndexWithID( const char *id ) const;
	/** \brief Retrieves the index of the coordinate system with the given alias id or -1 if not found. */
	int IndexWithAliasID( const char *id ) const;
	/** \brief Retrieves the index of the coordinate system with the given id or alias id or -1 if not found. */
	int IndexWithIDOrAliasID( const char *id ) const;
	/** \brief Determines if a coordinate system exists. */
	bool Has( ceCoordSystem *coordSystem ) const;
	/** \brief Determines if a coordinate system with the given id exists. */
	bool HasWithID( const char *id ) const;
	/** \brief Determines if a coordinate system with the given alias id exists. */
	bool HasWithAliasID( const char *id ) const;
	/** \brief Determines if a coordinate system with the given id or alias id exists. */
	bool HasWithIDOrAliasID( const char *id ) const;
	/** \brief Adds a coordinate system. */
	void Add( ceCoordSystem *coordSystem );
	/** \brief Inserts a new coordinate system. */
	void InsertAt( ceCoordSystem *coordSystem, int index );
	/** \brief Moves a coordinate system to a new position. */
	void MoveTo( ceCoordSystem *coordSystem, int index );
	/** \brief Removes a coordinate system. */
	void Remove( ceCoordSystem *coordSystem );
	/** \brief Removes all coordinate systems. */
	void RemoveAll();
	
	/** \brief Sets the list from another list. */
	ceCoordSystemList &operator=( const ceCoordSystemList &list );
	/*@}*/
};

#endif
