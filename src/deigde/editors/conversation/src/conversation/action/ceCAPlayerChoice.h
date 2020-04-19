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

#ifndef _CECAPLAYERCHOICE_H_
#define _CECAPLAYERCHOICE_H_

#include "ceConversationAction.h"
#include "ceCAPlayerChoiceOptionList.h"
#include "ceConversationActionList.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Player Choice Conversation Action.
 * Displays to the player a list of choices. Upon selecting one choice the
 * matching list of actions is processed. Optionally the choice can be stored
 * in a variable for later usage.
 */
class ceCAPlayerChoice : public ceConversationAction{
private:
	ceCAPlayerChoiceOptionList pOptions;
	ceConversationActionList pActions;
	decString pVariableName;
	bool pTIMExpanded;
	bool pTIMActionsExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new player choice conversation action. */
	ceCAPlayerChoice();
	/** Creates a new player choice conversation action. */
	ceCAPlayerChoice( const ceCAPlayerChoice &action );
	/** Cleans up the player choice conversation action. */
	virtual ~ceCAPlayerChoice();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of options. */
	inline ceCAPlayerChoiceOptionList &GetOptions(){ return pOptions; }
	inline const ceCAPlayerChoiceOptionList &GetOptions() const{ return pOptions; }
	/** Retrieves the list of actions. */
	inline ceConversationActionList &GetActions(){ return pActions; }
	inline const ceConversationActionList &GetActions() const{ return pActions; }
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/** Retrieves the variable name. */
	inline const decString &GetVariableName() const{ return pVariableName; }
	/** Sets the variable name. */
	void SetVariableName( const char *name );
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded( bool expanded );
	
	/** \brief Actions tree list item expanded state. */
	inline bool GetTIMActionsExpanded() const{ return pTIMActionsExpanded; }
	
	/** \brief Set actions tree list item expanded state. */
	void SetTIMActionsExpanded( bool expanded );
	/*@}*/
};

#endif
