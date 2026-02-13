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

#include "ceWPTMACreateAction.h"
#include "../../action/ceWPTTIMAction.h"
#include "../../ceWPTTreeModel.h"
#include "../../../ceWPTopic.h"
#include "../../../ceWindowProperties.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceConversationAction.h"
#include "../../../../../conversation/action/ceCAActorAdd.h"
#include "../../../../../conversation/action/ceCAActorCommand.h"
#include "../../../../../conversation/action/ceCAActorRemove.h"
#include "../../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../../conversation/action/ceCACameraShot.h"
#include "../../../../../conversation/action/ceCAComment.h"
#include "../../../../../conversation/action/ceCACoordSystemAdd.h"
#include "../../../../../conversation/action/ceCACoordSystemRemove.h"
#include "../../../../../conversation/action/ceCAGameCommand.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../conversation/action/ceCAMusic.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../../../conversation/action/ceCASetActorParameter.h"
#include "../../../../../conversation/action/ceCASetVariable.h"
#include "../../../../../conversation/action/ceCASnippet.h"
#include "../../../../../conversation/action/ceCAStopConversation.h"
#include "../../../../../conversation/action/ceCAStopTopic.h"
#include "../../../../../conversation/action/ceCATrigger.h"
#include "../../../../../conversation/action/ceCAWait.h"

#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMACreateAction::ceWPTMACreateAction(ceWindowMain &windowMain,
ceConversation &conversation,
ceConversationAction::eActionTypes actionType) :
ceWPTMenuAction(windowMain,
	ActionTypeText(windowMain, actionType),
	ActionTypeIcon(windowMain, actionType)),
pConversation(&conversation),
pActionType(actionType){
}



// Management
///////////////

void ceWPTMACreateAction::OnAction(){
	GetConversation().GetUndoSystem()->Add(CreateUndo(CreateAction()));
}

igdeUndo::Ref ceWPTMACreateAction::CreateUndo(ceConversationAction*){
	// only not pure-virtual because FOX toolkit requires final classes. if the system
	// moves over to the IGDE ToolKit this will become a pure virtual again
	DETHROW(deeInvalidParam);
}



ceConversationAction::Ref ceWPTMACreateAction::CreateAction(){
	switch(pActionType){
	case ceConversationAction::eatCameraShot:
		return ceCACameraShot::Ref::New();
		
	case ceConversationAction::eatMusic:
		return ceCAMusic::Ref::New();
		
	case ceConversationAction::eatActorSpeak:
		return ceCAActorSpeak::Ref::New(pConversation->GetEngine());
		
	case ceConversationAction::eatSetVariable:
		return ceCASetVariable::Ref::New();
		
	case ceConversationAction::eatSetActorParameter:
		return ceCASetActorParameter::Ref::New();
		
	case ceConversationAction::eatTrigger:
		return ceCATrigger::Ref::New();
		
	case ceConversationAction::eatIfElse:{
		const ceCAIfElse::Ref ifElse(ceCAIfElse::Ref::New());
		ifElse->GetCases().Add(ceCAIfElseCase::Ref::New());
		return ifElse;
		}
		
	case ceConversationAction::eatPlayerChoice:{
		const ceCAPlayerChoice::Ref playerChoice(ceCAPlayerChoice::Ref::New());
		playerChoice->GetOptions().Add(ceCAPlayerChoiceOption::Ref::New());
		return playerChoice;
		}
		
	case ceConversationAction::eatWait:
		return ceCAWait::Ref::New();
		
	case ceConversationAction::eatSnippet:
		return ceCASnippet::Ref::New();
		
	case ceConversationAction::eatStopConversation:
		return ceCAStopConversation::Ref::New();
		
	case ceConversationAction::eatStopTopic:
		return ceCAStopTopic::Ref::New();
		
	case ceConversationAction::eatActorCommand:
		return ceCAActorCommand::Ref::New();
		
	case ceConversationAction::eatGameCommand:
		return ceCAGameCommand::Ref::New();
		
	case ceConversationAction::eatActorAdd:
		return ceCAActorAdd::Ref::New();
		
	case ceConversationAction::eatActorRemove:
		return ceCAActorRemove::Ref::New();
		
	case ceConversationAction::eatCoordSystemAdd:
		return ceCACoordSystemAdd::Ref::New();
		
	case ceConversationAction::eatCoordSystemRemove:
		return ceCACoordSystemRemove::Ref::New();
		
	case ceConversationAction::eatComment:
		return ceCAComment::Ref::New();
		
	default:
		DETHROW(deeInvalidAction);
	}
}

