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

#ifndef _CEWPTMAPCHOICEOPTIONPASTESNIPPET_H_
#define _CEWPTMAPCHOICEOPTIONPASTESNIPPET_H_

#include "../ceWPTMAPasteSnippet.h"

class ceConversationTopic;
class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;



/**
 * \brief Menu action paste conversation snippet to topic.
 */
class ceWPTMAPChoiceOptionPasteSnippet : public ceWPTMAPasteSnippet{
private:
	ceConversationTopic *pTopic;
	ceCAPlayerChoice *pPlayerChoice;
	ceCAPlayerChoiceOption *pOption;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAPChoiceOptionPasteSnippet();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAPChoiceOptionPasteSnippet( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceCAPlayerChoice &playerChoice, ceCAPlayerChoiceOption &option, int index );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief If-else action. */
	inline ceCAPlayerChoice *GetPlayerChoice() const{ return pPlayerChoice; }
	
	/** \brief If-case. */
	inline ceCAPlayerChoiceOption *GetOption() const{ return pOption; }
	
	/** \brief Index to insert action at. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Create undo action for pasting actions. */
	virtual ceUCActionPaste *CreateUndo( const ceConversationActionList &actions );
	/*@}*/
};

#endif
