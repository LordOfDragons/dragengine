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
#include "../../../../conversation/action/ceConversationAction.h"
#include "../../../../conversation/action/ceConversationActionList.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/playback/cePlayback.h"
#include "../../../../conversation/playback/cePlaybackActionStackEntry.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMActions::ceWPTTIMActions(ceWindowMain &windowMain, ceConversation &conversation,
eTypes type, const ceConversationActionList &actions) :
ceWPTTreeItemModel(windowMain, conversation, type),
pActions(actions){
}

ceWPTTIMActions::~ceWPTTIMActions(){
}



// Management
///////////////

ceWPTTIMAction *ceWPTTIMActions::GetChildWith(ceConversationAction *action) const{
	const int count = GetChildCount();
	int i;
	
	for(i=0; i<count; i++){
		ceWPTTIMAction * const child = (ceWPTTIMAction*)GetChildAt(i);
		if(child->GetAction() == action){
			return child;
		}
	}
	
	return NULL;
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
		ceWPTTIMAction::Ref model = NULL;
		const int childCount = GetChildCount();
		for(j=i; j<childCount; j++){
			ceWPTTIMAction * const child = (ceWPTTIMAction*)GetChildAt(j);
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
			
			try{
				InsertChild(model, i);
				model->Update();
				
			}catch(const deException &){
				throw;
			}
		}
	}
	
	// remove non-matching nodes
	while(GetChildCount() > count){
		RemoveChild(GetChildAt(GetChildCount() - 1));
	}
}

void ceWPTTIMActions::BuildPlaybackFromHere() const{
	ceWPTTreeItemModel::BuildPlaybackFromHere();
	
	if(pActions.GetCount() == 0){
		GetConversation().GetPlayback()->AdvanceToNextAction();
		
	}else{
		GetConversation().GetPlayback()->GetMainActionStack()->Push(
			NULL, GetOwnerAction(), &pActions, 0);
	}
}



ceWPTTIMAction *ceWPTTIMActions::CreateActionModel(ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction *action){
	if(!action){
		DETHROW(deeInvalidParam);
	}
	
	switch(action->GetType()){
	case ceConversationAction::eatCameraShot:
		return new ceWPTTIMACameraShot(windowMain, conversation, (ceCACameraShot*)action);
		
	case ceConversationAction::eatMusic:
		return new ceWPTTIMAMusic(windowMain, conversation, (ceCAMusic*)action);
		
	case ceConversationAction::eatActorSpeak:
		return new ceWPTTIMAActorSpeak(windowMain, conversation, (ceCAActorSpeak*)action);
		
	case ceConversationAction::eatIfElse:
		return new ceWPTTIMAIfElse(windowMain, conversation, (ceCAIfElse*)action);
		
	case ceConversationAction::eatPlayerChoice:
		return new ceWPTTIMAPlayerChoice(windowMain, conversation, (ceCAPlayerChoice*)action);
		
	case ceConversationAction::eatStopConversation:
		return new ceWPTTIMAStopConversation(windowMain, conversation, (ceCAStopConversation*)action);
		
	case ceConversationAction::eatStopTopic:
		return new ceWPTTIMAStopTopic(windowMain, conversation, (ceCAStopTopic*)action);
		
	case ceConversationAction::eatSnippet:
		return new ceWPTTIMASnippet(windowMain, conversation, (ceCASnippet*)action);
		
	case ceConversationAction::eatSetVariable:
		return new ceWPTTIMASetVariable(windowMain, conversation, (ceCASetVariable*)action);
		
	case ceConversationAction::eatSetActorParameter:
		return new ceWPTTIMASetActorParameter(windowMain, conversation, (ceCASetActorParameter*)action);
		
	case ceConversationAction::eatActorCommand:
		return new ceWPTTIMAActorCommand(windowMain, conversation, (ceCAActorCommand*)action);
		
	case ceConversationAction::eatGameCommand:
		return new ceWPTTIMAGameCommand(windowMain, conversation, (ceCAGameCommand*)action);
		
	case ceConversationAction::eatWait:
		return new ceWPTTIMAWait(windowMain, conversation, (ceCAWait*)action);
		
	case ceConversationAction::eatTrigger:
		return new ceWPTTIMATrigger(windowMain, conversation, (ceCATrigger*)action);
		
	case ceConversationAction::eatActorAdd:
		return new ceWPTTIMAActorAdd(windowMain, conversation, (ceCAActorAdd*)action);
		
	case ceConversationAction::eatActorRemove:
		return new ceWPTTIMAActorRemove(windowMain, conversation, (ceCAActorRemove*)action);
		
	case ceConversationAction::eatCoordSystemAdd:
		return new ceWPTTIMACoordSystemAdd(windowMain, conversation, (ceCACoordSystemAdd*)action);
		
	case ceConversationAction::eatCoordSystemRemove:
		return new ceWPTTIMACoordSystemRemove(windowMain, conversation, (ceCACoordSystemRemove*)action);
		
	case ceConversationAction::eatComment:
		return new ceWPTTIMAComment(windowMain, conversation, (ceCAComment*)action);
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	DETHROW(deeInvalidParam);
}
