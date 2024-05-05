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
