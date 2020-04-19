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

#ifndef _CECAWAIT_H_
#define _CECAWAIT_H_

#include "ceConversationAction.h"
#include "ceConversationActionList.h"

#include <dragengine/common/string/decString.h>

class ceConversationCondition;



/**
 * \brief Wait Conversation Action.
 * Waits for a condition to becomes false. To avoid hammering the update interval is specified
 * in seconds. While the condition is true an optional lists of actions is processed in
 * a loop. As soon as the condition becomes false the loop is broken out of. Hence the actions
 * in the action list have to be chosen carefully to not stall the system. In general the
 * actions have to be chosen in a way that the result is sane no matter after which action
 * the processing stops. Blocking actions like the player choice action requiring user input
 * should be avoided unless the player is allowed to stop the waiting intentionally after a
 * given amount of time waiting. If the condition is missing it is assumed to be false hence
 * this action is skipped. The default test interval is 0.2 seconds.
 */
class ceCAWait : public ceConversationAction{
private:
	ceConversationActionList pActions;
	ceConversationCondition *pCondition;
	float pInterval;
	bool pTIMExpanded;
	bool pTIMConditionExpanded;
	bool pTIMActionsExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new wait conversation action. */
	ceCAWait();
	/** Creates a new wait conversation action. */
	ceCAWait( const ceCAWait &action );
	/** Cleans up the wait conversation action. */
	virtual ~ceCAWait();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of actions. */
	inline ceConversationActionList &GetActions(){ return pActions; }
	inline const ceConversationActionList &GetActions() const{ return pActions; }
	/** Retrieves the condition or NULL if there is none. */
	inline ceConversationCondition *GetCondition() const{ return pCondition; }
	/** Sets the condition or NULL if there is none. */
	void SetCondition( ceConversationCondition *condition );
	/** Retrieves the interval to check the condition. */
	inline float GetInterval() const{ return pInterval; }
	/** Sets the interval to check the condition. */
	void SetInterval( float interval );
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded( bool expanded );
	
	/** \brief Condition tree list item expanded state. */
	inline bool GetTIMConditionExpanded() const{ return pTIMConditionExpanded; }
	
	/** \brief Set condition tree list item expanded state. */
	void SetTIMConditionExpanded( bool expanded );
	
	/** \brief Actions tree list item expanded state. */
	inline bool GetTIMActionsExpanded() const{ return pTIMActionsExpanded; }
	
	/** \brief Set actions tree list item expanded state. */
	void SetTIMActionsExpanded( bool expanded );
	/*@}*/
};

#endif
