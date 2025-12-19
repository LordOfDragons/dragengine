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

#ifndef _CECAWAIT_H_
#define _CECAWAIT_H_

#include "ceConversationAction.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/string/decString.h>




/**
 * \brief Wait Conversation Action.
 * Waits for a condition to becomes false. While the condition is true an optional lists
 * of actions is processed in a loop. As soon as the condition becomes false the loop is
 * broken out of. Hence the actions in the action list have to be chosen carefully to not
 * stall the system. In general the actions have to be chosen in a way that the result is
 * sane no matter after which action the processing stops. Blocking actions like the player
 * choice action requiring user input should be avoided unless the player is allowed to stop
 * the waiting intentionally after a given amount of time waiting. If the condition is
 * missing it is assumed to be false hence this action is skipped. The default test
 * interval is 0 seconds.
 */
class ceCAWait : public ceConversationAction{
private:
	ceConversationAction::List pActions;
	ceConversationCondition::Ref pCondition;
	float pInterval;
	bool pTIMExpanded;
	bool pTIMConditionExpanded;
	bool pTIMActionsExpanded;
	
	
	
public:
	typedef deTObjectReference<ceCAWait> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new wait conversation action. */
	ceCAWait();
	/** Creates a new wait conversation action. */
	ceCAWait(const ceCAWait &action);
	/** Cleans up the wait conversation action. */
	~ceCAWait() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of actions. */
	inline ceConversationAction::List &GetActions(){ return pActions; }
	inline const ceConversationAction::List &GetActions() const{ return pActions; }
	/** Retrieves the condition or nullptr if there is none. */
	inline const ceConversationCondition::Ref &GetCondition() const{ return pCondition; }
	/** Sets the condition or nullptr if there is none. */
	void SetCondition(ceConversationCondition *condition);
	/** Retrieves the interval to check the condition. */
	inline float GetInterval() const{ return pInterval; }
	/** Sets the interval to check the condition. */
	void SetInterval(float interval);
	
	/** Create a copy of this action. */
	ceConversationAction::Ref CreateCopy() const override;
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded(bool expanded);
	
	/** \brief Condition tree list item expanded state. */
	inline bool GetTIMConditionExpanded() const{ return pTIMConditionExpanded; }
	
	/** \brief Set condition tree list item expanded state. */
	void SetTIMConditionExpanded(bool expanded);
	
	/** \brief Actions tree list item expanded state. */
	inline bool GetTIMActionsExpanded() const{ return pTIMActionsExpanded; }
	
	/** \brief Set actions tree list item expanded state. */
	void SetTIMActionsExpanded(bool expanded);
	/*@}*/
};

#endif
