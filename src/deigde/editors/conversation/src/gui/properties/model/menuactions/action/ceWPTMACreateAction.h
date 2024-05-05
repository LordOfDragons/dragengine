/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
