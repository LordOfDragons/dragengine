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

#ifndef _CEWPTMAWAITACTIONSREMOVEALLACTIONS_H_
#define _CEWPTMAWAITACTIONSREMOVEALLACTIONS_H_

#include "../ceWPTMARemoveAllActions.h"

class ceConversationTopic;
class ceCAWait;



/**
 * \brief Menu action remove all actions.
 */
class ceWPTMAWaitActionsRemoveAllActions : public ceWPTMARemoveAllActions{
private:
	ceConversationTopic *pTopic;
	ceCAWait *pWait;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAWaitActionsRemoveAllActions();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAWaitActionsRemoveAllActions( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, ceCAWait &wait );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief If-else action. */
	inline ceCAWait *GetWait() const{ return pWait; }

	/** \brief Create undo action for adding action. */
	virtual igdeUndo *CreateUndo();
	/*@}*/
};

#endif
