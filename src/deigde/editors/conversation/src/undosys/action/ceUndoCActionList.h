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
