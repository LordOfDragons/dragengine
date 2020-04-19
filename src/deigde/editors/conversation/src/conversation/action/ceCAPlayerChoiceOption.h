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

#ifndef _CECAPLAYERCHOICEOPTION_H_
#define _CECAPLAYERCHOICEOPTION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#include "ceConversationActionList.h"

class ceConversationCondition;


/**
 * \brief Player choice option.
 */
class ceCAPlayerChoiceOption : public deObject{
private:
	decUnicodeString pText;
	ceConversationCondition *pCondition;
	ceConversationActionList pActions;
	bool pTIMExpanded;
	bool pTIMConditionExpanded;
	bool pTIMActionsExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create player choice option. */
	ceCAPlayerChoiceOption();
	
	/** \brief Create copy of player choice option. */
	ceCAPlayerChoiceOption( const ceCAPlayerChoiceOption &option );
	
protected:
	/** \brief Clean up player choice option. */
	virtual ~ceCAPlayerChoiceOption();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Display text. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** \brief Set display text. */
	void SetText( const decUnicodeString &text );
	
	/** \brief Condition or \em NULL if there is none. */
	inline ceConversationCondition *GetCondition() const{ return pCondition; }
	
	/** \brief Set condition or \em NULL if there is none. */
	void SetCondition( ceConversationCondition *condition );
	
	/** \brief Actions. */
	inline ceConversationActionList &GetActions(){ return pActions; }
	inline const ceConversationActionList &GetActions() const{ return pActions; }
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
