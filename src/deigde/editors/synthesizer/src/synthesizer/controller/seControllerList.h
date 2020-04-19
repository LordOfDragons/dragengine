/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SECONTROLLERLIST_H_
#define _SECONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seController;



/**
 * \brief Controller list.
 */
class seControllerList{
private:
	decObjectOrderedSet pControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller list. */
	seControllerList();
	
	/** \brief Create copy of controller list. */
	seControllerList( const seControllerList &copy );
	
	/** \brief Clean up controller list. */
	~seControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seController *GetAt( int index ) const;
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seController *GetNamed( const char *name ) const;
	
	/**
	 * \brief Index of controller or -1 if not present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	int IndexOf( seController *controller ) const;
	
	/**
	 * \brief Determine if a controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	bool Has( seController *controller ) const;
	
	/**
	 * \brief Determine if a controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	bool HasNamed( const char *name ) const;
	
	/**
	 * \brief Add controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	void Add( seController *controller );
	
	/**
	 * \brief Insert controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( seController *controller, int index );
	
	/**
	 * \brief Move controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( seController *controller, int index );
	
	/**
	 * \brief Remove controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em controller is not present in the list.
	 */
	void Remove( seController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller list to this controller list. */
	seControllerList &operator=( const seControllerList &list );
	
	/** \brief Add controllers from controller list if not present in this controller list. */
	seControllerList &operator+=( const seControllerList &list );
	/*@}*/
};

#endif
