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

#ifndef _CEPLAYBACKACTIONSTACK_H_
#define _CEPLAYBACKACTIONSTACK_H_

#include <dragengine/deObject.h>

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
class cePlaybackActionStack : public deObject{
public:
	typedef deTObjectReference<cePlaybackActionStack> Ref;
	
private:
	cePlaybackActionStackEntry *pEntries;
	int pEntryCount;
	int pEntrySize;
	bool pActionWaiting;
	float pActionTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new stack. */
	cePlaybackActionStack();
	/** Cleans up the stack. */
	~cePlaybackActionStack() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline bool GetActionWaiting() const{ return pActionWaiting; }
	void SetActionWaiting(bool actionWaiting);
	
	inline float GetActionTime() const{ return pActionTime; }
	void SetActionTime(float time);
	
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
