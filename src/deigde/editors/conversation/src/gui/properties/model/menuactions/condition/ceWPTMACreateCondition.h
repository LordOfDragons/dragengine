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

#ifndef _CEWPTMACREATECONDITION_H_
#define _CEWPTMACREATECONDITION_H_

#include "../ceWPTMenuAction.h"
#include "../../../../../conversation/condition/ceConversationCondition.h"

class ceWindowMain;
class ceConversation;
class igdeUndo;



/**
 * \brief Base menu condition for tree item models creating conditions.
 */
class ceWPTMACreateCondition : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationCondition::eConditionTypes pConditionType;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMACreateCondition();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu condition. */
	ceWPTMACreateCondition( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationCondition::eConditionTypes conditionType );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	/** \brief Type of condition to create. */
	inline ceConversationCondition::eConditionTypes GetConditionType() const{ return pConditionType; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	
	/** \brief Create undo action for adding condition. */
	virtual igdeUndo *CreateUndo( ceConversationCondition *condition );
	
	
	
	/** \brief Create condition. */
	ceConversationCondition *CreateCondition();
	
	/** \brief Text for condition type. */
	static const char *ConditionTypeText( ceWindowMain &windowMain,
		ceConversationCondition::eConditionTypes conditionType );
	
	/** \brief Icon for condition type. */
	static igdeIcon *ConditionTypeIcon( ceWindowMain &windowMain,
		ceConversationCondition::eConditionTypes conditionType );
	/*@}*/
};

#endif
