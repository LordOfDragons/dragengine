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

#include "ceUCAASpeakMoveTbt2Translation.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>


// Class ceUCAASpeakMoveTbt2Translation
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakMoveTbt2Translation::ceUCAASpeakMoveTbt2Translation(ceConversationTopic *topic,
	ceCAActorSpeak *actorSpeak, const decString &translationName,
	const decUnicodeString *oldEntryText, const decUnicodeString &text) :
pTopic(topic),
pConversation(topic && topic->GetFile() ? topic->GetFile()->GetConversation() : nullptr),
pActorSpeak(actorSpeak),
pLangPack(pConversation ? pConversation->GetLanguagePack() : nullptr),
pTextBoxText(actorSpeak ? actorSpeak->GetTextBoxText() : decUnicodeString()),
pOldEntryText(oldEntryText ? *oldEntryText : decUnicodeString()),
pTranslationName(translationName),
pOldTranslationName(actorSpeak ? actorSpeak->GetTextBoxTextTranslate() : decString()),
pAddEntry(oldEntryText == nullptr)
{
	DEASSERT_NOTNULL(pConversation)
	DEASSERT_NOTNULL(pTopic)
	DEASSERT_NOTNULL(pLangPack)
	DEASSERT_NOTNULL(pActorSpeak)
	
	SetShortInfo("Move actor speak text box text to translation");
}

ceUCAASpeakMoveTbt2Translation::~ceUCAASpeakMoveTbt2Translation(){
}



// Management
///////////////

void ceUCAASpeakMoveTbt2Translation::Undo(){
	if(pAddEntry){
		pLangPack->GetEntries().Remove(pTranslationName);
		
	}else{
		pLangPack->GetEntries().SetAt(pTranslationName, pOldEntryText);
	}
	pLangPack->SetChanged(true);
	
	pActorSpeak->SetTextBoxText(pTextBoxText);
	pActorSpeak->SetTextBoxTextTranslate(pOldTranslationName);
	
	pTopic->NotifyActionChanged(pActorSpeak);
	pConversation->NotifyLanguagePackChanged();
}

void ceUCAASpeakMoveTbt2Translation::Redo(){
	pLangPack->GetEntries().SetAt(pTranslationName, pTextBoxText);
	pLangPack->SetChanged(true);
	
	pActorSpeak->SetTextBoxText(decUnicodeString());
	pActorSpeak->SetTextBoxTextTranslate(pTranslationName);
	
	pTopic->NotifyActionChanged(pActorSpeak);
	pConversation->NotifyLanguagePackChanged();
}
