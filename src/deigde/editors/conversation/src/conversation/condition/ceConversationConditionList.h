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

#ifndef _CECONVERSATIONCONDITIONLIST_H_
#define _CECONVERSATIONCONDITIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversationCondition;



/**
 * \brief Conversation Condition List.
 */
class ceConversationConditionList{
private:
	decObjectOrderedSet pConditions;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new condition list. */
	ceConversationConditionList();
	/** Cleans up the condition list. */
	~ceConversationConditionList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of conditions. */
	int GetCount() const;
	/** Retrieves the condition at the given position. */
	ceConversationCondition *GetAt( int index ) const;
	/** Retrieves the index of the given condition or -1 if not found. */
	int IndexOf( ceConversationCondition *condition ) const;
	/** Determines if a condition exists. */
	bool Has( ceConversationCondition *condition ) const;
	/** Adds a condition. */
	void Add( ceConversationCondition *condition );
	/** Inserts a new condition. */
	void InsertAt( ceConversationCondition *condition, int index );
	/** Moves a condition to a new position. */
	void MoveTo( ceConversationCondition *condition, int index );
	/** Removes a condition. */
	void Remove( ceConversationCondition *condition );
	/** Removes all conditions. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceConversationConditionList &operator=( const ceConversationConditionList &list );
	/*@}*/
};

#endif
