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

#ifndef _CEWPTMAPASTECONDITION_H_
#define _CEWPTMAPASTECONDITION_H_

#include "../ceWPTMenuAction.h"
#include "../../../../../conversation/condition/ceConversationConditionList.h"

class ceConversation;
class ceConversationCondition;
class igdeUndo;



/**
 * \brief Menu action paste condition.
 */
class ceWPTMAPasteCondition : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAPasteCondition();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAPasteCondition( ceWindowMain &windowMain,
		ceConversation &conversation );
	
	/** \brief Crete menu action. */
	ceWPTMAPasteCondition( ceWindowMain &windowMain,
		ceConversation &conversation, const char *text );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	
	/** \brief Create undo action for pasting condition. */
	virtual igdeUndo *CreateUndo( const ceConversationConditionList &conditions );
	/*@}*/
};

#endif
