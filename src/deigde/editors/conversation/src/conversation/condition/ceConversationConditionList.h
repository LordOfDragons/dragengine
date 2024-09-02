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
