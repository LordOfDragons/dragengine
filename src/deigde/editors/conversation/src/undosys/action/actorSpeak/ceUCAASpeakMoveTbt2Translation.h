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

#ifndef _CEUCAASPEAKMOVETBT2TRANSLATION_H_
#define _CEUCAASPEAKMOVETBT2TRANSLATION_H_

#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../langpack/ceLangPack.h"
#include "../../../langpack/ceLangPackEntry.h"

#include <deigde/undo/igdeUndo.h>


/**
 * Undo action move actor speak conversation action text box text to translation.
 */
class ceUCAASpeakMoveTbt2Translation : public igdeUndo{
private:
	const ceConversationTopic::Ref pTopic;
	const ceConversation::Ref pConversation;
	const ceCAActorSpeak::Ref pActorSpeak;
	const ceLangPack::Ref pLangPack;
	const ceLangPackEntry::Ref pEntry;
	const bool pAddEntry;
	const decUnicodeString pTextBoxText;
	const decUnicodeString pOldEntryText;
	const decString pOldTranslationName;
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCAASpeakMoveTbt2Translation> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	ceUCAASpeakMoveTbt2Translation( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak,
		ceLangPackEntry *entry, bool addEntry, const decUnicodeString &text );
	
protected:
	/** Clean up undo. */
	virtual ~ceUCAASpeakMoveTbt2Translation() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo() override;
	
	/** Redo. */
	virtual void Redo() override;
	/*@}*/
};

#endif
