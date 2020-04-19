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

#ifndef _CEWPTMACLOGICPASTECONDITION_H_
#define _CEWPTMACLOGICPASTECONDITION_H_

#include "../../condition/ceWPTMAPasteCondition.h"

class ceConversationTopic;
class ceCConditionLogic;
class ceConversationAction;


/**
 * \brief Menu action paste condition to logic.
 */
class ceWPTMACLogicPasteCondition : public ceWPTMAPasteCondition{
private:
	ceConversationTopic *pTopic;
	ceConversationAction *pAction;
	ceCConditionLogic *pLogic;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMACLogicPasteCondition();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMACLogicPasteCondition( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceConversationAction &action, ceCConditionLogic &logic );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief Action. */
	inline ceConversationAction *GetAction() const{ return pAction; }
	
	/** \brief If-else action. */
	inline ceCConditionLogic *GetLogic() const{ return pLogic; }
	
	/** \brief Create undo action for pasting condition. */
	virtual igdeUndo *CreateUndo( const ceConversationConditionList &conditions );
	/*@}*/
};

#endif
