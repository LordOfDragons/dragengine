/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SESKYCONTROLLERLIST_H_
#define _SESKYCONTROLLERLIST_H_

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
	
	/** \brief Clean up controller list. */
	~seControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/** \brief Controller at index. */
	seController *GetAt( int index ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOf( seController *controller ) const;
	
	/** \brief Controller is present. */
	bool Has( seController *controller ) const;
	
	/** \brief Add controller. */
	void Add( seController *controller );
	
	/** \brief Insert controller. */
	void InsertAt( seController *controller, int index );
	
	/** \brief Move controller. */
	void MoveTo( seController *controller, int index );
	
	/** \brief Remove controller. */
	void Remove( seController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	seControllerList &operator=( const seControllerList &list );
	/*@}*/
};

#endif
