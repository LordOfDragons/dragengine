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

#ifndef _CEWPTMAPCHOICEOPTIONCUTACTION_H_
#define _CEWPTMAPCHOICEOPTIONCUTACTION_H_
	
#include "../ceWPTMACopyAction.h"

class ceConversation;
class ceConversationTopic;
class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;



/**
 * \brief Menu action cut action to topic.
 */
class ceWPTMAPChoiceOptionCutAction : public ceWPTMACopyAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceCAPlayerChoice *pPlayerChoice;
	ceCAPlayerChoiceOption *pOption;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAPChoiceOptionCutAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAPChoiceOptionCutAction( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, ceCAPlayerChoice &playerChoice,
		ceCAPlayerChoiceOption &option, ceConversationAction *action );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	/** \brief Topic. */
	inline ceConversationTopic &GetTopic() const{ return *pTopic; }
	
	/** \brief If-else action. */
	inline ceCAPlayerChoice *GetPlayerChoice() const{ return pPlayerChoice; }
	
	/** \brief If-case. */
	inline ceCAPlayerChoiceOption *GetOption() const{ return pOption; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
