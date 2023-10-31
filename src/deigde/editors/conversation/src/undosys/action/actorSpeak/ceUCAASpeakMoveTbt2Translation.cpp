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

#include "ceUCAASpeakMoveTbt2Translation.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>


// Class ceUCAASpeakMoveTbt2Translation
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAASpeakMoveTbt2Translation::ceUCAASpeakMoveTbt2Translation( ceConversationTopic *topic,
	ceCAActorSpeak *actorSpeak, ceLangPackEntry *entry, bool addEntry, const decUnicodeString &text ) :
pTopic( topic ),
pConversation( topic && topic->GetFile() ? topic->GetFile()->GetConversation() : nullptr ),
pActorSpeak( actorSpeak ),
pLangPack( pConversation ? pConversation->GetLanguagePack() : nullptr ),
pEntry( entry ),
pAddEntry( addEntry ),
pTextBoxText( actorSpeak ? actorSpeak->GetTextBoxText() : decUnicodeString() ),
pOldEntryText( entry ? entry->GetText() : decUnicodeString() ),
pOldTranslationName( actorSpeak ? actorSpeak->GetTextBoxTextTranslate() : decString() )
{
	DEASSERT_NOTNULL( pConversation )
	DEASSERT_NOTNULL( pTopic )
	DEASSERT_NOTNULL( pLangPack )
	DEASSERT_NOTNULL( pActorSpeak )
	DEASSERT_NOTNULL( pEntry )
	
	SetShortInfo( "Move actor speak text box text to translation" );
}

ceUCAASpeakMoveTbt2Translation::~ceUCAASpeakMoveTbt2Translation(){
}



// Management
///////////////

void ceUCAASpeakMoveTbt2Translation::Undo(){
	if( pAddEntry ){
		pLangPack->RemoveEntry( pEntry );
		
	}else{
		pEntry->SetText( pOldEntryText );
	}
	
	pLangPack->SetChanged( true );
	
	pActorSpeak->SetTextBoxText( pTextBoxText );
	pActorSpeak->SetTextBoxTextTranslate( pOldTranslationName );
	
	pTopic->NotifyActionChanged( pActorSpeak );
	pConversation->NotifyLanguagePackChanged();
}

void ceUCAASpeakMoveTbt2Translation::Redo(){
	if( pAddEntry ){
		pLangPack->AddEntry( pEntry );
		
	}else{
		pEntry->SetText( pTextBoxText );
	}
	
	pLangPack->SetChanged( true );
	
	pActorSpeak->SetTextBoxText( decUnicodeString() );
	pActorSpeak->SetTextBoxTextTranslate( pEntry->GetName() );
	
	pTopic->NotifyActionChanged( pActorSpeak );
	pConversation->NotifyLanguagePackChanged();
}
