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

const ceConversationAction::eActionTypes ceWPTTIMAction::ListAddMenuActions[ 19 ] = {
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

ceWPTTIMAction::ceWPTTIMAction( ceWindowMain &windowMain, eTypes type,
ceConversation &conversation, ceConversationAction *action ) :
ceWPTTreeItemModel( windowMain, conversation, type ),
pAction( action )
{
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
}

ceWPTTIMAction::~ceWPTTIMAction(){
}



// Management
///////////////

ceWPTTIMAction *ceWPTTIMAction::DeepFindAction( ceConversationAction *action ){
	if( pAction == action ){
		return this;
		
	}else{
		return ceWPTTreeItemModel::DeepFindAction( action );
	}
}

void ceWPTTIMAction::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTreeItemModel * const parent = GetParent();
	if( parent ){
		parent->ContextMenuAction( contextMenu, GetAction() );
		
	}else if( GetTree() ){
		GetTree()->ContextMenuAction( contextMenu, GetAction() );
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
	
	cePlaybackActionStack &stack = GetConversation().GetPlayback()->GetActionStack();
	cePlaybackActionStackEntry &stackTop = stack.GetTop();
	if( ! stackTop.GetParentList() ){
		DETHROW( deeInvalidParam );
	}
	
	const int actionIndex = stackTop.GetParentList()->IndexOf( pAction );
	if( actionIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	stackTop.SetNextIndex( actionIndex );
}
