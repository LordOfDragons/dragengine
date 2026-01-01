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
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Actions
////////////

namespace {

class cActionWordsFromText : public igdeAction{
public:
	typedef deTObjectReference<cActionWordsFromText> Ref;
	
private:
protected:
	ceWDSLaneWord &pLane;
	
public:
	cActionWordsFromText(ceWDSLaneWord &lane) : igdeAction("Words From Text",
		nullptr, "Set words from Actor Speak Action text using speech animation information"),
	pLane(lane){}

	void OnAction() override{
		ceCAActorSpeak * const action = pLane.GetWindow().GetActionASpeak();
		if(!action){
			return;
		}
		
		ceConversation * const conversation = pLane.GetWindow().GetConversation();
		if(!conversation){
			return;
		}
		
		ceUCAASpeakWordFromText::Ref undo(ceUCAASpeakWordFromText::Ref::New(
			pLane.GetWindow().GetTopic(), action));
		undo->SetWordsFromText(action->ResolveTextBoxText(*conversation), 0.075f);
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetEnabled(pLane.GetWindow().GetActionASpeak());
	}
};

}



// Class ceWDSLaneWord
////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneWord::ceWDSLaneWord(ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description) :
ceWDSLane(dopeSheet, index, label, description){
}

ceWDSLaneWord::~ceWDSLaneWord(){
}



// Management
///////////////

void ceWDSLaneWord::OnContextMenu(igdeMenuCascade &menu, const decPoint &position){
	ceWDSLane::OnContextMenu(menu, position);
	
	igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, cActionWordsFromText::Ref::New(*this));
}



const ceStrip::List &ceWDSLaneWord::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetWords() : GetEmptyList();
}

void ceWDSLaneWord::FillIDList(decStringList &list){
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	if(!action){
		return;
	}
	
	const ceConversationActorList &actorList = GetWindow().GetConversation()->GetActorList();
	ceSpeechAnimation *speechAnimation = nullptr;
	
	ceConversationActor *actor = actorList.GetWithIDOrAliasID(action->GetActor());
	if(actor){
		speechAnimation = actor->GetSpeechAnimation();
	}
	if(!speechAnimation){
		return;
	}
	
	speechAnimation->GetWordList().Visit([&](const ceSAWord::Ref &w){
		list.Add(w->GetName());
	});
}

igdeUndo::Ref ceWDSLaneWord::UndoStripAdd(ceStrip *strip, int index){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordAdd::Ref::New(GetWindow().GetTopic(), action, strip, index) : ceUCAASpeakWordAdd::Ref();
}

igdeUndo::Ref ceWDSLaneWord::UndoStripRemove(ceStrip *strip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordRemove::Ref::New(GetWindow().GetTopic(), action, strip) : ceUCAASpeakWordRemove::Ref();
}

igdeUndo::Ref ceWDSLaneWord::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordClear::Ref::New(GetWindow().GetTopic(), action) : ceUCAASpeakWordClear::Ref();
}

igdeUndo::Ref ceWDSLaneWord::UndoStripReplace(ceStrip *strip, ceStrip *withStrip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordSet::Ref::New(GetWindow().GetTopic(), action, strip, withStrip) : ceUCAASpeakWordSet::Ref();
}

igdeUndo::Ref ceWDSLaneWord::UndoStripMove(ceStrip *strip, int toIndex){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordMove::Ref::New(GetWindow().GetTopic(), action, strip, toIndex) : ceUCAASpeakWordMove::Ref();
}

ceUCAASpeakStripSetPause::Ref ceWDSLaneWord::UndoStripSetPause(ceStrip *strip, float pause){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordSetPause::Ref::New(GetWindow().GetTopic(), action, strip, pause) : ceUCAASpeakWordSetPause::Ref();
}

ceUCAASpeakStripSetDuration::Ref ceWDSLaneWord::UndoStripSetDuration(ceStrip *strip, float duration){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordSetDuration::Ref::New(GetWindow().GetTopic(), action, strip, duration) : ceUCAASpeakWordSetDuration::Ref();
}

ceUCAASpeakStripsScale::Ref ceWDSLaneWord::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakWordsScale::Ref::New(GetWindow().GetTopic(), action) : ceUCAASpeakWordsScale::Ref();
}
