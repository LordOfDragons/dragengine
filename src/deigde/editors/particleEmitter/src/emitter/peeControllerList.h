/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEECONTROLLERLIST_H_
#define _PEECONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class peeController;



/**
 * \brief Emitter Controller List.
 */
class peeControllerList{
private:
	decObjectOrderedSet pControllers;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new controller list. */
	peeControllerList();
	/** \brief Clean up the controller list. */
	~peeControllerList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of controllers. */
	int GetCount() const;
	/** Retrieves the controller at the given position. */
	peeController *GetAt( int index ) const;
	/** Retrieves the controller with the given name or NULL if not found. */
	peeController *GetNamed( const char *name ) const;
	/** Retrieves the index of the given controller or -1 if not found. */
	int IndexOf( peeController *controller ) const;
	/** Retrieves the index of the controller with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a controller exists. */
	bool Has( peeController *controller ) const;
	/** Determines if a controller with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a controller. */
	void Add( peeController *controller );
	/** Inserts a new controller. */
	void InsertAt( peeController *controller, int index );
	/** Moves a controller to a new position. */
	void MoveTo( peeController *controller, int index );
	/** Removes a controller. */
	void Remove( peeController *controller );
	/** Removes all controllers. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	peeControllerList &operator=( const peeControllerList &list );
	/*@}*/
};

#endif
