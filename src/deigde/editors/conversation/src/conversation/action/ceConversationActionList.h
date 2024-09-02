/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
