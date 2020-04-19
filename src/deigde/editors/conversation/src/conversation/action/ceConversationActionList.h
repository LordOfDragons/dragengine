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

#ifndef _CECONVERSATIONACTIONLIST_H_
#define _CECONVERSATIONACTIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversationAction;



/**
 * \brief Conversation Action List.
 */
class ceConversationActionList{
private:
	decObjectOrderedSet pActions;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new action list. */
	ceConversationActionList();
	/** Cleans up the action list. */
	~ceConversationActionList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of actions. */
	int GetCount() const;
	/** Retrieves the action at the given position. */
	ceConversationAction *GetAt( int index ) const;
	/** Retrieves the index of the given action or -1 if not found. */
	int IndexOf( ceConversationAction *action ) const;
	/** Determines if a action exists. */
	bool Has( ceConversationAction *action ) const;
	/** Adds a action. */
	void Add( ceConversationAction *action );
	/** Inserts a new action. */
	void InsertAt( ceConversationAction *action, int index );
	/** Moves a action to a new position. */
	void MoveTo( ceConversationAction *action, int index );
	/** Removes a action. */
	void Remove( ceConversationAction *action );
	/** Removes all actions. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceConversationActionList &operator=( const ceConversationActionList &list );
	/*@}*/
};

#endif