decString ceWPTMACreateAction::ActionTypeText(ceWindowMain &windowMain,
ceConversationAction::eActionTypes actionType){
	switch(actionType){
	case ceConversationAction::eatCameraShot:
		return windowMain.Translate("Conversation.ActionType.CameraShot").ToUTF8();
		
	case ceConversationAction::eatMusic:
		return windowMain.Translate("Conversation.ActionType.Music").ToUTF8();
		
	case ceConversationAction::eatActorSpeak:
		return windowMain.Translate("Conversation.ActionType.ActorSpeak").ToUTF8();
		
	case ceConversationAction::eatSetVariable:
		return windowMain.Translate("Conversation.ActionType.SetVariable").ToUTF8();
		
	case ceConversationAction::eatSetActorParameter:
		return windowMain.Translate("Conversation.ActionType.SetActorParameter").ToUTF8();
		
	case ceConversationAction::eatTrigger:
		return windowMain.Translate("Conversation.ActionType.Trigger").ToUTF8();
		
	case ceConversationAction::eatIfElse:
		return windowMain.Translate("Conversation.ActionType.IfElse").ToUTF8();
		
	case ceConversationAction::eatPlayerChoice:
		return windowMain.Translate("Conversation.ActionType.PlayerChoice").ToUTF8();
		
	case ceConversationAction::eatWait:
		return windowMain.Translate("Conversation.ActionType.Wait").ToUTF8();
		
	case ceConversationAction::eatSnippet:
		return windowMain.Translate("Conversation.ActionType.Snippet").ToUTF8();
		
	case ceConversationAction::eatStopConversation:
		return windowMain.Translate("Conversation.ActionType.StopConversation").ToUTF8();
		
	case ceConversationAction::eatStopTopic:
		return windowMain.Translate("Conversation.ActionType.StopTopic").ToUTF8();
		
	case ceConversationAction::eatActorCommand:
		return windowMain.Translate("Conversation.ActionType.ActorCommand").ToUTF8();
		
	case ceConversationAction::eatGameCommand:
		return windowMain.Translate("Conversation.ActionType.GameCommand").ToUTF8();
		
	case ceConversationAction::eatActorAdd:
		return windowMain.Translate("Conversation.ActionType.ActorAdd").ToUTF8();
		
	case ceConversationAction::eatActorRemove:
		return windowMain.Translate("Conversation.ActionType.ActorRemove").ToUTF8();
		
	case ceConversationAction::eatCoordSystemAdd:
		return windowMain.Translate("Conversation.ActionType.CoordSystemAdd").ToUTF8();
		
	case ceConversationAction::eatCoordSystemRemove:
		return windowMain.Translate("Conversation.ActionType.CoordSystemRemove").ToUTF8();
		
	case ceConversationAction::eatComment:
		return windowMain.Translate("Conversation.ActionType.Comment").ToUTF8();
		
	default:
		return "??";
	}
}

igdeIcon *ceWPTMACreateAction::ActionTypeIcon(ceWindowMain &windowMain,
ceConversationAction::eActionTypes actionType){
	switch(actionType){
	case ceConversationAction::eatCameraShot:
		return windowMain.GetIconActionCameraShot();
		
	case ceConversationAction::eatMusic:
		return windowMain.GetIconActionMusic();
		
	case ceConversationAction::eatActorSpeak:
		return windowMain.GetIconActionActorSpeak();
		
	case ceConversationAction::eatSetVariable:
		return windowMain.GetIconActionVariable();
		
	case ceConversationAction::eatSetActorParameter:
		return windowMain.GetIconActionVariable();
		
	case ceConversationAction::eatTrigger:
		return windowMain.GetIconActionTrigger();
		
	case ceConversationAction::eatIfElse:
		return windowMain.GetIconActionIfElse();
		
	case ceConversationAction::eatPlayerChoice:
		return windowMain.GetIconActionPlayerChoice();
		
	case ceConversationAction::eatWait:
		return windowMain.GetIconActionWait();
		
	case ceConversationAction::eatSnippet:
		return windowMain.GetIconActionSnippet();
		
	case ceConversationAction::eatStopConversation:
		return windowMain.GetIconActionStop();
		
	case ceConversationAction::eatStopTopic:
		return windowMain.GetIconActionStop();
		
	case ceConversationAction::eatActorCommand:
		return windowMain.GetIconActionActorCommand();
		
	case ceConversationAction::eatGameCommand:
		return windowMain.GetIconActionCommand();
		
	case ceConversationAction::eatActorAdd:
		return windowMain.GetIconActionActorAdd();
		
	case ceConversationAction::eatActorRemove:
		return windowMain.GetIconActionActorRemove();
		
	case ceConversationAction::eatCoordSystemAdd:
		return windowMain.GetIconActionCoordSysAdd();
		
	case ceConversationAction::eatCoordSystemRemove:
		return windowMain.GetIconActionCoordSysRemove();
		
	case ceConversationAction::eatComment:
		return windowMain.GetIconActionComment();
		
	default:
		return nullptr;
	}
}
