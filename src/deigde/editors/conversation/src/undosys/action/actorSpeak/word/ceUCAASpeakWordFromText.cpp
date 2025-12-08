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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ceUCAASpeakWordFromText.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAASpeakWordFromText
//////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakWordFromText::ceUCAASpeakWordFromText(ceConversationTopic *topic, ceCAActorSpeak *actorSpeak){
	if(!topic || !actorSpeak){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pOldWords = actorSpeak->GetWordList();
	
	SetShortInfo("Actor Speak Words From Text");
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakWordFromText::~ceUCAASpeakWordFromText(){
}



// Management
///////////////

void ceUCAASpeakWordFromText::SetWordsFromText(const decUnicodeString &text, float letterDuration){
	const decUnicodeString padding = decUnicodeString::NewFromUTF8(" \t\n\r,.;:\"?!");
	decUnicodeString word;
	ceStrip *entry = NULL;
	int character;
	int i, length;
	
	pNewWords.RemoveAll();
	
	length = text.GetLength();
	
	try{
		for(i=0; i<=length; i++){
			if(i < length){
				character = text.GetAt(i);
				
			}else{
				character = ' ';
			}
			
			if(padding.Find(character, 0) != -1){
				if(!word.IsEmpty()){
					entry = new ceStrip(word.GetLower().ToUTF8().GetString(), letterDuration * (float)word.GetLength(), 0.0f);
					pNewWords.Add(entry);
					entry->FreeReference();
					entry = NULL;
					
					word.SetFromUTF8("");
				}
				
			}else{
				word.AppendCharacter(character);
			}
		}
		
	}catch(const deException &){
		if(entry){
			entry->FreeReference();
		}
		throw;
	}
}

void ceUCAASpeakWordFromText::Undo(){
	pActorSpeak->GetWordList() = pOldWords;
	pTopic->NotifyActionChanged(pActorSpeak);
}

void ceUCAASpeakWordFromText::Redo(){
	pActorSpeak->GetWordList() = pNewWords;
	pTopic->NotifyActionChanged(pActorSpeak);
}
