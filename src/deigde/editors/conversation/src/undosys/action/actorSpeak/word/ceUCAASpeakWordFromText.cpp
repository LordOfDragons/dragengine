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

ceUCAASpeakWordFromText::ceUCAASpeakWordFromText( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak ){
	if( ! topic || ! actorSpeak ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pActorSpeak = NULL;
	pOldWords = actorSpeak->GetWordList();
	
	SetShortInfo( "Actor Speak Words From Text" );
	
	pTopic = topic;
	topic->AddReference();
	
	pActorSpeak = actorSpeak;
	actorSpeak->AddReference();
}

ceUCAASpeakWordFromText::~ceUCAASpeakWordFromText(){
	if( pActorSpeak ){
		pActorSpeak->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAASpeakWordFromText::SetWordsFromText( const decUnicodeString &text, float letterDuration ){
	const decUnicodeString padding = decUnicodeString::NewFromUTF8( " \t\n\r,.;:\"?!" );
	decUnicodeString word;
	ceStrip *entry = NULL;
	int character;
	int i, length;
	
	pNewWords.RemoveAll();
	
	length = text.GetLength();
	
	try{
		for( i=0; i<=length; i++ ){
			if( i < length ){
				character = text.GetAt( i );
				
			}else{
				character = ' ';
			}
			
			if( padding.Find( character, 0 ) != -1 ){
				if( ! word.IsEmpty() ){
					entry = new ceStrip( word.GetLower().ToUTF8().GetString(), letterDuration * ( float )word.GetLength(), 0.0f );
					pNewWords.Add( entry );
					entry->FreeReference();
					entry = NULL;
					
					word.SetFromUTF8( "" );
				}
				
			}else{
				word.AppendCharacter( character );
			}
		}
		
	}catch( const deException & ){
		if( entry ){
			entry->FreeReference();
		}
		throw;
	}
}

void ceUCAASpeakWordFromText::Undo(){
	pActorSpeak->GetWordList() = pOldWords;
	pTopic->NotifyActionChanged( pActorSpeak );
}

void ceUCAASpeakWordFromText::Redo(){
	pActorSpeak->GetWordList() = pNewWords;
	pTopic->NotifyActionChanged( pActorSpeak );
}
