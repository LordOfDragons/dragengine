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

#ifndef _CEUNDOCACTIONLIST_H_
#define _CEUNDOCACTIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceUndoCAction;



/**
 * \brief Undo Conversation Action List.
 */
class ceUndoCActionList{
private:
	decObjectOrderedSet pActions;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new action list. */
	ceUndoCActionList();
	/** \brief Cleans up the action list. */
	~ceUndoCActionList();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of actions. */
	int GetCount() const;
	/** \brief Retrieves the action at the given position. */
	ceUndoCAction *GetAt( int index ) const;
	/** \brief Retrieves the index of the given action or -1 if not found. */
	int IndexOf( ceUndoCAction *action ) const;
	/** \brief Determines if a action exists. */
	bool Has( ceUndoCAction *action ) const;
	/** \brief Adds a action. */
	void Add( ceUndoCAction *action );
	/** \brief Removes a action. */
	void Remove( ceUndoCAction *action );
	/** \brief Removes all actions. */
	void RemoveAll();
	
	/** \brief Sets the list from another list. */
	ceUndoCActionList &operator=( const ceUndoCActionList &list );
	/*@}*/
};

#endif
