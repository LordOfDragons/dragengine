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

#ifndef _CEWPTMAIFELSECASEPASTEACTIONS_H_
#define _CEWPTMAIFELSECASEPASTEACTIONS_H_

#include "../ceWPTMAPasteActions.h"

class ceConversationTopic;
class ceCAIfElse;
class ceCAIfElseCase;



/**
 * \brief Menu action paste all topic actions.
 */
class ceWPTMAIfElseCasePasteActions : public ceWPTMAPasteActions{
private:
	ceConversationTopic *pTopic;
	ceCAIfElse *pIfElse;
	ceCAIfElseCase *pIfCase;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAIfElseCasePasteActions();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAIfElseCasePasteActions( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceCAIfElse &ifElse, ceCAIfElseCase &ifCase, int index );
	
	/** \brief Crete menu action. */
	ceWPTMAIfElseCasePasteActions( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceCAIfElse &ifElse, ceCAIfElseCase &ifCase, int index, const char *text );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief If-else action. */
	inline ceCAIfElse *GetIfElse() const{ return pIfElse; }
	
	/** \brief If-case. */
	inline ceCAIfElseCase *GetIfCase() const{ return pIfCase; }
	
	/** \brief Index to insert action at. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Create undo action for pasting actions. */
	virtual ceUCActionPaste *CreateUndo( const ceConversationActionList &actions );
	/*@}*/
};

#endif
