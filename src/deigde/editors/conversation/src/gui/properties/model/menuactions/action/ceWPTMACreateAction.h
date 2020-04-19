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

#ifndef _CEWPTMACREATEACTION_H_
#define _CEWPTMACREATEACTION_H_

#include "../ceWPTMenuAction.h"
#include "../../../../../conversation/action/ceConversationAction.h"

class ceWindowMain;
class ceConversation;
class igdeUndo;



/**
 * \brief Base menu action for tree item models creating actions.
 */
class ceWPTMACreateAction : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationAction::eActionTypes pActionType;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMACreateAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMACreateAction( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationAction::eActionTypes actionType );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	/** \brief Type of action to create. */
	inline ceConversationAction::eActionTypes GetActionType() const{ return pActionType; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	
	/** \brief Create undo action for adding action. */
	virtual igdeUndo *CreateUndo( ceConversationAction *action );
	
	
	
	/** \brief Create action. */
	ceConversationAction *CreateAction();
	
	/** \brief Text for action type. */
	static const char *ActionTypeText( ceWindowMain &windowMain,
		ceConversationAction::eActionTypes actionType );
	
	/** \brief Icon for action type. */
	static igdeIcon *ActionTypeIcon( ceWindowMain &windowMain,
		ceConversationAction::eActionTypes actionType );
	/*@}*/
};

#endif
