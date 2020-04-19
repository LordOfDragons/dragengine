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

#ifndef _CEWPTMATOPICPASTEACTIONS_H_
#define _CEWPTMATOPICPASTEACTIONS_H_

#include "../ceWPTMAPasteActions.h"

class ceConversationTopic;
class ceConversation;



/**
 * \brief Menu action paste all topic actions.
 */
class ceWPTMATopicPasteActions : public ceWPTMAPasteActions{
private:
	ceConversationTopic *pTopic;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMATopicPasteActions();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMATopicPasteActions( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, int index );
	
	/** \brief Crete menu action. */
	ceWPTMATopicPasteActions( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, int index,
		const char *text );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief Create undo action for pasting actions. */
	virtual ceUCActionPaste *CreateUndo( const ceConversationActionList &actions );
	/*@}*/
};

#endif
