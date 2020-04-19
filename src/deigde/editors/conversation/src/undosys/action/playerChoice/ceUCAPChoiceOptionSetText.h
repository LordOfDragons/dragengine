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

#ifndef _CEUCAPCHOICEOPTIONSETTEXT_H_
#define _CEUCAPCHOICEOPTIONSETTEXT_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class ceConversationTopic;
class ceCAPlayerChoiceOption;
class ceCAPlayerChoice;



/**
 * \brief Undo Action Player Choice Option Set Text.
 */
class ceUCAPChoiceOptionSetText : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceCAPlayerChoice *pPlayerChoice;
	ceCAPlayerChoiceOption *pOption;
	decUnicodeString pOldText;
	decUnicodeString pNewText;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCAPChoiceOptionSetText( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
		ceCAPlayerChoiceOption *option, const decUnicodeString &newText );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCAPChoiceOptionSetText();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
