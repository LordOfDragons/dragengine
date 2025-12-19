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

#ifndef _CECAPLAYERCHOICE_H_
#define _CECAPLAYERCHOICE_H_

#include "ceConversationAction.h"
#include "ceCAPlayerChoiceOption.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Player Choice Conversation Action.
 * Displays to the player a list of choices. Upon selecting one choice the
 * matching list of actions is processed. Optionally the choice can be stored
 * in a variable for later usage.
 */
class ceCAPlayerChoice : public ceConversationAction{
private:
	ceCAPlayerChoiceOption::List pOptions;
	ceConversationAction::List pActions;
	decString pVariableName;
	bool pTIMExpanded;
	bool pTIMActionsExpanded;
	
	
	
public:
	typedef deTObjectReference<ceCAPlayerChoice> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new player choice conversation action. */
	ceCAPlayerChoice();
	/** Creates a new player choice conversation action. */
	ceCAPlayerChoice(const ceCAPlayerChoice &action);
	/** Cleans up the player choice conversation action. */
	~ceCAPlayerChoice() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of options. */
	inline ceCAPlayerChoiceOption::List &GetOptions(){ return pOptions; }
	inline const ceCAPlayerChoiceOption::List &GetOptions() const{ return pOptions; }
	/** Retrieves the list of actions. */
	inline ceConversationAction::List &GetActions(){ return pActions; }
	inline const ceConversationAction::List &GetActions() const{ return pActions; }
	/** Create a copy of this action. */
    ceConversationAction::Ref CreateCopy() const override;
	/** Retrieves the variable name. */
	inline const decString &GetVariableName() const{ return pVariableName; }
	/** Sets the variable name. */
	void SetVariableName(const char *name);
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded(bool expanded);
	
	/** \brief Actions tree list item expanded state. */
	inline bool GetTIMActionsExpanded() const{ return pTIMActionsExpanded; }
	
	/** \brief Set actions tree list item expanded state. */
	void SetTIMActionsExpanded(bool expanded);
	/*@}*/
};

#endif
