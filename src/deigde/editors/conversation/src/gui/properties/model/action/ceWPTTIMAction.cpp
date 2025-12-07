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
#include "../ceWPTTreeItem.h"
#include "../ceWPTTreeModel.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceConversationAction.h"
#include "../../../../conversation/playback/cePlayback.h"
#include "../../../../conversation/playback/cePlaybackActionStackEntry.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Static parameters
//////////////////////

const ceConversationAction::eActionTypes ceWPTTIMAction::ListAddMenuActions[19] = {
	ceConversationAction::eatActorAdd,
	ceConversationAction::eatActorCommand,
	ceConversationAction::eatActorRemove,
	ceConversationAction::eatActorSpeak,
	ceConversationAction::eatCameraShot,
	ceConversationAction::eatComment,
	ceConversationAction::eatCoordSystemAdd,
	ceConversationAction::eatCoordSystemRemove,
	ceConversationAction::eatGameCommand,
	ceConversationAction::eatIfElse,
	ceConversationAction::eatMusic,
	ceConversationAction::eatPlayerChoice,
	ceConversationAction::eatSetActorParameter,
	ceConversationAction::eatSetVariable,
	ceConversationAction::eatSnippet,
	ceConversationAction::eatStopConversation,
	ceConversationAction::eatStopTopic,
	ceConversationAction::eatTrigger,
	ceConversationAction::eatWait
};

const int ceWPTTIMAction::ListAddMenuActionsCount = 19;



// Constructor, destructor
////////////////////////////

ceWPTTIMAction::ceWPTTIMAction(ceWindowMain &windowMain, eTypes type,
ceConversation &conversation, ceConversationAction *action) :
ceWPTTreeItemModel(windowMain, conversation, type),
pAction(action)
{
	if(! action){
		DETHROW(deeInvalidParam);
	}
}

ceWPTTIMAction::~ceWPTTIMAction(){
}



// Management
///////////////

ceWPTTIMAction *ceWPTTIMAction::DeepFindAction(ceConversationAction *action){
	if(pAction == action){
		return this;
		
	}else{
		return ceWPTTreeItemModel::DeepFindAction(action);
	}
}

void ceWPTTIMAction::OnContextMenu(igdeMenuCascade &contextMenu){
	if(! GetTreeItem()){
		return;
	}
	
	ceWPTTreeItemModel * const parent = GetParent();
	if(parent){
		parent->ContextMenuAction(contextMenu, GetAction());
		
	}else if(GetTree()){
		GetTree()->ContextMenuAction(contextMenu, GetAction());
	}
}

void ceWPTTIMAction::Update(){
}

void ceWPTTIMAction::UpdateActionLists(){
}

ceConversationAction *ceWPTTIMAction::GetOwnerAction() const{
	return pAction;
}

void ceWPTTIMAction::BuildPlaybackFromHere() const{
	ceWPTTreeItemModel::BuildPlaybackFromHere();
	
	cePlaybackActionStack &stack = GetConversation().GetPlayback()->GetMainActionStack();
	cePlaybackActionStackEntry &stackTop = stack.GetTop();
	if(! stackTop.GetParentList()){
		DETHROW(deeInvalidParam);
	}
	
	const int actionIndex = stackTop.GetParentList()->IndexOf(pAction);
	if(actionIndex == -1){
		DETHROW(deeInvalidParam);
	}
	stackTop.SetNextIndex(actionIndex);
}
