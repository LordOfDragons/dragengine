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

#include "ceWPTTIMAction.h"
#include "ceWPTTIMActions.h"
#include "ceWPTTIMACameraShot.h"
#include "ceWPTTIMAMusic.h"
#include "ceWPTTIMAActorSpeak.h"
#include "ceWPTTIMAStopConversation.h"
#include "ceWPTTIMAStopTopic.h"
#include "ceWPTTIMASnippet.h"
#include "ceWPTTIMASetVariable.h"
#include "ceWPTTIMASetActorParameter.h"
#include "ceWPTTIMAActorCommand.h"
#include "ceWPTTIMAGameCommand.h"
#include "ceWPTTIMATrigger.h"
#include "ceWPTTIMAActorAdd.h"
#include "ceWPTTIMAActorRemove.h"
#include "ceWPTTIMACoordSystemAdd.h"
#include "ceWPTTIMACoordSystemRemove.h"
#include "ceWPTTIMAComment.h"
#include "ifelse/ceWPTTIMAIfElse.h"
#include "playerChoice/ceWPTTIMAPlayerChoice.h"
#include "wait/ceWPTTIMAWait.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/playback/cePlayback.h"
#include "../../../../conversation/playback/cePlaybackActionStackEntry.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMActions::ceWPTTIMActions(ceWindowMain &windowMain, ceConversation &conversation,
eTypes type, const ceConversationAction::List &actions) :
ceWPTTreeItemModel(windowMain, conversation, type),
pActions(actions){
}

ceWPTTIMActions::~ceWPTTIMActions(){
}



// Management
///////////////

ceWPTTIMAction *ceWPTTIMActions::GetChildWith(ceConversationAction *action) const{
	for(const auto &c : GetChildren()){
		const ceWPTTIMAction::Ref child(c.DynamicCast<ceWPTTIMAction>());
		if(child->GetAction() == action){
			return child;
		}
	}
	return nullptr;
}

void ceWPTTIMActions::StructureChanged(){
	Update();
}



void ceWPTTIMActions::Update(){
	UpdateActions();
}

void ceWPTTIMActions::UpdateActions(){
	const int count = pActions.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		ceConversationAction * const action = pActions.GetAt(i);
		
		// find item matching action if present
		ceWPTTIMAction::Ref model;
		const int childCount = GetChildren().GetCount();
		for(j=i; j<childCount; j++){
			const ceWPTTIMAction::Ref child(GetChildren().GetAt(j).DynamicCast<ceWPTTIMAction>());
			if(child->GetAction() == action){
				model = child;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if(model){
			if(j != i){
				MoveChild(j, i);
			}
			model->Update();
			
		}else{
			model = CreateActionModel(GetWindowMain(), GetConversation(), action);
			InsertChild(model, i);
			model->Update();
		}
	}
	
	// remove non-matching nodes
	while(GetChildren().GetCount() > count){
		RemoveChild(GetChildren().Last());
	}
}

void ceWPTTIMActions::BuildPlaybackFromHere() const{
	ceWPTTreeItemModel::BuildPlaybackFromHere();
	
	if(pActions.GetCount() == 0){
		GetConversation().GetPlayback()->AdvanceToNextAction();
		
	}else{
		GetConversation().GetPlayback()->GetMainActionStack()->Push(
			nullptr, GetOwnerAction(), &pActions, 0);
	}
}



ceWPTTIMAction::Ref ceWPTTIMActions::CreateActionModel(ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction *action){
	if(!action){
		DETHROW(deeInvalidParam);
	}
	
	switch(action->GetType()){
	case ceConversationAction::eatCameraShot:
		return ceWPTTIMACameraShot::Ref::New(windowMain, conversation, (ceCACameraShot*)action);
		
	case ceConversationAction::eatMusic:
		return ceWPTTIMAMusic::Ref::New(windowMain, conversation, (ceCAMusic*)action);
		
	case ceConversationAction::eatActorSpeak:
		return ceWPTTIMAActorSpeak::Ref::New(windowMain, conversation, (ceCAActorSpeak*)action);
		
	case ceConversationAction::eatIfElse:
		return ceWPTTIMAIfElse::Ref::New(windowMain, conversation, (ceCAIfElse*)action);
		
	case ceConversationAction::eatPlayerChoice:
		return ceWPTTIMAPlayerChoice::Ref::New(windowMain, conversation, (ceCAPlayerChoice*)action);
		
	case ceConversationAction::eatStopConversation:
		return ceWPTTIMAStopConversation::Ref::New(windowMain, conversation, (ceCAStopConversation*)action);
		
	case ceConversationAction::eatStopTopic:
		return ceWPTTIMAStopTopic::Ref::New(windowMain, conversation, (ceCAStopTopic*)action);
		
	case ceConversationAction::eatSnippet:
		return ceWPTTIMASnippet::Ref::New(windowMain, conversation, (ceCASnippet*)action);
		
	case ceConversationAction::eatSetVariable:
		return ceWPTTIMASetVariable::Ref::New(windowMain, conversation, (ceCASetVariable*)action);
		
	case ceConversationAction::eatSetActorParameter:
		return ceWPTTIMASetActorParameter::Ref::New(windowMain, conversation, (ceCASetActorParameter*)action);
		
	case ceConversationAction::eatActorCommand:
		return ceWPTTIMAActorCommand::Ref::New(windowMain, conversation, (ceCAActorCommand*)action);
		
	case ceConversationAction::eatGameCommand:
		return ceWPTTIMAGameCommand::Ref::New(windowMain, conversation, (ceCAGameCommand*)action);
		
	case ceConversationAction::eatWait:
		return ceWPTTIMAWait::Ref::New(windowMain, conversation, (ceCAWait*)action);
		
	case ceConversationAction::eatTrigger:
		return ceWPTTIMATrigger::Ref::New(windowMain, conversation, (ceCATrigger*)action);
		
	case ceConversationAction::eatActorAdd:
		return ceWPTTIMAActorAdd::Ref::New(windowMain, conversation, (ceCAActorAdd*)action);
		
	case ceConversationAction::eatActorRemove:
		return ceWPTTIMAActorRemove::Ref::New(windowMain, conversation, (ceCAActorRemove*)action);
		
	case ceConversationAction::eatCoordSystemAdd:
		return ceWPTTIMACoordSystemAdd::Ref::New(windowMain, conversation, (ceCACoordSystemAdd*)action);
		
	case ceConversationAction::eatCoordSystemRemove:
		return ceWPTTIMACoordSystemRemove::Ref::New(windowMain, conversation, (ceCACoordSystemRemove*)action);
		
	case ceConversationAction::eatComment:
		return ceWPTTIMAComment::Ref::New(windowMain, conversation, (ceCAComment*)action);
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	DETHROW(deeInvalidParam);
}
