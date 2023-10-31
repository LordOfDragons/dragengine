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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ceWDSLaneWord.h"
#include "../ceWindowDopeSheet.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/actor/speechAnimation/ceSpeechAnimation.h"
#include "../../../conversation/actor/speechAnimation/ceSAWord.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordAdd.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordMove.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordRemove.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordFromText.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordSetDuration.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordSet.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordSetPause.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordsScale.h"
#include "../../../undosys/action/actorSpeak/word/ceUCAASpeakWordClear.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Actions
////////////

namespace {

class cActionWordsFromText : public igdeAction{
protected:
	ceWDSLaneWord &pLane;
	
public:
	cActionWordsFromText( ceWDSLaneWord &lane ) : igdeAction( "Words From Text",
		nullptr, "Set words from Actor Speak Action text using speech animation information" ),
	pLane( lane ){ }

	virtual void OnAction(){
		ceCAActorSpeak * const action = pLane.GetWindow().GetActionASpeak();
		if( ! action ){
			return;
		}
		
		ceConversation * const conversation = pLane.GetWindow().GetConversation();
		if( ! conversation ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakWordFromText( pLane.GetWindow().GetTopic(), action ) );
		( ( ceUCAASpeakWordFromText& )( igdeUndo& )undo ).SetWordsFromText(
			action->ResolveTextBoxText( *conversation ), 0.075f );
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pLane.GetWindow().GetActionASpeak() );
	}
};

}



// Class ceWDSLaneWord
////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneWord::ceWDSLaneWord( ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description ) :
ceWDSLane( dopeSheet, index, label, description ){
}

ceWDSLaneWord::~ceWDSLaneWord(){
}



// Management
///////////////

void ceWDSLaneWord::OnContextMenu( igdeMenuCascade &menu, const decPoint &position ){
	ceWDSLane::OnContextMenu( menu, position );
	
	igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand( menu, new cActionWordsFromText( *this ), true );
}



const ceStripList &ceWDSLaneWord::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetWordList() : GetEmptyList();
}

void ceWDSLaneWord::FillIDList( decStringList &list ){
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	if( ! action ){
		return;
	}
	
	const ceConversationActorList &actorList = GetWindow().GetConversation()->GetActorList();
	ceSpeechAnimation *speechAnimation = NULL;
	
	ceConversationActor *actor = actorList.GetWithIDOrAliasID( action->GetActor() );
	if( actor ){
		speechAnimation = actor->GetSpeechAnimation();
	}
	if( ! speechAnimation ){
		return;
	}
	
	const ceSAWordList &saWordList = speechAnimation->GetWordList();
	const int saWordCount = saWordList.GetCount();
	int i;
	for( i=0; i<saWordCount; i++ ){
		list.Add( saWordList.GetAt( i )->GetName() );
	}
}

igdeUndo *ceWDSLaneWord::UndoStripAdd( ceStrip *strip, int index ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordAdd( GetWindow().GetTopic(), action, strip, index ) : NULL;
}

igdeUndo *ceWDSLaneWord::UndoStripRemove( ceStrip *strip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordRemove( GetWindow().GetTopic(), action, strip ) : NULL;
}

igdeUndo *ceWDSLaneWord::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordClear( GetWindow().GetTopic(), action ) : NULL;
}

igdeUndo *ceWDSLaneWord::UndoStripReplace( ceStrip *strip, ceStrip *withStrip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordSet( GetWindow().GetTopic(), action, strip, withStrip ) : NULL;
}

igdeUndo *ceWDSLaneWord::UndoStripMove( ceStrip *strip, int toIndex ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordMove( GetWindow().GetTopic(), action, strip, toIndex ) : NULL;
}

ceUCAASpeakStripSetPause *ceWDSLaneWord::UndoStripSetPause( ceStrip *strip, float pause ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordSetPause( GetWindow().GetTopic(), action, strip, pause ) : NULL;
}

ceUCAASpeakStripSetDuration *ceWDSLaneWord::UndoStripSetDuration( ceStrip *strip, float duration ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordSetDuration( GetWindow().GetTopic(), action, strip, duration ) : NULL;
}

ceUCAASpeakStripsScale *ceWDSLaneWord::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakWordsScale( GetWindow().GetTopic(), action ) : NULL;
}
