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
#include <stdlib.h>
#include <string.h>

#include "ceWPTTIMAActorSpeak.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../langpack/ceLangPackEntry.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAActorSpeak::ceWPTTIMAActorSpeak( ceWindowMain &windowMain,
ceConversation &conversation, ceCAActorSpeak *action ) :
ceWPTTIMAction( windowMain, etActionActorSpeak, conversation, action )
{
	SetIcon( windowMain.GetIconActionActorSpeak() );
	Update();
}

ceWPTTIMAActorSpeak::~ceWPTTIMAActorSpeak(){
}



// Management
///////////////

void ceWPTTIMAActorSpeak::Update(){
	const ceCAActorSpeak &action = *GetActionActorSpeak();
	decString text, description, tbtext;
	
	text.Format( "%s: ", action.GetActor().GetString() );
	description = text;
	
	if( ! action.GetTextBoxTextTranslate().IsEmpty() ){
		const ceLangPack * const langpack = GetConversation().GetLanguagePack();
		ceLangPackEntry::Ref entry;
		
		if( langpack ){
			entry = langpack->GetEntryNamed( action.GetTextBoxTextTranslate() );
		}
		
		if( entry ){
			tbtext.Format( "%s", entry->GetText().ToUTF8().GetString() );
			
		}else{
			tbtext.Format( "{%s}", action.GetTextBoxTextTranslate().GetString() );
		}
		
	}else{
		tbtext = action.GetTextBoxText().ToUTF8();
	}
	
	if( ! tbtext.IsEmpty() ){
		const decString lineTBText( tbtext.Split( '\n' ).GetAt( 0 ) );
		
		if( lineTBText.GetLength() > 40 ){
			text += lineTBText.GetLeft( 40 ) + "...";
			
		}else{
			text += lineTBText;
		}
		
		description += tbtext;
	}
	
	SetText( text );
	SetDescription( description );
}
