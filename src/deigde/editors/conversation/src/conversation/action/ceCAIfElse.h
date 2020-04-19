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

#ifndef _CECAIFELSE_H_
#define _CECAIFELSE_H_

#include "ceConversationAction.h"
#include "ceConversationActionList.h"
#include "ceCAIfElseCaseList.h"

#include <dragengine/common/string/decString.h>

class ceConversationCondition;



/**
 * \brief If-Else Conversation Action.
 * 
 * Process one of two lists of actions depending on a condition. If the condition
 * holds true the if list of actions is processed otherwise the else list of
 * actions.
 * 
 */
class ceCAIfElse : public ceConversationAction{
private:
	ceCAIfElseCaseList pCases;
	ceConversationActionList pElseActions;
	bool pTIMExpanded;
	bool pTIMElseExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create if-else conversation action. */
	ceCAIfElse();
	
	/** \brief Create copy of if-else conversation action. */
	ceCAIfElse( const ceCAIfElse &action );
	
	/** \brief Clean up if-else conversation action. */
	virtual ~ceCAIfElse();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of cases. */
	inline ceCAIfElseCaseList &GetCases(){ return pCases; }
	inline const ceCAIfElseCaseList &GetCases() const{ return pCases; }
	
	/** \brief List of else-actions. */
	inline ceConversationActionList &GetElseActions(){ return pElseActions; }
	inline const ceConversationActionList &GetElseActions() const{ return pElseActions; }
	
	/** \brief Create copy of action. */
	virtual ceConversationAction *CreateCopy() const;
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded( bool expanded );
	
	/** \brief Else tree list item expanded state. */
	inline bool GetTIMElseExpanded() const{ return pTIMElseExpanded; }
	
	/** \brief Set else tree list item expanded state. */
	void SetTIMElseExpanded( bool expanded );
	/*@}*/
};

#endif
