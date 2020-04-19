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

#ifndef _CEWPTMAIFELSEELSECUTACTION_H_
#define _CEWPTMAIFELSEELSECUTACTION_H_
	
#include "../ceWPTMACopyAction.h"

class ceConversation;
class ceConversationTopic;
class ceCAIfElse;



/**
 * \brief Menu action cut action to topic.
 */
class ceWPTMAIfElseElseCutAction : public ceWPTMACopyAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceCAIfElse *pIfElse;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAIfElseElseCutAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAIfElseElseCutAction( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, ceCAIfElse &ifElse,
		ceConversationAction *action );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	/** \brief Topic. */
	inline ceConversationTopic &GetTopic() const{ return *pTopic; }
	
	/** \brief If-else action. */
	inline ceCAIfElse *GetIfElse() const{ return pIfElse; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
