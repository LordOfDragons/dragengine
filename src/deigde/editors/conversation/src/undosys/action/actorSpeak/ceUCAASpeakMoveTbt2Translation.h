/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
