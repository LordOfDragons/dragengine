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

#ifndef _CEWPTMAIFELSECASEPASTECONDITION_H_
#define _CEWPTMAIFELSECASEPASTECONDITION_H_

#include "../../condition/ceWPTMAPasteCondition.h"

class ceConversationTopic;
class ceCAIfElse;
class ceCAIfElseCase;


/**
 * \brief Menu action paste condition to wait.
 */
class ceWPTMAIfElseCasePasteCondition : public ceWPTMAPasteCondition{
private:
	ceConversationTopic *pTopic;
	ceCAIfElse *pIfElse;
	ceCAIfElseCase *pIfCase;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAIfElseCasePasteCondition();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAIfElseCasePasteCondition( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceCAIfElse &ifElse, ceCAIfElseCase &option );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief If-else action. */
	inline ceCAIfElse *GetPlayerChoice() const{ return pIfElse; }
	
	/** \brief If-case. */
	inline ceCAIfElseCase *GetOption() const{ return pIfCase; }
	
	/** \brief Create undo action for pasting condition. */
	virtual igdeUndo *CreateUndo( const ceConversationConditionList &conditions );
	/*@}*/
};

#endif
