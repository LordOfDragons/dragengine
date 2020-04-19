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

#ifndef _CEWPTMACLOGICREMOVECONDITION_H_
#define _CEWPTMACLOGICREMOVECONDITION_H_

#include "../../ceWPTMenuAction.h"

class ceConversation;
class ceConversationCondition;
class ceConversationTopic;
class ceConversationAction;
class ceCConditionLogic;



/**
 * \brief Menu action remove action to topic.
 */
class ceWPTMACLogicRemoveCondition : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceConversationAction *pAction;
	ceCConditionLogic *pLogic;
	ceConversationCondition *pCondition;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMACLogicRemoveCondition();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMACLogicRemoveCondition( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceConversationAction &action, ceCConditionLogic &logic,
		ceConversationCondition *condition );
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
	inline ceCConditionLogic *GetLogic() const{ return pLogic; }
	
	/** \brief Action. */
	inline ceConversationCondition *GetCondition() const{ return pCondition; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
