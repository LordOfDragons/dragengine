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

#ifndef _CEWPTMAWAITACTIONSADDACTION_H_
#define _CEWPTMAWAITACTIONSADDACTION_H_

#include "../ceWPTMACreateAction.h"

class ceConversationTopic;
class ceCAWait;


/**
 * \brief Menu action add action to if-else case.
 */
class ceWPTMAWaitActionsAddAction : public ceWPTMACreateAction{
private:
	ceConversationTopic *pTopic;
	ceCAWait *pWait;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAWaitActionsAddAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAWaitActionsAddAction( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, ceCAWait &wait,
		ceConversationAction::eActionTypes actionType, int index );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief If-else action. */
	inline ceCAWait *GetWait() const{ return pWait; }
	
	/** \brief Index to insert action at. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Create undo action for adding action. */
	virtual igdeUndo *CreateUndo( ceConversationAction *action );
	/*@}*/
};

#endif
