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

#ifndef _CEWPTMAWAITACTIONSMOVEACTION_H_
#define _CEWPTMAWAITACTIONSMOVEACTION_H_

#include "../../ceWPTMenuAction.h"

class ceConversation;
class ceConversationAction;
class ceConversationTopic;
class ceCAWait;



/**
 * \brief Menu action remove action to topic.
 */
class ceWPTMAWaitActionsMoveAction : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceCAWait *pWait;
	ceConversationAction *pAction;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAWaitActionsMoveAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create menu action. */
	ceWPTMAWaitActionsMoveAction( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceCAWait &wait, ceConversationAction *action, int index,
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
	inline ceCAWait *GetWait() const{ return pWait; }
	
	/** \brief Index of new position. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
