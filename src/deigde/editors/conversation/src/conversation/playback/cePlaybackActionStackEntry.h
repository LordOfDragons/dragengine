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

#ifndef _CEPLAYBACKACTIONSTACKENTRY_H_
#define _CEPLAYBACKACTIONSTACKENTRY_H_

#include "../action/ceConversationActionReference.h"

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
	ceConversationTopic *pParentTopic;
	ceConversationActionReference pParentAction;
	const ceConversationActionList *pParentList;
	
	ceConversationCondition *pLoopCondition;
	bool pLooping;
	
	ceConversationActionReference pNextAction;
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
	void SetParentTopic( ceConversationTopic *topic );
	/** Retrieves the parent action or NULL if there is none. */
	inline ceConversationAction *GetParentAction() const{ return pParentAction; }
	/** Sets the parent action or NULL if there is none. */
	void SetParentAction( ceConversationAction *action );
	/** Retrieves the parent action list or NULL if there is none. */
	inline const ceConversationActionList *GetParentList() const{ return pParentList; }
	/** Sets the parent action list or NULL if there is none. */
	void SetParentList( const ceConversationActionList *list );
	
	/** \brief Retrieves the loop condition or NULL if always true. */
	inline ceConversationCondition *GetLoopCondition() const{ return pLoopCondition; }
	/** \brief Sets the loop condition or NULL if always true. */
	void SetLoopCondition( ceConversationCondition *condition );
	/** \brief Determines if the list of actions is looped. */
	inline bool GetLooping() const{ return pLooping; }
	/** \brief Sets if the list of actions is looped. */
	void SetLooping( bool looping );
	
	/** Retrieves the next action index. */
	inline int GetNextIndex() const{ return pNextIndex; }
	/** Sets the next action index and sets the next action itself if possible. */
	void SetNextIndex( int index );
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
