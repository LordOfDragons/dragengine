/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AECONTROLLERLIST_H_
#define _AECONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class aeController;



/**
 * \brief Controller List.
 */
class aeControllerList{
private:
	decObjectOrderedSet pControllers;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new controller list. */
	aeControllerList();
	
	/** \brief Create copy of a controller list. */
	aeControllerList( const aeControllerList &copy );
	
	/** \brief Clean up controller list. */
	~aeControllerList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	aeController *GetAt( int index ) const;
	
	/** \brief Named controller. */
	aeController *GetNamed( const char *name ) const;
	
	/**
	 * \brief Index of controller or -1 if not present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	int IndexOf( aeController *controller ) const;
	
	/**
	 * \brief Index of named controller or -1 if not present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	int IndexOfNamed( const char *name ) const;
	
	/**
	 * \brief Determine if a controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	bool Has( aeController *controller ) const;
	
	/**
	 * \brief Determine if named controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	bool HasNamed( const char *name ) const;
	
	/**
	 * \brief Add controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	void Add( aeController *controller );
	
	/**
	 * \brief Insert controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( aeController *controller, int index );
	
	/**
	 * \brief Move controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( aeController *controller, int index );
	
	/**
	 * \brief Remove controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em controller is not present in the list.
	 */
	void Remove( aeController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller list to this controller list. */
	aeControllerList &operator=( const aeControllerList &list );
	
	/** \brief Add controllers from controller list if not present in this controller list. */
	aeControllerList &operator+=( const aeControllerList &list );
	/*@}*/
};

#endif
