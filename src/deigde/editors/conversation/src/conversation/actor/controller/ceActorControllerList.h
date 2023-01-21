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
 * Actor controller list.
 */
class ceActorControllerList{
private:
	decObjectOrderedSet pControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller list. */
	ceActorControllerList();
	
	/** Create copy of controller list. */
	ceActorControllerList( const ceActorControllerList &list );
	
	/** Clean up controller list. */
	~ceActorControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of controllers. */
	int GetCount() const;
	
	/** Controller at index. */
	ceActorController *GetAt( int index ) const;
	
	/** Named controller or nullptr. */
	ceActorController *GetNamed( const char *name ) const;
	
	/** Index of controller or -1 if absent. */
	int IndexOf( ceActorController *controller ) const;
	
	/** Controller is present. */
	bool Has( ceActorController *controller ) const;
	
	/** Named controller is present. */
	bool HasNamed( const char *name ) const;
	
	/** Add controller. */
	void Add( ceActorController *controller );
	
	/** Remove controller. */
	void Remove( ceActorController *controller );
	
	/** Remove all controller. */
	void RemoveAll();
	
	/** Set from another list. */
	ceActorControllerList &operator=( const ceActorControllerList &list );
	/*@}*/
};

#endif
