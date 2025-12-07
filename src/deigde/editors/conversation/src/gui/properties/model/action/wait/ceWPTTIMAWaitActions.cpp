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

#include "ceWPTTIMAWait.h"
#include "ceWPTTIMAWaitActions.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsAddAction.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsCopyActions.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsPasteActions.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsPasteSnippet.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsRemoveAllActions.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsRemoveAction.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsMoveAction.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsCutAction.h"
#include "../../menuactions/action/ceWPTMACopyAction.h"
#include "../../ceWPTTreeItem.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAWait.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../conversation/playback/cePlayback.h"
#include "../../../../../conversation/playback/cePlaybackActionStackEntry.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAWaitActions::ceWPTTIMAWaitActions( ceWindowMain &windowMain,
ceConversation &conversation, const ceConversationActionList &actions ) :
ceWPTTIMActions( windowMain, conversation, etActionWaitActions, actions )
{
	SetText( "Actions" );
}

ceWPTTIMAWaitActions::~ceWPTTIMAWaitActions(){
}



// Management
///////////////

ceWPTTIMAWait *ceWPTTIMAWaitActions::GetModelWait() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionWait ){
		return ( ceWPTTIMAWait* )parent;
		
	}else{
		return NULL;
	}
}



void ceWPTTIMAWaitActions::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAWait * const modelWait = GetModelWait();
	if( ! modelWait ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAWait &wait = *modelWait->GetActionWait();
	
	// add action to end of action list
	const int indexAppend = wait.GetActions().GetCount();
	int i;
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Add Action", environment.GetStockIcon( igdeEnvironment::esiPlus )));
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAWaitActionsAddAction( windowMain, conversation, *topic,
			wait, ceWPTTIMAction::ListAddMenuActions[ i ], indexAppend ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsCopyActions( windowMain, wait ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsPasteActions( windowMain, conversation,
		*topic, wait, indexAppend ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsPasteSnippet( windowMain, conversation,
		*topic, wait, indexAppend ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsRemoveAllActions(
		windowMain, conversation, *topic, wait ), true );
}

void ceWPTTIMAWaitActions::ContextMenuAction(
igdeMenuCascade &contextMenu, ceConversationAction *action ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAWait * const modelWait = GetModelWait();
	if( ! modelWait ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	ceCAWait &wait = *modelWait->GetActionWait();
	const ceConversationActionList &actions = wait.GetActions();
	const int indexAction = action ? actions.IndexOf( action ) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsMoveAction( windowMain, conversation,
		*topic, wait, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsMoveAction( windowMain, conversation,
		*topic, wait, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMACopyAction( windowMain, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsCutAction(
		windowMain, conversation, *topic, wait, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsPasteActions( windowMain, conversation,
		*topic, wait, indexAction, "Paste Actions Before" ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsPasteActions( windowMain, conversation,
		*topic, wait, indexAction + 1, "Paste Actions After" ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsRemoveAction(
		windowMain, conversation, *topic, wait, action ), true );
	helper.MenuSeparator( contextMenu );
	
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Insert Action Before", environment.GetStockIcon( igdeEnvironment::esiPlus )));
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAWaitActionsAddAction( windowMain, conversation, *topic,
			wait, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction ), true );
	}
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action After",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu , new ceWPTMAWaitActionsAddAction( windowMain, conversation, *topic,
			wait, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction + 1 ), true );
	}
}

void ceWPTTIMAWaitActions::Update(){
	ceWPTTIMAWait * const wait = GetModelWait();
	if( wait ){
		SetExpanded( wait->GetActionWait()->GetTIMActionsExpanded() );
	}
	
	ceWPTTIMActions::Update();
}

void ceWPTTIMAWaitActions::OnExpandedChanged(){
	ceWPTTIMAWait * const wait = GetModelWait();
	if( wait ){
		wait->GetActionWait()->SetTIMActionsExpanded( GetExpanded() );
	}
}

void ceWPTTIMAWaitActions::BuildPlaybackFromHere() const{
	ceWPTTreeItemModel::BuildPlaybackFromHere(); // super.super call on purpose
	
	ceWPTTIMAWait * const modelWait = GetModelWait();
	if( ! modelWait ){
		DETHROW( deeInvalidParam );
	}
	ceCAWait &wait = *modelWait->GetActionWait();
	
	cePlaybackActionStack &stack = GetConversation().GetPlayback()->GetMainActionStack();
	stack.Push( NULL, &wait, &wait.GetActions(), 0 );
	stack.GetTop().SetLoopCondition( wait.GetCondition() );
	stack.GetTop().SetLooping( true );
}
