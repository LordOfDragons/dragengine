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

#ifndef _CEUCAASPEAKWORDFROMTEXT_H_
#define _CEUCAASPEAKWORDFROMTEXT_H_

#include "../../../../conversation/strip/ceStripList.h"

#include <deigde/undo/igdeUndo.h>
#include "ceCAActorSpeak.h"
#include "ceConversationTopic.h"

class decUnicodeString;



/**
 * \brief Undo Action Actor Speak Conversation Action Word From Text.
 * Undo action replacing the words in an actor speak action with a list of words extracted
 * from a given text. In contrary to other undo actions this one here has to be initialized
 * after it has been created.
 */
class ceUCAASpeakWordFromText : public igdeUndo{
private:
	ceConversationTopic::Ref pTopic;
	ceCAActorSpeak::Ref pActorSpeak;
	ceStripList pOldWords;
	ceStripList pNewWords;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCAASpeakWordFromText> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCAASpeakWordFromText(ceConversationTopic *topic, ceCAActorSpeak *actorSpeak);
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCAASpeakWordFromText();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set words list from text using 0.1 as length for each letter in the word. */
	void SetWordsFromText(const decUnicodeString &text, float letterDuration);
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
