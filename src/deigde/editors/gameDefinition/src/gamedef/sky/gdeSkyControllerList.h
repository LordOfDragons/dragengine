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

#ifndef _GDESKYCONTROLLERLIST_H_
#define _GDESKYCONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeSkyController;



/**
 * \brief Sky controller list.
 */
class gdeSkyControllerList{
private:
	decObjectOrderedSet pControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller list. */
	gdeSkyControllerList();
	
	/** \brief Create copy of controller list. */
	gdeSkyControllerList( const gdeSkyControllerList &list );
	
	/** \brief Clean up controller list. */
	~gdeSkyControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/** \brief Controller at position. */
	gdeSkyController *GetAt( int index ) const;
	
	/** \brief Named controller or \em NULL. */
	gdeSkyController *GetNamed( const char *name ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOf( gdeSkyController *controller ) const;
	
	/** \brief Controller is present. */
	bool Has( gdeSkyController *controller ) const;
	
	/** \brief Named controller is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add controller. */
	void Add( gdeSkyController *controller );
	
	/** \brief Remove controller. */
	void Remove( gdeSkyController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAll();
	
	/** \brief Set from another controller list. */
	gdeSkyControllerList &operator=( const gdeSkyControllerList &list );
	/*@}*/
};

#endif
