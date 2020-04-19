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

#ifndef _CEPLAYBACKACTIONSTACK_H_
#define _CEPLAYBACKACTIONSTACK_H_

class ceConversationTopic;
class ceConversationAction;
class ceConversationActionList;
class cePlaybackActionStackEntry;



/**
 * \brief Playback Action Stack.
 * List of actions where the playback jumped to a different action list. Each entry
 * stores the action, the position of the action in the parent action list (previous
 * stack entry if existing) as well as the action list being processed. The stack is
 * maintained as a growing list to avoid ongoing memory (de)allocation.
 */
class cePlaybackActionStack{
private:
	cePlaybackActionStackEntry *pEntries;
	int pEntryCount;
	int pEntrySize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new stack. */
	cePlaybackActionStack();
	/** Cleans up the stack. */
	~cePlaybackActionStack();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the size of the stack. */
	inline int GetStackSize() const{ return pEntrySize; }
	/** Sets the size of the stack. */
	void SetStackSize( int size );
	/** Retrieves the number of entries in the stack. */
	inline int GetCount() const{ return pEntryCount; }
	/** Determines if the stack is empty. */
	inline bool IsEmpty() const{ return pEntryCount == 0; }
	/** Determines if the stack is not empty. */
	inline bool IsNotEmpty() const{ return pEntryCount > 0; }
	/** Retrieves the top entry. */
	cePlaybackActionStackEntry &GetTop() const;
	/** Retrieves the entry from the given position. */
	cePlaybackActionStackEntry &GetAt( int position ) const;
	/** Pushes an entry to the top of the stack. */
	void Push( ceConversationTopic *topic, ceConversationAction *action,
		const ceConversationActionList *list, int index );
	/** Pops the entry from the top of the stack. */
	void Pop();
	/** Determines the next entry which is NULL if the stack is empty or GetNextAction from GetTop. */
	bool HasNextAction() const;
	/** Clears the stack. */
	void Clear();
	/*@}*/
};

#endif
