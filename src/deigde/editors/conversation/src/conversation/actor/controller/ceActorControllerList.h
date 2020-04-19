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

#ifndef _CEACTORCONTROLLERLIST_H_
#define _CEACTORCONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorController;



/**
 * \brief Actor controller list.
 */
class ceActorControllerList{
private:
	decObjectOrderedSet pControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller list. */
	ceActorControllerList();
	
	/** \brief Create copy of controller list. */
	ceActorControllerList( const ceActorControllerList &list );
	
	/** \brief Clean up controller list. */
	~ceActorControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/** \brief Controller at index. */
	ceActorController *GetAt( int index ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOf( ceActorController *controller ) const;
	
	/** \brief Controller is present. */
	bool Has( ceActorController *controller ) const;
	
	/** \brief Add controller. */
	void Add( ceActorController *controller );
	
	/** \brief Remove controller. */
	void Remove( ceActorController *controller );
	
	/** \brief Remove all controller. */
	void RemoveAll();
	
	/** \brief Set from another list. */
	ceActorControllerList &operator=( const ceActorControllerList &list );
	/*@}*/
};

#endif
