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

#ifndef _CEWPTMAIFELSECASEMOVEACTION_H_
#define _CEWPTMAIFELSECASEMOVEACTION_H_

#include "../../ceWPTMenuAction.h"

class ceConversation;
class ceConversationAction;
class ceConversationTopic;
class ceCAIfElse;
class ceCAIfElseCase;



/**
 * \brief Menu action remove action to topic.
 */
class ceWPTMAIfElseCaseMoveAction : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceCAIfElse *pIfElse;
	ceCAIfElseCase *pIfCase;
	ceConversationAction *pAction;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAIfElseCaseMoveAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create menu action. */
	ceWPTMAIfElseCaseMoveAction( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, ceCAIfElse &ifElse,
		ceCAIfElseCase &ifCase, ceConversationAction *action, int index,
		const char *text, igdeIcon *icon );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief Action. */
	inline ceConversationAction *GetAction() const{ return pAction; }
	
	/** \brief If-else action. */
	inline ceCAIfElse *GetIfElse() const{ return pIfElse; }
	
	/** \brief If-case. */
	inline ceCAIfElseCase *GetIfCase() const{ return pIfCase; }
	
	/** \brief Index of new position. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
