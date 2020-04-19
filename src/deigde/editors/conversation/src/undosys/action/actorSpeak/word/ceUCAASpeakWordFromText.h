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

#ifndef _CEUCAASPEAKWORDFROMTEXT_H_
#define _CEUCAASPEAKWORDFROMTEXT_H_

#include "../../../../conversation/strip/ceStripList.h"

#include <deigde/undo/igdeUndo.h>

class ceCAActorSpeak;
class ceConversationTopic;
class decUnicodeString;



/**
 * \brief Undo Action Actor Speak Conversation Action Word From Text.
 * Undo action replacing the words in an actor speak action with a list of words extracted
 * from a given text. In contrary to other undo actions this one here has to be initialized
 * after it has been created.
 */
class ceUCAASpeakWordFromText : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceCAActorSpeak *pActorSpeak;
	ceStripList pOldWords;
	ceStripList pNewWords;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCAASpeakWordFromText( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCAASpeakWordFromText();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set words list from text using 0.1 as length for each letter in the word. */
	void SetWordsFromText( const decUnicodeString &text, float letterDuration );
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
