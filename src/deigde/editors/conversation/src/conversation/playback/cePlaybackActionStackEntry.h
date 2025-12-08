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

#ifndef _CEPLAYBACKACTIONSTACKENTRY_H_
#define _CEPLAYBACKACTIONSTACKENTRY_H_

#include "../action/ceConversationAction.h"

class ceConversationActionList;
class ceConversationTopic;
class ceConversationCondition;



/**
 * \brief Playback Action Stack Entry.
 * Entry in a playback action stack. Stores the action, the position of the action in
 * the parent action list (previous stack entry if existing) as well as the action
 * list being processed. Stack entries are mutable.
 */
class cePlaybackActionStackEntry{
private:
	ceConversationTopic::Ref pParentTopic;
	ceConversationAction::Ref pParentAction;
	const ceConversationActionList *pParentList;
	
	ceConversationCondition::Ref pLoopCondition;
	bool pLooping;
	
	ceConversationAction::Ref pNextAction;
	int pNextIndex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new stack. */
	cePlaybackActionStackEntry();
	/** Cleans up the stack. */
	~cePlaybackActionStackEntry();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent topic or NULL if there is none. */
	inline ceConversationTopic *GetParentTopic() const{ return pParentTopic; }
	/** Sets the parent topic or NULL if there is none. */
	void SetParentTopic(ceConversationTopic::Ref topic);
	/** Retrieves the parent action or NULL if there is none. */
	inline ceConversationAction *GetParentAction() const{ return pParentAction; }
	/** Sets the parent action or NULL if there is none. */
	void SetParentAction(ceConversationAction *action);
	/** Retrieves the parent action list or NULL if there is none. */
	inline const ceConversationActionList *GetParentList() const{ return pParentList; }
	/** Sets the parent action list or NULL if there is none. */
	void SetParentList(const ceConversationActionList *list);
	
	/** \brief Retrieves the loop condition or NULL if always true. */
	inline ceConversationCondition *GetLoopCondition() const{ return pLoopCondition; }
	/** \brief Sets the loop condition or NULL if always true. */
	void SetLoopCondition(ceConversationCondition::Ref condition);
	/** \brief Determines if the list of actions is looped. */
	inline bool GetLooping() const{ return pLooping; }
	/** \brief Sets if the list of actions is looped. */
	void SetLooping(bool looping);
	
	/** Retrieves the next action index. */
	inline int GetNextIndex() const{ return pNextIndex; }
	/** Sets the next action index and sets the next action itself if possible. */
	void SetNextIndex(int index);
	/** Advances the next index if not at the end of the list and sets the next action itself if possible. */
	void AdvanceIndex();
	/** Retrieves the next action or NULL if at the end of the current list. */
	inline ceConversationAction *GetNextAction() const{ return pNextAction; }
	/** Determines if there is a next action or false if at the end of the list. */
	bool HasNextAction() const;
	
	/** Clears the entry. */
	void Clear();
	/** \brief Forward to last action. */
	void ForwardLast();
	/** \brief Forward to end. */
	void ForwardEnd();
	
	/** \brief Cancel looping. */
	void CancelLooping();
	/*@}*/
};

#endif
